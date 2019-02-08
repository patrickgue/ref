#ifndef REF_H
#define REF_H

#define MENU_TITLE_LEN 255

#define MENU_BACK "<- Back"
#define MENU_EXIT "(x) Exit"

#define REF_TEXT_VIEW_HELP ""

int menu(char* options[32], int nroptions, char* exit_title);
void ref_text_view(char* text);
void init();


#endif
