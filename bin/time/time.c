#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void help() {
  printf(
	 "time\n"
	 );
  exit(1);
}

int main(int argc, char *argv[]) {

  if (argc < 1) {
    help();
  } 

  pid_t child;
  clock_t timer = clock();
  int status;
  
  child = fork();
  if (child == 0) {
    execv(argv[1], argv); // FIXME
  }

  if(waitpid(child, &status, 0) > 0) {
    printf("%d", timer / CLOCKS_PER_SEC);
  }
  
  return 0;
}
