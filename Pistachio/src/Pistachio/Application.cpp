#include "ptcpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "GLFW/glfw3.h"

namespace Pistachio {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& e) {
		PTC_CORE_INFO("{0}", e);
	}

	void Application::Run() {

		while (m_Running) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Window->OnUpdate();
		}
	}

}