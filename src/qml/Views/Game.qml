import QtQuick 2.5

View {
    id: game

    Image {
        anchors.fill: parent
        source: 'qrc:/Assets/Images/water.jpeg'
        fillMode: Image.PreserveAspectCrop
    }

    Image {
        anchors.fill: parent
        anchors.margins: Math.min(parent.width, parent.height) / 4
        source: 'qrc:/Assets/Images/grid.png'
        mipmap: true
        fillMode: Image.PreserveAspectFit
    }
}
