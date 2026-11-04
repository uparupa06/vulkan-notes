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
    create_window_surface();
    initialise_debug_messenger();
    pick_physical_device();
    create_logical_device();
    build_swapchain();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}