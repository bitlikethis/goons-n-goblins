# TODO


## Done

[x] feat: read layout from file
    [x] chore: dynamic board size allocation on startup
        - `Board Board_from_file(CHAR* filename)`
        - **limits:** 256 x 256 Board
    [x] chore: remove newline characters from buffer
        - `Row Row_from_line(CHAR* line)`

[x] FIX: not rendering first row
    - always initialize index
