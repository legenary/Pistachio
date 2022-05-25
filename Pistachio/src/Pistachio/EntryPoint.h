#pragma once

#ifdef PTC_PLATFORM_WINDOWS

extern Pistachio::Application* Pistachio::CreateApplication();

void main(int argc, char** argv) {
	printf("Pistachio Engine started.\n");
	auto app = Pistachio::CreateApplication();
	app->Run();
	delete app;
}

#else
#error Only support Windows!
#endif
