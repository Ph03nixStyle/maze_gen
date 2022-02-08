#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include "headers/main.h"
#include "headers/alg.h"


void alg(int n, int m) {
    Maze* maze;
    initMaze(&maze, n, m);
    createMaze(*maze);
    printMaze(*maze, -1, -1);
    freeMaze(maze);
}


void createMaze(Maze maze) {

    _breakCell(maze, maze.cells[1][1], maze.cells[1][0]);
    maze.cells[maze.n - 2][maze.m - 1]->visited = true;
}


void initMaze(Maze** maze, int n, int m) {
    _fixMazeSize(&n, &m);
    (*maze) = malloc(sizeof(Maze));
    (*maze)->n = n;
    (*maze)->m = m;
    (*maze)->cells = malloc(n * sizeof(Cell));
    for (int i=0; i<n; i++) {
        (*maze)->cells[i] = (Cell**)malloc(m * sizeof(Cell*));
    }
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            Cell** cell = &((*maze)->cells[i][j]);
            (*cell) = malloc(sizeof(Cell));
            initCell((*maze), cell, i, j);
        }
    }
    (*maze)->cells[1][0]->broken = true;
    (*maze)->cells[1][1]->broken = true;
    (*maze)->cells[n-2][m-1]->broken = true;
}


void initCell(Maze* maze, Cell** cell, int i, int j) {
    // TYPE: 0=unbreakable, 1=open, 2=walls
    (*cell)->i = i;
    (*cell)->j = j;
    (*cell)->broken = false;
    (*cell)->visited = false;
    (*cell)->right = NULL;
    (*cell)->top = NULL;
    (*cell)->left = NULL;
    (*cell)->bottom = NULL;

    if ((i % 2 == 0 && j % 2 == 0) || i == 0 || j == 0 || i == maze->n - 1 || j == maze->m - 1) {   // 0: Unbreakable
        (*cell)->type = 0;
    }
    else if (i % 2 == 1 && j % 2 == 1) {                                                            // 1: Open
        (*cell)->type = 1;

        if (i != 1) {
            (*cell)->top = maze->cells[i-2][j];
            maze->cells[i-2][j]->bottom = (*cell);
        }
        if (j != 1) {
            (*cell)->left   = maze->cells[i][j-2];
            maze->cells[i][j-2]->right = (*cell);
        }
    }
    else {                                                                                          // 2: Walls
        (*cell)->type = 2;
    }
}


void freeMaze(Maze* maze) {
    for (int i=0; i < maze->n; i++) {
        for (int j=0; j < maze->m; j++) {
            free(maze->cells[i][j]); // Free chaque cell
        }
        if (i < maze->n-1) free(maze->cells[i]); // Free chaque ligne
    }
    free(maze); // Free le maze
}


void printMaze(Maze maze, int i, int j) {
    static bool drawn = false;

    if (!drawn) drawn = true;
    else {
        MOV_CUR_LEFT(maze.n);
        MOV_CUR_UP(maze.m);
    }

    for (int y=0; y<maze.n; y++) {
        for (int x=0; x<maze.m; x++) {
            if (maze.cells[y][x]->broken) {
                if (y == i && x == j) {
                    printf(COLOR_PLAYER "██" RESET);
                }
                else if (maze.cells[y][x]->visited) {
                    printf(COLOR_VISITED "██" RESET);
                }
                else {
                    printf(COLOR_PATH "██" RESET);
                }
            }
            else {
                printf(COLOR_BG "██" RESET);
            }
        }
        printf("\n");
    }
    printf("\n");
    SLEEP;
}


void _breakCell(Maze maze, Cell* cell, Cell* last_wall) {
    printMaze(maze, cell->i, cell->j);
    int choice = 0;

    for (int x = 0; x<4; x++) {
        choice = _chooseDirection(maze, cell);

        if (choice == 0) {
            maze.cells[cell->i][cell->j+1]->broken = true;
            cell->right->broken = true;
            _breakCell(maze, cell->right, maze.cells[cell->i][cell->j+1]);
        }
        else if (choice == 1) {
            maze.cells[cell->i-1][cell->j]->broken = true;
            cell->top->broken = true;
            _breakCell(maze, cell->top, maze.cells[cell->i-1][cell->j]);
        }
        else if (choice == 2) {
            maze.cells[cell->i][cell->j-1]->broken = true;
            cell->left->broken = true;
            _breakCell(maze, cell->left, maze.cells[cell->i][cell->j-1]);
        }
        else if (choice == 3) {
            maze.cells[cell->i+1][cell->j]->broken = true;
            cell->bottom->broken = true;
            _breakCell(maze, cell->bottom, maze.cells[cell->i+1][cell->j]);
        }
    }
    printMaze(maze, cell->i, cell->j);
    cell->visited = true;
    last_wall->visited = true;
}


void _fixMazeSize(int* n, int* m) {
    *n = ((*n)/2)*2 + 1;
    *m = ((*m)/2)*2 + 1;
}


void _shuffle(int *array, int n) {
    if (n > 1) {
        int j;
        int t;
        for (int i=0; i < n-1; i++) {
          j = i + rand() / (RAND_MAX / (n - i) + 1);
          t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}


bool _isDirectionPossible(Cell* cell, int n) {
    if (n == 0) return (cell->right  != NULL && !cell->right->broken);
    if (n == 1) return (cell->top    != NULL && !cell->top->broken);
    if (n == 2) return (cell->left   != NULL && !cell->left->broken);
    if (n == 3) return (cell->bottom != NULL && !cell->bottom->broken);
    return 4;
}


int _chooseDirection(Maze maze, Cell* cell) {
    int liste[4] = {0, 1, 2, 3};
    _shuffle(liste, 4);

    for (int i=0; i<4; i++) {
        if (_isDirectionPossible(cell, liste[i])) return liste[i]; 
    }
    return 4;
}