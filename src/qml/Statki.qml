import QtQuick 2.3
import QtQuick.Window 2.2

Item {
    id: statkiMain

    MouseArea {
        anchors.fill: parent
        onClicked: {
            basicHelper.tryMe(200123);
        }
    }
}
