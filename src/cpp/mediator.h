#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QObject>
#include <QCoreApplication>

#include "eview.h"

class Mediator : public QObject
{
    Q_OBJECT
public:
    explicit Mediator(QObject *parent = nullptr);


    Q_PROPERTY(EView::View activeView READ activeView NOTIFY activeViewChanged)

    EView::View activeView();

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void endGame();

signals:
    void activeViewChanged(EView::View view);

private:
    void setActiveView(EView::View activeView);

    EView::View m_activeView = EView::MainMenu;
};

#endif // MEDIATOR_H
