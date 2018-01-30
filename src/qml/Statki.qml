import QtQuick 2.3
import QtQuick.Window 2.2

import EView 1.0

Item {
    id: statkiMain

    readonly property int boardSize: 10

    FontLoader {
        id: statkiFont
        source: 'qrc:/Assets/Fonts/It is Definitely Possible - TTF.ttf'
    }

    ViewsManager {
        id: viewsManager
        anchors.fill: parent
        activeView: mediator.activeView
    }
}
