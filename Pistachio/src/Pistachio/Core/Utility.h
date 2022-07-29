#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 TRS2Transform(const glm::vec3& pos = { 0.0f, 0.0f, 0.0f }, const glm::vec3& rot = {0.0f, 0.0f, 0.0f}, const glm::vec3& size = {1.0f, 1.0f, 1.0f}) {

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 rotation =
		(rot.x == 0 ? glm::mat4(1.0f) : glm::rotate(glm::mat4(1.0f), rot.x, { 1, 0, 0 })) *
		(rot.y == 0 ? glm::mat4(1.0f) : glm::rotate(glm::mat4(1.0f), rot.y, { 0, 1, 0 })) *
		(rot.z == 0 ? glm::mat4(1.0f) : glm::rotate(glm::mat4(1.0f), rot.z, { 0, 0, 1 }));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size.x, size.y, size.z });
	return translate * rotation * scale;

}