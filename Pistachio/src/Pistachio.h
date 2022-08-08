#pragma once

// For use by Pistachio applications
#include <stdio.h>
#include "Pistachio/Core/Application.h"
#include "Pistachio/Core/Input.h"
#include "Pistachio/Core/MouseButtonCodes.h"
#include "Pistachio/Core/Keycodes.h"
#include "Pistachio/Core/Layer.h"
#include "Pistachio/Core/Log.h"
#include "Pistachio/ImGui/ImGuiLayer.h"
#include "Pistachio/Core/Timestep.h"
#include "Pistachio/Core/Utility.h"

// renderer
#include "Pistachio/Renderer/Renderer.h"
#include "Pistachio/Renderer/Renderer2D.h"
#include "Pistachio/Renderer/RenderCommand.h"

#include "Pistachio/Renderer/Buffer.h"
#include "Pistachio/Renderer/Shader.h"
#include "Pistachio/Renderer/FrameBuffer.h"
#include "Pistachio/Renderer/Texture.h"
#include "Pistachio/Renderer/VertexArray.h"
#include "Pistachio/Renderer/SubTexture2D.h"

#include "Pistachio/Renderer/OrthographicCamera.h"
#include "Pistachio/Renderer/OrthographicCameraController.h"

// ECS
#include "Pistachio/Scene/Scene.h"
#include "Pistachio/Scene/Entity.h"
#include "Pistachio/Scene/ScriptableEntity.h"
#include "Pistachio/Scene/Component.h"
#include "Pistachio/Scene/SceneSerializer.h"


#include "Pistachio/Debug/Instrumentor.h"
