#pragma once

#include "Core.h"
#include "Window.h"
#include "Pistachio/Events/ApplicationEvent.h"
#include "LayerStack.h"

#include "Pistachio/ImGui/ImGuiLayer.h"

#include "Pistachio/Renderer/Shader.h"
#include "Pistachio/Renderer/Buffer.h"
#include "Pistachio/Renderer/VertexArray.h"
#include "Pistachio/Core/Timestep.h"

#include "Pistachio/Renderer/OrthographicCamera.h"

namespace Pistachio {


	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		 
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}



