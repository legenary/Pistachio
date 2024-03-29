#include "ptcpch.h"
#include "ImGuiLayer.h"

#include "Pistachio/Core/Keycodes.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include "Pistachio/Core/Application.h"

// temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <ImGuizmo.h>

namespace Pistachio {
	ImGuiLayer::ImGuiLayer()
	: Layer("ImGuiLayer") {

	}

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::OnAttach() {
		PTC_PROFILE_FUNCTION();

		// setup dear ImGui context
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); //(void io);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// enable keyboard controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// enable docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// enable multi-vieport

		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/Roboto-font/Roboto-Regular.ttf", 16.0f);

		// Setup dear imgui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindoWRounding/WindowBg so platform windows can look identical to regular ones
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// setup platform/renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {
		PTC_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender() {
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::OnEvent(Event& e) {
		//ImGuiIO& io = ImGui::GetIO();
		//e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		//e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}


	void ImGuiLayer::Begin() {
		PTC_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End() {
		PTC_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}