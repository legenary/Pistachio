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

		// Update
		//if (m_ViewportFocused) {
		//	m_CameraController.OnUpdate(ts);
		//}

		// Renderer Prep
		{
			PTC_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		}
		
	
		// sprite sheet test
		{
			//Renderer2D::BeginScene(m_CameraController.GetCamera());
			//Renderer2D::DrawQuad(m_Texture, { -50.0f, -50.0f, -0.5f }, glm::radians(0.0f), { 100.0f, 100.0f }, 50.0f);
			//Renderer2D::DrawQuad(m_TextureMap['W'], { 0.0f, 0.0f, 1.0f }, glm::radians(0.0f), { 1.0f, 1.0f });
			//Renderer2D::DrawQuad(m_TextureMap['D'], { 0.0f, -1.0f, 1.0f }, glm::radians(0.0f), { 1.0f, 1.0f });
			//Renderer2D::DrawQuad(m_TextureMap['G'], { -1.0f, -1.0f, 1.0f }, glm::radians(0.0f), { 1.0f, 1.0f });
			//Renderer2D::DrawQuad({1.0f, 1.0f, 0.0f, 1.0f}, { -1.0f, 2.0f, 1.0f }, glm::radians(30.0f), { 1.0f, 1.0f });
			// Draw scene
			m_ActiveScene->OnUpdate(ts);


			//Renderer2D::EndScene();
			m_Framebuffer->Unbind();
		}


	}

	void EditorLayer::OnImGuiRender() {
		PTC_PROFILE_FUNCTION();


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

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
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

		m_SceneHierarchyPanel.OnImGuiRender();

		//{
		//	ImGui::Begin("Settings");
		//	if (m_SquareEntity) {
		//		ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());
		//		auto& color = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
		//		ImGui::ColorEdit4("SquareColor", glm::value_ptr(color));
		//		ImGui::Separator();
		//	}
		//	//auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
		//	//glm::float32 cs = camera.GetOrthographicSize();
		//	//if (ImGui::DragFloat("camera size", &cs)) {
		//	//	camera.SetOrthographicSize(cs);
		//	//}
		//	ImGui::End();
		//}

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
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			// Gizmos
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			auto primaryCameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			if (selectedEntity && primaryCameraEntity && m_GizmoType != -1) {
				
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				// Get primary camera from the scene
				const auto& camera = primaryCameraEntity.GetComponent<CameraComponent>().Camera;
				const glm::mat4& cameraProjection = camera.GetProjectionMatrix();
				glm::mat4 cameraView = glm::inverse(primaryCameraEntity.GetComponent<TransformComponent>().GetTransform());

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

