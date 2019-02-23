#include <ncurses.h>
#include <stdlib.h>
#include <strings.h>

#include "util.h"
#include "ref.h"
#include "fileio.h"
#include "log.h"
#include "gui.h"

#define DEBUG 1
#define LOG true


char **main_menu_text;
int main_menu_len;

int main(int argc, char** argv) {
  int ch, nrtitles = 4;

  init_file_log(NULL);
  
  init();
  
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  while(true) {
    int choice = menu(main_menu_text, main_menu_len, MENU_EXIT);
    if(choice == main_menu_len) {
      break;
    }
    else if(choice == 0) {
      int text = choose_text();
      ref_text_list list = load_text_list();
      if(text != list.entries_count) {
	info(list.entries[text].path);
	ref_text_view(read_file(list.entries[text].path));
      }
    }
    else if(choice == 1) {
      add_text();
    }
  }

  endwin();
  return 0;
}


void init() {
  ref_text_list list = load_text_list();
  main_menu_len = 5;
  main_menu_text = (char**) malloc(sizeof(char) * main_menu_len * MENU_TITLE_LEN);
  main_menu_text[0] = "Texts";
  main_menu_text[1] = "Add Text";
  main_menu_text[2] = "Remove Text";
  main_menu_text[3] = "References";
  main_menu_text[4] = "Help";
}


ref_refset load_refs() {
  char path[128] = REF_DATA_LOCATION;
  strcat(path, "ref");
  char* raw_string = read_file(path);

  char *word, *line, *brkt, *brkt2;
  const char *sep = ":", *sep2 = "\n";


  ref_refset set = {NULL, 0};
  set.references = malloc(0);
  
  for(line = strtok_r(raw_string, sep2, &brkt2); line; line = strtok_r(NULL, sep2, &brkt2)) {
    ref_reference *reference = malloc(sizeof(ref_text_entry));
    
    word = strtok_r(line, sep, &brkt);
    reference->id = atoi(word);
  
    word = strtok_r(NULL, sep, &brkt);
    reference->from_id = atoi(word);

    word = strtok_r(NULL, sep, &brkt);
    reference->from_word = atoi(word);

    word = strtok_r(NULL, sep, &brkt);
    reference->to_id = atoi(word);

    word = strtok_r(NULL, sep, &brkt);
    reference->to_word = atoi(word);

    word = strtok_r(NULL, sep, &brkt);
    strcpy(reference->comment, word);

    set.references_count++;
    set.references = realloc(set.references, set.references_count * sizeof(ref_reference));
    set.references[set.references_count-1] = *reference;
  }
  return set;
}

ref_text_list load_text_list() {
  char path[128] = REF_DATA_LOCATION;
  strcat(path, "text");
  char* raw_string = read_file(path);

  char *word, *line, *brkt, *brkt2;
  const char *sep = ":", *sep2 = "\n";

  ref_text_list list = {NULL,0};
  list.entries = malloc(0);

  for(line = strtok_r(raw_string, sep2, &brkt2); line; line = strtok_r(NULL, sep2, &brkt2)) {
    ref_text_entry *entry = malloc(sizeof(ref_text_entry));
    
    word = strtok_r(line, sep, &brkt);
    entry->id = atoi(word);
  
    word = strtok_r(NULL, sep, &brkt);
    strcpy(entry->title, word);

    word = strtok_r(NULL, sep, &brkt);
    strcpy(entry->path, word);

    list.entries_count++;
    list.entries = realloc(list.entries, list.entries_count * sizeof(ref_text_entry));
    list.entries[list.entries_count-1] = *entry;
  }
  return list;
}

