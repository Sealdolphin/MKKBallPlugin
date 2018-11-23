import QtQuick 2.0

Item {
    id : root

    Timer {
        interval: 50; running: (root.visible && mainData.rotating) ; repeat: true
        onTriggered: {
            mainData.rotateWinningTicketStep()
        }
    }

    MkkText {
        id: winningTicket
        cname: "winningTicket"
        anchors.centerIn: parent
        text: mainData.winningTicket
        window: root
        font.pixelSize: parent.height / 3
    }
}
