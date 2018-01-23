import QtQuick 2.3
import QtQuick.Window 2.2

import EView 1.0

Item {
    id: statkiMain

    ViewsManager {
        id: viewsManager
        anchors.fill: parent
        activeView: mediator.activeView
        onActiveViewChanged: {
            console.log(activeView)
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            basicHelper.tryMe(200123);
            mediator.requestNewView(EView.Game)
        }
    }
}
