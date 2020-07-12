
#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "gl_loader.hpp"

#include <chrono>
#include <exception>
#include <stdexcept>

class Window {
private:
	GLFWwindow* _window;
	std::chrono::steady_clock::time_point _start_time;

public:
	Window(int width, int height)
		: _window(nullptr) {

		if(!glfwInit()) {
			throw std::runtime_error("Failed to initialize glfw");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		_window = glfwCreateWindow(width, height, APP_NAME, NULL, NULL);

		if(nullptr == _window) {
			throw std::runtime_error("Failed to create window");
		}

		glfwSetWindowUserPointer(_window, this);
		glfwSetWindowSizeCallback(_window, Window::GLFWwindowSizeChangedEvent);
		glfwSetWindowRefreshCallback(_window, Window::GLFWwindowRefreshRequiredEvent);
		glfwSetWindowCloseCallback(_window, Window::GLFWwindowCloseRequestedEvent);
		glfwSetKeyCallback(_window, Window::GLFWwindowKeyEvent);
		glfwSetMouseButtonCallback(_window, Window::GLFWwindowMouseButtonEvent);
		glfwSetScrollCallback(_window, Window::GLFWwindowScrollEvent);
		glfwSetCharCallback(_window, Window::GLFWwindowCharEvent);

		glfwMakeContextCurrent(_window);
		glfwSwapInterval(1);// Enable vsync

		if(gl_loader::init()) {
			throw std::runtime_error("Failed to initialize OpenGL");
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(_window, true);
		ImGui_ImplOpenGL3_Init("#version 150");
	}

	virtual ~Window() {
		cleanupHandler();

		// Cleanup ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(_window);
		glfwTerminate();;
	}

	void run() {
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		initialize();

		while(!glfwWindowShouldClose(_window)) {

			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();

			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);

			// call the user draw function
			uint64_t elapsedNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::steady_clock::now() - _start_time
			).count();
			drawHandler(elapsedNanos);

			// draw the UI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin(APP_NAME, nullptr);

			uiHandler(elapsedNanos);

			ImGui::End();

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(_window);
		}
	}

private:
	void initialize() {
		_start_time = std::chrono::steady_clock::now();
		initHandler();
	}

	static void GLFWwindowSizeChangedEvent(GLFWwindow* window, int width, int height);
	static void GLFWwindowRefreshRequiredEvent(GLFWwindow* window);
	static void GLFWwindowCloseRequestedEvent(GLFWwindow* window);
	static void GLFWwindowKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void GLFWwindowMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	static void GLFWwindowScrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	static void GLFWwindowCharEvent(GLFWwindow* window, unsigned int c);

protected:
	virtual void initHandler(){};
	virtual void drawHandler(uint64_t timeNanos) = 0;
	virtual void uiHandler(uint64_t timeNanos) = 0;
	virtual void cleanupHandler(){};

	virtual void onWindowSizeChanged(int width, int height){};
	virtual void onWindowRefreshRequired(){};
	virtual void onWindowCloseRequested(){};
	virtual void onKeyEvent(int key, int scancode, int action, int mods){};
	virtual void onMouseButtonEvent(int button, int action, int mods){};
	virtual void onScrollEvent(double xoffset, double yoffset){};
	virtual void onCharEvent(unsigned int c){};


};
