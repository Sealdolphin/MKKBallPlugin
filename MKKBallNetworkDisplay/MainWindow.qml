import QtQuick 2.0
import QtQuick.Window 2.11

Window {
    id: root
    visible: true
    width: 640
    height: 480
    visibility: "Windowed"
    title: qsTr("MKK Display")

    MainScreen {
        anchors.fill: parent
    }
}
