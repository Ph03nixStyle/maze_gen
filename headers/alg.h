
#ifndef ALG_H
#define ALG_H

#define BLACK "\x1b[30m"
#define WHITE "\x1b[37m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define BLUE "\x1B[34m"
#define YELLOW "\x1B[33m"
#define PURPLE "\x1B[35m"
#define RESET "\x1b[0m"

#define MOV_CUR_LEFT(x)  printf("\x1B[%dD", x);
#define MOV_CUR_UP(x)    printf("\x1B[%dA", x);


typedef struct Cell Cell;
typedef struct Maze Maze;

struct Cell {
    bool broken;
    bool visited;
    int type;
    int i;
    int j;
    Cell* right;
    Cell* top;
    Cell* left;
    Cell* bottom;
};

struct Maze {
    Cell*** cells; //Tableau à 2 dimension de pointeurs sur Cell
    int n;
    int m;
};

void alg(int n, int m);

void initMaze(Maze** maze, int n, int m);
void initCell(Maze* maze, Cell** cell, int i, int j);
void createMaze(Maze maze);
void freeMaze(Maze* maze);

void printMaze(Maze maze, int i, int j);

void _breakCell(Maze maze, Cell* cell, Cell* last_wall);
void _fixMazeSize(int* n, int* m);
void _shuffle(int *array, int n);
bool _isDirectionPossible(Cell* cell, int n);
int _chooseDirection(Maze maze, Cell* cell);


#endif