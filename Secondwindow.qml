import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

import "js/style.js" as MyStyle

Item {
    id: deviceWindow
    width: 1024
    height: 768

    //    property TextField textField_FIO: textField_FIO
    //    property string etap: "Регулировка"


    Image {
        id: mainCard
        anchors.fill: parent
        source: "images/Card2.png"
        opacity: 1

        GroupBox {
            id: groupBox_ist_1
            x: 8
            y: 13
            width: 200
            height: 87
            bottomPadding: 0
            topPadding: 16
            rightPadding: 0
            leftPadding: 0
            font.pointSize: 8
            font.family: "Logo"
            title: qsTr("Источник питания №1")

            ColumnLayout {
                anchors.bottomMargin: 5
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.fill: parent

                Label {
                    id: label
                    text: qsTr("Прибор не подключен")
                }

                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        id: label1
                        text: qsTr("IP :")
                    }

                    TextField {
                        id: textField
                        text: qsTr("Text Field")
                        Layout.preferredHeight: 25
                        Layout.preferredWidth: 117
                    }

                    Button {
                        id: button
                        text: qsTr("Найти")
                        Layout.fillWidth: true
                        Layout.preferredHeight: 25
                        Layout.preferredWidth: 33
                    }
                }

                Button {
                    id: button1
                    text: qsTr("Подключить")
                    Layout.fillWidth: true
                    Layout.preferredHeight: 21
                    Layout.preferredWidth: 179
                }
            }
        }

        GroupBox {
            id: groupBox_ist_2
            x: 8
            y: 204
            width: 200
            height: 85
            leftPadding: 0
            title: qsTr("Источник питания №2")
        }

        GroupBox {
            id: groupBox_gsg
            x: 8
            y: 295
            width: 200
            height: 83
            leftPadding: 0
            title: qsTr("Имитатор GPS/GLONASS")
        }

        GroupBox {
            id: groupBox_os
            x: 214
            y: 13
            width: 200
            height: 87
            leftPadding: 0
            title: qsTr("Осцилограф")
        }

        GroupBox {
            id: groupBox_tp
            x: 220
            y: 204
            width: 200
            height: 85
            leftPadding: 0
            title: qsTr("Пульт")
        }

        GroupBox {
            id: groupBox_pg2j
            x: 220
            y: 295
            width: 200
            height: 83
            leftPadding: 0
            title: qsTr("Климатическая камера PG-2J")
        }


    }

}

/*##^## Designer {
    D{i:22;anchors_x:9;anchors_y:0}
}
 ##^##*/
