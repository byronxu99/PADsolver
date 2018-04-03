#ifndef COMBOTHREAD_H
#define COMBOTHREAD_H

#include <QThread>

#include <board.h>

class ComboThread : public QThread
{
    Q_OBJECT
public:
    ComboThread();
    void setBoard(Board *b);

signals:
    void repaint();
    void combosFinished();
    void oneComboRemoved();

public slots:

protected:
    void run() Q_DECL_OVERRIDE;

private:
    Board *board;

};

#endif // COMBOTHREAD_H
