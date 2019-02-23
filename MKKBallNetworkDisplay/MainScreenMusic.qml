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
        cXR: 0.025 * root.width
        cYR: 0.15 * root.height
        text: "Most szól: " + mainData.genre1
        window: root
    }

    MkkText {
        id: currentlyPlayedTimeLeft
        cname: "currentlyPlayedTimeLeft"
        cXR: 1.05 * root.width
        cYR: 0.15 * root.height
        text: mainData.minLeft + ":" + root.formatTime(mainData.secLeft)
        window: root
    }

    MkkText {
        id: currentlyPlayed
        cname: "currentlyPlayed"
        width: root.width - currentlyPlayed.x
        cXR: 0.025 * root.width
        cYR: 0.30 * root.height
        wrapMode: Text.Wrap
        text: mainData.artist1 + " - " + mainData.title1
        window: root
    }

    MkkText {
        id: nextPlayedLabel
        cname: "nextPlayedLabel"
        cXR: 0.025 * root.width
        cYR: 0.75 * root.height
        text: "Következik: " + mainData.genre2
        window: root
    }

    MkkText {
        id: news
        cname: "messageOfDJ"
        cXR: 0
        cYR: 0.875 * root.height
        text: mainData.news
        window: root
        property bool empty: true

        NumberAnimation {
            id: textAnim
            target: news
            properties: "cXR"
            running: true
            alwaysRunToEnd: true

            function startOver(newStart,newEnd,newDuration){
                console.log("Starting over")
                from = newStart === null ? root.width : newStart
                to = newEnd === null ? -1*news.width : newEnd
                duration = newDuration === null ? 10000 : newDuration
                start()
            }

            onStopped: {
                var newEnd = -1*news.width;
                console.log("ANIM OVER. x = " + news.cXR + " endGoal = " + newEnd)
                if(news.cXR > newEnd){
                    console.log("Ok, we move a litte more...")
                    startOver(news.cXR,newEnd,1000)
                } else {
                    //We are gone start over
                    console.log("We vanished. Yess!")
                    startOver(null,null,null)
                }
            }
        }

        onWidthChanged: {
            console.log("News width changed: " + width)
            if(empty) {
                console.log("Empty text changed")
                textAnim.stop()
                textAnim.startOver(null,null,null)
            }
            empty = (text.length == 0)
        }

    }

    onWidthChanged: {
        console.log("Width changed: " + root.width)

    }

    MkkText {
        id: currentTime
        cname: "timeNow"
        cXR: 0.025 * root.width
        cYR: 0.02 * root.height
        text: "IDŐ: " + root.formatTime(mainData.hourCurrent) + ":" + root.formatTime(mainData.minCurrent)
        window: root
    }

    MkkText {
        id: ladies
        cname: "ladies"
        cXR: 0.025 * root.width
        cYR: 0.57 * root.height
        text: mainData.ladies ? "Ez a szám Hölgyválasz" : ""
        window: root
    }
}
