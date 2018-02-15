import QtQuick 2.0

View {
    id: credits
    anchors.fill: parent;
    Rectangle
    {
        anchors.fill: parent
        color: "black"


        Flickable
        {
            id: flick
            width: parent.width * 0.75
            height: parent.height * 1.2
            anchors.centerIn: parent;
            transform: Rotation {
                origin.x: text.width / 2
                origin.y: text.height / 2
                axis { x: 1; y: 0; z: 0 }
                angle: 60
            }

            flickableDirection: Flickable.VerticalFlick
            interactive: false;
            contentWidth: width
            contentHeight: height


            Text
            {
                id: text
                font.family: starWarsFont.name
                text: '<div style="font-size:'+font.pixelSize*2.5+'px;">Autorzy:</div><br>-Maciej Gurgul<br>-Filip Hazubski<br><br>Gra powstała jako projekt zaliczeniowy z <u>Inżynierii Oprogramowania</u>.
                <br><br>Aplikacja wykonana całkowicie przez jej autorów, stworzona w technologii Qt (C++/QML) na licencji LGPL-3.0
                <br><div style="font-size:'+font.pixelSize*1.5+'px;">Podziękowania dla:</div><br><br>
                - Miguel Á. Padriñán<br>
                - Misti<br>
                - Lukas<br><br>
                za udostępnienie zasobów.'
                color: "#e5b13a"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                y: flick.height
                width: parent.width
                height: parent.height
                font.pixelSize: parent.width * 0.08
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop

            }

        }

        NumberAnimation
        {
            target: flick
            property: "contentY"
            to: flick.contentHeight * 6
            from: flick.contentY
            duration: 30000
            running: credits.isActive
            onRunningChanged:
            {
                if(!running)
                    mediator.exitFromCredits();
            }

        }


    }
}
