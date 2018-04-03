#include <cstdlib>
#include <cmath>

#include <QDebug>
#include <QThread>

#include "padgui.h"

PadGUI::PadGUI(QQuickItem *parent) : QQuickPaintedItem(parent), comboThread(), solveThread(), board()
{
    // graphics settings
    setAntialiasing(true);
    setRenderTarget(QQuickPaintedItem::FramebufferObject);

    // signals and slots
    connect(&comboThread, &ComboThread::repaint, this, [this]() { this->update(); }, Qt::QueuedConnection);
    connect(&comboThread, &ComboThread::combosFinished, this, &PadGUI::combosFinished);
    connect(&comboThread, &ComboThread::oneComboRemoved, this, [&]() { setComboCount(comboCount() + 1); });
    connect(&solveThread, &SolveThread::solveFinished, this, &PadGUI::solveFinished);
    connect(this, &PadGUI::resetSignal, &comboThread, &ComboThread::quit);
    connect(this, &PadGUI::resetSignal, &solveThread, &SolveThread::quit);

    setShowPath(true);
    newBoard();
}

void PadGUI::paint(QPainter *painter)
{
    int i, j;
    int orbsize;
    std::vector<std::vector<Combo>> combos;

    orbsize = (width() / BOARD_X);

    // draw background
    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X; i++) {
            if((i+j)%2 == 0) {
                painter->drawPixmap(i*orbsize, j*orbsize, orbsize, orbsize, QPixmap(QString(":images/bg-dark.png")));
            } else {
                painter->drawPixmap(i*orbsize, j*orbsize, orbsize, orbsize, QPixmap(QString(":images/bg-light.png")));
            }
        }
    }

    // draw orb highlight for orbs in correct positions
    if(state == CLICKED || state == MOVING) {
        // for each combo of the solution
        for(auto &cascade_level_iter : solution.board.getCombos()) {
            for(auto &combo_iter : cascade_level_iter) {

                // check if all orbs are in place
                int n_orbs = 0;
                for(int i=0; i<BOARD_X; i++) {
                    for(int j=0; j<BOARD_Y; j++) {
                        if(combo_iter.orbs[i][j] != EMPTY) {
                            // Consider the orb at combo_iter.orbs[i][j]
                            // where i,j is the location after orbs fall down.
                            // When the board is solved, its position in
                            // board.orbs before orbs fall down
                            // is at combo_iter.orbs[i][j].finishX/Y.
                            //
                            // Orbs are uniquely identified by startX/Y.
                            // Therefore we check the location
                            // combo_iter.orbs[i][j].finishX/Y of board.orbs
                            // and see if that orb's startX/Y
                            // matches with combo_iter.orbs[i][j].startX/Y.

                            int fx = combo_iter.orbs[i][j].finishX;
                            int fy = combo_iter.orbs[i][j].finishY;
                            int sx = combo_iter.orbs[i][j].startX;
                            int sy = combo_iter.orbs[i][j].startY;

                            if(board.orbs[fx][fy].startX == sx
                            && board.orbs[fx][fy].startY == sy) {
                                n_orbs++;
                            }
                        }
                    }
                }

                // if all orbs in place,
                // highlight the orbs of the combo
                if(n_orbs == combo_iter.n_orbs) {
                    for(int i=0; i<BOARD_X; i++) {
                        for(int j=0; j<BOARD_Y; j++) {
                            if(combo_iter.orbs[i][j] != EMPTY) {
                                paintHighlight(painter,
                                    combo_iter.orbs[i][j].finishX,
                                    combo_iter.orbs[i][j].finishY);
                            }
                        }
                    }
                }

            }
        }

    }

    // draw orbs
    for(j=0; j<BOARD_Y; j++) {
        for(i=0; i<BOARD_X; i++) {
            if(board.orbs[i][j] == EMPTY) continue;
            if((state != CLICKED && state != MOVING) || i != orbX || j != orbY)
                painter->drawPixmap(i*orbsize, j*orbsize, orbsize, orbsize, getOrbImage(board.orbs[i][j].color));
            else
                painter->drawPixmap(m_mouseX - orbsize/2, m_mouseY - orbsize/2, orbsize, orbsize, getOrbImage(board.orbs[i][j].color));
        }
    }

    // draw path
    if(showPath() && (state == WAITING || state == CLICKED || state == MOVING)) {
        painter->setBrush(QColor(255,255,255));
        painter->setPen(QPen(QBrush(QColor(255,255,255)), orbsize/10, Qt::SolidLine, Qt::RoundCap));
        paintPath(painter);

        painter->setBrush(QColor(0,0,0));
        painter->setPen(QPen(QBrush(QColor(0,0,0)), orbsize/20, Qt::SolidLine, Qt::RoundCap));
        paintPath(painter);
    }

}

void PadGUI::paintHighlight(QPainter *painter, int x, int y)
{
    int orbsize;

    orbsize = (width() / BOARD_X);

    painter->setBrush(QBrush(QColor(200,200,200,100)));
    painter->setPen(Qt::NoPen);
    //painter->setPen(QPen(QBrush(QColor(255,255,255,100)), orbsize/20, Qt::SolidLine, Qt::RoundCap));
    //painter->drawEllipse(QRect(orbsize*x, orbsize*y, orbsize, orbsize));
    painter->drawRect(QRect(orbsize*x, orbsize*y, orbsize, orbsize));
}

void PadGUI::paintPath(QPainter *painter)
{
    int x, y, x1, y1;
    int orbsize;
    int arrowsize;

    orbsize = (width() / BOARD_X);
    arrowsize = orbsize / 10;

    x = solution.startX; y = solution.startY;
    for(auto &iter : solution.moves) {
        x1 = x; y1 = y;
        if(iter == RIGHT) x1 = x + 1;
        if(iter == LEFT)  x1 = x - 1;
        if(iter == DOWN)  y1 = y + 1;
        if(iter == UP)    y1 = y - 1;

        QPointF point1 = QPointF((x+0.5)*orbsize, (y+0.5)*orbsize);
        QPointF point2 = QPointF((x1+0.5)*orbsize, (y1+0.5)*orbsize);
        QPointF midpoint = (point1 + point2) / 2;
        QPointF arrow1, arrow2;

        // draw the line segment
        painter->drawLine(point1, point2);

        // draw the arrow
        if(iter == RIGHT) {
            arrow1 = midpoint + QPointF(-1*arrowsize, arrowsize);
            arrow2 = midpoint + QPointF(-1*arrowsize, -1*arrowsize);
        }
        if(iter == LEFT) {
            arrow1 = midpoint + QPointF(arrowsize, arrowsize);
            arrow2 = midpoint + QPointF(arrowsize, -1*arrowsize);
        }
        if(iter == UP) {
            arrow1 = midpoint + QPointF(arrowsize, arrowsize);
            arrow2 = midpoint + QPointF(-1*arrowsize, arrowsize);
        }
        if(iter == DOWN) {
            arrow1 = midpoint + QPointF(arrowsize, -1*arrowsize);
            arrow2 = midpoint + QPointF(-1*arrowsize, -1*arrowsize);
        }
        painter->drawLine(midpoint, arrow1);
        painter->drawLine(midpoint, arrow2);

        x = x1; y = y1;
    }

    // draw start location
    painter->drawEllipse(QPoint((solution.startX+0.5)*orbsize, (solution.startY+0.5)*orbsize), orbsize/20, orbsize/20);

    // draw end location
    painter->drawEllipse(QPoint((solution.fingerX+0.5)*orbsize, (solution.fingerY+0.5)*orbsize), orbsize/20, orbsize/20);
}

QPixmap PadGUI::getOrbImage(int color)
{
    if(color == R) return QPixmap(QString(":/images/Red.png"));
    if(color == B) return QPixmap(QString(":/images/Blue.png"));
    if(color == G) return QPixmap(QString(":/images/Green.png"));
    if(color == L) return QPixmap(QString(":/images/Light.png"));
    if(color == D) return QPixmap(QString(":/images/Dark.png"));
    if(color == H) return QPixmap(QString(":/images/Heart.png"));
    if(color == J) return QPixmap(QString(":/images/Jammer.png"));
    if(color == P) return QPixmap(QString(":/images/Poison.png"));
    qDebug() << "Could not load image: invalid orb number " << color;
    return QPixmap();
}

void PadGUI::resetBoard()
{
    if(solveThread.isRunning())
        return;

    if(comboThread.isRunning()) {
        emit resetSignal();
        comboThread.wait();
    }

    board = originalBoard;
    state = WAITING;

    findComboCount();
    setMovesCount(0);

    emit repaintSignal();
}

void PadGUI::newBoard()
{
    if(solveThread.isRunning())
        return;

    if(comboThread.isRunning()) {
        emit resetSignal();
        comboThread.wait();
    }

    board.loadRandom();
    originalBoard = board;

    findComboCount();
    setMovesCount(0);
    runSolver();

    emit repaintSignal();
}

void PadGUI::findComboCount()
{
    board.evaluateBoard();
    setComboCount(board.combos);
}

void PadGUI::runSolver()
{
    state = SOLVING;
    setLoading(true);
    solveThread.setBoard(&board);
    solveThread.setSolutionOutput(&solution);
    solveThread.start();
}

void PadGUI::combosFinished()
{
    if(state == COMBOING)
        state = FINISHED;
    emit repaintSignal();
}

void PadGUI::solveFinished()
{
    if(state == SOLVING)
        state = WAITING;
    setLoading(false);
    emit repaintSignal();
}

void PadGUI::handleMouseClick(bool type)
{
    // handle mouse down
    if(type) {
        if(state != WAITING) return;

        mouseToOrbBox(m_mouseX, m_mouseY, &orbX, &orbY);
        selectedOrb = board.orbs[orbX][orbY];

        if(selectedOrb == EMPTY)
            return;

        state = CLICKED;
    }
    // handle mouse up
    else {
        // nothing was clicked
        if(state != CLICKED && state != MOVING) return;

        // orb was never moved
        if(state == CLICKED) {
            state = WAITING;
            return;
        }

        // remove combos
        state = COMBOING;
        board.setFinishLocations();
        emit repaintSignal();
        comboThread.setBoard(&board);
        comboThread.start();
    }
}

void PadGUI::handleMouseMotion()
{
    int new_x, new_y;

    if(state != CLICKED && state != MOVING)
        return;

    if(mouseToOrbCircle(m_mouseX, m_mouseY, &new_x, &new_y)) {
        if(state == CLICKED)
            state = MOVING;

        board.swapOrbs(orbX, orbY, new_x, new_y);
        orbX = new_x;
        orbY = new_y;

        setMovesCount(movesCount() + 1);
    }
}

bool PadGUI::mouseToOrbBox(int mx, int my, int *orbx, int *orby)
{
    int orbsize = width() / BOARD_X;

    *orbx = mx / orbsize;
    *orby = my / orbsize;

    return *orbx != orbX || *orby != orbY;
}

bool PadGUI::mouseToOrbCircle(int mx, int my, int *orbx, int *orby)
{
    int orbsize = width() / BOARD_X;
    int center_x, center_y;

    center_x = (int)(( (double)(mx/orbsize) + 0.5) * orbsize);
    center_y = (int)(( (double)(my/orbsize) + 0.5) * orbsize);

    if(sqrt((mx-center_x)*(mx-center_x) + (my-center_y)*(my-center_y)) <= (double)orbsize / 2) {
        *orbx = mx / orbsize;
        *orby = my / orbsize;
        return *orbx != orbX || *orby != orbY;
    }

    return false;
}

void PadGUI::mouseExited()
{
    handleMouseClick(false);
}


// Stuff for Q_PROPERTY variables

int  PadGUI::mouseX()    { return m_mouseX; }
int  PadGUI::mouseY()    { return m_mouseY; }
bool PadGUI::mouseDown() { return m_mouseDown; }

void PadGUI::setMouseX(int mx)
{
    m_mouseX = mx;
    handleMouseMotion();
    emit repaintSignal();
}

void PadGUI::setMouseY(int my)
{
    m_mouseY = my;
    handleMouseMotion();
    emit repaintSignal();
}

void PadGUI::setMouseDown(bool md)
{
    m_mouseDown = md;
    handleMouseClick(md);
    emit repaintSignal();
}

int PadGUI::comboCount() { return m_comboCount; }
int PadGUI::movesCount() { return m_movesCount; }

void PadGUI::setComboCount(int n)
{
    m_comboCount = n;
    emit comboCountChanged();
}

void PadGUI::setMovesCount(int n)
{
    m_movesCount = n;
    emit movesCountChanged();
}

bool PadGUI::showPath() { return m_showPath; }
bool PadGUI::loading()  { return m_loading; }

void PadGUI::setShowPath(bool show)
{
    m_showPath = show;
    emit showPathChanged();
    emit repaintSignal();
}

void PadGUI::setLoading(bool load)
{
    m_loading = load;
    emit loadingChanged();
}
