#pragma once

#include "Entity.h"

namespace Pistachio {

	class Entity;
	class ScriptableEntity {
	public:
		template<typename T>
		T& GetComponent() const {
			return m_Entity.GetComponent<T>();
		}


	private:
		Entity m_Entity;

		friend class Scene;

	};
}