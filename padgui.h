#ifndef PADGUI_H
#define PADGUI_H

#include <vector>
#include <QQuickPaintedItem>
#include <QPainter>

#include "orb.h"
#include "combo.h"
#include "board.h"
#include "solver.h"
#include "solvestate.h"
#include "combothread.h"
#include "solvethread.h"

class PadGUI : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(int  mouseX    READ mouseX    WRITE setMouseX    NOTIFY mouseXChanged)
    Q_PROPERTY(int  mouseY    READ mouseY    WRITE setMouseY    NOTIFY mouseYChanged)
    Q_PROPERTY(bool mouseDown READ mouseDown WRITE setMouseDown NOTIFY mouseDownChanged)

    Q_PROPERTY(int comboCount READ comboCount WRITE setComboCount NOTIFY comboCountChanged)
    Q_PROPERTY(int movesCount READ movesCount WRITE setMovesCount NOTIFY movesCountChanged)

    Q_PROPERTY(bool showPath READ showPath WRITE setShowPath NOTIFY showPathChanged)
    Q_PROPERTY(bool loading  READ loading  WRITE setLoading  NOTIFY loadingChanged)

public:
    PadGUI(QQuickItem *parent = Q_NULLPTR);

    void paint(QPainter *painter);

    int  mouseX();
    int  mouseY();
    bool mouseDown();
    void setMouseX(int mx);
    void setMouseY(int my);
    void setMouseDown(bool md);

    int comboCount();
    int movesCount();
    void setComboCount(int n);
    void setMovesCount(int n);

    bool showPath();
    bool loading();
    void setShowPath(bool show);
    void setLoading(bool load);

signals:
    void mouseXChanged();
    void mouseYChanged();
    void mouseDownChanged(bool);

    void comboCountChanged();
    void movesCountChanged();

    void showPathChanged();
    void loadingChanged();

    void repaintSignal();
    void resetSignal();

public slots:
    // these are called when the buttons are clicked
    void resetBoard();
    void newBoard();

    // when combo thread finishes
    void combosFinished();

    // when solve thread finishes
    void solveFinished();

    // when mouse leaves the puzzle
    void mouseExited();

private:
    // thread for doing combos
    ComboThread comboThread;
    SolveThread solveThread;

    // possible states
    enum State {
        SOLVING,
        WAITING,
        CLICKED,
        MOVING,
        COMBOING,
        FINISHED
    };

    // the actual board
    Board board;
    Board originalBoard;
    SolveState solution;
    std::vector<Combo> solutionCombos;

    // mouse status
    int  m_mouseX;
    int  m_mouseY;
    bool m_mouseDown;

    // solution status
    int m_comboCount;
    int m_movesCount;

    // path status
    bool m_showPath;
    bool m_loading;

    // board status
    int orbX;
    int orbY;
    orb selectedOrb;
    enum State state;

    QPixmap getOrbImage(int color);

    // painting helper functions
    void paintHighlight(QPainter *painter, int x, int y);
    void paintPath(QPainter *painter);

    void findComboCount();
    void runSolver();

    void handleMouseMotion();
    void handleMouseClick(bool type);

    // convert mouse location to orb location, writing result with pointers
    // return whether the new location is different than orbX, orbY
    // box version is literal grid box
    // circle version checks inscribed circle and allows for diagonal motion
    bool mouseToOrbBox(int mx, int my, int *orbx, int *orby);
    bool mouseToOrbCircle(int mx, int my, int *orbx, int *orby);

};

#endif // PADGUI_H
