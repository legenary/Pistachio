#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

#include "Pistachio/ImGui/ImGuiLayer.h"

#include "Pistachio/Renderer/Shader.h"
#include "Pistachio/Renderer/Buffer.h"
#include "Pistachio/Renderer/VertexArray.h"

#include "Pistachio/Renderer/OrthographicCamera.h"

namespace Pistachio {


	class PISTACHIO_API Application
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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		// first triangle
		std::shared_ptr<Shader> m_TriShader;
		std::shared_ptr<VertexArray> m_TriVA;

		// second square
		std::shared_ptr<Shader> m_SquareShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;

	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}



