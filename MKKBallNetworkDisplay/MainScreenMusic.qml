import QtQuick 2.0
//Zenelejátszó képernyő elemei
Item {
    //Gyökérelem
    id: root
    //Idő átkonvertálása szövegformátumba
    function formatTime(timeBit){
        var r = "" + timeBit;
        while (r.length < 2) {
            r = "0" + r;
        }
        return r;
    }

    //Hölgyválasz szöveg
    function ladiesChoice(ladies){
        if(ladies){
            return "Ez a szám Hölgyválasz";
        }
        return "";
    }

    //Háttérkép
    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        visible: mainSettings.bgImage.length > 0
        source: (mainSettings.bgImage.length > 0) ? mainSettings.bgImage : ""
    }

    //Most Játszott felirat
    MkkText {
        id: currentlyPlayedLabel
        cname: "currentlyPlayedLabel"
        cXR: 0.05
        cYR: 0.15
        text: "Most szól: " + mainData.genre1
        window: root
    }

    //Most játszott tánc stílusa
    /*
    MkkText {
        id: currentlyPlayedGenre
        cname: "currentlyPlayedGenre"
        cXR: 0.45
        cYR: 0.05
        text: mainData.genre1
        window: root
    }
    */

    //Most játszott tánc hátralévő idő
    MkkText {
        id: currentlyPlayedTimeLeft
        cname: "currentlyPlayedTimeLeft"
        cXR: 1.1
        cYR: 0.15
        text: mainData.minLeft + ":" + root.formatTime(mainData.secLeft)
        window: root
    }

    //Most játszott tánc előadó és cím
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

    //Következő tánc felirat
    MkkText {
        id: nextPlayedLabel
        cname: "nextPlayedLabel"
        cXR: 0.05
        cYR: 0.75
        text: "Következik: " + mainData.genre2
        window: root
    }

    //Következő tánc stílusa
    /*
    MkkText {
        id: nextPlayedGenre
        cname: "nextPlayedGenre"
        cXR: 0.60
        cYR: 0.75
        text: mainData.genre2
        window: root
    }
    */

    //A DJ üzenete
    MkkText {
        id: news
        cname: "messageOfDJ"
        cXR: 0.05
        cYR: 0.875
        text: mainData.news
        window: root
    }

    //Aktuális idő
    MkkText {
        id: currentTime
        cname: "timeNow"
        cXR: 0.05
        cYR: 0.02
        text: "IDŐ: " + root.formatTime(mainData.hourCurrent) + ":" + root.formatTime(mainData.minCurrent)
        window: root
    }

    //Hölgyválasz
    MkkText {
        id: ladies
        cname: "ladies"
        cXR: 0.05
        cYR: 0.57
        text: root.ladiesChoice(mainData.ladies)
        window: root
    }
}
