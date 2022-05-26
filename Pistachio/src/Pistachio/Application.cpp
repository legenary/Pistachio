#include "ptcpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Pistachio/Log.h"

namespace Pistachio {

	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {

		WindowResizeEvent e(1280, 720);
		PTC_TRACE(e);

		while (true);
	}

}