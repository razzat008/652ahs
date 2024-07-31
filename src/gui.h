#ifndef GUI_H
#define GUI_H

void runGUI();
void runMainWindow(bool* dark_mode);
void ToggleButton(const char* str_id, bool* v);
void SetupImGuiStyle(bool* dark_mode);

#endif // GUI_H
