/*
    uname: print system information
    Copyright (C) 2022 0kalekale

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

void help() {
  printf(
	 "uname: print system information\n"
	 "uname [OPTION]...\n"
	 "-a\tprint all information\n"
	 "-s\tkernel name(default)\n"
	 "-n\tnodename/hostname\n"
	 "-r\tkernel release\n"
	 "-v\tkernel version\n"
	 "-m\tmachine/hardware name\n"
	 "-o\toperating system\n"
	 "-h\thelp\n\n"
	 "sysk userspace programs <https://gitlab.com/0kalekale/sysk/bin>\n"
	 "licensed under AGPL3\n"
	 );
  exit(1);
}

// just werks implementation
int main(int argc, char *argv[]) {

  struct utsname name;
  uname(&name);
 
  if (argc == 1) {
    printf("%s\n", name.sysname);
    exit(0);
  }

  for (int i = 0; i < argc; i++) {
    
    if (strcmp(argv[i], "-a") == 0) {
      printf("%s ", name.sysname);
      printf("%s ", name.nodename);
      printf("%s ", name.release);
      printf("%s ", name.version);
      printf("%s\n", name.machine);
      exit(0);
    }
    
    // TODO: figure out how to get OS name instead of printing kernel name
    // TODO: add proper arg support like `uname -srn`, `uname -sr -nv` or `uname --kernel-version`
    
    if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "-o") == 0) {
      printf("%s ", name.sysname);
    }

    if (strcmp(argv[i], "-n") == 0) {
      printf("%s ", name.nodename);
    }

    if (strcmp(argv[i], "-r") == 0) {
      printf("%s ", name.release);
    }
    
    if (strcmp(argv[i], "-v") == 0) {
      printf("%s ", name.version);
    }
          
    if (strcmp(argv[i], "-m") == 0) {
      printf("%s ", name.machine);
    }

    else {
      if (i == argc)
	help(); 
      else
	continue;
    }

  }
  
  printf("\n");
  return 0;
  
}
