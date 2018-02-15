import QtQuick 2.0

import EView 1.0
import "Views" as Views

Item {
    id: viewsManager

    property int activeView: 0

    Views.MainMenu {
        id: mainMenu
        anchors.fill: parent
        isActive: activeView == EView.MainMenu
    }

    Views.Game {
        id: game
        anchors.fill: parent
        isActive: activeView == EView.Game
    }

    Views.Credits {
        id: credits
        anchors.fill: parent;
        isActive: activeView == EView.Credits
    }
}
