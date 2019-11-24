import QtQuick 2.9
import QtQuick.Window 2.2
import AppEnums 1.0
import QtQuick.Controls 2.0

Window {
    id: rootScreen
    width: 480
    height: 720
    visible: true

    Text {
        text: qsTr("Phong Đẹp Trai")
        font.pixelSize: 15
        anchors.centerIn: parent
    }
}
