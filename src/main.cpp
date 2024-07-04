#include "../libs/config.h" // sourcing all the libraries
#include <iostream>
#include <stdio.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}



int main() {

  //imgui initializations
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

        // Decide GL+GLSL versions
    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif



  // connecting with database
  // sqlite3 *db;
  // int dbOpen = sqlite3_open("hashfile_db", &db);
  // if (!dbOpen) {
  //   showerror<const char *, int>(__FILE__, __LINE__);
  //   std::cerr << " Failed opening database file!!" << std::endl;
  // }


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

  //adding fonts
  io.Fonts->AddFontFromFileTTF("../assets/fonts/Roboto.ttf", 13.5f);

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

  char UserInput; //this is a dummy input var for now replace this with the input string for hash
  bool display = false;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowSize(ImVec2(500, 500));
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    {
      ImGui::Begin("Test", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
      
      ImGui::Text("This is some useful text. Here we will take a user text input for now and display the generated hash.\n");

      ImGui::Text("What do you want to hash ? (File (f) or input text (t) ");


      //Below are the buttons that we can use to select whether to has files or the text. More work needed on this to implement logic.
      ImGui::Text("Enter the text: \n");

      ImGui::InputText("##userinput",&UserInput, 50);

      if(ImGui::Button("Text", ImVec2(50, 70))){
        display = true;
      }

      if(display){
        ImGui::Text("button pressed!!!");    
      }

    
    ImGui::End();
    }

  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);    

  //sets the color in the context window as declared in clear_color variable of ImVec4 type. Change the value to change the color of GUI.
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
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


  return EXIT_SUCCESS;
}
