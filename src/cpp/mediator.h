#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QObject>
#include <QCoreApplication>

#include "commons.h"
#include "eview.h"
#include "etile.h"
#include "networkaction.h"
#include "network.h"

class Mediator : public QObject
{
    Q_OBJECT
public:
    explicit Mediator(QObject *parent = nullptr);


    Q_PROPERTY(EView::View activeView  READ activeView  NOTIFY activeViewChanged )
    Q_PROPERTY(QList<int>  shipsNeeded READ shipsNeeded NOTIFY shipsNeededChanged)

    EView::View activeView();
    QList<int> shipsNeeded();

    // user clicked "start game" in main menu
    Q_INVOKABLE void startGame();
    // user clicked "exit game" in main menu
    Q_INVOKABLE void exitGame();

    // called when user presses the mouse down, position is valid
    Q_INVOKABLE void toggleFirstTile(int x, int y);
    // called every time user drags mouse over another tile while holding the mouse down, position is valid
    Q_INVOKABLE void toggleTile(int x, int y);

signals:
    void activeViewChanged();
    void shipsNeededChanged();

    void renderShip(int id, int x, int y, int length, bool horizontal);
    void removeShip(int id);

private:
    void setActiveView(EView::View activeView);
    // const and reference is used to help compiler optimize this function
    inline int positionToIndex(const int &x, const int &y);

    // active view which should be displayed in GUI
    EView::View m_activeView = EView::MainMenu;
    // list of amounts of needed ships per type
    // [1-cell ship, 2-cells ship, 3-cells ship, 4-cells ship]
    QList<int> m_shipsNeeded;
    // list of target amounts of ships per type
    const QList<int> m_shipsTarget = {4, 3, 2, 1};


    ETile::Tile m_playerBoard[m_boardSize];
    ETile::Tile m_enemyBoard[m_boardSize];

    // current user's action
    bool m_userPlacesShips;

    Network m_network;
};

#endif // MEDIATOR_H
