#pragma once

#include "Pistachio/Renderer/OrthographicCamera.h"
#include "Pistachio/Core/Timestep.h"

#include "Pistachio/Events/ApplicationEvent.h"
#include "Pistachio/Events/MouseEvent.h"

namespace Pistachio {

	struct OrthographicCameraBounds {
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(unsigned int width, unsigned int height, bool rotation = true);
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		inline const OrthographicCamera& GetCamera() const { return m_Camera; }
		inline OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
		void SetZoomLevel(float zoom);
		void SetAspectRatio(int width, int height);


	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);

		void CalculateView();
	private:
		unsigned int m_WindowWidth, m_WindowHeight;
		float m_AspectRatio;
		float m_ZoomLevel = 10.0f;
		
		OrthographicCamera m_Camera;
		OrthographicCameraBounds m_Bounds;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 3.0f;
		float m_CameraRotationSpeed = 90.0f;

		struct MouseButtonPressed {
			bool Right = false;
			bool Left = false;
			int MoveCount = 0;
			glm::vec2 MouseStartPos;
			glm::vec3 CameraStartPos;
		};
		MouseButtonPressed m_PressedMouse;
	};

}


