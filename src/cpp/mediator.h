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


    Q_PROPERTY(EView::View activeView           READ activeView           NOTIFY activeViewChanged          )
    Q_PROPERTY(QList<int>  shipsNeeded          READ shipsNeeded          NOTIFY shipsNeededChanged         )
    Q_PROPERTY(bool        yourTurn             READ yourTurn             NOTIFY yourTurnChanged            )
    Q_PROPERTY(bool        boardIsValid         READ boardIsValid         NOTIFY boardIsValidChanged        )
    Q_PROPERTY(bool        preparingBoardPhase  READ preparingBoardPhase  NOTIFY preparingBoardPhaseChanged )
    Q_PROPERTY(bool        waitingForEnemyBoard READ waitingForEnemyBoard NOTIFY waitingForEnemyBoardChanged)

    EView::View activeView();
    QList<int> shipsNeeded();
    bool yourTurn();
    bool boardIsValid();
    bool preparingBoardPhase();
    bool waitingForEnemyBoard();

    // user clicked "create a server" in main menu
    Q_INVOKABLE void createServer();
    // user clicked "join a game" in main menu
    Q_INVOKABLE void joinGame();

    // user clicked "exit game" in main menu
    Q_INVOKABLE void exitGame();

    // user wants to submit his board
    Q_INVOKABLE void playerIsReady();

    // called when user presses the mouse down, position is valid
    Q_INVOKABLE void toggleFirstTile(int x, int y);
    // called every time user drags mouse over another tile while holding the mouse down, position is valid
    Q_INVOKABLE void toggleTile(int x, int y);

    // user selected a tile to attack
    Q_INVOKABLE void attackTile(int x, int y);

signals:
    void activeViewChanged();
    void shipsNeededChanged();
    void yourTurnChanged();
    void boardIsValidChanged();
    void preparingBoardPhaseChanged();
    void waitingForEnemyBoardChanged();

    // id is a unique value, used then in removeShip()
    void renderShip(int id, int x, int y, int length, bool horizontal);
    void removeShip(int id);
    void tileAttacked(int x, int y);

private:

    // user clicked "start game" in main menu
    Q_INVOKABLE void startGame();

    void setActiveView(EView::View a_activeView);
    void setYourTurn(bool a_yourTurn);
    void setBoardIsValid(bool a_boardIsValid);
    void setPreparingBoardPhase(bool a_preparingBoardPhase);
    void setWaitingForEnemyBoard(bool a_waitingForEnemyBoard);
    // const and reference is used to help compiler optimize this function
    inline int positionToIndex(const int &x, const int &y);

    // active view which should be displayed in GUI
    EView::View m_activeView = EView::MainMenu;
    bool m_yourTurn;
    bool m_boardIsValid;
    bool m_preparingBoardPhase;
    bool m_waitingForEnemyBoard;
    // list of amounts of needed ships per type
    // [1-cell ship, 2-cells ship, 3-cells ship, 4-cells ship]
    QList<int> m_shipsNeeded;
    // list of target amounts of ships per type
    const QList<int> m_shipsTarget = {4, 3, 2, 1};


    const QList<int> m_shipsBoardValid = {0, 3, 2, 1}; /* TODO change to 0, 0, 0, 0 */


    ETile::Tile m_playerBoard[g_boardSize];
    ETile::Tile m_enemyBoard[g_boardSize];

    // current user's action
    bool m_userPlacesShips;

    Network &m_network = Network::getSingleton();
};

#endif // MEDIATOR_H
