#include "ptcpch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>
#include "Pistachio/Renderer/Renderer.h"

namespace Pistachio {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		PTC_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(PTC_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		
		m_TriVA.reset(VertexArray::Create());

		// vertex buffer //////////////////////////////////////////////////////////////////
		float vertices[3 * 7] = {
			-0.5f,	-0.5f,	0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
			0.5f,	-0.5f,	0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
			0.0f,	0.5f,	0.0F,	1.0f, 1.0f, 0.0f, 1.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color"}
			};
			vertexBuffer->SetLayout(layout);
		}
		m_TriVA->AddVertexBuffer(vertexBuffer);

		// index buffer //////////////////////////////////////////////////////////////////
		std::shared_ptr<IndexBuffer> indexBuffer;
		unsigned int indices[3] = { 0, 1, 2 };
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		m_TriVA->SetIndexBuffer(indexBuffer);

		// shader ////////////////////////////////////////////////////////////////////////
		std::string vertexSrc = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;	

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = v_Color;
			}

		)";

		m_TriShader.reset(new Shader(vertexSrc, fragmentSrc));

		// now do a second square
		m_SquareVA.reset(VertexArray::Create());
		// vertex buffer
		float SquareVertices[4 * 3] = {
			-0.75f,	-0.75f,	0.0f,
			0.75f,	-0.75f,	0.0f,
			0.75f,	0.75f,	0.0F,
			-0.75f,	0.75f,	0.0F
		};
		std::shared_ptr<VertexBuffer> SquareVB;
		SquareVB.reset(VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));
		// vertex buffer layout
		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
		};
		SquareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(SquareVB);
		// index buffer
		unsigned int SquareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> SquareIB;
		SquareIB.reset(IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(SquareIB);
		// shader
		std::string SquareVertexSrc = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string SquareFragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;

			void main() {
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}

		)";

		m_SquareShader.reset(new Shader(SquareVertexSrc, SquareFragmentSrc));
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e) {
		PTC_CORE_INFO("{0}", e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(PTC_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			// query which layer can handle the event
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::Run() {

		while (m_Running) {

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_SquareShader->Bind();
			Renderer::Submit(m_SquareVA);
			
			m_TriShader->Bind();
			Renderer::Submit(m_TriVA);

			Renderer::EndScene();

			//Renderer::Flush();

			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}