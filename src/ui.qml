import QtQuick 2.2
import QtQuick.Window 2.2
import QtWebEngine 1.2


Window {
    id: window
    width: 1100
    height: 800
    visible: true
    title: "Photon"

    WebEngineView {
        id: webEngineView
        anchors.fill: parent
        url: destURL
        profile: WebEngineProfile {
            storageName: "Default"
        }
    }
}
