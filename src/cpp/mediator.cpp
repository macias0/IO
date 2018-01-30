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

void Mediator::startGame()
{
    // initialize
    m_shipsNeeded = m_shipsTarget;
    for (int i = 0 ; i < m_boardSize ; i++)
        m_playerBoard[i] = ETile::Empty;

    // TODO clear all rendered ships from previous game with removeShip() signal

    emit shipsNeededChanged();
    setActiveView(EView::Game);
}

void Mediator::exitGame()
{
    QCoreApplication::quit();
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
    // Code after this comment is to be changed

    if (m_userPlacesShips) {
        renderShip(tile, x, y, 1, false);
        m_shipsNeeded[0]--;
    } else {
        removeShip(tile);
        m_shipsNeeded[0]++;
    }

    emit shipsNeededChanged();
}

void Mediator::setActiveView(EView::View activeView)
{
    if (m_activeView != activeView) {
        m_activeView = activeView;
        emit activeViewChanged();
    }
}

int Mediator::positionToIndex(const int &x, const int &y)
{
    return x + y * m_boardWidth;
}
