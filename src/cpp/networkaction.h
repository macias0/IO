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
        ETile::Tile board[g_boardSize];
        QPoint point;

        //because of compiler error
        NetworkActionData(){memset( this, 0, sizeof( NetworkActionData ) );}
    };


    NetworkAction(const NetworkActionType a_networkActionType = NetworkActionType::Surrend);
    NetworkAction(const QPoint &a_point);
    NetworkAction(const ETile::Tile (&a_board)[g_boardSize]);
    //for implicit conversion from QByteArray
    NetworkAction(const QByteArray &a_data);


    NetworkActionType m_networkActionType;
    NetworkActionData m_data;

    //for implicit conversion to QByteArray
    operator QByteArray() const;


};

#endif // NETWORKACTION_H
