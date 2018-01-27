import QtQuick 2.0

Item {
    id: view

    enabled: isActive
    property bool isActive
    opacity: isActive
    Behavior on opacity {
        NumberAnimation { duration: 500 }
    }


}
