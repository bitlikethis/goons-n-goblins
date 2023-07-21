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

typedef struct Rule {
  size_t inpc;
  char* inp;
  size_t outc;
  char* out;
} Rule;

// app
typedef struct Row {
  size_t len;
  char* cols;
} Row;
typedef const Row ROW;

Row Row_from_str(CHAR* str) {
  size_t len = strlen(str);
  char* cols = Malloc(len * sizeof(char));

  memcpy(cols, str, len);

  Row new = {
      .len = len,
      .cols = cols,
  };
  return new;
}

Row Row_from_line(CHAR* line) {
  size_t len = strlen(line) - 1; // remove newline
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
  Free(self->cols);
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

char* Board_slice_str(Board* self, size_t row_start, size_t row_end,
                      size_t col_start, size_t col_end) {
  if (row_start > self->height || row_end > self->height ||
      col_start > self->width || col_end > self->width) {
    fprintf(stderr, "slice outside of bounds");
    exit(1);
  }
  if (row_start > row_end || col_start > col_end) {
    fprintf(stderr, "slice start is greater than slice end");
    exit(1);
  }
  char* slice =
      Malloc((row_end - row_start + 1) * (col_end - col_start) * sizeof(char));
  size_t slicei = 0;

  for (size_t row = row_start; row < row_end; row++) {
    for (size_t col = col_start; col < col_end; col++) {
      slice[slicei] = self->rows[row].cols[col];
      slicei++;
    }
    slice[slicei] = '\n';
    slicei++;
  }
  slice[slicei] = '\0';

  return slice;
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
