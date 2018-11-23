import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Window 2.11
import QtQuick 2.0
import QtQuick.Dialogs 1.0

Window {
    property var mainWindow
    color: "#cdcdce"
    visible: true
    width: 640
    height: 480

    Button {
        id: button
        x: 43
        y: 32
        text: qsTr("Toggle FullScreen")
        onReleased: {
            console.log("AAAAA");
            if (mainWindow.visibility === Window.FullScreen)
            {
                mainWindow.visibility = Window.Windowed;
            }
            else
            {
                mainWindow.visibility = Window.FullScreen;
            }
        }
    }

    Button {
        id: button1
        x: 43
        y: 278
        width: 136
        height: 40
        text: qsTr("Music")
        onReleased: {
             mainData.displayedState = "music"
        }
    }

    Button {
        id: button2
        x: 43
        y: 334
        width: 136
        height: 40
        text: qsTr("Tombola")
        onReleased: {
             mainData.displayedState = "tombola"
        }
    }

    Button {
        id: button3
        x: 43
        y: 394
        width: 136
        height: 40
        text: qsTr("Black")
        onReleased: {
            mainData.displayedState = "black"
        }
    }

    Text {
        id: text1
        x: 307
        y: 394
        width: 293
        height: 34
        text: "Loaded tickets: " + mainData.tickets.length
        opacity: 1
        font.pixelSize: 20
    }

    Button {
        id: button4
        x: 297
        y: 284
        text: qsTr("Get")
        onReleased: {
            mainData.getTombola()
        }
    }

    Button {
        id: button5
        x: 410
        y: 284
        text: qsTr("Rotate")
        onReleased: {
            mainData.rotateTombola()
        }
    }


    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            mainData.loadTombola(fileDialog.fileUrl)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    Button {
        id: button6
        x: 532
        y: 284
        text: qsTr("Load")
        onReleased: {
            fileDialog.open()
        }
    }

    Text {
        id: text2
        x: 308
        y: 426
        width: 324
        height: 23
        text: mainData.result
        font.pixelSize: 20
    }

    Rectangle
    {
        anchors.fill: newsInput
        anchors.topMargin: -10
        anchors.bottomMargin: -10
        color: "white"
    }

    TextInput {
        id: newsInput
        x: 198
        y: 42
        width: 423
        height: 20
        text: qsTr("Jó a buli ŰÜ üű őö Őö é É")
        font.pixelSize: 20
    }

    Button {
        id: button7
        x: 410
        y: 83
        text: qsTr("Hírdet")
        onReleased: {
            mainData.news = newsInput.text
        }
    }

    Button {
        id: button8
        x: 521
        y: 83
        text: qsTr("Töröl")
        onReleased: {
            mainData.news = ""
            newsInput.text = ""
        }

    }

}
