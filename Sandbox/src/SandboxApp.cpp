#include <Pistachio.h>
#include <Pistachio/Core/EntryPoint.h>

#include "Sandbox2D.h"


class Sandbox : public Pistachio::Application {
public:
	Sandbox() {
		PushLayer(new Sandbox2D());
	}
	virtual ~Sandbox() {

	}
};

Pistachio::Application* Pistachio::CreateApplication() {
	return new Sandbox();
}