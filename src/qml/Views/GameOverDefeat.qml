import QtQuick 2.0
import '../Common'

View {

    Image {
        anchors.fill: parent
        source: 'qrc:/Assets/Images/SunsetBoat.jpeg'
        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        id: buttonsContainer
        color: '#40000000'
        x: width / 5
        y: x
        width: Math.min(parent.width, parent.height * 3 / 2) * 1 / 3
        height: width / 2
        radius: width / 10

        Column {
            anchors.centerIn: parent
            Text {
                id: title
                width: button1.width
                height: button1.height * 1
                font.family: statkiFont.name
                text: 'Przegrana'
                color: '#F76934'
                font.pixelSize: button1.height * 0.7
            }

            Button {
                id: button1
                width: buttonsContainer.width * 3 / 4
                height: buttonsContainer.height / 2.5
                text: 'Powrót'
                onClicked: {
                    mediator.goBackToMainMenu()
                }
            }
        }
    }
}
