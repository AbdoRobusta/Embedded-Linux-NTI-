import QtQuick
import QtQuick.Layouts

Item {
    id: root
    signal squareClicked(int index)
    width: 300
    height: 300

    GridLayout {
        id: grid
        anchors.fill: parent
        columns: 3
        rowSpacing: 6
        columnSpacing: 6

        Repeater {
            model: 9
            Square {
                index: model.index
                onClicked: root.squareClicked(index)
            }
        }
    }
}
