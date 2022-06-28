#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D")
	, m_CameraController(1280, 720) {
	
}

void Sandbox2D::OnAttach() {
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

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Pistachio::Timestep ts) {
	//PTC_TRACE("Delta time: {0}s", ts.GetSeconds());

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Pistachio::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Pistachio::RenderCommand::Clear();

	// draw calls
	Pistachio::Renderer::BeginScene(m_CameraController.GetCamera());

	auto textureShader = m_ShaderLibrary.Get("Texture");
	auto flatColorShader = m_ShaderLibrary.Get("FlatColor");

	std::dynamic_pointer_cast<Pistachio::OpenGLShader>(flatColorShader)->Bind();
	std::dynamic_pointer_cast<Pistachio::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.09f));
	int nX = 10, nY = 10;
	for (int x = 0; x < nX; x++) {
		for (int y = 0; y < nY; y++) {
			glm::vec3 pos = { (x - nX / 2 + 0.5) * 0.1f , (y - nY / 2 + 0.5) * 0.1f, 0.0f };
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Pistachio::Renderer::Submit(flatColorShader, m_SquareVA, transform);
		}
	}

	m_Texture->Bind(/*slot 0 by default*/);
	Pistachio::Renderer::Submit(textureShader, m_SquareVA,
		glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)));

	m_EmojiTexture->Bind(/*slot 0 by default*/);
	Pistachio::Renderer::Submit(textureShader, m_SquareVA,
		glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

	//triangle
	Pistachio::Renderer::Submit(m_ShaderLibrary.Get("Triangle"), m_TriVA,
		glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) * 
		glm::scale(glm::mat4(1.0f), glm::vec3(0.8f)));

	Pistachio::Renderer::EndScene();

	//Renderer::Flush();
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Pistachio::Event& event) {
	m_CameraController.OnEvent(event);
}
