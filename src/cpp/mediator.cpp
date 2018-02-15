#include "mediator.h"

Mediator::Mediator(QObject *parent) : QObject(parent)
{
    setWaitingToConnect(false);
    m_connectionTimeoutTimer.setInterval(m_connectionTimeout);
    m_connectionTimeoutTimer.setSingleShot(true);
    m_connectionTimeoutTimer.setTimerType(Qt::VeryCoarseTimer);

    // connecting signals
    connect(&m_network, &Network::connected, [this]() //i love lambda ;]
    {
        qDebug() << "Connected";
        startGame();
    });
    connect(&m_network, &Network::messageReceived, [this](const QByteArray a_message)
    {
        enemyActionReceived(a_message);
    });
    connect(&m_connectionTimeoutTimer, &QTimer::timeout, [this]()
    {
        qDebug() << "Connection timed out";
        // TODO abort connection
        setWaitingToConnect(false);
        emit newMessageToDisplay(m_connectionTimedOutMessage);
    });

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

bool Mediator::boardHasErrors()
{
    return m_boardHasErrors;
}

bool Mediator::preparingBoardPhase()
{
    return m_preparingBoardPhase;
}

bool Mediator::waitingForEnemyBoard()
{
    return m_waitingForEnemyBoard;
}

bool Mediator::waitingToConnect()
{
    return m_waitingToConnect;
}

void Mediator::createServer()
{
    m_network.startServer();
    setWaitingToConnect(true);
    m_connectionTimeoutTimer.start();
}

void Mediator::joinGame()
{
    m_network.connectToServer();
    setWaitingToConnect(true);
    m_connectionTimeoutTimer.start();
}

void Mediator::startGame()
{
    // initialize
    m_shipsNeeded = m_shipsTarget;
    for (int i = 0 ; i < g_boardSize ; i++)
    {
        if ((ETile::Occupied == m_playerBoard[i])
                || (ETile::ShotOccupied == m_playerBoard[i]))
        {
            emit removeShip(i);
        }
        m_playerBoard[i] = ETile::Empty;
    }
    setBoardIsValid(false);
    setPreparingBoardPhase(true);
    setWaitingForEnemyBoard(true);
    setWaitingToConnect(false);
    m_connectionTimeoutTimer.stop();

    emit shipsNeededChanged();
    setActiveView(EView::Game);
}

void Mediator::updateShipsNeeded()
{
    ETile::Tile boardCopy[g_boardSize];
    std::copy(m_playerBoard, m_playerBoard + g_boardSize, boardCopy);

    m_shipsNeeded = m_shipsTarget;

    for (int y = 0 ; y < g_boardHeight ; y++)
    {
        for (int x = 0 ; x < g_boardWidth ; x++)
        {
            int index = positionToIndex(x, y);
            if (ETile::Occupied == boardCopy[index])
            {
                int shipLength = 1;
                boardCopy[index] = ETile::Empty;

                for (int _x = x + 1, _index = index + 1 ;
                     (_x < g_boardWidth)
                     && (ETile::Occupied == boardCopy[_index]) ;
                     _x++, _index += 1)
                {
                    shipLength++;
                    boardCopy[_index] = ETile::Empty;
                }
                if (shipLength == 1)
                {
                    for (int _y = y + 1, _index = index + g_boardWidth ;
                         (_y < g_boardWidth)
                         && (ETile::Occupied == boardCopy[_index]) ;
                         _y++, _index += g_boardWidth)
                    {
                        shipLength++;
                        boardCopy[_index] = ETile::Empty;
                    }
                }

                if (shipLength <= m_shipsNeeded.length())
                {
                    m_shipsNeeded[shipLength - 1]--;
                }
            }
        }
    }

    emit shipsNeededChanged();
}

void Mediator::updateBoardHasErrors()
{
    for (int y = 0 ; y < g_boardHeight - 1 ; y++)
    {
        for (int x = 0 ; x < g_boardWidth - 1 ; x++)
        {
            if ( isOccupied(x, y) /* center tile is occupied */
                 && ( /* any of the neighbor corner tile is occupied */
                      ( isOccupied(x - 1, y - 1)
                        || isOccupied(x + 1, y - 1)
                        || isOccupied(x - 1, y + 1)
                        || isOccupied(x + 1, y + 1)
                        )
                      || ( /* or both horizontal or vertical neighbor tiles are occupied */
                           ( (isOccupied(x - 1, y))
                             || (isOccupied(x + 1, y)) )
                           && ( (isOccupied(x, y - 1))
                                || (isOccupied(x, y + 1)) ) ) ) )
            {
                setBoardHasErrors(true);
                return;
            }
        }
    }

    setBoardHasErrors(false);
}

bool Mediator::isOnBoard(const int &x, const int &y)
{
    if ((x < 0) || (x >= g_boardWidth) || (y < 0) || (y >= g_boardHeight))
        return false;
    return true;
}

bool Mediator::isOccupied(const int &x, const int &y, const bool &enemyBoard)
{
    if (!isOnBoard(x, y))
        return false;
    return (ETile::Occupied == (enemyBoard ? m_enemyBoard : m_playerBoard)[positionToIndex(x, y)]);
}

void Mediator::exitGame()
{
    QCoreApplication::quit();
}

void Mediator::playerIsReady()
{
    //sharing board with other player
    m_network.sendMessage(NetworkAction(m_playerBoard));

    // set "yourTurn" variable
    setPreparingBoardPhase(false);

    if(m_network.getNetworkState() == Network::Server) //server always has first move
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

    if (m_userPlacesShips) {
        renderShip(tile, x, y);
    } else {
        removeShip(tile);
    }

    updateShipsNeeded();
    setBoardIsValid(m_shipsNeeded == m_shipsBoardValid);
    updateBoardHasErrors();
}

void Mediator::attackTile(int x, int y)
{
    if (m_yourTurn)
        attackTile(x, y, false);
}

void Mediator::attackTile(int x, int y, bool enemy)
{
    //TODO check if the game has been won or lost and end the session


    ETile::Tile *boardToModify = enemy ? m_playerBoard : m_enemyBoard;
    int shotIndex = positionToIndex(x, y);
    // if tile was already attacked, ignore - player has to choose other tile
    if (ETile::ShotEmpty <= boardToModify[shotIndex])
        return;

    if (!enemy)
    {
        qDebug() << "Sending attack message";
        m_network.sendMessage(NetworkAction(QPoint(x,y)));
    }

    setYourTurn(enemy);

    emit renderShot(x, y, (ETile::Occupied == boardToModify[shotIndex]), !enemy);
    boardToModify[shotIndex] = static_cast<ETile::Tile>(boardToModify[shotIndex] + ETile::ShotEmpty);
    if (isShipSunk(x, y, !enemy))
        updateTilesAfterShipSunk(x, y, !enemy);
}

bool Mediator::isShipSunk(int startX, int startY, bool enemyShip)
{
    QList<QPair<int, int>> tilesToCheck;
    getShipTiles(startX, startY, enemyShip, tilesToCheck);
    const ETile::Tile *boardToCheck = enemyShip ? m_enemyBoard : m_playerBoard;
    for (const QPair<int, int> &tile : tilesToCheck)
    {
        if (ETile::Occupied == boardToCheck[positionToIndex(tile.first, tile.second)])
            return false;
    }
    return true;
}

void Mediator::getShipTiles(int startX, int startY, bool enemyShip, QList<QPair<int, int>> &tiles)
{
    tiles.append(QPair<int, int>(startX, startY));
    for (int y = -1 ; isOccupied(startX    , startY + y, enemyShip) ; y--)
        tiles.append(QPair<int, int>(startX    , startY + y));
    for (int x = -1 ; isOccupied(startX + x, startY    , enemyShip) ; x--)
        tiles.append(QPair<int, int>(startX + x, startY    ));
    for (int x =  1 ; isOccupied(startX + x, startY    , enemyShip) ; x++)
        tiles.append(QPair<int, int>(startX + x, startY    ));
    for (int y =  1 ; isOccupied(startX    , startY + y, enemyShip) ; y++)
        tiles.append(QPair<int, int>(startX    , startY + y));
}

void Mediator::updateTilesAfterShipSunk(int startX, int startY, bool enemyShip)
{
    QList<QPair<int, int>> tilesToUpdate;
    getShipTiles(startX, startY, enemyShip, tilesToUpdate);

    for (const QPair<int, int> &tile : tilesToUpdate)
    {
        for (int y = -1 ; y <= 1 ; y++)
        {
            for (int x = -1 ; x <= 1 ; x++)
            {
                if (!isOnBoard(tile.first + x, tile.second + y))
                    continue;
                int index = positionToIndex(tile.first + x, tile.second + y);
                if (ETile::Empty == (enemyShip ? m_enemyBoard : m_playerBoard)[index])
                {
                    (enemyShip ? m_enemyBoard : m_playerBoard)[index] = ETile::ShotEmpty;
                    emit renderShot(x, y, false, enemyShip);
                }
            }
        }
    }
}

void Mediator::surrender()
{
    m_network.sendMessage(NetworkAction());

    //TODO shutdown server / disconnect
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

void Mediator::setBoardHasErrors(bool a_boardHasErrors)
{
    if (m_boardHasErrors != a_boardHasErrors) {
        m_boardHasErrors = a_boardHasErrors;
        emit boardHasErrorsChanged();
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

void Mediator::setWaitingToConnect(bool a_waitingToConnect)
{
    if (m_waitingToConnect != a_waitingToConnect) {
        m_waitingToConnect = a_waitingToConnect;
        emit waitingToConnectChanged();
    }
}

int Mediator::positionToIndex(const int &x, const int &y)
{
    return x + y * g_boardWidth;
}

#ifdef ENABLE_DEBUG_BOARD_PRINTS
void Mediator::debugPrintBoard(const ETile::Tile *a_board)
{
    int index = 0;
    for (int y = 0 ; y < g_boardHeight ; y++)
    {
        for (int x = 0 ; x < g_boardWidth ; x++)
        {
            std::cout << a_board[index];
            index++;
        }
        std::cout << std::endl;
    }
}
#endif

void Mediator::enemyActionReceived(const NetworkAction &a_action)
{
    qDebug() << "EnemyActionReceived, type: " << a_action.m_networkActionType;
    switch(a_action.m_networkActionType)
    {
        case NetworkAction::BoardExchange:
            qDebug() << "BoardExchange:";
            for(int i=0; i<g_boardSize; ++i)
            {
                m_enemyBoard[i] = a_action.m_data.board[i];
                qDebug() << (int)m_enemyBoard[i];
            }
            setWaitingForEnemyBoard(false);
            break;

        case NetworkAction::Shot:
            qDebug() << "I got a shot on " << a_action.m_data.point;
            attackTile(a_action.m_data.point.x(), a_action.m_data.point.y(), true);
            break;
        case NetworkAction::Surrend:
            // TODO ensure GUI cleans up
            setActiveView(EView::MainMenu);
            emit newMessageToDisplay(m_enemyHasSurrendered);
            break;
    }
}
