import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQml.Models 2.2

Item {

    property string textResult: textResult
    property string creatPDF: creatPDF

    /* С помощью объекта Connections
            * Устанавливаем соединение с классом ядра приложения
            * */
    Connections {
        target: n6700_1 // Указываем целевое соединение
        /* Объявляем и реализуем функцию, как параметр
                * объекта и с имененем похожим на название сигнала
                * Разница в том, что добавляем в начале on и далее пишем
                * с заглавной буквы
                * */
        onGetMeasureVoltSignal: {
            resultText.text = "Vkl"//qsTr("Вкл") // Устанавливаем счётчик в текстовый лейбл
        }
    }

    Label {
        id:resultText
        // text:      //qsTr("Результаты")
        anchors.centerIn: parent
    }

    Connections
    {
        target:  pdf

        //Устройства
        onSignal_list:
        {
            libraryModel.clear();
            libraryModelFIO.clear();
            libraryModelEtap.clear();
            libraryModelDate.clear();

            for(var i=0;i < lol.length;i++)
            {
                libraryModel.append( {"Device": lol[i], "Id": (i+1).toString()});
            }


        }

        //Загрузка Пользователся относительно устройства
        onSignal_listSerial:
        {
            libraryModelFIO.clear();
            libraryModelEtap.clear();
            libraryModelDate.clear();

            for(var i=0;i < lol.length;i++)
            {
                libraryModelFIO.append( {"FIO": lol[i], "Id": (i+1).toString()});
            }

        }

        //Загрузка Этапа относительно устройства и пользователя
        onSignal_listSerialEtap:
        {
            libraryModelEtap.clear();
            libraryModelDate.clear();

            console.log(lol);

            for(var i=0;i < lol.length;i++)
            {
                libraryModelEtap.append( {"Etap": lol[i], "Id": (i+1).toString()});
            }
        }

        onSignal_listSerialDate:
        {
            libraryModelDate.clear();

            for(var i=0;i < lol.length;i++)
            {
                libraryModelDate.append( {"DateStart": lol[i],"DateEnd": end[i], "Id": (i+1).toString()});
            }
        }

        onSignal_listResult:
        {
            libraryModelResult.clear();
            libraryModelLiters.clear();

            for(var i=0;i < list_Result_IdProverki.length;i++)
            {
                libraryModelResult.append( {"Name":list_Result_NameProverki[i],"Treb":list_Result_Treb[i],"Sootv":list_Result_Sootv[i],"DateStart": list_Result_DateStayStart[i],"DateEnd": list_Result_DateStayEnd[i], "Id": list_Result_IdProverki[i]});
            }


            for(var i=0;i < Np.length;i++)
            {
                libraryModelLiters.append( {"list_NP":Np[i],"SootvNp":Np_sootv[i]});
            }

        }
    }


    GridView {
        anchors.fill: parent
        width: 200
        height: 400
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 0
        RowLayout
        {
            id: row
            height: 400
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            Rectangle {
                width: 100
                id:rectDevice
                // Layout.maximumWidth: 400
                color: "#EDEDED"
                Layout.fillHeight: true
                Layout.fillWidth: true




                ListModel {
                    id: libraryModel
                }





                TableView {
                    id:rectDevicetable
                    anchors.bottom: rectDeviceButton.top
                    anchors.bottomMargin: 0
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.top: parent.top
                    TableViewColumn {
                        id:rectDevicetableId
                        role: "Id"
                        title: "№"
                        width: 30
                        horizontalAlignment: Text.AlignHCenter



                    }
                    TableViewColumn {
                        role: "Device"
                        title: "Устройство"
                        width: rectDevice.width - rectDevicetableId.width -3
                        horizontalAlignment: Text.AlignHCenter
                    }
                    model: libraryModel


                    //                      onActiveFocusChanged:
                    //                      {
                    //                          pdf.slot_GetSerial();
                    //                      }



                    Component.onCompleted:
                    {
                        pdf.slot_GetSerial();
                    }

                    onCurrentRowChanged:
                    {

                        pdf.slot_setSelect(libraryModel.get(rectDevicetable.currentRow).Device,0);
                        pdf.slot_setSelect("",1);
                        pdf.slot_setSelect("",2);
                        pdf.slot_setSelect("",3);
                        pdf.slot_setSelect("",4);

                        console.log(pdf.listSelect);

                        pdf.slot_GetFIO(libraryModel.get(rectDevicetable.currentRow).Device);

                        textResult = "";
                        textResult += pdf.listSelect[0] + " -> ";



                        creatPDF = "Сгенирировать отчет : " + pdf.listSelect[0];

                        rectDevicetableFIO.currentRow = -1;

                    }

                }

                Button
                {
                    id: rectDeviceButton
                    text: "Обновить"
                    tooltip: "Обновить информацию из БД"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    onClicked:
                    {

                        rectDevicetable.currentRow = -1;
                        rectDevicetableFIO.currentRow = -1;
                        pdf.slot_GetSerial();
                    }
                }





            }
            Rectangle {
                width: 100
                id:rectFIO
                //Layout.minimumWidth: 50
                //Layout.fillWidth: true
                color: "#EDEDED"
                Layout.fillHeight: true
                Layout.fillWidth: true




                ListModel {
                    id: libraryModelFIO
                }

                TableView {
                    id:rectDevicetableFIO
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: rectFIOButton.top

                    TableViewColumn {
                        id:rectDevicetableFIOId
                        role: "Id"
                        title: "№"
                        width: 30
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        role: "FIO"
                        title: "ФИО"
                        width: rectFIO.width - rectDevicetableFIOId.width -3
                        horizontalAlignment: Text.AlignHCenter
                    }
                    model: libraryModelFIO




                    onCurrentRowChanged:
                    {

                        console.log("libraryModelFIO.count  = " + libraryModelFIO.count );
                        if(libraryModelFIO.count > 0)
                        {


                            pdf.slot_setSelect(libraryModelFIO.get(rectDevicetableFIO.currentRow).FIO,1);
                            pdf.slot_setSelect("",2);
                            pdf.slot_setSelect("",3);
                            pdf.slot_setSelect("",4);

                            console.log(pdf.listSelect);



                            textResult = "";
                            textResult = pdf.listSelect[0] + " -> " + pdf.listSelect[1] + " -> ";


                            pdf.slot_GetEtap(pdf.listSelect[0],libraryModelFIO.get(rectDevicetableFIO.currentRow).FIO);

                            rectDevicetableEtap.currentRow = -1;

                        }
                    }

                    onRowCountChanged:
                    {
                        if(libraryModelFIO.count > 0)
                        {
                            textFIO.visible = false;

                        }
                        else
                        {
                            textFIO.visible = true;
                            textEtap.visible = true;
                            textDate.visible = true;


                        }
                    }

                }

                Button
                {
                    id: rectFIOButton
                    text: "Обновить"
                    tooltip: "Обновить информацию из БД"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    onClicked:
                    {
                        rectDevicetableEtap.currentRow = -1;
                        pdf.slot_GetFIO(libraryModel.get(rectDevicetable.currentRow).Device);
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#EDEDED"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    id: textFIO
                    Text {
                        z:1
                        text: "ФИО"
                        anchors.centerIn: parent

                    }

                    visible: true
                }

            }
            Rectangle {
                width: 100
                color: "#EDEDED"
                Layout.fillHeight: true
                Layout.fillWidth: true
                id:rectEtap

                ListModel {
                    id: libraryModelEtap
                }

                TableView {
                    id:rectDevicetableEtap
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: rectEtapButton.top

                    TableViewColumn {
                        id:rectDevicetableEtapId
                        role: "Id"
                        title: "№"
                        width: 30
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        role: "Etap"
                        title: "Этап"
                        width: rectEtap.width - rectDevicetableEtapId.width -3
                        horizontalAlignment: Text.AlignHCenter
                    }
                    model: libraryModelEtap


                    onCurrentRowChanged:
                    {

                        console.log("rectDevicetableEtap.currentRow " + rectDevicetableEtap.currentRow);

                        pdf.slot_setSelect(libraryModelEtap.get(rectDevicetableEtap.currentRow).Etap,2);


                        console.log(libraryModelEtap.get(rectDevicetableEtap.currentRow).Etap);

                        console.log("listSelect = " + pdf.listSelect);
                        textResult = "";
                        textResult = pdf.listSelect[0] + " -> " + pdf.listSelect[1] + " -> " + pdf.listSelect[2] + " -> ";

                        pdf.slot_GetEtapData(pdf.listSelect[0],pdf.listSelect[1],pdf.listSelect[2]);




                    }

                    onRowCountChanged:
                    {
                        if(libraryModelEtap.count > 0)
                        {
                            textEtap.visible = false;
                        }
                        else
                        {
                            textEtap.visible = true;

                            rectDevicetableDate.currentRow = -1;
                        }
                    }

                }

                Button
                {
                    id: rectEtapButton
                    text: "Обновить"
                    tooltip: "Обновить информацию из БД"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    onClicked:
                    {
                        pdf.slot_GetEtap(pdf.listSelect[0],pdf.listSelect[1]);
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#EDEDED"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    id: textEtap
                    Text {
                        z:1
                        text: "Этап"
                        anchors.centerIn: parent

                    }

                    visible: true
                }

            }
            Rectangle {
                width: 100
                color: "#EDEDED"
                Layout.fillHeight: true
                Layout.fillWidth: true
                ListModel {
                    id: libraryModelDate
                }

                TableView {
                    id:rectDevicetableDate
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: rectDateButton.top

                    TableViewColumn {
                        id:rectDevicetableDateId
                        role: "Id"
                        title: "№"
                        width: 25
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        id:rectDevicetableDateId2
                        role: "DateStart"
                        title: "Старт"
                        width: (rectEtap.width - rectDevicetableDateId.width -3)/2
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        role: "DateEnd"
                        title: "Конец"
                        width: (rectEtap.width - rectDevicetableDateId.width -3)/2
                        horizontalAlignment: Text.AlignHCenter
                    }
                    model: libraryModelDate


                    onCurrentRowChanged:
                    {

                        pdf.slot_setSelect(libraryModelDate.get(rectDevicetableDate.currentRow).DateStart,3);


                        console.log(libraryModelDate.get(rectDevicetableDate.currentRow).DateStart);

                        console.log("listSelect = " + pdf.listSelect);
                        textResult = "";
                        textResult = pdf.listSelect[0] + " -> " + pdf.listSelect[1] + " -> " + pdf.listSelect[2] + " -> " + pdf.listSelect[3] + ":";


                        pdf.slot_GetResult(pdf.listSelect[0],pdf.listSelect[1],pdf.listSelect[2],pdf.listSelect[3]);


                        // rectDevicetableEtap.currentRow = -1;

                    }

                    onRowCountChanged:
                    {
                        if(libraryModelDate.count > 0)
                        {
                            textDate.visible = false;
                        }
                        else
                        {
                            textDate.visible = true;

                            rectDevicetableResult.currentRow = -1;
                            rectDevicetableLiters.currentRow = -1;
                            libraryModelResult.clear();
                            libraryModelLiters.clear();
                        }
                    }

                }

                Button
                {
                    id: rectDateButton
                    text: "Обновить"
                    tooltip: "Обновить информацию из БД"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    onClicked:
                    {
                        pdf.slot_GetResult(pdf.listSelect[0],pdf.listSelect[1],pdf.listSelect[2],pdf.listSelect[3]);
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#EDEDED"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    id: textDate
                    Text {
                        z:1
                        text: "Дата"
                        anchors.centerIn: parent

                    }

                    visible: true
                }
            }
        }


        RowLayout {
            anchors.top: row.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            Rectangle {
                color: "#EDEDED"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                id:rectResult

                ListModel {
                    id: libraryModelResult
                }

                TableView {
                    id:rectDevicetableResult
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: rectResultButton.top

                    TableViewColumn {
                        role: "Id"
                        title: "№"
                        width: 25
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        role: "Name"
                        title: "Проверка"
                        Layout.fillWidth: true
                        width: 300
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        role: "Treb"
                        title: "Требования"
                        width: 100

                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        role: "Sootv"
                        title: "Соответствие"
                        width: 100
                       // width: (rectResult.width - rectDevicetableDateId.width -3)/5
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        id:rectDevicetableResultId2
                        role: "DateStart"
                        title: "Старт"
                        width: 150
                      //  width: (rectResult.width - rectDevicetableDateId.width -3)/5
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        role: "DateEnd"
                        title: "Конец"
                        width: 150
                       // width: (rectResult.width - rectDevicetableDateId.width -3)/5
                        horizontalAlignment: Text.AlignHCenter
                    }
                    model: libraryModelResult


                    onCurrentRowChanged:
                    {

//                        pdf.slot_setSelect(libraryModelResult.get(rectDevicetableResult.currentRow).DateStart,3);


//                        console.log(libraryModelResult.get(rectDevicetableResult.currentRow).DateStart);

//                        console.log("listSelect = " + pdf.listSelect);
//                        textResult = "";
//                        textResult = pdf.listSelect[0] + " -> " + pdf.listSelect[1] + " -> " + pdf.listSelect[2] + " -> " + pdf.listSelect[3] + ":";


//                        pdf.slot_GetResult(pdf.listSelect[0],pdf.listSelect[1],pdf.listSelect[2],pdf.listSelect[3]);


//                        rectDevicetableResult.currentRow = -1;

                    }

                    onRowCountChanged:
                    {
                        if(libraryModelResult.count > 0)
                        {
                            textResultRect.visible = false;
                        }
                        else
                        {
                            textResultRect.visible = true;
                        }
                    }

                }

                Button
                {
                    id: rectResultButton
                    text: "Обновить"
                    tooltip: "Обновить информацию из БД"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    onClicked:
                    {
                        pdf.slot_GetResult(pdf.listSelect[0],pdf.listSelect[1],pdf.listSelect[2],pdf.listSelect[3]);
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#EDEDED"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    id: textResultRect
                    Text {
                        z:1
                        text: "Результаты"
                        anchors.centerIn: parent

                    }

                    visible: true
                }

            }

            Rectangle {
                color: "#EDEDED"
                Layout.maximumWidth: 300
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                id:rectLiters

                ListModel {
                    id: libraryModelLiters
                }

                TableView {
                    id:rectDevicetableLiters
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: rectLitersButton.top

                    TableViewColumn {
                        role: "list_NP"
                        title: "№"
                        width: 25
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        role: "SootvNp"
                        title: "Соответствие"
                        Layout.fillWidth: true
                        width: 100
                        horizontalAlignment: Text.AlignHCenter
                    }

                    model: libraryModelLiters


   //                 onCurrentRowChanged:
  //                  {

//                        pdf.slot_setSelect(libraryModelResult.get(rectDevicetableResult.currentRow).DateStart,3);


//                        console.log(libraryModelResult.get(rectDevicetableResult.currentRow).DateStart);

//                        console.log("listSelect = " + pdf.listSelect);
//                        textResult = "";
//                        textResult = pdf.listSelect[0] + " -> " + pdf.listSelect[1] + " -> " + pdf.listSelect[2] + " -> " + pdf.listSelect[3] + ":";


//                        pdf.slot_GetResult(pdf.listSelect[0],pdf.listSelect[1],pdf.listSelect[2],pdf.listSelect[3]);


//                        rectDevicetableResult.currentRow = -1;

 //                   }

                    onRowCountChanged:
                    {
                        if(libraryModelResult.count > 0)
                        {
                            textResultLiters.visible = false;
                        }
                        else
                        {
                            textResultLiters.visible = true;
                        }
                    }

                }

                Button
                {
                    id: rectLitersButton
                    text: "Обновить"
                    tooltip: "Обновить информацию из БД"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    onClicked:
                    {
                        pdf.slot_GetResult(pdf.listSelect[0],pdf.listSelect[1],pdf.listSelect[2],pdf.listSelect[3]);
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#EDEDED"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    id: textResultLiters
                    Text {
                        z:1
                        text: "Литеры"
                        anchors.centerIn: parent

                    }

                    visible: true
                }

            }
        }
    }

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
