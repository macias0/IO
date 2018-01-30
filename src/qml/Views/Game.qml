import QtQuick 2.5

/* to implement (usage of):

    Q_PROPERTY(QList<int>  shipsNeeded READ shipsNeeded NOTIFY shipsNeededChanged)

    // called when user presses the mouse down
    Q_INVOKABLE void toggleFirstTile(QList<int> position);
    // called every time user drags mouse over another tile while holding the mouse down
    Q_INVOKABLE void toggleTile(QList<int> position);

signals:
    void renderShip(int id, int x, int y, int length, bool horizontal);
    void removeShip(int id);

  */

View {
    id: game

    // readonly property var neededShips: {return [4, 3, 2, 1]}

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
            console.log("Needed ships:", mediator.shipsNeeded)
        }
        onRenderShip: { //(int id, int x, int y, int length, bool horizontal);
            privateSpace.renderedShips[id] = smallShipComponent.createObject(boardMouseArea, {"posX": x, "posY": y});
        }
        onRemoveShip: { //(int id);
            privateSpace.renderedShips[id].destroy()
        }
    }

    Image {
        anchors.fill: parent
        source: 'qrc:/Assets/Images/water.jpeg'
        fillMode: Image.PreserveAspectCrop
    }

    Image {
        id: boardImage
        anchors.fill: parent
        anchors.margins: Math.min(parent.width, parent.height) / 4
        source: 'qrc:/Assets/Images/grid.png'
        mipmap: true
        fillMode: Image.PreserveAspectFit
        readonly property double boardPixelSize: Math.min(width, height)

        MouseArea {
            id: boardMouseArea
            anchors.centerIn: parent
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
}
