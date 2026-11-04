#include "application.hpp"

Application::Application() {}

Application::~Application() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Application::run() {
    
    initialise_window();
    create_instance();
    initialise_debug_messenger();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}