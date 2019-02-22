import QtQuick 2.0

Item {
    id: root
    function formatTime(timeBit){
        var r = "" + timeBit;
        while (r.length < 2) {
            r = "0" + r;
        }
        return r;
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
        cYR: 0.15
        text: "Most szól: " + mainData.genre1
        window: root
    }

    MkkText {
        id: currentlyPlayedTimeLeft
        cname: "currentlyPlayedTimeLeft"
        cXR: 1.05
        cYR: 0.15
        text: mainData.minLeft + ":" + root.formatTime(mainData.secLeft)
        window: root
    }

    MkkText {
        id: currentlyPlayed
        cname: "currentlyPlayed"
        width: root.width - currentlyPlayed.x
        cXR: 0.05
        cYR: 0.30
        wrapMode: Text.Wrap
        text: mainData.artist1 + " - " + mainData.title1
        window: root
    }

    MkkText {
        id: nextPlayedLabel
        cname: "nextPlayedLabel"
        cXR: 0.05
        cYR: 0.75
        text: "Következik: " + mainData.genre2
        window: root
    }

    MkkText {
        id: news
        cname: "messageOfDJ"
        cXR: 0.05
        cYR: 0.875     
        text: mainData.news
        window: root
    }

    MkkText {
        id: currentTime
        cname: "timeNow"
        cXR: 0.05
        cYR: 0.02
        text: "IDŐ: " + root.formatTime(mainData.hourCurrent) + ":" + root.formatTime(mainData.minCurrent)
        window: root
    }

    MkkText {
        id: ladies
        cname: "ladies"
        cXR: 0.05
        cYR: 0.57
        text: mainData.ladies ? "Ez a szám Hölgyválasz" : ""
        window: root
    }
}
