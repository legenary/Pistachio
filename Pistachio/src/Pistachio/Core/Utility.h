#pragma once

#include <glm/glm.hpp>

glm::mat4 TRS2Transform(const glm::vec3& pos = { 0.0f, 0.0f, 0.0f }, const glm::vec3& rot = { 0.0f, 0.0f, 0.0f }, const glm::vec3& size = { 1.0f, 1.0f, 1.0f });
bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
