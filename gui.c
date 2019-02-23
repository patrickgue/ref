#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "ref.h"
#include "gui.h"
#include "util.h"
#include "log.h"

#define MAX_WIDTH 160

int menu(char** options_orig, int options_count, char *exit_title) {
  clear();
  int ch, selection = 0, i;
  char **options = (char**) malloc((1 + options_count) * sizeof(char) * MENU_TITLE_LEN);
  for(i = 0; i < options_count; i++) {
    options[i] = options_orig[i];
  }

  options_count++;
  options[options_count-1] = exit_title;
  while(true) {
    int i; for(i = 0; i < options_count; i++) {
      if(selection == i)
	attron(A_REVERSE);
      mvprintw(i+1,2,"%s\n", options[i]);
      if(selection == i)
	attroff(A_REVERSE);
    }

    refresh();
    ch = getch();

    if(ch == KEY_DOWN && selection < options_count - 1) {
      selection++;

    }
    else if(ch == KEY_UP && selection > 0) {
      selection--;
    }
    else if(ch == 10) {
      break;
    }
  }
  return selection;
}


int choose_text() {
  info("Enter choose_text");
  ref_text_list list = load_text_list();
  int i;
  char** text_list = malloc(64 * list.entries_count * sizeof(char));
  info("Found following texts");
  for(i = 0; i < list.entries_count; i++) {
    text_list[i] = list.entries[i].title;
    info(list.entries[i].title);
  }
  info("Exit coose_text");
  
  return menu(text_list, i, MENU_BACK);
}


void add_text() {
  clear();

  char title[64];
  char* path[64];
  int c = 0;
  bool accept = false;

  echo();
  mvprintw(1,1,"Title:");
  mvprintw(2,1,"Path:");
  mvscanw(1,10,"%[^\n]s", title);
  mvscanw(2,10,"%[^\n]s", path);
  noecho();

  mvprintw(5, 1, "Add Text to ref?");
  while(c != 10 && c != 13) {
    if(accept)
      attron(A_REVERSE);

    mvprintw(6,1,"Add");
    if(accept)
      attroff(A_REVERSE);

    if(!accept)
      attron(A_REVERSE);

    mvprintw(6,5,"Cancel");
    if(!accept)
      attroff(A_REVERSE);

    c = getch();

    if(c == KEY_LEFT || c == KEY_RIGHT) {
      accept = accept == false;
    }

  }
  if(accept != false) {
    clear();
    getch();
  } 
}


void ref_text_view(char* text) {
  clear();
  int ch,
    i = 0,
    position =  0,
    word_position = 0,
    word_position_select = 0,
    word_position_last = 0;
  char *sep = " ";
  char *word, *brkt, *text_a;
  bool ref_mode = false;
  text_a = (char*) malloc(sizeof(char) * (strlen(text) + 1)); 
  strcpy(text_a, text);
  while(true) {
    clear();
    int current_line = 0, line_len = 2;
    word_position = 0;
    char* line = (char*) malloc((COLS+1) * sizeof(char));
    sprintf(line, "  ");
    for (word = strtok_r(text_a, sep, &brkt); word; word = strtok_r(NULL, sep, &brkt)) {
      
      if(line_len + strlen(word) > (COLS > MAX_WIDTH ? MAX_WIDTH : COLS) - 5 || contains_char(word, '\\')) {
	if(
	   (current_line < position  && word_position_select < word_position) ||
	   word_position_select > word_position_last) {
	    word_position_select = word_position;
	}
	if(current_line >= position) {
	 
	  mvprintw(current_line - position, 0, "%3i: ", current_line);
	  int ch = -1, chp = 0;
	  bool attr = false;

	  while(ch != 0) {
	    ch = line[chp];
	    if(ch == '#') {
	      if(!attr) {
		attron(A_REVERSE);
	      }
	      else {
		attroff(A_REVERSE);
	      }
	      attr = attr != true;
	      }
	    // TODO: find cleaner solution
	    else if(ch != 0) {
	      printw("%c",ch);
	    }
	    chp++;
	  }
	}
	
	if(contains_char(word, '\\')) {
	  line_len = 2;
	  sprintf(line,"  ");
	}
	else {
	  line_len = 0;
	  sprintf(line,"");
	}
	current_line += (contains_char(word, '\\')?1:1);
      }

      char* word_cpy = malloc(sizeof(char) + (strlen(word) + 1));
      strcpy(word_cpy, word);
      
      if(word_position_select == word_position && ref_mode == true) {
	word_cpy = wrap_word(word_cpy, "#");
      }
      
      sprintf(line, "%s%s ", line, replace_word(word_cpy, "\\", ""));
      line_len += strlen(word) + 1;
      word_position++;
      if(current_line - position >= LINES - 1) {
	break;
      }
    }
    word_position_last = word_position;
    brkt =  NULL, word = NULL;
    attron(A_REVERSE);
    mvprintw(LINES-1,0,"[v] down [^] up [r] toggle reference mode [e/q] exit text view [?] help | %i / %i / %i / %i / %i", position, i, current_line, word_position, word_position_select);
    attroff(A_REVERSE);
    refresh();
    ch = getch();
    if(ch == 'q' || ch == 'e')
      break;
    else if(ch == 'r')
      ref_mode = !ref_mode;
    else if(ch == 'v')
      position += LINES;
    else if(ch == 'b') {
      position -= LINES;
      if(position < 0) {
	position = 0;
      }
    }
    if(ref_mode) {
      if(ch == KEY_RIGHT) {
	word_position_select++;
      }
      else if(ch == KEY_LEFT && word_position > 0) {
	word_position_select--;
      }
      else if(ch == ' ') {
	//ref_selection();
      }
    }
    else {
      if(ch == KEY_DOWN) {
	position++;
      }
      else if(ch == KEY_UP) {
	if(position > 0) {
	  position--;
	}
      }
    }
    strcpy(text_a, text);
    i++;
  }
}
