import QtQuick 2.0
import QtQuick.Window 2.11

Window {
    id: root
    visible: true
    width: 640
    height: 480
    visibility: "Windowed"
    title: qsTr("Hello World")
    color: mainSettings.bgColor

    function timeString(min, secs) {
        var r = "" + secs;
        while (r.length < 2) {
            r = "0" + r;
        }
        return min + ":" + r;
    }

    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        visible: mainSettings.bgImage.length > 0
        source: (mainSettings.bgImage.length > 0) ? mainSettings.bgImage : ""
    }

    MkkText {
        id: currentlyPlayedLabel
        cname: "currentlyPlayedLabel"
        cXR: 0.05
        cYR: 0.05
        text: "Most szól: "
        window: root
    }

    MkkText {
        id: currentlyPlayedGenre
        cname: "currentlyPlayedGenre"
        cXR: 0.45
        cYR: 0.05
        text: mainData.genre1
        window: root
    }
    MkkText {
        id: currentlyPlayedTimeLeft
        cname: "currentlyPlayedTimeLeft"
        cXR: 1.1
        cYR: 0.05
        text: root.timeString(mainData.minLeft,mainData.secLeft)
        window: root
    }

    MkkText {
        id: currentlyPlayed
        cname: "currentlyPlayed"
        width: root.width - currentlyPlayed.x
        cXR: 0.05
        cYR: 0.20
        wrapMode: Text.Wrap
        text: mainData.artist1 + " - " + mainData.title1
        window: root
    }

    MkkText {
        id: nextPlayedLabel
        cname: "nextPlayedLabel"
        cXR: 0.05
        cYR: 0.75
        text: "Következik: "
        window: root
    }

    MkkText {
        id: nextPlayedGenre
        cname: "nextPlayedGenre"
        cXR: 0.60
        cYR: 0.75
        text: mainData.genre2
        window: root
    }
}
