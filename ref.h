#ifndef REF_H
#define REF_H

#define MENU_TITLE_LEN 255

#define MENU_BACK "<- Back"
#define MENU_EXIT "(x) Exit"

#define REF_TEXT_VIEW_HELP ""

#define REF_DATA_LOCATION "./test/" // "~/.ref/" in release

struct s_ref_text_entry {
  int id;
  char title[64];
  char path[64];
};

typedef struct s_ref_text_entry ref_text_entry;

struct s_ref_text_list {
  ref_text_entry* entries;
  int entries_count;
};

typedef struct s_ref_text_list ref_text_list;

enum e_ref_type {
		     ref,
		     comment
};

typedef enum e_ref_type ref_type;

struct s_ref_reference {
  int id;
  int from_id;
  int from_word;
  int to_id;
  int to_word;
  char* comment;
};

typedef struct s_ref_reference ref_reference;


struct s_ref_refset {
  ref_reference* references;
  int references_count;
};

typedef struct s_ref_refset ref_refset;
  

void init();
ref_refset load_refs();
ref_text_list load_text_list();


#endif
