#ifndef BASICHELPER_H
#define BASICHELPER_H

#include <QObject>

#include <iostream>

class BasicHelper : public QObject
{
    Q_OBJECT

public:
    explicit BasicHelper(QObject *parent = nullptr);

    Q_INVOKABLE void tryMe(int x);
};

#endif // BASICHELPER_H
