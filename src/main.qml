import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true
    width: 1066
    height: 600
    title: qsTr("Statki")

    Statki {
        anchors.fill: parent
    }
}
