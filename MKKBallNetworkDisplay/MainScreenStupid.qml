import QtQuick 2.0

Rectangle {
    id: root
    color: mainSettings.bgColor

    function timeString(min, secs) {
        var r = "" + secs;
        while (r.length < 2) {
            r = "0" + r;
        }
        return min + ":" + r;
    }
    property double fontSize: 0.2
    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        visible: mainSettings.bgImage.length > 0
        source: (mainSettings.bgImage.length > 0) ? mainSettings.bgImage : ""
    }

    MkkText {
        id: currentlyPlayedGenre
        cname: "currentlyPlayedGenre"
        cYR: 0.05
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: root.height * root.fontSize
        text: mainData.genre1
        window: root
    }
    MkkText {
        id: currentlyPlayedTimeLeft
        cname: "currentlyPlayedTimeLeft"
        font.pixelSize: root.height * root.fontSize
        anchors.horizontalCenter: parent.horizontalCenter
        cYR: 0.25
        text: " " + root.timeString(mainData.minLeft,mainData.secLeft)
        window: root
    }

    MkkText {
        id: nextPlayedLabel
        cname: "nextPlayedLabel"
        cXR: 0.03
        cYR: 0.5
        font.pixelSize: root.height * root.fontSize
        text: "Köv:"
        window: root
    }

    MkkText {
        id: nextPlayedGenre
        cname: "nextPlayedGenre"
        cYR: 0.75
        font.pixelSize: root.height  * root.fontSize
        anchors.horizontalCenter: parent.horizontalCenter
        text: " " + mainData.genre2
        window: root
    }
}
