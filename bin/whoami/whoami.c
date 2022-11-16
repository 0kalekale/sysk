/*
    whoami: print uid
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
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main() {

  struct passwd *p;
  uid_t id;

  id = geteuid();
  p = getpwuid(id);
  if(p==NULL) {
    return 127;
  }
  else {
    printf("%s\n", p->pw_name);
    return 0;
  }
  
}
