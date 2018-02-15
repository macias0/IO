import QtQuick 2.0
import '../Common'

View {
    // TODOGUI handle waitingToConnect

    Image {
        anchors.fill: parent
        source: 'qrc:/Assets/Images/PaperBoatPhoto.jpg'
        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        id: buttonsContainer
        color: '#40000000'
        x: width / 5
        y: x
        width: Math.min(parent.width, parent.height * 3 / 5) * 2 / 3
        height: width * 5 / 3
        radius: width / 10

        Column {
            anchors.centerIn: parent
            Text {
                id: title
                width: button1.width
                height: button1.height * 1.5
                font.family: statkiFont.name
                text: 'Statki'
                color: '#F76934'
                font.pixelSize: button1.height
            }

            Button {
                id: button1
                width: buttonsContainer.width * 3 / 4
                height: buttonsContainer.height / 6.5
                text: 'Stwórz grę'
                onClicked: {
                    mediator.createServer()
                }
            }

            Button {
                width: button1.width
                height: button1.height
                text: 'Dołącz do gry'
                onClicked: {
                    mediator.joinGame()
                }
            }

            Button {
                width: button1.width
                height: button1.height
                text: 'Autorzy'
                onClicked: {
                    mediator.showCredits()
                }
            }

            Button {
                width: button1.width
                height: button1.height
                text: 'Wyjście'
                onClicked: {
                    mediator.exitGame()
                }
            }
        }
    }

    Rectangle
    {
        id: waitingForConnectionContainer
        anchors.fill: parent;
        visible: mediator.waitingToConnect
        color: "transparent"

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
                id: waitingForConnectionText
                anchors.centerIn: parent;
                property string displayText: "Waiting for connection"
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
                running: waitingForConnectionContainer.visible
                repeat: true
                onTriggered:
                {
                    waitingForConnectionText.text += "."
                    counter++;
                    if(counter==5)
                    {
                        waitingForConnectionText.text = waitingForConnectionText.displayText
                        counter = 0;
                    }
                }

            }
        }
    }


}
