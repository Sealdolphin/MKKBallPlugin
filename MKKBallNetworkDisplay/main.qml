import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4

QtObject {
    id: rootItem
    property var controlWindow: ControlWindow {

        title: qsTr("Display control")

    }

    property var mainWindow: MainWindow {

    }

    Component.onCompleted: {
        rootItem.controlWindow.mainWindow = mainWindow;
    }

}
