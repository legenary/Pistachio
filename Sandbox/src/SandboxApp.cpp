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

		
		/// triangle primitive
		m_TriVA = Pistachio::VertexArray::Create();
		// vertex buffer //////////////////////////////////////////////////////////////////
		float vertices[3 * 7] = {
			-0.5f,	-0.5f,	0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
			0.5f,	-0.5f,	0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
			0.0f,	0.5f,	0.0F,	1.0f, 1.0f, 0.0f, 1.0f
		};
		Pistachio::Ref<Pistachio::VertexBuffer> vertexBuffer;
		vertexBuffer = Pistachio::VertexBuffer::Create(vertices, sizeof(vertices));
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
		indexBuffer = Pistachio::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_TriVA->SetIndexBuffer(indexBuffer);

		// shader ////////////////////////////////////////////////////////////////////////
		m_ShaderLibrary.Load("assets/shaders/Triangle.glsl");
		

		/// square primitive
		m_SquareVA = Pistachio::VertexArray::Create();
		// vertex buffer
		float SquareVertices[4 * 5] = {
			-0.5f,	-0.5f,	0.0f,	0.0f, 0.0f,
			0.5f,	-0.5f,	0.0f,	1.0f, 0.0f,
			0.5f,	0.5f,	0.0f,	1.0f, 1.0f,
			-0.5f,	0.5f,	0.0f,	0.0f, 1.0f
		};
		Pistachio::Ref<Pistachio::VertexBuffer> SquareVB;
		SquareVB = Pistachio::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices));
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
		SquareIB = Pistachio::IndexBuffer::Create(SquareIndices, sizeof(SquareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(SquareIB);
		// shader
		m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");

		/// texture
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		//m_TextureShader = Pistachio::Shader::Create();

		m_Texture = Pistachio::Texture2D::Create("assets/textures/Checkerboard.png");
		m_EmojiTexture = Pistachio::Texture2D::Create("assets/textures/emoji.png");

		std::dynamic_pointer_cast<Pistachio::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Pistachio::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

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

		auto textureShader = m_ShaderLibrary.Get("Texture");
		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");

		std::dynamic_pointer_cast<Pistachio::OpenGLShader>(flatColorShader)->Bind();
		std::dynamic_pointer_cast<Pistachio::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		int nX = 10, nY = 10;
		for (int x = 0; x < nX; x++) {
			for (int y = 0; y < nY; y++) {
				glm::vec3 pos = { (x - nX / 2) * 0.11f , (y - nY / 2) * 0.11f, 0.0f };
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Pistachio::Renderer::Submit(flatColorShader, m_SquareVA, transform);
			}
		}

		

		m_Texture->Bind(/*slot 0 by default*/);
		Pistachio::Renderer::Submit(textureShader, m_SquareVA,
			glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_EmojiTexture->Bind(/*slot 0 by default*/);
		Pistachio::Renderer::Submit(textureShader, m_SquareVA,
			glm::scale(glm::mat4(1.0f), glm::vec3(0.8f)));


		//triangle
		Pistachio::Renderer::Submit(m_ShaderLibrary.Get("Triangle"), m_TriVA);

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
	Pistachio::ShaderLibrary m_ShaderLibrary;
	// first triangle
	Pistachio::Ref<Pistachio::VertexArray> m_TriVA;

	// second square
	Pistachio::Ref<Pistachio::VertexArray> m_SquareVA;

	// third texture
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