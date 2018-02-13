#ifndef ETILE_H
#define ETILE_H

#include <QObject>

class ETile : public QObject
{
    Q_OBJECT
public:
    explicit ETile(QObject *parent = nullptr) : QObject(parent) {}

    enum Tile {
        Empty,
        Occupied,
        ShotEmpty,
        ShotOccupied
    };
    Q_ENUMS(Tile)



};

#endif // ETILE_H
