#ifndef EVIEW_H
#define EVIEW_H

#include <QObject>

class EView : public QObject
{
    Q_OBJECT
public:
    explicit EView(QObject *parent = nullptr) : QObject(parent) {}

    enum View {
        MainMenu,
        Game,
        GameOverWin,
        GameOverDefeat
    };
    Q_ENUMS(View)

};

#endif // EVIEW_H
