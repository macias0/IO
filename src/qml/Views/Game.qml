import QtQuick 2.5
import 'qrc:/Common'

View {
    id: gamePrepareBoard

    // readonly property var neededShips: {return [4, 3, 2, 1]}

    // TODOGUI add surrender button

    QtObject {
        id: privateSpace
        property var renderedShips: []
    }

    Rectangle {
        id: smallShipSprite
        width: (boardImage.boardPixelSize * 0.8) / boardSize
        height: width
        radius: width / 2
        color: "#DDD"
        visible: false
        readonly property double offset: (boardImage.boardPixelSize / boardSize - width) / 2.0
    }

    Component {
        id: smallShipComponent
        ShaderEffectSource {
            x: smallShipSprite.offset + posX * boardImage.boardPixelSize / boardSize
            y: smallShipSprite.offset + posY * boardImage.boardPixelSize / boardSize
            width: smallShipSprite.width
            height: smallShipSprite.height
            sourceItem: smallShipSprite
            property int posX
            property int posY
        }
    }

    Connections {
        target: mediator

        onShipsNeededChanged: {
        }
        onRenderShip: { //(int id, int x, int y, int length, bool horizontal);
            privateSpace.renderedShips[id] = smallShipComponent.createObject(boardMouseArea, {"posX": x, "posY": y});
        }
        onRemoveShip: { //(int id);
            privateSpace.renderedShips[id].destroy()
        }
        onTileAttacked: {
            console.log("tile was attacked: ", x, y)
        }
    }

    Image {
        id: gameBackground
        anchors.fill: parent
        source: 'qrc:/Assets/Images/water.jpeg'
        fillMode: Image.PreserveAspectCrop
    }

    Image {
        id: boardImage
        x: (mediator.preparingBoardPhase ? (parent.width - width) / 2 : parent.width * 8 / 18 - width)
        y: (parent.height - height) / 2
        width: Math.min(parent.width / 3, parent.height * 2 / 3)
        height: width
        source: 'qrc:/Assets/Images/grid.png'
        mipmap: true
        fillMode: Image.PreserveAspectFit
        readonly property double boardPixelSize: Math.min(width, height)
        Behavior on x { NumberAnimation { duration: 500 } }

        MouseArea {
            id: boardMouseArea
            anchors.centerIn: parent
            enabled: mediator.preparingBoardPhase
            width: Math.min(parent.width, parent.height)
            height: width
            property int lastModifiedX
            property int lastModifiedY
            property bool gestureIsValid
            onPressed: {
                var posX = Math.floor((mouseX / width) * boardSize)
                var posY = Math.floor((mouseY / height) * boardSize)
                if ((posX < 0)
                        || (posX >= boardSize)
                        || (posY < 0 )
                        || (posY >= boardSize)) {
                    gestureIsValid = false
                    return
                }
                gestureIsValid = true
                lastModifiedX = posX
                lastModifiedY = posY
                mediator.toggleFirstTile(posX, posY)
            }
            onPositionChanged: {
                var posX = Math.floor((mouseX / width) * boardSize)
                var posY = Math.floor((mouseY / height) * boardSize)
                if ((posX < 0)
                        || (posX >= boardSize)
                        || (posY < 0 )
                        || (posY >= boardSize)
                        || (!gestureIsValid)) {
                    return
                }
                if ((posX != lastModifiedX) || (posY != lastModifiedY)) {
                    lastModifiedX = posX
                    lastModifiedY = posY
                    mediator.toggleTile(posX, posY)
                }
            }
        }
    }

    Image {
        id: enemyBoardImage
        x: parent.width * 10 / 18
        y: (parent.height - height) / 2
        width: Math.min(parent.width / 3, parent.height * 2 / 3)
        height: width
        source: 'qrc:/Assets/Images/grid.png'
        mipmap: true
        fillMode: Image.PreserveAspectFit
        readonly property double boardPixelSize: Math.min(width, height)
        opacity: (mediator.preparingBoardPhase ? 0 : 1)
        Behavior on opacity { NumberAnimation { duration: 500 } }

        MouseArea {
            enabled: !mediator.preparingBoardPhase && mediator.yourTurn
            anchors.fill: parent
            onPressed: {
                var posX = Math.floor((mouseX / width) * boardSize)
                var posY = Math.floor((mouseY / height) * boardSize)
                if ((posX < 0)
                        || (posX >= boardSize)
                        || (posY < 0 )
                        || (posY >= boardSize)) {
                    return
                }
                mediator.attackTile(posX, posY)
                console.log("attacking tile:", posX, posY)
            }
        }
    }



    Button {
        // TODOGUI make the button horizontally centered
        x: (parent.width - width) / 2
        y: boardImage.y + boardImage.paintedHeight + (parent.height - boardImage.y - boardImage.paintedHeight - height) / 2
        width: boardImage.paintedWidth / 2
        height: boardImage.paintedHeight / 3
        text: 'Gotowe'
        opacity: (enabled ? 1 : 0.5)
        Behavior on opacity { NumberAnimation { duration: 500 } }
        enabled: mediator.boardIsValid
        onClicked: {
            mediator.playerIsReady()
        }
    }
}
