#include "../libs/config.h" // Ensure all necessary libraries are included
  #include <iostream>
  #include <stdio.h>
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Callback function for resizing input text buffer
int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        std::vector<char>* pBuffer = (std::vector<char>*)data->UserData;
        pBuffer->resize(data->BufTextLen + 1);
        data->Buf = pBuffer->data();
    }
    return 0;
}

int main() {
    //imgui initializations
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Hashing Application", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    std::vector<char> UserInputBuffer(1024); // Initial buffer size for input text

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Center the input box
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        // Set the size of the input box dynamically to allow text wrapping
        ImGui::SetNextWindowSize(ImVec2(800, 600));

        {
            ImGui::Begin("Hash Input", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            ImGui::Text("Enter text to hash:");

            ImGui::InputTextMultiline("##userinput", UserInputBuffer.data(), UserInputBuffer.size(), ImVec2(400,100), ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackResize, InputTextCallback, (void*)&UserInputBuffer);

            if (ImGui::Button("Hash")) {
                // Placeholder action for hashing
                // Replace with actual hash calculation and display logic
                // For now, just print the input text
                printf("Text to hash: %s\n", UserInputBuffer.data());
            }

            // Display area for hash result
            ImGui::Text("Hash result:");
            ImGui::TextWrapped("Hash will be displayed here once implemented.");

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
