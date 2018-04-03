#include "settings.h"
#include "solvestate.h"

SolveState::SolveState() {  }

SolveState::SolveState(Board b, int x, int y): board(b)
{
    startX = fingerX = x;
    startY = fingerY = y;

    calculateScore();
}

void SolveState::addNewStates(std::vector<SolveState> &states)
{
    SolveState newState;

    // when making a move, check
    // that the move doesn't "undo" the immediate previous move
    // and we are not at an edge of a board and unable to further move

    // move left
    if((moves.size() == 0 || moves.back() != RIGHT) && fingerX-1 >= 0) {
        newState = *this;
        newState.board.swapOrbs(fingerX, fingerY, fingerX-1, fingerY);
        newState.moves.push_back(LEFT);
        newState.fingerX = fingerX - 1;

        newState.board.setFinishLocations();
        newState.calculateScore();
        if(newState.score - score >= SOLUTION_THRESHOLD)
            states.push_back(newState);
    }

    // move right
    if((moves.size() == 0 || moves.back() != LEFT) && fingerX+1 < BOARD_X) {
        newState = *this;
        newState.board.swapOrbs(fingerX, fingerY, fingerX+1, fingerY);
        newState.moves.push_back(RIGHT);
        newState.fingerX = fingerX + 1;

        newState.board.setFinishLocations();
        newState.calculateScore();
        if(newState.score - score >= SOLUTION_THRESHOLD)
            states.push_back(newState);
    }

    // move up
    if((moves.size() == 0 || moves.back() != DOWN) && fingerY-1 >= 0) {
        newState = *this;
        newState.board.swapOrbs(fingerX, fingerY, fingerX, fingerY-1);
        newState.moves.push_back(UP);
        newState.fingerY = fingerY - 1;

        newState.board.setFinishLocations();
        newState.calculateScore();
        if(newState.score - score >= SOLUTION_THRESHOLD)
            states.push_back(newState);
    }

    // move down
    if((moves.size() == 0 || moves.back() != UP) && fingerY+1 < BOARD_Y) {
        newState = *this;
        newState.board.swapOrbs(fingerX, fingerY, fingerX, fingerY+1);
        newState.moves.push_back(DOWN);
        newState.fingerY = fingerY + 1;

        newState.board.setFinishLocations();
        newState.calculateScore();
        if(newState.score - score >= SOLUTION_THRESHOLD)
            states.push_back(newState);
    }

}

void SolveState::calculateScore()
{
    board.evaluateBoard();


    if(moves.size() < CURVE_POINT)
        score = board.score;
    else
        score = board.score * (1 - ((double)moves.size() - CURVE_POINT)/CURVE_FACTOR);

    score -= (double)moves.size() / 100;
}

/*
void SolveState::getStartLocation(int *x, int *y)
{
    int fx, fy;

    fx = fingerX;
    fy = fingerY;

    // move backwards
    for(int i = moves.size() - 1; i >= 0; i--) {
        if(moves[i] == LEFT)  fx++;
        if(moves[i] == RIGHT) fx--;
        if(moves[i] == UP)    fy++;
        if(moves[i] == DOWN)  fy--;
    }

    *x = fx;
    *y = fy;
}
*/



bool operator<(SolveState const& a, SolveState const& b)
{
    return a.score > b.score;
}

std::ostream &operator<<(std::ostream &os, SolveState & s)
{
    int i, j;
    int n_moves = 0;

    os << "Solution found (score = " << s.score << ")\n";

    os << "Start here:\n";
    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X; i++) {
            if(i == s.startX && j == s.startY)
                os << "X";
            else
                os << ".";
        }
        os << "\n";
    }

    os << s.moves.size() << " moves: ";
    for(auto &iter : s.moves) {
        if(iter == RIGHT) os << "R";
        if(iter == UP)    os << "U";
        if(iter == LEFT)  os << "L";
        if(iter == DOWN)  os << "D";
        if(n_moves%5 == 4) os << " ";
        n_moves++;
    }
    os << "\n";

    os << "Combos:   " << s.board.combos   << "\n";
    os << "Cascades: " << s.board.cascades << "\n";

    return os << s.board;
}
