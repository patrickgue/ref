#include <string.h> 
#include <stdlib.h>
#include <stdbool.h>

#include "util.h"


char *replace_word(const char *s, const char *old_w, 
		  const char *new_w) { 
  char *result; 
  int i, cnt = 0; 
  int new_w_len = strlen(new_w); 
  int old_w_len = strlen(old_w); 
  
  for (i = 0; s[i] != '\0'; i++) { 
    if (strstr(&s[i], old_w) == &s[i]) { 
      cnt++; 
  
      i += old_w_len - 1; 
    } 
  } 
  
  result = (char *)malloc(i + cnt * (new_w_len - old_w_len) + 1); 
  
  i = 0; 
  while (*s) { 
    if (strstr(s, old_w) == s) { 
      strcpy(&result[i], new_w); 
      i += new_w_len; 
      s += old_w_len; 
    } 
    else
	result[i++] = *s++; 
  } 
  
  result[i] = '\0'; 
  return result; 
} 

bool contains_char(const char* str, char search) {
  int i, length = strlen(str);
  for(i = 0; i < length; i++) {
    if(str[i] == search) {
      return true;
    }
  }
  return false;
}

