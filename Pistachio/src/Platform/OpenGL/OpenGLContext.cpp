#include "ptcpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pistachio {


	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		PTC_CORE_ASSERT(m_WindowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PTC_CORE_ASSERT(status, "Failed to initialize Glad!");

		PTC_CORE_INFO("OpenGL Info:");
		PTC_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		PTC_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		PTC_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}

