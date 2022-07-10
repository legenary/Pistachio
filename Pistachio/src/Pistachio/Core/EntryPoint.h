#pragma once

#ifdef PTC_PLATFORM_WINDOWS

extern Pistachio::Application* Pistachio::CreateApplication();

void main(int argc, char** argv) {

	Pistachio::Log::Init();
	
	PTC_PROFILE_BEGIN_SESSION("Startup", "PistachioProfile-Startup.json");
	auto app = Pistachio::CreateApplication();
	PTC_PROFILE_END_SESSION();

	PTC_PROFILE_BEGIN_SESSION("Runtime", "PistachioProfile-Runtime.json");
	app->Run();
	PTC_PROFILE_END_SESSION();

	PTC_PROFILE_BEGIN_SESSION("Shutdown", "PistachioProfile-Shutdown.json");
	delete app;
	PTC_PROFILE_END_SESSION();
}

#else
#error Only support Windows!
#endif
