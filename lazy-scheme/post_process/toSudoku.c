#include <stdio.h>
#include <string.h>

#define SZ 512
int size;
void read_input_line_and_convert(int, char*);
FILE* yyin;

int main(int argc, char *argv[]) 
{
    char sudoku[SZ*SZ];    char tmpSudoku[SZ*SZ];
    char row[SZ][SZ];
    fprintf(stderr,
            "Format:\n<size>\n<sudoku as a side-X-side board, 0 as blanks\n"); 

    if (argc < 2) {
        yyin = stdin;
    }
    else {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            fprintf(stderr, "Error opening %s. Using stdin\n", argv[1]);
            yyin = stdin;
        }
    }
    
    (void)fscanf(yyin, "%d", &size);
    int i;
    for (i = 0; i < size; ++i) {
        fgetc(yyin); /* ignore \n*/
        read_input_line_and_convert(size, row[i]);
    }
    sprintf(tmpSudoku, "()");
    for (i = size - 1; i>=0; i--) {
        sprintf(sudoku, "(cons %s\n %s)", row[i], tmpSudoku);
        strcpy(tmpSudoku, sudoku);
    }
    
    fprintf(stdout, "\n(let ((b1 %s))\n     (solve b1))\n\n", sudoku);
    return 0;
}

void read_input_line_and_convert(int col, char *rest) 
{
    if (col == 0) {
        sprintf(rest, "()");
    }
    else {
        char tail[SZ*SZ];
        char num;
        num = fgetc(yyin);
        if (!isdigit(num)) {
            num = num + '0' - 'a' + 10;
        }
        num = num-'0';
        read_input_line_and_convert(col - 1, tail);
        sprintf(rest, "(cons %d %s)", num, tail);
    }
}
