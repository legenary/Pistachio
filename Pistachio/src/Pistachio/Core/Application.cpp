#include "ptcpch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>
#include "Pistachio/Renderer/Renderer.h"

#include "Pistachio/Renderer/OrthographicCamera.h"

#include <GLFW/glfw3.h>

namespace Pistachio {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		PTC_PROFILE_FUNCTION();

		PTC_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(PTC_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		PTC_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		PTC_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		PTC_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(PTC_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(PTC_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			// query which layer can handle the event
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		PTC_PROFILE_FUNCTION();

		while (m_Running) {
			PTC_PROFILE_SCOPE("RunLoop");
			
			float time = (float)glfwGetTime(); // platform
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				PTC_PROFILE_SCOPE("layer:Layerstack->OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			// we still want ImGui active when main rendering window is minimized
			m_ImGuiLayer->Begin();
			{
				PTC_PROFILE_SCOPE("layer:Layerstack->OnImGuiRender");
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		PTC_PROFILE_FUNCTION();

		// if minimized (|| and && both fine)
		if (e.GetHeight() == 0 || e.GetWidth() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		Renderer::OnWindoeResize(e.GetWidth(), e.GetHeight());
			
		return false;
	}


}