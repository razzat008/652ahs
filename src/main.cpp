#include "../libs/config.h" // sourcing all the libraries
#include <iostream>
#include <stdio.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


class SHA256 {
public:
  SHA256();
  void update(const std::string &data);
  std::string digest();

private:
  static constexpr size_t BlockSize = 64; // 512 bits
  static constexpr size_t HashValues = 8; // 256 bits (8 * 32-bit words)
  void processBlock(const uint8_t block[BlockSize]);
  void finalize();
  static uint32_t rotr(uint32_t x, uint32_t n);
  static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);
  static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);
  static uint32_t sigma0(uint32_t x);
  static uint32_t sigma1(uint32_t x);
  static uint32_t gamma0(uint32_t x);
  static uint32_t gamma1(uint32_t x);
  void padMessage();

  std::vector<uint8_t> dataBuffer;
  uint64_t bitLength;
  uint32_t h[HashValues];
};

SHA256::SHA256() {
  // Initialize hash values
  h[0] = 0x6a09e667;
  h[1] = 0xbb67ae85;
  h[2] = 0x3c6ef372;
  h[3] = 0xa54ff53a;
  h[4] = 0x510e527f;
  h[5] = 0x9b05688c;
  h[6] = 0x1f83d9ab;
  h[7] = 0x5be0cd19;
  bitLength = 0;
}

void SHA256::update(const std::string &data) {
  dataBuffer.insert(dataBuffer.end(), data.begin(), data.end());
  bitLength += data.size() * 8;
  while (dataBuffer.size() >= BlockSize) {
    processBlock(dataBuffer.data());
    dataBuffer.erase(dataBuffer.begin(), dataBuffer.begin() + BlockSize);
  }
}

void SHA256::processBlock(const uint8_t block[BlockSize]) {
  static const uint32_t k[64] = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
      0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
      0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
      0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
      0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
      0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

  uint32_t w[64];
  for (size_t i = 0; i < 16; ++i) {
    w[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) |
           (block[i * 4 + 2] << 8) | (block[i * 4 + 3]);
  }
  for (size_t i = 16; i < 64; ++i) {
    w[i] = gamma1(w[i - 2]) + w[i - 7] + gamma0(w[i - 15]) + w[i - 16];
  }

  uint32_t a = h[0];
  uint32_t b = h[1];
  uint32_t c = h[2];
  uint32_t d = h[3];
  uint32_t e = h[4];
  uint32_t f = h[5];
  uint32_t g = h[6];
  uint32_t h_ = h[7];

  for (size_t i = 0; i < 64; ++i) {
    uint32_t T1 = h_ + sigma1(e) + choose(e, f, g) + k[i] + w[i];
    uint32_t T2 = sigma0(a) + majority(a, b, c);
    h_ = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
  }

  h[0] += a;
  h[1] += b;
  h[2] += c;
  h[3] += d;
  h[4] += e;
  h[5] += f;
  h[6] += g;
  h[7] += h_;
}

void SHA256::finalize() {
  padMessage();
  if (!dataBuffer.empty()) {
    processBlock(dataBuffer.data());
  }
}

void SHA256::padMessage() {
  size_t originalSize = dataBuffer.size();
  dataBuffer.push_back(0x80);
  while ((dataBuffer.size() % BlockSize) != 56) {
    dataBuffer.push_back(0x00);
  }
  uint64_t bitLengthBigEndian = bitLength;
  for (int i = 7; i >= 0; --i) {
    dataBuffer.push_back(static_cast<uint8_t>(bitLengthBigEndian >> (i * 8)));
  }
  while (dataBuffer.size() >= BlockSize) {
    processBlock(dataBuffer.data());
    dataBuffer.erase(dataBuffer.begin(), dataBuffer.begin() + BlockSize);
  }
}

std::string SHA256::digest() {
  finalize();
  std::ostringstream result;
  for (size_t i = 0; i < HashValues; ++i) {
    result << std::hex << std::setfill('0') << std::setw(8) << h[i];
  }
  return result.str();
}

uint32_t SHA256::rotr(uint32_t x, uint32_t n) {
  return (x >> n) | (x << (32 - n));
}

uint32_t SHA256::choose(uint32_t e, uint32_t f, uint32_t g) {
  return (e & f) ^ (~e & g);
}

uint32_t SHA256::majority(uint32_t a, uint32_t b, uint32_t c) {
  return (a & b) ^ (a & c) ^ (b & c);
}

uint32_t SHA256::sigma0(uint32_t x) {
  return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

uint32_t SHA256::sigma1(uint32_t x) {
  return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

uint32_t SHA256::gamma0(uint32_t x) {
  return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

uint32_t SHA256::gamma1(uint32_t x) {
  return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
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
  sqlite3 *db;
  int dbOpen = sqlite3_open("hashfile_db", &db);
  if (!dbOpen) {
    showerror<const char *, int>(__FILE__, __LINE__);
    std::cerr << " Failed opening database file!!" << std::endl;
  }

  SHA256 sha256;
  std::string input;
  std::string file;
  char chk;

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

      // Suraj - I have commented this part we need to work together to implement via the GUI instead.

      // std::cin >> chk;

      // switch (chk) {

      // case 't': {

      //   std::cout << " Enter the text to hash: ";
      //   std::cin.ignore();
      //   std::getline(std::cin, input);
      //   break;
      // }

      // case 'f': {

      //   std::cout << " Enter the file to hash: ";
      //   std::cin >> file;

      //   std::ifstream inputFile(file, std::ios::binary);
      //   if (!inputFile) {
      //     showerror<const char *, int>(__FILE__, __LINE__);
      //     std::cerr << " Error opening file: " << file << std::endl;
      //     return EXIT_FAILURE;
      //   }

      //   std::stringstream buffer;
      //   buffer << inputFile.rdbuf();
      //   input = buffer.str();

      //   inputFile.close();
      //   break;
      // }

      // default:

      //   std::cerr << " Invalid Selection!!!";
      //   return EXIT_FAILURE;
      // }

      //Below are the buttons that we can use to select whether to has files or the text. More work needed on this to implement logic.
      if(ImGui::Button("File", ImVec2(50, 70))){
        display = true;
      }
      if(ImGui::Button("Text", ImVec2(50, 70))){
        display = true;
      }

      if(display){
        ImGui::Text("button pressed!!!");    
      }

      //Suraj - Again I have commented the code below. We need more discussion on this for the integration of GUI into the logic.

      // sha256.update(input);
      // std::string hash = sha256.digest();

      // std::cout << " SHA-256 hash: " << hash << std::endl;
    
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
