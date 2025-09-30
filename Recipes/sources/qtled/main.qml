import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    width: 400
    height: 300
    visible: true
    title: "LED Toggle"

    Rectangle {
        anchors.fill: parent
        color: "#1e1e2e"

        Column {
            anchors.centerIn: parent
            spacing: 30

            Item {
                id: ledWrap
                width: 200
                height: 200

                // Glow using radial gradient
                Rectangle {
                    id: glow
                    anchors.centerIn: parent
                    width: 200
                    height: 200
                    radius: width / 2
                    opacity: ledController.ledOn ? 0.6 : 0.0
                    Behavior on opacity { NumberAnimation { duration: 200 } }

                    gradient: Gradient {
                        GradientStop { position: 0.0; color: ledController.ledOn ? "#00ff6a88" : "transparent" }
                        GradientStop { position: 1.0; color: "transparent" }
                    }
                }

                // LED circle
                Rectangle {
                    id: led
                    anchors.centerIn: parent
                    width: 120
                    height: 120
                    radius: width / 2
                    color: ledController.ledOn ? "#00ff6a" : "#444"
                    border.color: "white"
                    border.width: 3
                    opacity: ledController.ledOn ? 1.0 : 0.5

                    Behavior on color { ColorAnimation { duration: 180 } }
                    Behavior on opacity { NumberAnimation { duration: 180 } }
                }
            }

            // Toggle Button
            Button {
                text: ledController.ledOn ? "Turn OFF" : "Turn ON"
                font.bold: true
                font.pointSize: 14
                background: Rectangle {
                    color: ledController.ledOn ? "#ff5555" : "#3aaaff"
                    radius: 12
                }
                onClicked: ledController.toggleLed()
                width: 140
                height: 50
            }
        }
    }
}
