import QtQuick 2.0

Item {
    id: view

    enabled: isActive
    property bool isActive

    Component.onCompleted: {
        opacity = isActive
    }

    onIsActiveChanged: {
        if (isActive)
            animationShow.restart()
        else
            animationHide.restart()
    }

    NumberAnimation on opacity {
        id: animationShow
        running: false
        to: 1
        duration: 500
        easing.type: Easing.OutCubic
    }

    NumberAnimation on opacity {
        id: animationHide
        running: false
        to: 0
        duration: 500
        easing.type: Easing.InCubic
    }
}
