#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <string>
#include <iostream>

#include "board.h"
#include "solver.h"
#include "solvestate.h"

int main(int argc, char **argv)
{
    Board board;

    srand(time(NULL));

    // run test
    if(argc == 2 && strcmp(argv[1], "TEST") == 0) {
        int n_tests = 100;
        double moves = 0;
        double combos = 0;
        double cascades = 0;

        for(int n = 0; n < n_tests; n++) {
            printf("\rTesting: %d/%d complete", n, n_tests);
            fflush(stdout);
            board.loadRandom();
            SolveState solution = solveBoard(board);
            moves += solution.moves.size();
            combos += solution.board.combos;
            cascades += solution.board.cascades;
        }
        
        printf("\rAverage moves:    %f\n", moves / n_tests);
        printf("Average combos:   %f\n", combos / n_tests);
        printf("Average cascades: %f\n", cascades / n_tests);
        return 0;
    }

    if(argc == 1) {
        std::cout << "Making random board\n";
        board.loadRandom();
    } else if(argc == 2) {
        board.loadOrbcode(std::string(argv[1]));
    } else {
        fprintf(stderr, "Usage: %s ORBCODE\n", argv[0]);
        fprintf(stderr, "where ORBCODE is a 30-character string\n");
        fprintf(stderr, "of R, G, B, L, D, H, J, P representing orbs\n");
        return -1;
    }

    std::cout << "Solving board:\n";
    std::cout << board;

    SolveState solution = solveBoard(board);
    
    std::cout << "\nBest solution:\n";
    std::cout << solution;

    return 0;
}

