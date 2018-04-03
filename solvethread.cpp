#include "solvethread.h"

SolveThread::SolveThread()
{

}

void SolveThread::setBoard(Board *b)
{
    board = b;
}

void SolveThread::setSolutionOutput(SolveState *s)
{
    solution = s;
}

void SolveThread::run()
{
    *solution = solveBoard(*board);

    emit solveFinished();
}
