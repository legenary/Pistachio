#include <Pistachio.h>
#include <Pistachio/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Pistachio {

	class HazelEditor : public Application {
	public:
		HazelEditor()
			: Application("Pistachio Editor") {
			PushLayer(new EditorLayer());
		}
		virtual ~HazelEditor() {

		}
	};

	Application* CreateApplication() {
		return new HazelEditor();
	}
}
