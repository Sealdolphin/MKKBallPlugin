import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4

Item {
    property var sv
    Rectangle {
        color: "red"
        anchors.fill: parent


        Rectangle {
            color: "lightyellow"
            height: ipconnect.cursorRectangle.height
            width: ipconnect.width
            y: ipconnect.y
            x: ipconnect.x
        }
        TextEdit {
            id: ipconnect
            x : 0
            width: parent.width
            y: 20
            height: 40
            text: qsTr("192.168.1.2:6112")
            font.pixelSize: 40
            font.family: "Times New Roman"
            selectionColor: "#004a80"
            font.wordSpacing: 0.1
            font.weight: Font.Light
        }


        Button {
            id: connectButton
            x: 19
            y: 75
            text: qsTr("Connect")
            onReleased: roConnection.connect(ipconnect.text)
        }
    }
    Connections
    {
        target: roConnection
        onConnected: {
            sv.push("MainScreen.qml")
        }
    }

    Component.onCompleted: {
        roConnection
    }
}
