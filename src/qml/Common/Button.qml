import QtQuick 2.0

Item {
    id: button

    property string text: ''
    property color buttonColor: 'white'
    signal clicked()

    Text {
        id: text
        anchors.fill: parent
        anchors.margins: parent.height / 4
        font.pixelSize: height
        text: parent.text
        font.family: statkiFont.name
        color: buttonColor
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
    }

    Item {
        id: underlineContainer
        x: text.x
        y: parent.height - height - text.anchors.margins / 2
        width: text.paintedWidth
        height: parent.height / 40
        clip: true
        Rectangle {
            id: underline
            x: (opacity - 1) * width
            width: parent.width
            height: parent.height
            color: buttonColor
            opacity: mouseArea.containsMouse
            Behavior on opacity { NumberAnimation { duration: 200 } }

        }
    }


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            button.clicked()
        }
    }



}
