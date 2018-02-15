import QtQuick 2.0

Item {
    id: container
    anchors.fill: parent;
    property bool isVisible: false
    property string messageText

    enabled: (opacity > 0.01)
    opacity: isVisible
    Behavior on opacity { NumberAnimation { duration: 200 } }

    //blocking GUI
    MultiPointTouchArea
    {
        anchors.fill: parent;
    }

    Rectangle //blur
    {
        opacity: 0.6
        color: "white"
        anchors.fill: parent;
    }

    Rectangle
    {
        color: '#40000000'
        radius: width / 20
        anchors.centerIn: parent;
        width: parent.width / 2
        height: width / 4
        Text
        {
            id: windowTextObject
            anchors.centerIn: parent;
            property string displayText: messageText
            text: displayText
            color: "white"
            font.family: statkiFont.name
            horizontalAlignment: Text.AlignLeft
            verticalAlignment:  Text.AlignVCenter
            font.pixelSize: height / 2
            width: parent.width / 2
            height: parent.height / 2
        }
        Timer
        {
            property int counter: 0;
            interval: 500
            running: container.visible
            repeat: true
            onTriggered:
            {
                windowTextObject.text += "."
                counter++;
                if(counter==5)
                {
                    windowTextObject.text = windowTextObject.displayText
                    counter = 0;
                }
            }

        }
    }
}
