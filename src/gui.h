#ifndef GUI_H
#define GUI_H

void runMainWindow(bool* dark_mode, bool* file_hash_state, bool* is_sha_selected);
void runGUI();
void ToggleButton(const char* str_id, bool* v);
void ToggleFile(const char* str_id, bool* f);
void SetupImGuiStyle(bool* dark_mode);
void ShowDropdownMenu(bool* is_sha_selected);
void dropWindow();


#endif // GUI_H
