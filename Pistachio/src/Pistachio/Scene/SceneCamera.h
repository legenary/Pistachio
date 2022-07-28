#pragma once

#include "Pistachio/Renderer/Camera.h"


namespace Pistachio {

	class SceneCamera : public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		inline ProjectionType GetProjectionType() const { return m_ProjectionType; }
		inline void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		inline float GetOrthographicSize() const { return m_OrthographicSize; }
		inline void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		inline float GetOrthographicNearClip() const { return m_OrthographicNear; }
		inline float GetOrthographicFarClip() const { return m_OrthographicFar; }
		inline void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		inline void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		inline float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		inline void SetPerspectiveVerticalFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
		inline float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		inline float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		inline void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		inline void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 1.0f;
	};

}