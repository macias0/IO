#include "networkaction.h"

#include <QDataStream>
#include <QDebug>

NetworkAction::NetworkAction()
{
    m_networkAction = Shot;
    m_data.point = QPoint(-1,-1);
}

NetworkAction::NetworkAction(const QByteArray &a_data)
{
    QDataStream dataStream(a_data);
    dataStream >> (int&)m_networkAction;
    switch(m_networkAction)
    {
        case Shot:
            dataStream >> m_data.point;
            break;
        case BoardExchange:
            for(int i=0; i<m_boardSize; ++i)
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

    dataStream << (int)m_networkAction;
    switch(m_networkAction)
    {
        case Shot:
            dataStream << m_data.point;
            break;
        case BoardExchange:
            for(int i=0; i<m_boardSize; ++i)
                dataStream << (int)m_data.board[i];
            break;
            break;
        default:
            break;
    }

    return data;
}



