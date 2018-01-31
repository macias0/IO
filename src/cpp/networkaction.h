#ifndef NETWORKACTION_H
#define NETWORKACTION_H

#include <QPoint>
#include "etile.h"
#include "commons.h"

class NetworkAction
{
public:
    enum NetworkActionType
    {
        Shot =0,
        BoardExchange,
        Surrend
    };

    union NetworkActionData
    {
        ETile::Tile board[m_boardSize];
        QPoint point;
        NetworkActionData(){memset( this, 0, sizeof( NetworkActionData ) );}
    };


    NetworkAction();
    //for implicit conversion from QByteArray
    NetworkAction(const QByteArray &a_data);

    NetworkActionType m_networkAction;
    NetworkActionData m_data;

    //for implicit conversion to QByteArray
    operator QByteArray() const;


};

#endif // NETWORKACTION_H
