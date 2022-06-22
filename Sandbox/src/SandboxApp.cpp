#include <Pistachio.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Pistachio::Layer {
public: 
	ExampleLayer()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) /* aspecto ratio 16:9 */
		, Layer("Example")
		, m_CameraPosition(0.0f), m_CameraRotation(0.0f) {

		/*
		// triangle primitive
		// vertex buffer //////////////////////////////////////////////////////////////////
		float vertices[3 * 7] = {
			-0.5f,	-0.5f,	0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
			0.5f,	-0.5f,	0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
			0.0f,	0.5f,	0.0F,	1.0f, 1.0f, 0.0f, 1.0f
		};
		Pistachio::Ref<Pistachio::VertexBuffer> vertexBuffer;
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
		Pistachio::Ref<Pistachio::IndexBuffer> indexBuffer;
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

		m_TriShader.reset(Pistachio::Shader::Create(vertexSrc, fragmentSrc));
		*/

		// square primitive
		m_SquareVA.reset(Pistachio::VertexArray::Create());
		// vertex buffer
		float SquareVertices[4 * 5] = {
			-0.5f,	-0.5f,	0.0f,	0.0f, 0.0f,
			0.5f,	-0.5f,	0.0f,	1.0f, 0.0f,
			0.5f,	0.5f,	0.0f,	1.0f, 1.0f,
			-0.5f,	0.5f,	0.0f,	0.0f, 1.0f
		};
		Pistachio::Ref<Pistachio::VertexBuffer> SquareVB;
		SquareVB.reset(Pistachio::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));
		// vertex buffer layout
		Pistachio::BufferLayout squareLayout = {
			{ Pistachio::ShaderDataType::Float3, "a_Position" },
			{ Pistachio::ShaderDataType::Float2, "a_TexCoord" }
		};
		SquareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(SquareVB);
		// index buffer
		unsigned int SquareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Pistachio::Ref<Pistachio::IndexBuffer> SquareIB;
		SquareIB.reset(Pistachio::IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(SquareIB);
		// shader
		std::string flatColorShaderVertexSrc = R"(
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

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;
			uniform vec3 u_Color;

			void main() {
				color = vec4(u_Color, 1.0);
			}

		)";

		m_FlatColorShader.reset(Pistachio::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		// texture shader
		std::string textureShaderVertexSrc = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main() {
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}

		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec2 v_TexCoord;
			uniform sampler2D u_Texture;

			void main() {
				color = texture(u_Texture, v_TexCoord);
			}

		)";

		m_TextureShader.reset(Pistachio::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		// texture
		m_Texture = Pistachio::Texture2D::Create("assets/textures/Checkerboard.png");
		m_EmojiTexture = Pistachio::Texture2D::Create("assets/textures/emoji.png");

		std::dynamic_pointer_cast<Pistachio::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Pistachio::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Pistachio::Timestep ts) override {
		Pistachio::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Pistachio::RenderCommand::Clear();

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
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		// draw calls
		Pistachio::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<Pistachio::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Pistachio::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		int nX = 10, nY = 10;
		for (int x = 0; x < nX; x++) {
			for (int y = 0; y < nY; y++) {
				glm::vec3 pos = { (x - nX / 2) * 0.11f , (y - nY / 2) * 0.11f, 0.0f };
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Pistachio::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind(/*slot 0 by default*/);
		Pistachio::Renderer::Submit(m_TextureShader, m_SquareVA, 
			glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_EmojiTexture->Bind(/*slot 0 by default*/);
		Pistachio::Renderer::Submit(m_TextureShader, m_SquareVA, 
			glm::scale(glm::mat4(1.0f), glm::vec3(0.8f)));


		//triangle
		//Pistachio::Renderer::Submit(m_TriShader, m_TriVA);

		Pistachio::Renderer::EndScene();

		//Renderer::Flush();
	}
	
	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Pistachio::Event& event) override {
	}

private:
	// first triangle
	Pistachio::Ref<Pistachio::Shader> m_TriShader;
	Pistachio::Ref<Pistachio::VertexArray> m_TriVA;

	// second square
	Pistachio::Ref<Pistachio::Shader> m_FlatColorShader, m_TextureShader;
	Pistachio::Ref<Pistachio::VertexArray> m_SquareVA;

	Pistachio::Ref<Pistachio::Texture2D> m_Texture, m_EmojiTexture;

	Pistachio::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;
	float m_CameraRotation;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

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