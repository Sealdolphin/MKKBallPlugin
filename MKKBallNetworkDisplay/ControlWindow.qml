import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Window 2.11

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

}
