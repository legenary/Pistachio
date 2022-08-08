#include "ptcpch.h"
#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace Pistachio {


	ContentBrowserPanel::ContentBrowserPanel() {

	}



	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser");

		ImGui::End();
	}
}