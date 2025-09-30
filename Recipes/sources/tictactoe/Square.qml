import QtQuick

Rectangle {
    id: square
    width: 90
    height: 90
    radius: 8
    color: "white"
    border.width: 2

    property int index: 0
    property int value: tictactoegame.board[index]
    signal clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: square.clicked()
        enabled: value === 0 && !tictactoegame.gameOver
    }

    Text {
        anchors.centerIn: parent
        text: value === 1 ? "X" : (value === -1 ? "O" : "")
        font.pixelSize: 40
        font.bold: true
    }
}
