import QtQuick 2.0
import '../Common'

View {

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
                height: buttonsContainer.height / 5.5
                text: 'Graj'
                onClicked: {
                    mediator.startGame()
                }
            }

            Button {
                width: button1.width
                height: button1.height
                text: 'Autorzy'
            }

            Button {
                width: button1.width
                height: button1.height
                text: 'Wyjście'
                onClicked: {
                    mediator.endGame()
                }
            }
        }
    }


}
