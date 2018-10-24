import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1


import QtCharts 2.2


Rectangle {
    id: rectangle1
    width: 500          // Устанавливаем ширину, необходимо для Android-устройства
    height: 500         // Устанавливаем высоту, необходимо для Android-устройства
    color: "#f7f7f7"
    radius: 0
    // Задаём цвет

    // Область для сообщения диалогового окна
    Rectangle {
        id: rectangle
        width: 80
        /* Прибиваем область к левой, правой и верхней частям диалога,
                                     * а также снизу к разделителю, который отделяет область от кнопок
                                     */
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: dividerHorizontal.top
        color: "#f7f7f7"  // Задаём цвет области

        // Задаём сообщение диалогового окна
        Label {
            id: textLabel
            anchors.margins: 10
            text: "ПРМ № " + model.modelData.indexNP//(model.index + 1)
            elide: Text.ElideLeft
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            //anchors.centerIn: parent // Помещаем сообщение в центре области для сообщения
            anchors.top:  tableSpytniks
        }


        ListModel {
            id: dataModel

            ListElement {
                number: "1"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"

            }
            ListElement {
                number: "2"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "3"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "4"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "5"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "6"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "7"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "8"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "9"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "10"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "11"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "12"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "13"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "14"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "15"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "16"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "17"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "18"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "19"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "20"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "21"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "22"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "23"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
            ListElement {
                number: "24"
                spytnik: "NaN"
                mode: "NaN"
                level: "NaN"
            }
        }

        TableView {
            id: tableSpytniks
            width: 222
            height: 410
            backgroundVisible: true
            alternatingRowColors: true
            anchors.leftMargin: 8
            anchors.topMargin: 24

            anchors.margins: 10
            anchors.top:     parent.top
            anchors.left:    parent.left

            model: dataModel
            clip: true

            TableViewColumn {
                width: 40
                horizontalAlignment: Text.AlignHCenter
                resizable: false
                title: "№"
                role: "number"
            }
            TableViewColumn {
                width: 60
                horizontalAlignment: Text.AlignHCenter
                title: "Спутник"
                role: "spytnik"
            }
            TableViewColumn {
                width: 60
                horizontalAlignment: Text.AlignHCenter
                title: "Режим"
                role: "mode"
            }
            TableViewColumn {
                width: 60
                horizontalAlignment: Text.AlignHCenter
                title: "Уровень"
                role: "level"
            }




            itemDelegate: Item {

                property int modelRow: styleData.row ? styleData.row : 0

                Rectangle {
                    id: rowDel


                    height: parent.height
                    width: parent.width

                    color: getColor();


                    function getColor()
                    {

                        if(dataModel.get(styleData.row).mode >= 7)
                        {

                            return "#99FF99";
                        }


                        if(dataModel.get(styleData.row).mode === 5 )
                        {

                            return "#FFF000";
                        }

                        if(dataModel.get(styleData.row).mode === 6)
                        {

                            return "#FFA500";
                        }

                        return "#FFFFFF";
                    }




                    Text {
                        id: text2
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: styleData.value

                    }

                    Connections
                    {
                        target: dataModel

                        onObjectNameChanged:
                        {

                            rowDel.color =  rowDel.getColor();
                        }
                    }
                }



            }

            onModelChanged:
            {

                if(rowDel != null)
                    rowDel.getColor();

            }
        }



        ///////////////////////////////////////////////////////////////////////



        //        ChartView
        //        {
        //            id: graph_Amplitud
        //            y: 13
        //            height: 250
        //            anchors.top: graph.bottom
        //            anchors.topMargin: 10
        //            anchors.left: tableSpytniks.right
        //            anchors.leftMargin: 6
        //            anchors.right: parent.right
        //            anchors.rightMargin: 14
        //            title: qsTr("Уровень")
        //            smooth: true
        //            Layout.fillHeight: true
        //            Layout.fillWidth: true
        //            antialiasing: false

        //            clip: true

        //            // @disable-check M17
        //            legend.visible: false

        //            dropShadowEnabled : true

        //            BarSeries{
        //                id: mySeries_Amplitud
        //                useOpenGL: true


        //                axisY:ValueAxis {id: axisY_bar_Amplitud ; min: 0;   max: 300}
        //                // axisX:ValueAxis{visible: false} BarCategoryAxis {categories: ["1", "2", "3", "4", "5", "6","7", "8", "9", "10", "11", "12","13", "14", "15", "16", "17", "18","19", "20", "21", "22", "23", "24" ] }
        //                // BarSet {id: bar; values: [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]}
        //                BarSet {id: bar_Amplitud;
        //                    values: [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
        //                }


        //            }

        //        }


///Происходит утечка памяти надо думать
        Connections{
            target: model.modelData


            onSignal_listSpytnikChanged:
            {
                console.log("listSpytnik_Liters = " + model.modelData.listSpytnik)

                console.log(model.modelData.listSpytnik.count)

                for(var i =0; i <= 24; i++ )
                {
                    gc();
                    dataModel.objectName = i;
                    if(i != 24)
                    {

                        dataModel.get(i).mode = model.modelData.listSpytnik[i];
                        dataModel.get(i).level = model.modelData.listSpytnik_Amplitude[i];
                        dataModel.get(i).spytnik = model.modelData.listSpytnik_Name[i];
                    }
                }
            }
        }



        //        //////////////////////////////ChartView/////////////////////////////////////////////

        //                ChartView
        //                {
        //                    id: graph
        //                    y: 16
        //                    height: 250
        //                    anchors.left: tableSpytniks.right
        //                    anchors.leftMargin: 10
        //                    anchors.right: parent.right
        //                    anchors.rightMargin: 10
        //                    localizeNumbers: true
        //                    z: 0
        //                    rotation: 0
        //                    scale: 1
        //                    clip: true
        //                    anchors.top: npNameGLONASS2.bottom
        //                    anchors.topMargin: -28
        //                    anchors.bottom: idColumnLayer.top
        //                    anchors.bottomMargin: -40

        //                    // @disable-check M17
        //                    legend.visible: false
        //                    antialiasing: true
        //                    title: qsTr("Режим")
        //                    dropShadowEnabled : true


        //                    BarSeries{
        //                        id: mySeries
        //                        useOpenGL: true


        //                        axisY:BarCategoryAxis {categories: ["0","1", "2", "3", "4", "5", "6","7" ] }
        //                        // axisX:ValueAxis{visible: false} BarCategoryAxis {categories: ["1", "2", "3", "4", "5", "6","7", "8", "9", "10", "11", "12","13", "14", "15", "16", "17", "18","19", "20", "21", "22", "23", "24" ] }
        //                        BarSet {id: bar; values: [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]}

        //                    }





        //                        }
    }



    ColumnLayout {
        x: 240
        y: 25
        width: 252
        height: 178

        // Создаём горизонтальный разделитель с помощью Rectangle
        Rectangle {
            id: dividerHorizontal1
            color: "#d7d7d7"
            Layout.fillWidth: true
            height: 2 // Устанавливаем ширину в два пикселя
            //                anchors.left: parent.left
            //                anchors.right: parent.right
            //                anchors.bottom: row.top
        }

        Label {
            id: npI
            text: qsTr("I: ") + model.modelData.tok + qsTr(" А")
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Label {
            id: npV
            text: qsTr("V: ") + model.modelData.v +qsTr(" В")
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Label {
            id: npP
            text: qsTr("P: ") + model.modelData.power +qsTr(" Вт")
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        // Создаём горизонтальный разделитель с помощью Rectangle
        Rectangle {
            id: dividerHorizontal2
            color: "#d7d7d7"
            Layout.fillWidth: true
            height: 2 // Устанавливаем ширину в два пикселя
            //                anchors.left: parent.left
            //                anchors.right: parent.right
            //                anchors.bottom: row.top
        }


        Label {
            id: npName
            text: qsTr("Количество сгенирированных спутников: ") + sum(model.modelData.gpsGen,model.modelData.glonassGen)
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Label {
            id: npNameGPS
            text: qsTr("GPS: ") + model.modelData.gpsGen
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Label {
            id: npNameGLONASS
            text: qsTr("GLONASS: ") + model.modelData.glonassGen
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        // Создаём горизонтальный разделитель с помощью Rectangle
        Rectangle {
            id: dividerHorizontal3
            color: "#d7d7d7"
            Layout.fillWidth: true
            height: 2 // Устанавливаем ширину в два пикселя
            //                anchors.left: parent.left
            //                anchors.right: parent.right
            //                anchors.bottom: row.top
        }

        Label {
            id: npName2
            text: qsTr("Поймано спутников: ") +  sum(model.modelData.gpsFind,model.modelData.glonassFind)
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        Label {
            id: npNameGPS2
            text: qsTr("GPS: ") + model.modelData.gpsFind
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }
        Label {
            id: npNameGLONASS2
            text: qsTr("GLONASS: ") +model.modelData.glonassFind
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////

    // Создаём горизонтальный разделитель с помощью Rectangle
    Rectangle {
        id: dividerHorizontal
        color: "#d7d7d7"
        height: 2 // Устанавливаем ширину в два пикселя
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: row.top
    }

    /* Создаём подложку для кнопок в виде объекта Строки
                                 * В данном объекте для объектов детей не работают некоторые параметры
                                 * anchors, кроме параметров anchors.top и anchors.bottom
                                 */
    Row {
        id: row
        height: 50 // Задаём высоту
        // А также прибиваем строку к низу диалогового окна
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: gButtonCancel
            // Растягиваем кнопку по высоте строки
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            // Задаём ширину кнопки на половину строки минус 1 пиксель
            width: parent.width / 2 - 1

            // Стилизуем кнопку
            style: ButtonStyle {
                background: Rectangle {
                    color: control.pressed ? "#d7d7d7" : "#f7f7f7"
                    border.width: 0
                }

                label: Text {
                    text: qsTr("Cancel")
                    // Устанавливаем текст в центр кнопки
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            // По нажатию кнопки закрываем диалог
            onClicked: {
                dialogNp.close();
                            gc();
            }
        }

        // Создаём разделитель между кнопками шириной в 2 пикселя
        Rectangle {
            id: dividerVertical
            width: 2
            // Растягиваем разделитель по высоте объекта строки
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: "#d7d7d7" // Задаём цвет разделителя
        }

        Button {
            id: gButtonOk
            // Растягиваем кнопку по высоте строки
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            // Задаём ширину кнопки на половину строки минус 1 пиксель
            width: parent.width / 2 - 1

            // Стилизуем кнопку
            style: ButtonStyle {
                background: Rectangle {
                    color: control.pressed ? "#d7d7d7" : "#f7f7f7"
                    border.width: 0
                }

                label: Text {
                    text: qsTr("Ok")
                    // Устанавливаем текст в центр кнопки
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            // По нажатию кнопки закрываем диалог
            onClicked: {
                dialogNp.close();
                          gc();
            }

        }
    }
}

