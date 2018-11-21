import QtQuick 2.0

Item {
    id : root

    MkkText {
        id: winningTicket
        cname: "winningTicket"
        anchors.centerIn: parent
        text: mainData.winningTicket
        window: root
        font.pixelSize: parent.height / 3
    }
}
