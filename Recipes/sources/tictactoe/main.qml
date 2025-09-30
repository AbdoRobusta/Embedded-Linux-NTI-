import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ApplicationWindow {
id: root
width: 360
height: 420
visible: true
title: qsTr("Mini XO — Tic-Tac-Toe")


ColumnLayout {
anchors.fill: parent
anchors.margins: 12
spacing: 12


RowLayout {
spacing: 12
Label { text: "Current:" }
Label {
text: tictactoegame.currentPlayer === 1 ? "X" : "O"
font.bold: true
}
Button { text: "Reset"; onClicked: tictactoegame.reset() }
}


GameBoard {
id: board
anchors.horizontalCenter: parent.horizontalCenter
onSquareClicked: tictactoegame.makeMove(index)
}


Label {
text: tictactoegame.gameOver ? (tictactoegame.winner === 0 ? "Draw!" : (tictactoegame.winner === 1 ? "X wins!" : "O wins!")) : ""
font.pixelSize: 20
horizontalAlignment: Text.AlignHCenter
Layout.alignment: Qt.AlignHCenter
}
}
}
