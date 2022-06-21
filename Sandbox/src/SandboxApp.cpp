#include <Pistachio.h>

#include "Pistachio/ImGui/ImGuiLayer.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Pistachio::Layer {
public: 
	ExampleLayer()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) /* aspecto ratio 16:9 */
		, Layer("Example")
		, m_CameraPosition(0.0f), m_CameraRotation(0.0f) {

		m_TriVA.reset(Pistachio::VertexArray::Create());

		// vertex buffer //////////////////////////////////////////////////////////////////
		float vertices[3 * 7] = {
			-0.5f,	-0.5f,	0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
			0.5f,	-0.5f,	0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
			0.0f,	0.5f,	0.0F,	1.0f, 1.0f, 0.0f, 1.0f
		};
		std::shared_ptr<Pistachio::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Pistachio::VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			Pistachio::BufferLayout layout = {
				{ Pistachio::ShaderDataType::Float3, "a_Position" },
				{ Pistachio::ShaderDataType::Float4, "a_Color"}
			};
			vertexBuffer->SetLayout(layout);
		}
		m_TriVA->AddVertexBuffer(vertexBuffer);

		// index buffer //////////////////////////////////////////////////////////////////
		std::shared_ptr<Pistachio::IndexBuffer> indexBuffer;
		unsigned int indices[3] = { 0, 1, 2 };
		indexBuffer.reset(Pistachio::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_TriVA->SetIndexBuffer(indexBuffer);

		// shader ////////////////////////////////////////////////////////////////////////
		std::string vertexSrc = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;	

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_TriShader.reset(new Pistachio::Shader(vertexSrc, fragmentSrc));

		// now do a second square
		m_SquareVA.reset(Pistachio::VertexArray::Create());
		// vertex buffer
		float SquareVertices[4 * 3] = {
			-0.5f,	-0.5f,	0.0f,
			0.5f,	-0.5f,	0.0f,
			0.5f,	0.5f,	0.0F,
			-0.5f,	0.5f,	0.0F
		};
		std::shared_ptr<Pistachio::VertexBuffer> SquareVB;
		SquareVB.reset(Pistachio::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));
		// vertex buffer layout
		Pistachio::BufferLayout squareLayout = {
			{ Pistachio::ShaderDataType::Float3, "a_Position" },
		};
		SquareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(SquareVB);
		// index buffer
		unsigned int SquareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Pistachio::IndexBuffer> SquareIB;
		SquareIB.reset(Pistachio::IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(SquareIB);
		// shader
		std::string SquareVertexSrc = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_SquareShader.reset(new Pistachio::Shader(SquareVertexSrc, SquareFragmentSrc));

	}

	void OnUpdate(Pistachio::Timestep ts) override {
		PTC_TRACE("Delta time: {0}s", ts.GetSeconds());
		//camera
		if (Pistachio::Input::IsKeyPressed(PTC_KEY_A)) {
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		} else if (Pistachio::Input::IsKeyPressed(PTC_KEY_D)) {
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		if (Pistachio::Input::IsKeyPressed(PTC_KEY_W)) {
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		} else if (Pistachio::Input::IsKeyPressed(PTC_KEY_S)) {
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		if (Pistachio::Input::IsKeyPressed(PTC_KEY_Q)) {
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		else if (Pistachio::Input::IsKeyPressed(PTC_KEY_E)) {
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		Pistachio::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Pistachio::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Pistachio::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		int nX = 10, nY = 10;
		for (int x = 0; x < nX; x++) {
			for (int y = 0; y < nY; y++) {
				glm::vec3 pos = { (x - nX / 2) * 0.11f , (y - nY / 2) * 0.11f, 0.0f };
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Pistachio::Renderer::Submit(m_SquareShader, m_SquareVA, transform);
			}
		}

		//Pistachio::Renderer::Submit(m_TriShader, m_TriVA);

		Pistachio::Renderer::EndScene();

		//Renderer::Flush();
	}
	
	void OnEvent(Pistachio::Event& event) override {
	}

private:
	// first triangle
	std::shared_ptr<Pistachio::Shader> m_TriShader;
	std::shared_ptr<Pistachio::VertexArray> m_TriVA;

	// second square
	std::shared_ptr<Pistachio::Shader> m_SquareShader;
	std::shared_ptr<Pistachio::VertexArray> m_SquareVA;

	Pistachio::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;
	float m_CameraRotation;
	float m_CameraRotationSpeed = 90.0f;

};


class Sandbox : public Pistachio::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};

Pistachio::Application* Pistachio::CreateApplication() {
	return new Sandbox();
}