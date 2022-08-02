#include "EditorLayer.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

#include "Pistachio/Utils/PlatformUtils.h"
#include "Pistachio/Scene/SceneSerializer.h"
#include "Pistachio/Core/Input.h"
#include "Pistachio/Core/Utility.h"

#include <ImGuizmo.h>

namespace Pistachio {

	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1280, 720) {
	}

	void EditorLayer::OnAttach() {
		//m_CameraController.SetZoomLevel(4.0f);

		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_EmojiTexture = Texture2D::Create("assets/textures/emoji.png");
		m_SpriteSheet = Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png", {128, 128} /*stride*/);

		m_TextureMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11});
		m_TextureMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 11});
		m_TextureMap['G'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, {1, 11});

		// framebuffer
		FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = FrameBuffer::Create(fbSpec);

		// Scene
		m_ActiveScene = CreateRef<Scene>();

		// Editor Camera
		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		//Scene hierarchy
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		// load my work
		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize("assets/scenes/test.ptc");
		m_cachedSavePath = "assets/scenes/test.ptc";

	}

	void EditorLayer::OnDetach() {
	}

	void EditorLayer::OnUpdate(Timestep ts) {
		PTC_PROFILE_FUNCTION();

		// Update camera controller
		if (m_ViewportFocused) {
			m_EditorCamera.OnUpdate(ts);
			//m_CameraController.OnUpdate(ts);
		}

		// Renderer Prep
		{
			PTC_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind(); 
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		}
	
		// Rendering
		{
			PTC_PROFILE_SCOPE("Renderering");
			// Draw scene
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

			//Renderer2D::EndScene();
			m_Framebuffer->Unbind();
		}


	}

	void EditorLayer::ImGuiPrep() {
		static bool DockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// 
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &DockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
	}

	void EditorLayer::OnImGuiRender() {
		PTC_PROFILE_FUNCTION();

		ImGuiPrep();

		// Menu bar
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New (Default scene)", "Ctrl+N"))
					SceneNew();
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					SceneOpen();
				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SceneSave();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SceneSaveAs();
				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Scene Hierarchy Panel
		m_SceneHierarchyPanel.OnImGuiRender();

		// Viewport
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			//PTC_CORE_TRACE("viewport size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) 
				&& viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				//m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
				m_ActiveScene->OnViewportResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
				m_EditorCamera.SetViewportSize(viewportPanelSize.x, viewportPanelSize.y);
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			// Gizmos
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && m_GizmoType != -1) {
				
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				// Get view and projection from editor camera
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
				glm::mat4 cameraProjection = m_EditorCamera.GetProjectionMatrix();

				// Entity transform
				auto& transComp = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = transComp.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(PTC_KEY_LEFT_CONTROL);
				float snapValue = (m_GizmoType == ImGuizmo::OPERATION::ROTATE) ? 15.0f : 0.5f;
				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing()) {
					glm::vec3 t, r, s;
					DecomposeTransform(transform, t, r, s);
					transComp.Translation = t;
					transComp.Rotation += (r - transComp.Rotation);
					transComp.Scale = s;
				}
			}


			ImGui::End();
			ImGui::PopStyleVar();
		}

		ImGui::End();
			
	}

	void EditorLayer::OnEvent(Event& event) {
		//if (m_ViewportHovered) {
		//	m_CameraController.OnEvent(event);
		//}
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(PTC_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
		// Hotkey
		if (e.GetRepearCount() > 0)
			return false;

		bool ctrl = Input::IsKeyPressed(PTC_KEY_LEFT_CONTROL) || Input::IsKeyPressed(PTC_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(PTC_KEY_LEFT_SHIFT) || Input::IsKeyPressed(PTC_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode()) {
			case PTC_KEY_S: {
				if (ctrl && shift)
					SceneSaveAs();
				else if (ctrl)
					SceneSave();
				break;
			}
			case PTC_KEY_O: {
				if (ctrl)
					SceneOpen();
				break;
			}
			case PTC_KEY_N: {
				if (ctrl)
					SceneNew();
				break;
			}
			
			// Gizmos
			case PTC_KEY_Q: {
				m_GizmoType = -1;
				break;
			}
			case PTC_KEY_W: {
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case PTC_KEY_E: {
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case PTC_KEY_R: {
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}

		}
	}
	void EditorLayer::SceneSave() {
		if (!m_cachedSavePath.empty()) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(m_cachedSavePath);
		}
		else {
			SceneSaveAs();
		}
	}
	void EditorLayer::SceneSaveAs() {
		std::string filepath = FileDialogs::SaveFile("Pistachio Scene (*.ptc)\0*.ptc\0");
		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
			m_cachedSavePath = filepath;
		}
	}
	void EditorLayer::SceneNew() {
		m_SceneHierarchyPanel.Clear();
		CreateDefaultScene();
		m_cachedSavePath = {};
	}
	void EditorLayer::SceneOpen() {
		std::string filepath = FileDialogs::OpenFile("Pistachio Scene (*.ptc)\0*.ptc\0");
		if (!filepath.empty()) {
			m_SceneHierarchyPanel.Clear();
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
			m_cachedSavePath = filepath;
		}
	}

	void EditorLayer::CreateDefaultScene() {
		// This is default scene

		// Square sprite
		Entity squareEntity = m_ActiveScene->CreateEntity("Square");
		squareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		// Camera
		Entity cameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		cameraEntity.AddComponent<CameraComponent>();

		// Native script for camera
		class CameraController : public ScriptableEntity {
		public:
			void OnCreate() {
				PTC_CORE_INFO("Create camera controller!");
			}
			void OnDestroy() {

			}
			void OnUpdate(Timestep ts) override {
				auto& translation = GetComponent<TransformComponent>().Translation;
				if (Input::IsKeyPressed(PTC_KEY_A)) {
					translation[0] -= 5 * ts;
				}
			}
		};
		cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

}

