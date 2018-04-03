#include "combothread.h"

ComboThread::ComboThread()
{

}

void ComboThread::setBoard(Board *b)
{
    board = b;
}

void ComboThread::run()
{
    while(board->hasMatches()) {
        while(board->removeComboOne()) {
            emit oneComboRemoved();
            QThread::msleep(500);
            emit repaint();
        }

        while(board->dropDownOne()) {
            QThread::msleep(200);
            emit repaint();
        }
    }

    emit combosFinished();
}
