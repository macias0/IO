import QtQuick 2.0

Item {
    id: messageBox

    function nextMessage(a_message) {
        privateSpace.messageQueue.push(a_message)
        if (!messageAnimation.running)
            privateSpace.showNextMessage()
    }

    QtObject {
        id: privateSpace
        readonly property int messageTime: 5000 // 5s
        readonly property var messageQueue: []

        function showNextMessage() {
            if (messageQueue.length === 0)
                return;
            textObject.text = messageQueue[0]
            messageQueue.splice(0, 1);
            messageAnimation.start();
        }
    }

    Rectangle {
        id: messageBubble
        x: (parent.width - width) / 2
        width: textObject.paintedWidth + height
        height: parent.height
        radius: height / 2
        color: "#40000000"
        opacity: 0
        Text {
            id: textObject
            anchors.centerIn: parent
            font.family: statkiFont.name
            font.pixelSize: parent.height * 0.6
            color: "white"
        }
        SequentialAnimation {
            id: messageAnimation
            onStopped: {
                privateSpace.showNextMessage()
            }
            NumberAnimation {
                target: messageBubble
                property: "opacity"
                to: 1
                duration: 500
            }
            PauseAnimation {
                duration: privateSpace.messageTime
            }
            NumberAnimation {
                target: messageBubble
                property: "opacity"
                to: 0
                duration: 500
            }
        }
    }

}
