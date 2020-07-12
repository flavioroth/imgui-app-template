#include "Window.hpp"
#include <exception>

namespace {
	Window* getWindow(GLFWwindow* window) {
		void* userData = glfwGetWindowUserPointer(window);
		if(nullptr == userData) {
			throw std::runtime_error("invalid user data pointer");
		}
		return static_cast<Window*>(userData);
	}
}

void Window::GLFWwindowMouseButtonEvent(GLFWwindow* window, int button, int action, int mods) {
	getWindow(window)->onMouseButtonEvent(button, action, mods);
}

void Window::GLFWwindowScrollEvent(GLFWwindow* window, double xoffset, double yoffset) {
	getWindow(window)->onScrollEvent(xoffset, yoffset);
}

void Window::GLFWwindowCharEvent(GLFWwindow* window, unsigned int c) {
	getWindow(window)->onCharEvent(c);
}

void Window::GLFWwindowKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	getWindow(window)->onKeyEvent(key, scancode, action, mods);
}

void Window::GLFWwindowCloseRequestedEvent(GLFWwindow* window) {
	getWindow(window)->onWindowCloseRequested();
}
void Window::GLFWwindowRefreshRequiredEvent(GLFWwindow* window) {
	getWindow(window)->onWindowRefreshRequired();
}

void Window::GLFWwindowSizeChangedEvent(GLFWwindow* window, int width, int height) {
	getWindow(window)->onWindowSizeChanged(width, height);
}
