#include "gui.h"
#include "sha.h"
#include "db.h"
#include "../libs/config.h"

void runGUI() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "SHA-256 GUI", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Increase font size
    ImGui::GetStyle().ScaleAllSizes(1.5f); // Scale all UI elements by 1.5 times
    ImFont* font = io.Fonts->AddFontDefault();
    font->Scale = 1.5f; // Scale the font size

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Initialize database
    Database db("hashes.db");
    if (!db.init()) {
        std::cerr << "Failed to initialize database" << std::endl;
        return;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Get the window size
        int window_width, window_height;
        glfwGetWindowSize(window, &window_width, &window_height);

        // Set the next window size and position to cover the whole main window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)));

        // UI Code
        ImGui::Begin("SHA-256 Hasher", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        static char inputText[256] = "";
        static std::string hashResult;

        ImGui::Text("Text to hash:");
        ImGui::InputText("##TextToHash", inputText, IM_ARRAYSIZE(inputText));

        if (ImGui::Button("Hash")) {
            SHA256 sha256;
            sha256.update(std::string(inputText));
            hashResult = sha256.digest();
            // Insert into database
            time_t now = time(0);
            std::string dt = ctime(&now);
            dt.pop_back(); // remove newline character
            if (!db.insertData(inputText, hashResult, dt)) {
                std::cerr << "Failed to insert data into database" << std::endl;
            }
        }

        ImGui::Text("Hash: %s", hashResult.c_str());

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
