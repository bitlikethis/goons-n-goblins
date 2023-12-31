#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned int uint;

void* oom_guard(void* ptr) {
  if (ptr == NULL) {
    fprintf(stderr, "out of memory");
    exit(1);
  }
  return ptr;
}
void* fopen_guard(void* ptr) {
  if (ptr == NULL) {
    fprintf(stderr, "failed to open");
    exit(1);
  }
  return ptr;
}


typedef struct Row {
  size_t len;
  char* ptr;
} Row;
typedef const Row ROW;

Row Row_from_str(char str[]) {
  size_t len = strlen(str);
  char* ptr = oom_guard(malloc(len * sizeof(char)));

  memcpy(ptr, str, len);

  Row new = {
      .len = len,
      .ptr = ptr,
  };
  return new;
}

void Row_free(Row* self) {
  free(self->ptr);
  self->ptr = NULL;
}

void Row_print(ROW* self) {
  char printer[self->len + 1];
  memcpy(printer, self->ptr, self->len);
  printer[self->len] = '\0';

  printf("%s\n", printer);
}

typedef struct Mask {
  uint ref_row;
  uint ref_col;
  uint rows;
  uint cols;
  char** mask;
} Mask;

typedef struct Board {
  uint rows;
  uint cols;
  char layout[8][8];
} Board;
typedef const Board BOARD;

Board Board_new() {
  Board new = {
      .rows = 8,
      .cols = 8,
      .layout =
          {
              "xx..o.oo",
              "xx..x.oo",
              "xx..o.oo",
              "xx....oo",
              "xx....oo",
              "xx....oo",
              "xx....oo",
              "xx....oo",
          },
  };
  return new;
}

char Board_at(BOARD* self, uint row, uint col) {
  return self->layout[row][col];
}

void Board_move_mob_to(Board* self, uint mob_row, uint mob_col, uint to_row,
                       uint to_col) {
  self->layout[to_row][to_col] = Board_at(self, mob_row, mob_col);
  self->layout[mob_row][mob_col] = '.';
}

bool Board_compare_mask(BOARD* b, uint row, uint col, Mask* m) {
  // if (Board_at(b, row, uint col)) {
  // }
  return false;
}

// NOTE: everything should be done as row column
void Board_move_down_right(Board* self, uint mob_row, uint mob_col, int down,
                           int right) {
  self->layout[mob_row + down][mob_col + right] =
      Board_at(self, mob_row, mob_col);
  self->layout[mob_row][mob_col] = '.';
}

void Board_render_row(BOARD* self, uint row) {
  size_t padding = 2 * self->cols;
  char rend[(padding) + 1];

  for (int i = 0; i < padding; i++) {
    char even = (i) % 2;
    rend[i] = even ? self->layout[row][i / 2] : ' ';
  }
  rend[padding] = '\0';

  printf("%s\n", rend);
}

void Board_render(BOARD* self) {
  for (uint row = 0; row < self->rows; row++) {
    Board_render_row(self, row);
  }
}

typedef struct Game {
  uint turn;
} Game;
typedef const Game GAME;

// changes board based on rules
// handles logic to take turns
Game Game_new(BOARD* b) {
  // 1. board has placed pieces
  // 1. define rules and conditions
  // 1. begin game
  Game new = {
      .turn = 0,
  };
  return new;
}

void Game_welome() {
  printf("  goons and goblins\npress enter to begin\n");
  char start;
  scanf("%c", &start); // enter is 10
}

void Game_take_turn(Game* self, Board* b, char action) {
  // if (action = 'h') {
  //   Board_move_mob_by(b, 0, 0, 0, 1);
  // } else if (action = 'j') {
  //   Board_move_mob_by(b, 0, 0, 0, 1);
  // } else if (action = 'k') {
  //
  // } else if (action = 'l') {
  // }

  self->turn++;
}

// run through rules and conditions to take actions
// ". is empty"
// "x is a goon"
// "o is a goblin"
// "x dies if x is surrounded by o"
// "o dies if o is surrounded by x"
void Game_update(Game* self, Board* b) {
  for (uint row = 0; row < b->rows; row++) {
    for (uint col = 0; col < b->cols; col++) {
      char piece = Board_at(b, row, col);

      if (piece == 'x') {
        char surrounded_up_down[3][1] = {
            {"o"},
            {"x"},
            {"o"},
        };

        // check down/right
        // if (row == 0 ||
        //     Board_at(b, row - 1, col) == 'o' && row == b->rows - 1 ||
        //     Board_at(b, row + 1, col) == 'o') {
        //   b->layout[row][col] = '.';
        // }
      }

      // if (piece == 'o') {
      //   // check down/right
      //   if (row == 0 ||
      //       Board_at(b, row - 1, col) == 'x' && row == b->rows - 1 ||
      //       Board_at(b, row + 1, col) == 'x') {
      //     b->layout[row][col] = '.';
      //   }
      // }
    }
  }
}

void Game_render(GAME* self, BOARD* b) {
  system("clear");

  Board_render(b);

  printf("\nturn: %d\n", self->turn);
}

int main(int argc, char* argv[]) {
  system("clear");

  // Game_welome();

  time_t start_time = time(NULL);
  time_t current_time;
  uint seconds = 1;
  uint turnc = 1;
  Board board = Board_new();
  Game game = Game_new(&board);

  Game_render(&game, &board);
  game.turn++;

  // simulate
  do {
    current_time = time(NULL);

    if (current_time - start_time >= seconds) {
      if (game.turn <= turnc) {
        // take turn
        // Board_move_mob_to(&board, 0, game.turn - 1, 0, game.turn);
        // switch (game.turn) {
        // case 1:
        //   Board_move_down_right(&board, 0, 1, 0, 1);
        //   break;
        // case 2:
        //   Board_move_down_right(&board, 1, 1, 0, 1);
        //   break;
        // case 3:
        //   Board_move_down_right(&board, 2, 1, 0, 1);
        //   break;
        // case 4:
        //   Board_move_down_right(&board, 2, 2, 0, 1);
        //   break;
        // default:
        //   break;
        // }
        Game_update(&game, &board);
        Game_render(&game, &board);
      } else {
        break;
      }

      game.turn++;
    }

    start_time = current_time;
  } while (1);

  return EXIT_SUCCESS;
}
