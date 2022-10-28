import Qt.labs.platform 1.1
import QtQuick 2.13
//import QtQuick.Controls 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.14
import QtQuick.Shapes 1.0
import QtQuick.Window 2.2
import "qrc:/QuickQanava" as Qan

ApplicationWindow {
    id: window

    visible: true
    width: 1280
    height: 720
    title: "QuickQanavaPlayground"

    Pane { anchors.fill: parent }
    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: tabBar
            Layout.preferredWidth: 450;
            TabButton { text: qsTr("button1") }
            TabButton { text: qsTr("button2") }
        }
        StackLayout {
            clip: true
            Layout.fillWidth: true; Layout.fillHeight: true
            currentIndex: tabBar.currentIndex
            Item { Qan.GraphView{anchors.fill: parent } }
            Item { Qan.Node{anchors.fill: parent} }
        }
    }
}

