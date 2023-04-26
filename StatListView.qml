import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ListView {
    id: listView
    interactive: contentHeight > height

    header: Label {
        width: listView.width
        visible: listView.count > 0
        text: qsTr("Statistics")
        color: Material.accentColor
        font.bold: true
        elide: Qt.ElideRight
        bottomPadding: font.pixelSize
    }

    delegate: StatLine {
        width: ListView.view.width
        title: wordText
        value: wordCount
    }
}
