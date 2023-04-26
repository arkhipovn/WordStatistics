import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import QtQuick.Dialogs 1.3

import QtQuick.Layouts 1.12
import Qt.labs.settings 1.0

import Items 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Word statistics")

    font.capitalization: Font.MixedCase
    font.pixelSize: Qt.application.font.pixelSize

    property real indent: font.pixelSize * 2

    ToolTip {
        id: toolTip
        delay: 0; timeout: 2000
        anchors.centerIn: parent

        function view(str, color = Material.primary) {
            toolTip.Material.background = color
            text = str
            open()
        }
    }

    header: Flow {
        id: buttonFlow
        width: parent.width
        leftPadding: window.indent; rightPadding: window.indent
        spacing: window.font.pixelSize

        property bool started: false
        property bool paused: false

        Connections {
            target: controller
            onParseFinished: {
                buttonFlow.started = false
                buttonFlow.paused = false
            }
        }

        Button {
            text: qsTr("Open")
            onClicked: fileDialogLoader.active = true
            enabled: !buttonFlow.started
        }

        Loader {
            sourceComponent: buttonFlow.started ? pauseButton
                                                : startButton
        }

        Loader {
            active: buttonFlow.started
            visible: active
            sourceComponent: Button {
                text: qsTr("Stop")
                onClicked: controller.stop()
            }
        }

        Loader {
            active: !buttonFlow.started && statListView.count > 0
            visible: active
            sourceComponent: Button {
                text: qsTr("Clear")
                onClicked: controller.clear()
            }
        }

        Component {
            id: startButton

            Button {
                enabled: programSettings.fileName.length > 0
                text: qsTr("Start")

                onClicked: {
                    if(programSettings.fileExists()) {
                        buttonFlow.started = controller.start()
                    }
                    else {
                        toolTip.view(qsTr("File does not exist"), "red")
                    }
                }
            }
        }

        Component {
            id: pauseButton

            Button {
                text: buttonFlow.paused ? qsTr("Start")
                                        : qsTr("Pause")

                onClicked: {
                    if(buttonFlow.paused) {
                        controller.unpause()
                    }
                    else {
                        controller.pause()
                    }

                    buttonFlow.paused = !buttonFlow.paused
                }
            }
        }
    }   // Flow

    RowLayout {
        anchors {
            left: parent.left; right: parent.right;
            leftMargin: window.indent;
            rightMargin: window.indent
        }
        height: parent.height

        spacing: window.indent

        BarChart {
            Layout.fillWidth: true; Layout.preferredWidth: 3
            Layout.fillHeight: true
            wordStatistics: controller.wordStatistics
            font: window.font
        }

        StatListView {
            id: statListView
            Layout.fillWidth: true; Layout.preferredWidth: 1
            Layout.fillHeight: true
            model: controller.wordStatistics
        }   // StatListView

    }   // RowLayout

    footer: Column {
        width: parent.width

        // Выбранный файл
        Label {
            anchors.right: parent.right
            width: Math.min(parent.width, implicitWidth)
            topPadding: window.indent * 0.5; bottomPadding: window.indent * 0.5
            leftPadding: window.indent; rightPadding: window.indent
            elide: Label.ElideMiddle
            text: programSettings.fileName
        }

        ProgressBar {
            visible: buttonFlow.started
            width: parent.width
            from: 0
            to: controller.wordStatistics.numberOfLines
            value: controller.wordStatistics.numberOfParsedLines
        }   // ProgressBar
    }

    Loader {
        id: fileDialogLoader
        active: false

        sourceComponent: FileDialog {
            id: fileDialog
            title: qsTr("Please choose a file")
            visible: true

            onAccepted: {
                programSettings.setFileUrl(fileDialog.fileUrl)
                fileDialogLoader.active = false
            }

            onRejected: fileDialogLoader.active = false
        }
    }

}


