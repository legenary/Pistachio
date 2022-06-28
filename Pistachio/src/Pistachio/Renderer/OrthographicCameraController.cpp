#include "ptcpch.h"
#include "OrthographicCameraController.h"

#include "Pistachio/Core/Input.h"
#include "Pistachio/Core/Keycodes.h"


namespace Pistachio {

	
	OrthographicCameraController::OrthographicCameraController(unsigned int width, unsigned int height, bool rotation)
		: m_WindowWidth(width), m_WindowHeight(height)
		, m_AspectRatio((float)width/(float)height)
		, m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
		, m_Rotation(rotation) {

	}

	void OrthographicCameraController::OnUpdate(Timestep ts) {
		//camera
		if (Input::IsKeyPressed(PTC_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(PTC_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(PTC_KEY_W)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(PTC_KEY_S)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}

		if (m_Rotation) {
			if (Input::IsKeyPressed(PTC_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			else if (Input::IsKeyPressed(PTC_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			m_Camera.SetRotation(m_CameraRotation);
		}
		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel * 3;
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PTC_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(PTC_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseButtonPressedEvent>(PTC_BIND_EVENT_FN(OrthographicCameraController::OnMousePressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(PTC_BIND_EVENT_FN(OrthographicCameraController::OnMouseReleased));
		dispatcher.Dispatch<MouseMovedEvent>(PTC_BIND_EVENT_FN(OrthographicCameraController::OnMouseMoved));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel = std::max(m_ZoomLevel - e.GetYOffset() / 5, 0.3f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		m_WindowWidth = e.GetWidth();
		m_WindowHeight = e.GetHeight();
		m_AspectRatio = (float)m_WindowWidth / (float)m_WindowHeight;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnMousePressed(MouseButtonPressedEvent& e) {
		// right button clicked
		if (e.GetMouseButton() == 1) {
			m_PressedMouse.Right = true;
			m_PressedMouse.MoveCount = 0;
		}
		return false;
	}

	bool OrthographicCameraController::OnMouseReleased(MouseButtonReleasedEvent& e) {
		// right button unclicked
		if (e.GetMouseButton() == 1) {
			m_PressedMouse.Right = false;
			m_PressedMouse.MoveCount = 0;
		}
		return false;
	}

	bool OrthographicCameraController::OnMouseMoved(MouseMovedEvent& e) {
		if (!m_PressedMouse.Right)
			return false;
		// initialize this drag
		if (m_PressedMouse.MoveCount == 0) {
			m_PressedMouse.MouseStartPos = { e.GetX(), e.GetY() };
			m_PressedMouse.CameraStartPos = m_Camera.GetPosition();
		}
		float dx = e.GetX() - m_PressedMouse.MouseStartPos.x, dy = e.GetY() - m_PressedMouse.MouseStartPos.y;
		m_CameraPosition.x = m_PressedMouse.CameraStartPos.x - dx / m_WindowWidth * 2 * m_AspectRatio * m_ZoomLevel;
		m_CameraPosition.y = m_PressedMouse.CameraStartPos.y + dy / m_WindowHeight * 2 * m_ZoomLevel;
		m_Camera.SetPosition(m_CameraPosition);
		m_PressedMouse.MoveCount++;
		return false;
	}

}