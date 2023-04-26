import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.12

import QtQuick.Layouts 1.12

import Items 1.0

RowLayout {
    property alias title: titleLabel.text
    property alias value: valueLabel.text

    property alias titleFont: titleLabel.font
    property alias valueFont: valueLabel.font

    property alias titleColor: titleLabel.color
    property alias valueColor: valueLabel.color

    Label {
        id: titleLabel
        Layout.fillWidth: true
        Layout.maximumWidth: Math.min(parent.width * 0.9, implicitWidth)
        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
        elide: Qt.ElideRight
    }

    Line {
        Layout.alignment: Qt.AlignVCenter
        Layout.fillWidth: true
        color: Material.shade(Material.foreground, Material.Shade100)
        spacing: 4; dashSize: 2
    }

    Label {
        id: valueLabel
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
        Layout.maximumWidth: implicitWidth
        elide: Qt.ElideRight
    }
}
