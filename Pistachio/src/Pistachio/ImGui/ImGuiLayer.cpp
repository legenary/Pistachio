#include "ptcpch.h"
#include "ImGuiLayer.h"

#include "Pistachio/Keycodes.h"
#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Pistachio/Application.h"

// temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pistachio {
	ImGuiLayer::ImGuiLayer()
	: Layer("ImGuiLayer") {

	}

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::OnAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.KeyMap[ImGuiKey_Tab] = PTC_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = PTC_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = PTC_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = PTC_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = PTC_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = PTC_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = PTC_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = PTC_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = PTC_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = PTC_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = PTC_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = PTC_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = PTC_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = PTC_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = PTC_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = PTC_KEY_A;
		io.KeyMap[ImGuiKey_C] = PTC_KEY_C;
		io.KeyMap[ImGuiKey_V] = PTC_KEY_V;
		io.KeyMap[ImGuiKey_X] = PTC_KEY_X;
		io.KeyMap[ImGuiKey_Y] = PTC_KEY_V;
		io.KeyMap[ImGuiKey_Z] = PTC_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {

	}

	void ImGuiLayer::OnUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(PTC_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(PTC_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(PTC_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(PTC_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(PTC_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(PTC_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(PTC_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(PTC_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[PTC_KEY_LEFT_CONTROL] || io.KeysDown[PTC_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[PTC_KEY_LEFT_SHIFT] || io.KeysDown[PTC_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[PTC_KEY_LEFT_ALT] || io.KeysDown[PTC_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[PTC_KEY_LEFT_SUPER] || io.KeysDown[PTC_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}
}