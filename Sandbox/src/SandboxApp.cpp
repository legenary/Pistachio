
#include <Pistachio.h>

class ExampleLayer : public Pistachio::Layer {
public: 
	ExampleLayer()
		:Layer("Example") {
	}

	void OnUpdate() override {
		if (Pistachio::Input::IsKeyPressed(PTC_KEY_TAB))
			PTC_TRACE("Tab key");
	}
	
	void OnEvent(Pistachio::Event& event) override {
		//PTC_TRACE("{0}", event);
	}

};


class Sandbox : public Pistachio::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Pistachio::ImGuiLayer());
	}
	~Sandbox() {

	}
};

Pistachio::Application* Pistachio::CreateApplication() {
	return new Sandbox();
}