#include "networkaction.h"

#include <QDataStream>
#include <QDebug>



NetworkAction::NetworkAction(const NetworkActionType a_networkActionType)
{
    m_networkActionType = a_networkActionType;
}

NetworkAction::NetworkAction(const QPoint &a_point) : NetworkAction(NetworkActionType::Shot)
{
    m_data.point = a_point;
}

NetworkAction::NetworkAction(const ETile::Tile (&a_board)[g_boardSize]) : NetworkAction(NetworkActionType::BoardExchange)
{
    memcpy(&m_data.board, &a_board, sizeof(ETile::Tile) * g_boardSize);
}


NetworkAction::NetworkAction(const QByteArray &a_data)
{
    QDataStream dataStream(a_data);

    dataStream >> (int&)m_networkActionType;
    switch(m_networkActionType)
    {
        case Shot:
            dataStream >> m_data.point;
            break;
        case BoardExchange:
            for(int i=0; i<g_boardSize; ++i)
                dataStream >> (int&)m_data.board[i];
            break;
        default:
            break;
    }
}


NetworkAction::operator QByteArray() const
{
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly);

    dataStream << (int)m_networkActionType;
    switch(m_networkActionType)
    {
        case Shot:
            dataStream << m_data.point;
            break;
        case BoardExchange:
            for(int i=0; i<g_boardSize; ++i)
                dataStream << (int)m_data.board[i];
            break;
        default:
            break;
    }

    return data;
}



