import QtQuick 2.0

Item {
    id: view

    enabled: (opacity > 0.01)
    property bool isActive
    opacity: isActive
    Behavior on opacity {
        NumberAnimation { duration: 500 }
    }


}
