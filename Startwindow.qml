import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

import "js/style.js" as MyStyle

Item {
    id: startWindow
    width: 640
    height: 480

    property TextField textField_FIO: textField_FIO
    property string etap: "Регулировка"

    Image {
        id: mainCard
        anchors.fill: parent
        source: "images/Card2.png"
        opacity: 1

        Label {
            id: label
            y: 8
            color: "#5a5f70"
            text: qsTr("Логин (ФИО)")
            anchors.right: parent.right
            anchors.rightMargin: 171
            anchors.left: parent.left
            anchors.leftMargin: 171
            styleColor: "#5a5f70"
            font.pixelSize: 50
            font.family: "Lato"
        }

        TextField {
            id: textField_FIO
            selectByMouse: true
            placeholderText: "Введите ФИО"
            text: qsTr("")
            anchors.bottom: label_etap.top
            anchors.bottomMargin: 10
            anchors.top: label.bottom
            anchors.topMargin: 10
            font.family: "Logo"
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10


        }

        Label {
            id: label_etap
            y: 140
            color: "#5a5f70"
            text: qsTr("Режим")
            horizontalAlignment: Text.AlignHCenter
            anchors.right: parent.right
            anchors.rightMargin: 172
            anchors.left: parent.left
            anchors.leftMargin: 172
            styleColor: "#5a5f70"
            font.pixelSize: 50
            font.family: "Lato"
        }

        GroupBox
        {
            id: groupBox
            anchors.bottom: reportButton.top
            anchors.bottomMargin: 5
            anchors.top: label_etap.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            ButtonGroup {
                id: buttonGroup
            }

            ListView {
                id: listView
                anchors.fill: parent
                model: ["Регулировка", "После технической тренировки в НУ", "ПСИ","Проверка неприрывной работы"]
                delegate: RadioDelegate {
                    text: modelData
                    font.pixelSize: 24
                    font.family: "Logo"
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    leftPadding: -1
                    checked: index == 0
                    ButtonGroup.group: buttonGroup

                    onCheckedChanged:  etap = this.text;
                }
            }
        }

        Button {
            id: reportButton
            y: 476
            text: qsTr("Посмотреть результаты")
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10

            onClicked:
            {
                view.currentIndex = 5;
                header.exit_label.text = "Назад";
            }
        }




   }

}


/*##^## Designer {
    D{i:9;anchors_x:171;anchors_y:476}
}
 ##^##*/
