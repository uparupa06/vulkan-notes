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
    pick_physical_device();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}