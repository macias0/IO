#include "mediator.h"

Mediator::Mediator(QObject *parent) : QObject(parent)
{
    // nothing to do
}

EView::View Mediator::activeView()
{
    return m_activeView;
}

QList<int> Mediator::shipsNeeded()
{
    return m_shipsNeeded;
}

bool Mediator::yourTurn()
{
    return m_yourTurn;
}

bool Mediator::boardIsValid()
{
    return m_boardIsValid;
}

bool Mediator::preparingBoardPhase()
{
    return m_preparingBoardPhase;
}

bool Mediator::waitingForEnemyBoard()
{
    return m_waitingForEnemyBoard;
}

void Mediator::createServer()
{
    // TODO create the server, wait for player, prepare the game

    startGame(); /* TODO remove */
}

void Mediator::joinGame()
{
    // TODO join the game, prepare the game, or return error string with signal

    startGame(); /* TODO remove */
}

// TODO this function should probably change to something like "prepareTheGame"
// or be completely removed - up to you
void Mediator::startGame()
{
    // initialize
    m_shipsNeeded = m_shipsTarget;
    for (int i = 0 ; i < g_boardSize ; i++)
        m_playerBoard[i] = ETile::Empty;
    setBoardIsValid(false);
    setPreparingBoardPhase(true);
    setWaitingForEnemyBoard(false);

    // TODO clear all rendered ships from previous game with removeShip() signal

    emit shipsNeededChanged();
    setActiveView(EView::Game);
}

void Mediator::exitGame()
{
    QCoreApplication::quit();
}

void Mediator::playerIsReady()
{
    // TODO share the board with the other player and prepare to start the game
    // remember to set waitingForEnemyBoard while waiting for the other player to be ready
    // set "yourTurn" variable
    setPreparingBoardPhase(false);
    setYourTurn(true);
}

void Mediator::toggleFirstTile(int x, int y)
{
    int tile = positionToIndex(x, y);
    // keep the same action for all tiles until mouse click is released
    m_userPlacesShips = (m_playerBoard[tile] == ETile::Empty ? true : false);
    toggleTile(x, y);
}

void Mediator::toggleTile(int x, int y)
{
    int tile = positionToIndex(x, y);
    if (((m_userPlacesShips) && (m_playerBoard[tile] == ETile::Occupied))
            || ((!m_userPlacesShips) && (m_playerBoard[tile] == ETile::Empty))) {
        // ignore this action - it is not the same type as first toggle
        return;
    }

    if (m_userPlacesShips)
        m_playerBoard[tile] = ETile::Occupied;
    else
        m_playerBoard[tile] = ETile::Empty;

    // TODO update info about ships and send changes to GUI
    // rendered ships may be stored in some container or just as points on the
    // playerBoard. Up to you
    // Code after this comment is to be changed, so the ships have proper length and orientation

    if (m_userPlacesShips) {
        renderShip(tile, x, y, 1, false);
        m_shipsNeeded[0]--;
    } else {
        removeShip(tile);
        m_shipsNeeded[0]++;
    }

    bool boardIsValid = (m_shipsNeeded == m_shipsBoardValid);

    // TODO check if ships are not too close each other
    // boardIsValid = ...

    setBoardIsValid(boardIsValid);

    emit shipsNeededChanged();
}

void Mediator::attackTile(int x, int y)
{
    // TODO handle request to attack
    // if tile was already attacked, ignore - player has to choose other tile
}

void Mediator::setActiveView(EView::View a_activeView)
{
    if (m_activeView != a_activeView) {
        m_activeView = a_activeView;
        emit activeViewChanged();
    }
}

void Mediator::setYourTurn(bool a_yourTurn)
{
    if (m_yourTurn != a_yourTurn) {
        m_yourTurn = a_yourTurn;
        emit yourTurnChanged();
    }
}

void Mediator::setBoardIsValid(bool a_boardIsValid)
{
    if (m_boardIsValid != a_boardIsValid) {
        m_boardIsValid = a_boardIsValid;
        emit boardIsValidChanged();
    }
}

void Mediator::setPreparingBoardPhase(bool a_preparingBoardPhase)
{
    if (m_preparingBoardPhase != a_preparingBoardPhase) {
        m_preparingBoardPhase = a_preparingBoardPhase;
        emit preparingBoardPhaseChanged();
    }
}

void Mediator::setWaitingForEnemyBoard(bool a_waitingForEnemyBoard)
{
    if (m_waitingForEnemyBoard != a_waitingForEnemyBoard) {
        m_waitingForEnemyBoard = a_waitingForEnemyBoard;
        emit waitingForEnemyBoardChanged();
    }
}

int Mediator::positionToIndex(const int &x, const int &y)
{
    return x + y * g_boardWidth;
}
