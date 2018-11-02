import QtQuick 2.0

Text {
    id : root
    property string cname : "Unknown"
    property double cXR: 0.1
    property double cYR: 0.1
    property var window

    x: mainSettings.getX(cname,cXR,window.height)
    y: mainSettings.getY(cname,cYR,window.height)
    color: mainSettings.getFontColor(cname)
    font.pixelSize: mainSettings.getFontSize(cname,window.height)
}
