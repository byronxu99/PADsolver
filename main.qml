import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.0

import PadGUI 1.0

ApplicationWindow {
    visible: true

    width: 640
    height: 480

//    maximumHeight: height
//    maximumWidth: width
    minimumWidth: width
    minimumHeight: height

    title: qsTr("PAD")

    header: ToolBar{
        RowLayout {
            anchors.fill: parent

            Text {
                text: "PAD"
                anchors.centerIn: parent
            }
        }
    }

    Item {
            id: content
            anchors.fill: parent
            anchors.margins: 10

        Item {
            id: padItem

            property int orbsX: 6
            property int orbsY: 5
            property int orbsize: parent.width/orbsX < parent.height/orbsY/1.2? parent.width/orbsX : parent.height/orbsY/1.2

            width:  orbsize * orbsX
            height: orbsize * orbsY * 1.2;

            anchors.centerIn: parent

            Item {
                id: statusbar

                width: parent.width
                height: 0.1 * padGUI.height

                anchors.bottom: padGUI.top
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    text: padGUI.comboCount + " Combos"

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                }

                Text {
                    text: padGUI.movesCount + " Moves"

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                }

                Behavior on width  { NumberAnimation { } }
                Behavior on height { NumberAnimation { } }
            }

            PadGUI {
                id: padGUI

                width: parent.width;
                height: parent.height / 1.2;

                anchors.centerIn: parent

                mouseX: mouseArea.mouseX
                mouseY: mouseArea.mouseY
                mouseDown: mouseArea.pressedButtons & Qt.LeftButton != 0

                onRepaintSignal: update()
                onComboCountChanged: statusbar.update()
                onMovesCountChanged: statusbar.update()

                Behavior on width  { NumberAnimation { } }
                Behavior on height { NumberAnimation { } }
            }


            Rectangle {
                id: loadScreen

                visible: padGUI.loading
                color: "#aaaaaaaa"

                anchors.fill: padGUI

                BusyIndicator {
                    anchors.centerIn: parent
                }

            }

            RowLayout {
                id: buttons

                width: parent.width
                height: 0.1 * padGUI.height
                spacing: 0.02 * width

                anchors.top: padGUI.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: "New Board"
                    onClicked: padGUI.newBoard()
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                Button {
                    text: padGUI.showPath? "Showing path" : "Hiding path"
                    onClicked: padGUI.showPath = ! padGUI.showPath
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                Button {
                    text: "Reset"
                    onClicked: padGUI.resetBoard()
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                Behavior on width  { NumberAnimation { } }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: padGUI

                acceptedButtons: Qt.LeftButton
                hoverEnabled: true

                onExited: padGUI.mouseExited()

            }
        }
    }


    /*
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1 {
        }

        Page {
            Label {
                text: qsTr("Second page")
                anchors.centerIn: parent
            }
        }
    }
    */

   /*
    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
    }
    */
}
