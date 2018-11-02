import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4

Window {
    id: root
    title: "MKK Display"
    visible: true
    width: 640
    height: 360

    StackView
    {
        id: stackView
        anchors.fill: parent
    }
    Component.onCompleted:{
        stackView.push("IpSelectPage.qml",{ sv: stackView  })
    }
}
