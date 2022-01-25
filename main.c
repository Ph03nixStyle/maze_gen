#include <stdio.h>
#include <stdlib.h>
#include "headers/main.h"
#include "alg.c"


int main() {
    srand(time(NULL));
    alg(N, M);
    return 0;
}