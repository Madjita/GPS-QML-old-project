import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Universal 2.1


Item {
    id: item1

    readonly property int dpi: Screen.pixelDensity * 25.4
    function dp(x){ return (dpi < 120) ? x : x*(dpi/160); }

    Rectangle
    {
        id: mainItem

        // Пересчёт независимых от плотности пикселей в физические пиксели устройства

        width: 300
        height: 200
        radius: 10
        border.width: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        Rectangle {
            id: menuRectItemDashboard
            width: 600
            height: dp(24)
            color: "#7e827f"
            anchors.left: parent.left
            Text {
                id: nameStart
                color: "#ffffff"
                text: "Выбор этапа работы"
                padding: 5
                font.bold: true
                anchors.fill: parent
                renderType: Text.NativeRendering
            }
            border.color: "#45110000"
            anchors.top: parent.top
            border.width: 1
            anchors.right: parent.right
        }

        GridLayout {
            id: grid
            width: 260
            anchors.top: menuRectItemDashboard.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.topMargin: 0
            columnSpacing: 5
            rowSpacing: 10
            columns: 2
            rows: 0




            Text {
                id: text1
                padding: 5
                text: qsTr("Укажите ФИО:")
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: false
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                font.pixelSize: 12
            }

            TextField {
                id: textInputName
                width: 300
                height: 20
                text: qsTr("Смоглюк Сергей Юрьевич")
                Layout.minimumWidth: 130
                Layout.fillWidth: true
                font.family: "Arial"
                font.pixelSize: 13
            }

            GroupBox {
                id: groupBox
                width: 360
                height: 300
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                title: qsTr("Режим")

                ColumnLayout {
                    id: columnLayout
                    anchors.fill: parent

                    ExclusiveGroup { id: tabPositionGroup }

                    RadioButton {
                        id: radioButton_mode_Regylirovka
                        text: qsTr("Регулировка")
                        checked: true
                        exclusiveGroup: tabPositionGroup
                    }

                    RadioButton {
                        id: radioButton_mode_NextTex
                        text: qsTr("После технической тренировки в НУ")
                        exclusiveGroup: tabPositionGroup
                    }

                    RadioButton {
                        id: radioButton_mode_PSI
                        text: qsTr("ПСИ")
                        exclusiveGroup: tabPositionGroup
                    }

                    RadioButton {
                        id: radioButton_mode_LongWork
                        text: qsTr("Проверка неприрывной работы")
                        exclusiveGroup: tabPositionGroup
                    }
                }
            }

            RowLayout
            {
                Layout.minimumWidth: 280
                Layout.fillHeight: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                spacing: 5
                Layout.fillWidth: false
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                Button {
                    id: button_cansel
                    width: 130
                    text: qsTr("Выход")
                    Layout.minimumWidth: 120
                    Layout.fillWidth: true

                    // @disable-check M306
                    onClicked: Qt.quit()
                }

                Button {
                    id: button_Ok
                    width: 130
                    text: qsTr("Работа")
                    Layout.minimumWidth: 120
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillWidth: true

                    onClicked:
                    {



                        swipeView.setCurrentIndex(1);
                        groupBox.enabled = false;
                        button_Ok.enabled = false;
                        button_Ok.visible = false;
                        textInputName.enabled = false;
                        button_Replay.visible = true;

                        //menuProverka.visible = true;
                        menuTimer.visible = true;

                        if(textInputName.text != proverka.fioStart )
                        {
                            proverka.setfioStart(textInputName.text);
                        }

                        if(radioButton_mode_Regylirovka.checked)
                        {
                           proverka.setModeStart(radioButton_mode_Regylirovka.text);
                        }
                        if(radioButton_mode_NextTex.checked)
                        {
                           proverka.setModeStart(radioButton_mode_NextTex.text);
                        }
                        if(radioButton_mode_PSI.checked)
                        {
                           proverka.setModeStart(radioButton_mode_PSI.text);
                        }
                        if(radioButton_mode_LongWork.checked)
                        {
                           proverka.setModeStart(radioButton_mode_LongWork.text);
                        }
                        console.log(proverka.modeStart);
                        console.log(proverka.fioStart);





                        textFioWork.text = "Регулировщик: " + proverka.fioStart + "\nЭтап: " + proverka.modeStart;

                        tabBar.visible = true;

                         gc();

                    }
                }

                Button {
                    id: button_Replay
                    width: 130
                    text: qsTr("Изменить")
                    Layout.minimumWidth: 120
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillWidth: true
                    visible:  false

                    onClicked:
                    {
                        button_Ok.enabled = true;
                        button_Ok.visible = true;
                        groupBox.enabled = true;
                        textInputName.enabled = true;
                        button_Replay.visible = false;

                        menuProverka.visible = false;
                        menuTimer.visible = false;
                        tabBar.visible = false;

                    }
                }
            }
        }
    }
}
