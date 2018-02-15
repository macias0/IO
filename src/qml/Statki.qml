import QtQuick 2.3
import QtQuick.Window 2.2

import EView 1.0

Item {
    id: statkiMain

    readonly property int boardSize: 10

    Connections {
        target: mediator
        onNewMessageToDisplay: {
            messageBox.nextMessage(message)
        }
    }

    FontLoader {
        id: statkiFont
        source: 'qrc:/Assets/Fonts/It is Definitely Possible - TTF.ttf'
    }

    ViewsManager {
        id: viewsManager
        anchors.fill: parent
        activeView: mediator.activeView
    }

    MessageBox {
        id: messageBox
        y: parent.height - height * 2
        width: parent.width
        height: parent.height * 0.08
    }
}
