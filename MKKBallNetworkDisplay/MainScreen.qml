import QtQuick 2.0

Rectangle {
    id: root
    color: mainSettings.bgColor

    MainScreenMusic
    {
        anchors.fill: parent
        visible: mainData.displayedState === "music"
    }

    MainScreenTombola
    {
        anchors.fill: parent
        visible: mainData.displayedState === "tombola"
    }


    Rectangle{
        anchors.fill: parent
        color: "black"
        visible: mainData.displayedState === "black"
    }
}
