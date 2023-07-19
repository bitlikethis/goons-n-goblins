#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// consts
typedef const char CHAR;

// utils
void* Malloc(size_t size) {
  void* ptr = malloc(size);
  if (ptr == NULL) {
    fprintf(stderr, "out of memory");
    exit(1);
  }
  return ptr;
}

void Free(void* ptr) {
  free(ptr);
  ptr = NULL;
}

FILE* Fopen(CHAR* filename, CHAR* mode) {
  FILE* fp = fopen(filename, mode);

  if (fp == NULL) {
    fprintf(stderr, "failed to open");
    exit(1);
  }
  return fp;
}

// app
typedef struct Row {
  size_t len;
  char* ptr;
} Row;
typedef const Row ROW;

Row Row_from_str(char* str) {
  size_t len = strlen(str);
  char* ptr = Malloc(len * sizeof(char));

  memcpy(ptr, str, len);

  Row new = {
      .len = len,
      .ptr = ptr,
  };
  return new;
}

Row Row_from_line(char* line) {
  size_t len = strlen(line);
  line[len - 1] = '\0'; // remove newline
  char* ptr = Malloc(len * sizeof(char));

  memcpy(ptr, line, len);

  Row new = {
      .len = len,
      .ptr = ptr,
  };
  return new;
}

// this could be consumed in rust
// by not passing a reference
void Row_free(Row* self) {
  free(self->ptr);
  self->ptr = NULL;
}

// add newline to print
void Row_print(ROW* self) {
  char printer[self->len + 1];
  memcpy(printer, self->ptr, self->len);
  printer[self->len] = '\0';

  printf("%s\n", printer);
}

typedef struct Board {
  size_t width;
  size_t height;
  Row* rows;
} Board;
typedef const Board BOARD;

Board Board_from_file(CHAR* filename) {
  FILE* fp = Fopen(filename, "r");

  char line[256];
  Row rows[256];
  size_t width = 0;
  size_t height = 0;

  while (fgets(line, sizeof(line), fp)) {
    Row row = Row_from_line(line);
    rows[height] = row;
    height++;
  }

  fclose(fp);

  Board new = {
      .width = width,
      .height = height,
      .rows = rows,
  };
  return new;
}

void Board_print(BOARD* self) {
  for (size_t i = 0; i < self->height; i++) {
    Row_print(&self->rows[i]);
  }
}

 void Board_free(Board* self) {
  for (size_t i = 0; i < self->height; i++) {
    Row_free(&self->rows[i]);
  }
}

int main(void) {

  Board board = Board_from_file("src/map1.txt");
  Board_print(&board);
  Board_free(&board);

  return EXIT_SUCCESS;
}
