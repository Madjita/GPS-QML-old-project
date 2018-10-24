import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import QtQuick.Window 2.11
import QtQuick.Dialogs 1.2

Item {
    id: proverkiPage


    ListModel {
        id: libraryModel
        ListElement {
            name: "Проверка работоспособности приемника НП-101"
            ip: "1"
            connect: "Подробно"
        }
        ListElement {
            name: "Проверка работоспособности приемника НП-101 при пониженном напряжении питания"
            ip: "2"
            connect: "Подробно"
        }
        ListElement {
            name: "Проверка работоспособности приемника НП-101 при повышенном напряжении питания"
            ip: "3"
            connect: "Подробно"
        }

        ListElement {
            name: "Проверка мощности, потребляемой приемником от источника постоянного тока"
            ip: "4"
            connect: "Подробно"
        }
        ListElement {
            name: "Проверка поиска, захвата и сопровождения сигнала с уровнем мощности минус 160 дБВт"
            ip: "5"
            connect: "Подробно"
        }
        ListElement {
            name: "Проверка работы приемника от внешнего опорного генератора"
            ip: "6"
            connect: "Подробно"
        }
        ListElement {
            name: "Проверка напряжения сигнала выходной частоты 10 МГц"
            ip: "7"
            connect: "Подробно"
        }
        ListElement {
            name: "Проверка амплитуды выходного сигнала секундной метки"
            ip: "8"
            connect: "Подробно"
        }
        ListElement {
            name: "Проверка напряжения питания МШУ"
            ip: "9"
            connect: "Подробно"
        }
        ListElement {
            name: "Проверка смещения секундной метки"
            ip: "10"
            connect: "Подробно"
        }


    }


    Window {
        id:dialogMode
        visible: false
        title: "Выбрать режим"
        flags: Qt.Window

        width: 300
        height: 200

        maximumWidth : 300
        maximumHeight : 200
        x:mainWindow.width/2
        y:mainWindow.height/2


        Rectangle {
            implicitWidth: 300
            implicitHeight: 200
            color: "#f7f7f7"



            GridLayout {
                id: grid
                width: parent.width

                anchors.centerIn: parent.Center

                anchors.topMargin: 0
                columnSpacing: 5
                rowSpacing: 10
                columns: 2
                rows: 0


                Text {
                    id: text1
                    padding: 5
                    text: qsTr("Выбирите темпиратуру:")
                    horizontalAlignment: Text.AlignLeft
                    Layout.fillWidth: false
                    Layout.columnSpan: 2
                    Layout.rowSpan: 1
                    font.pixelSize: 12
                }

                GroupBox {
                    id: groupBox
                    width: parent.width
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
                            id: radioButton_mode_NY
                            text: qsTr("НУ")
                            checked: true
                            exclusiveGroup: tabPositionGroup
                        }

                        RadioButton {
                            id: radioButton_mode_Cold
                            text: qsTr("-60")
                            exclusiveGroup: tabPositionGroup
                        }

                        RadioButton {
                            id: radioButton_mode_NyCold
                            text: qsTr("НУ после -60")
                            exclusiveGroup: tabPositionGroup
                        }

                        RadioButton {
                            id: radioButton_mode_Hord
                            text: qsTr("+50")
                            exclusiveGroup: tabPositionGroup
                        }

                        RadioButton {
                            id: radioButton_mode_NyHord
                            text: qsTr("НУ после +50")
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
                        text: qsTr("Отмена")
                        Layout.minimumWidth: 120
                        Layout.fillWidth: true

                        // @disable-check M306
                        onClicked: {

                            camber.slot_stopCamberWork();
                            gc();

                            dialogMode.close();
                        }
                    }

                    Button {
                        id: button_Ok
                        width: 130
                        text: qsTr("Старт")
                        Layout.minimumWidth: 120
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.fillWidth: true

                        onClicked:
                        {

                            dialogMode.visible = false;

                            //Запустить установку темпиратуры
                            if(true) //camber.slot_TestConnect()
                            {
                                view.currentIndex = 3;

                                //boxSliver.visible = true;

                                //Запуск окна камеры (ждем установки темпиратуры)
                                //camberTimer.visible = true; // поменять обратно на true


                                //Сначало установить темпиратуру 27 градусов (прграмма номер 3)

                                proverka.namberModeCamber = 0;

                                if(radioButton_mode_NY.checked)
                                {
                                    proverka.namberModeCamber = 1;
                                    camber.slot_SetNY();
                                }

                                if(radioButton_mode_Cold.checked)
                                {
                                    proverka.namberModeCamber = 2;
                                    camber.slot_SetCold();
                                }

                                if(radioButton_mode_NyCold.checked)
                                {
                                    proverka.namberModeCamber = 3;
                                    camber.slot_SetNY();
                                }

                                if(radioButton_mode_Hord.checked)
                                {
                                    proverka.namberModeCamber = 4;
                                    camber.slot_SetHord();
                                }

                                if(radioButton_mode_NyHord.checked)
                                {
                                    proverka.namberModeCamber = 5;
                                    camber.slot_SetNY();
                                }

                                //Затем запустить камеру.
                                //camber.slot_startCamberWork();
                                //camber.slot_StartTimer();

                                proverka.slot_StartProverka();

                            }
                            else
                            {
                                console.log("EROROROOROROR CONNECT");
                            }
                            gc();



                        }
                    }
                }
            }


        }
    }


    GridView {

        // The standard size
        property int idealCellHeight: 200;
        property int idealCellWidth: 200;


        id: viewProverka
        snapMode: GridView.NoSnap
        flow: GridView.FlowLeftToRight

        anchors.fill: parent

        cellHeight: idealCellHeight
        cellWidth: width / Math.floor(width / idealCellWidth)


        model: libraryModel // findDevice.listDevice // libraryModel
        clip: true

        highlight: Rectangle {
            color: "skyblue"
        }



        delegate: Item {
            property var view: GridView.view
            property var isCurrent: GridView.isCurrentItem


            height: view.cellHeight
            width: view.cellWidth

            // Пересчёт независимых от плотности пикселей в физические пиксели устройства
            readonly property int dpi: Screen.pixelDensity* 25.4

            function dp(x){ return (dpi < 120) ? x : x*(dpi/160); }

            Rectangle {
                id: rectangle1
                anchors.margins: 5
                anchors.fill: parent
                border {
                    color: "black"
                    width: 1
                }

                Rectangle
                {
                    id: menuRectItemDashboard
                    width: 600
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: dp(24)
                    color: "#7e827f"
                    border.width: 1
                    border.color: "#45110000"

                    Text {
                        id: modelIndex
                        color: "#ffffff"

                        anchors.fill:   parent
                        renderType: Text.NativeRendering
                        text: "№ " + (model.index + 1)
                        font.bold: true
                        padding:  5
                    }
                }


                Text {
                    id: modelName
                    text: model.name
                    anchors {
                        right: parent.right
                        rightMargin: 10
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 10
                        bottom: mdelButton.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }



                Button{
                    id:mdelButton
                    height: 23
                    text: qsTr("Запустить")
                    z: 1
                    anchors { bottom: parent.bottom
                        bottomMargin: 0
                        right: parent.right
                        rightMargin: 0
                        left: parent.left
                        leftMargin: 0
                    }

                    onClicked: {
//                        view.currentIndex = model.index

                        //                            if(proverka.modeStart == "Регулировка")
                        //                            {
                        //                                console.log(model.name);
                        //                            }
                        //                            else
                        //                            {
                        radioButton_mode_Cold.enabled = true;
                        radioButton_mode_Hord.enabled = true;
                        radioButton_mode_NyCold.enabled = true;
                        radioButton_mode_NyHord.enabled = true;

                        if(proverka.modeStart === "После технической тренировки в НУ" || proverka.modeStart === "Проверка неприрывной работы")
                        {
                            radioButton_mode_Cold.enabled = false;
                            radioButton_mode_Hord.enabled = false;
                            radioButton_mode_NyCold.enabled = false;
                            radioButton_mode_NyHord.enabled = false;
                        }

                        proverka.namberProverkaStart =  model.index +1;
                        dialogMode.show();

                        //  }


                    }

                }


                MouseArea {
                    visible: true
                    anchors.fill: parent
                    onClicked: view.currentIndex = model.index

                }



            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
