#pragma once

#ifdef PTC_PLATFORM_WINDOWS

extern Pistachio::Application* Pistachio::CreateApplication();

void main(int argc, char** argv) {

	Pistachio::Log::Init();
	PTC_CORE_WARN("Initialized Log!");
	int a = 5;
	PTC_INFO("Hello Var={0}", a); 

	auto app = Pistachio::CreateApplication();
	app->Run();
	delete app;
}

#else
#error Only support Windows!
#endif
