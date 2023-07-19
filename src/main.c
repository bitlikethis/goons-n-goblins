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
  char* cols;
} Row;
typedef const Row ROW;

Row Row_from_str(char* str) {
  size_t len = strlen(str);
  char* cols = Malloc(len * sizeof(char));

  memcpy(cols, str, len);

  Row new = {
      .len = len,
      .cols = cols,
  };
  return new;
}

Row Row_from_line(char* line) {
  size_t len = strlen(line) - 1; // remove newline
  line[len] = '\0';
  char* cols = Malloc(len * sizeof(char));

  memcpy(cols, line, len);

  Row new = {
      .len = len,
      .cols = cols,
  };
  return new;
}

// this could be consumed in rust
// by not passing a reference
void Row_free(Row* self) {
  free(self->cols);
  self->cols = NULL;
}

// add newline to print
void Row_print(ROW* self) {
  char printer[self->len + 1];
  memcpy(printer, self->cols, self->len);
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
    width = row.len > width ? row.len : width;
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

void Board_stats(BOARD* self) {
  printf("width: %lu, height: %lu", self->width, self->height);
}

void Board_free(Board* self) {
  for (size_t i = 0; i < self->height; i++) {
    Row_free(&self->rows[i]);
  }
}

int main(void) {

  Board board = Board_from_file("src/map1.txt");
  Board_print(&board);
  Board_stats(&board);
  Board_free(&board);

  return EXIT_SUCCESS;
}
