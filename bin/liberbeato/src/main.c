#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sysk/vtesc.h>

#define BASE "/home/kalekale/data/books/novels/western"

/*
book filename format
"author: title.ext"
"author1, author2: title.ext"
*/

struct book {
  char author[128];
  char title[128];
};
  
int main(int argc, char *argv[]) {
  DIR *d;
  struct dirent *dir;
  const char sc[2] = ":";
  const char dt[2] = ".";
  const char cm[2] = ",";
  char *token;
  char *bdir;
  
  if (argc==2) {
    bdir = argv[1];
  }
  else {
    bdir = BASE;
  }
  
  struct book list[32];
  int f=0;
  d = opendir(bdir);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if(dir->d_type!=DT_DIR) {
	token = strtok(dir->d_name, sc);
	while( token != NULL) {
	  if (f%2==0) {
	    strcpy(list[f/2].author, token);
	  }
	  else {
	    strcpy(list[f/2].title, token);
	  }
	  f+=1;
	  token = strtok(NULL, sc);
	}
      }
    }
    closedir(d);
  }

  printf(WHT BLUB"LiberBeato: %s"RST"\n", bdir);
  f=f/2;
  int j = 1;
  for(int i = 0; i<f; i++) {
    printf(GRN"%d." YEL"%s"RST " by "CYN "%s\n"RST, j, list[i].title, list[i].author);
    j++;
  }
  return 0;
}
