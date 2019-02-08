#include <ncurses.h>
#include <stdlib.h>
#include <strings.h>

#include "util.h"
#include "ref.h"
#include "fileio.h"

#define DEBUG 1

#define MAX_WIDTH 160

char **main_menu_text;
int main_menu_len;

int main(void) {
  int ch, nrtitles = 4;

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
      ref_text_view(read_file("test/file"));
    }
    
  }


    

  endwin();
  return 0;
}




void init() {
  main_menu_len = 2;
  main_menu_text = (char**) malloc(sizeof(char) * main_menu_len * MENU_TITLE_LEN);
  main_menu_text[0] = "Text";
  main_menu_text[1] = "References";
}



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

      /* copy of 'char* word' needed because of weird behaviour of strtok */
      char* word_cpy = malloc(sizeof(char) + (strlen(word) + 1));
      strcpy(word_cpy, word);
      
      if(word_position_select == word_position) {
	/* find better solution for sprintf issue */
	char* a = malloc(sizeof(char) + (strlen(word) + 3));
	strcpy(a, "#");
	strcat(a, word_cpy);
	strcpy(word_cpy, a);
	strcat(word_cpy, "#");
	//sprintf(word_cpy, "#%s#", word_cpy);
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
    if(DEBUG) {
      mvprintw(LINES-1,0,"[v] down [^] up [r] toggle reference mode [e/q] exit text view [?] help | %i / %i / %i / %i / %i", position, i, current_line, word_position, word_position_select);
    }
    else {
      mvprintw(LINES-1,0,"[v] down [^] up [r] toggle reference mode [e/q] exit text view [?] help");;
    }
    attroff(A_REVERSE);
    refresh();
    ch = getch();
    if(ch == 'q' || ch == 'e') {
      break;
    }
    else if(ch == KEY_DOWN) {
      position++;
    }
    else if(ch == KEY_UP) {
      if(position > 0) {
	position--;
      }
    }
    else if(ch == 'v') {
      position += LINES;
    }
    else if(ch == 'b') {
      position -= LINES;
      if(position < 0) {
	position = 0;
      }
    }
    else if(ch == 'o')
      word_position_select++;
    else if(ch == 'p')
      word_position_select--;
    
    strcpy(text_a, text);
    i++;
  }
  

}
