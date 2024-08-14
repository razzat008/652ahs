#ifndef CONFIG_H
#define CONFIG_H

#include <bitset>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <sqlite3.h>
#include <filesystem>

// namespace {
// char *file_name = "hashfile_db";
// }

/*  Template to show error for every if conditions
    To use:
    showerror<const char *, int>(__FILE__, __LINE__);
    inside the if conditions*/
template <typename T, typename U> inline void showerror(T file, U line) {

  std::cerr << " FILE: [ " << file << " ] At line: [ " << line - 1
            << " ]\n Error Message:" << std::endl;
}

#endif
