import QtQuick 2.5
import 'qrc:/Common'

View {
    id: gamePrepareBoard

    // TODOGUI add surrender button

    QtObject {
        id: privateSpace
        property var renderedShips: []
        property var otherElementsToDeleteOnNewGame: []
    }

    Rectangle {
        id: smallShipSprite
        width: (boardImage.boardPixelSize * 0.8) / boardSize
        height: width
        radius: width / 2
        color: '#DDD'
        visible: false
        readonly property double offset: (boardImage.boardPixelSize / boardSize - width) / 2.0
    }
    Rectangle {
        id: shotSprite
        width: (boardImage.boardPixelSize * 0.3) / boardSize
        height: width
        radius: width / 2
        color: '#F76934'
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
    Component {
        id: shotComponent
        ShaderEffectSource {
            x: shotSprite.offset + posX * boardImage.boardPixelSize / boardSize
            y: shotSprite.offset + posY * boardImage.boardPixelSize / boardSize
            width: shotSprite.width
            height: shotSprite.height
            sourceItem: shotSprite
            property int posX
            property int posY
        }
    }

    Connections {
        target: mediator

        onShipsNeededChanged: {
            console.log("shipsneeded:", mediator.shipsNeeded)
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
        onRenderShot: {
            if (accurate && enemyBoard) {
                privateSpace.otherElementsToDeleteOnNewGame.push(
                            smallShipComponent.createObject(enemyBoardMouseArea, {"posX": x, "posY": y})
                            )
            }
            privateSpace.otherElementsToDeleteOnNewGame.push(
                        shotComponent.createObject((enemyBoard ? enemyBoardMouseArea : boardMouseArea),
                                                   {"posX": x, "posY": y})
                        )
        }
        onPrepareToNewGame: {
            for (var i = 0 ; i < privateSpace.otherElementsToDeleteOnNewGame.length ; i++)
                privateSpace.otherElementsToDeleteOnNewGame[i].destroy()
            privateSpace.otherElementsToDeleteOnNewGame = []
        }
    }

    Image {
        id: gameBackground
        anchors.fill: parent
        source: 'qrc:/Assets/Images/water.jpeg'
        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        x: (boardImage.x - width) / 2
        y: (parent.height - height) / 2
        width: (parent.width - boardImage.paintedWidth) / 3
        height: width
        radius: width / 6
        color: "#80000000"
        opacity: (mediator.preparingBoardPhase ? 1 : 0)
        Behavior on opacity { NumberAnimation { duration: 500 } }
        Text {
            anchors.fill: parent
            anchors.margins: parent.radius / 2
            font.pixelSize: parent.radius * 0.6
            color: "white"
            font.family: statkiFont.name
            text: "Umieść na planszy 1 statek długości 4, 2 długości 3, 3 długości 2 i 4 długości 1. Statki nie mogą się stykać ani bokami, ani rogami."
            wrapMode: Text.WordWrap
        }
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
            id: enemyBoardMouseArea
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
        x: (parent.width - width) / 2
        y: boardImage.y + boardImage.paintedHeight + (parent.height - boardImage.y - boardImage.paintedHeight - height) / 2
        width: boardImage.paintedWidth / 2
        height: boardImage.paintedHeight / 3
        text: 'Gotowe'
        opacity: (mediator.preparingBoardPhase ? (enabled ? 1 : 0.5) : 0)
        textAlignment: Text.AlignHCenter
        Behavior on opacity { NumberAnimation { duration: 500 } }
        enabled: mediator.boardIsValid && (!mediator.boardHasErrors)
        onClicked: {
            mediator.playerIsReady()
        }
    }

    Text {
        font.pixelSize: (parent.height - boardImage.paintedHeight) / 5
        x: (parent.width - width) / 2
        y: (boardImage.y + (boardImage.height - boardImage.paintedHeight) / 2 - height) / 2
        font.family: statkiFont.name
        color: "white"
        text: mediator.yourTurn ? "Twoja tura" : "Tura przeciwnika"
        opacity: (!mediator.waitingForEnemyBoard) && (!mediator.preparingBoardPhase)
        Behavior on opacity { NumberAnimation { duration: 500 } }
    }

    WaitingWindow {
        isVisible: mediator.waitingForEnemyBoard && (!mediator.preparingBoardPhase)
        messageText: "Oczekiwanie na przeciwnika"
    }

    MouseArea {
        id: inputBlockerOnEnemyTurn
        anchors.fill: parent
        enabled: (!mediator.waitingForEnemyBoard) && (!mediator.preparingBoardPhase) && (!mediator.yourTurn)
    }
}
