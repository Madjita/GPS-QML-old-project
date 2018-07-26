import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item
{

    ColumnLayout {
        x: 0
        y: 0
        width: 56
        height: 78
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Label {
            id: label
            text: qsTr("Загрузка имитатора на имитацию сигнала...")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        BusyIndicator {
            id: busyIndicator
            spacing: 10

            running: true
        }
    }

}
