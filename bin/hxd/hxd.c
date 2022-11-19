/*
    hxd: hex dump utility
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
#include <stdbool.h>
#include <stdint.h>


// TODO: this should be in <sysk/vtesc.h>
// terminal escape codes
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define RST "\e[0m"
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"

void dump_file(FILE* file, int offset, int bytes_to_read, int line_length);
void print_line(uint8_t* buffer, int num_bytes, int offset, int line_length);
void help();

int main(int argc, char **argv) {
 
  int offset = 0;
  int bytes_to_read = -1;
  int line_length = 16;

  FILE *file = NULL;
  if (argc>1) {
    for (int i = 1; i<argc; i++) {
      file = fopen(argv[i], "rb");
      if (file == NULL) {
	fprintf(stdout, RED "error:" RST " cannot open the file");
	exit(1);
      }
      printf(WHT BLUB"DUMP FILE: %s"RST"\n", argv[i]);
      dump_file(file, offset, bytes_to_read, line_length);
      fclose(file);
    }
   }
  else {
    help();
  }
  return 0;
}

void print_line(uint8_t* buffer, int num_bytes, int offset, int line_length) {
    printf(GRN "%00006X: " RST, offset);

    for (int i = 0; i < line_length; i+=2) {
        if (i < num_bytes) {
	  printf(CYN BLKB"%02X%02X" RST " ", buffer[i+1], buffer[i]);
        } else {
	  printf(BLK WHTB"NULL" RST " ");
	}
    }
    
    printf("\t");
    for (int i = 0; i < num_bytes; i++) {
      if (buffer[i] > 31 && buffer[i] < 127) {
	printf(BLU WHTB"%c" RST, buffer[i]);
      } else {
	printf(CYN WHTB"." RST);
      }
    }
    printf("\n");
}                   

void dump_file(FILE* file, int offset, int bytes_to_read, int line_length) {
  uint8_t* buffer = (uint8_t*)calloc(0, line_length);
  if (buffer == NULL) {
    fprintf(stderr, "calloc failed.\n");
    exit(1);
  }
  
  while (true) {
    int num_bytes = fread(buffer, sizeof(uint8_t), line_length, file);
    if (num_bytes > 0) {
      print_line(buffer, num_bytes, offset, line_length);
      offset += num_bytes;
      bytes_to_read -= num_bytes;
    } else {
      break;
    }
  }
  
  free(buffer);
}

void help() {
  printf("hxd: hex dump utility\n");
  printf("hxd [FILE]s\n\n");
  printf("sysk userspace programs <https://gitlab.com/0kalekale/sysk/bin>\n");
  printf("licensed under AGPL3\n");
}
