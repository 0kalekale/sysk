/*
    sleep: sleep for s seconds
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
#include <unistd.h>

void help() {
  printf(
	 "sleep: sleep for s seconds\n"
	 "sleep [s]\n\n"
	 "sysk userspace programs <https://gitlab.com/0kalekale/sysk/bin>\n"
	 "licensed under AGPL3\n"
	 );
  exit(1);
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    help();
  }

  int s = atoi(argv[1]);

  if(!s) {
    help();
  }
  sleep(s);
  
  return 0;
} 
