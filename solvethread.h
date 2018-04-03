#ifndef SOLVETHREAD_H
#define SOLVETHREAD_H

#include <QThread>

#include "board.h"
#include "solver.h"
#include "solvestate.h"

class SolveThread : public QThread
{
    Q_OBJECT
public:
    SolveThread();
    void setBoard(Board *b);
    void setSolutionOutput(SolveState *s);

signals:
    void solveFinished();

public slots:

protected:
    void run() Q_DECL_OVERRIDE;

private:
    Board *board;
    SolveState *solution;

};

#endif // SOLVETHREAD_H
