#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 TRS2Transform(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const float rotation = 0, const glm::vec2& size = { 1.0f, 1.0f }) {

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotate = (rotation == 0) ? glm::mat4(1.0f) : glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	return translate * rotate * scale;

}