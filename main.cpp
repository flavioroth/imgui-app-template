#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <gl_loader.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdio>

#include "utils/RollingBuffer.hpp"

#include "utils/Window.hpp"

class AppWindow : public Window {
private:
	utils::RollingBuffer<60, float> _fpsHistogram;

public:
	using Window::Window;
	static AppWindow& instance() {
		static AppWindow appWindow(1024, 768);
		return appWindow;
	}

	AppWindow(const AppWindow&) = delete;
	AppWindow& operator=(const AppWindow&) = delete;

protected:
	virtual void initHandler() {
		// Put your custom openGL code here
	}

	virtual void drawHandler(uint64_t timeNanos) {
		// Put your custom openGL code here
	}

	virtual void uiHandler(uint64_t timeNanos) {
		// Put your custom ImGui code here
		auto io = ImGui::GetIO();
		ImGui::Text("Time : %.2fs", ((double)timeNanos / (double)1000000000));
		ImGui::Text("FPS : %.1f", io.Framerate);
		ImGui::PlotHistogram(""
							 ,_fpsHistogram.values()
							 ,_fpsHistogram.size()
							 ,0
							 ,nullptr
							 ,.0f
							 ,120.0f
							 ,ImVec2(100, 30)
		);
		_fpsHistogram.push(io.Framerate);
	}
};

int main() {
	AppWindow::instance().run();
}
