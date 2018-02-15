import QtQuick 2.0

Item {
    id: button

    property string text: ''
    property color buttonColor: 'white'
    property int textAlignment: Text.AlignLeft
    signal clicked()
    readonly property alias textPaintedWidth: text.paintedWidth

    Text {
        id: text
        anchors.fill: parent
        anchors.margins: parent.height / 4
        font.pixelSize: height
        text: parent.text
        font.family: statkiFont.name
        color: buttonColor
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: textAlignment
    }

    Item {
        id: underlineContainer
        x: text.x + (text.width - text.paintedWidth) * (textAlignment === Text.AlignLeft ? 0 : 0.5)
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
