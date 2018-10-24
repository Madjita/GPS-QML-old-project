import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1

import QtQuick.Window 2.2

import QtCharts 2.2

import Qt.labs.platform 1.0

Item {
    clip: true

    property GridView viewProverka: viewProverka


    MessageDialog
    {
        id: windowMessageDialog

        buttons: MessageDialog.Ok
        text: "Переключите провода."

        onOkClicked: proverka.slot_next();

    }

    MessageDialog
    {
        id: windowMessageDialogFinish

        buttons: MessageDialog.Ok
        text: "Все проверки завершины."

        onOkClicked:{
            view.currentIndex = 1;
        }

    }

    Connections
    {
        target: proverka


//        onSignal_Next:
//        {
//            windowMessageDialog.open();
//        }

        onSignal_Finish:
        {
            console.log("windowMessageDialogFinish.open");
            windowMessageDialogFinish.close();
            windowMessageDialogFinish.open();
            header.autoFlag.enabled = true;
        }
    }


    GridView {
        id: viewProverka

        // Пересчёт независимых от плотности пикселей в физические пиксели устройства
        readonly property int dpi: Screen.pixelDensity * 25.4
        anchors.bottomMargin: 0
        cellHeight: 400
        cellWidth: 300
        function dp(x){ return (dpi < 120) ? x : x*(dpi/160); }


        anchors.fill: parent
        //anchors.right: groupBox.left

        model:proverka.listProverka // libraryModel
        clip: true

        highlight: Rectangle {
            color: "skyblue"
        }

        onWindowChanged:  {
            cellHeight: mainWindow.height/3
            cellWidth: cellHeight
        }

        delegate: Item {
            property var view: GridView.view
            property var isCurrent: GridView.isCurrentItem

            height: view.cellHeight
            antialiasing: true
            clip: true
            width: view.cellWidth

            // Пересчёт независимых от плотности пикселей в физические пиксели устройства
            readonly property int dpi: Screen.pixelDensity* 25.4

            function dp(x){ return (dpi < 120) ? x : x*(dpi/160); }

            function sum(gps,glonass)
            {
                var stringValue = String(parseInt(gps)+parseInt(glonass));
                return stringValue;
            }

            Rectangle {
                id: rectangle1
                clip: true
                anchors.margins: 5
                anchors.fill: parent
                border {
                    color: "black"
                    width: 0
                }

                Rectangle
                {
                    //z:5 // убрать
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
                        id: npIndex
                        color: "#ffffff"

                        anchors.left:  parent
                        renderType: Text.NativeRendering
                        // text: "ПРМ № " + (model.index + 1)
                        text: "ПРМ № " + model.modelData.indexNP

                        font.bold: true
                        padding:  5
                    }

                    Rectangle
                    {

                        id: proverkaBox
                        height: dp(24)
                        color: "#7e827f"
                        anchors.topMargin: 1
                        anchors.rightMargin: 1
                        anchors.bottomMargin: 1
                        anchors.left: npIndex.right
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.top: parent.top
                        anchors.leftMargin: 0

                        GridLayout
                        {

                            columnSpacing: 0
                            rowSpacing: 0

                            anchors.verticalCenter: parent.verticalCenter

                            RadioButton
                            {
                                id: radiobutton_proverka_1
                                width: 5
                                height: 12


                                checked:  true


                                // @disable-check M16
                                style: RadioButtonStyle {
                                    indicator: Rectangle {

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */


                                            onSignal_StartProverkaIndex:
                                            {

                                                switch(namberProverkaStart)
                                                {
                                                case "1": radiobutton_proverka_1.checked = true;
                                                    break;
                                                case "2": radiobutton_proverka_2.checked = true;
                                                    break;
                                                case "3": radiobutton_proverka_3.checked = true;
                                                    break;
                                                case "4": radiobutton_proverka_4.checked = true;
                                                    break;
                                                case "5": radiobutton_proverka_5.checked = true;
                                                    break;
                                                case "6": radiobutton_proverka_6.checked = true;
                                                    break;
                                                case "7": radiobutton_proverka_7.checked = true;
                                                    break;
                                                case "8": radiobutton_proverka_8.checked = true;
                                                    break;
                                                case "9": radiobutton_proverka_9.checked = true;
                                                    break;
                                                case "10": radiobutton_proverka_10.checked = true;
                                                    break;

                                                }
                                            }

                                            onIndexProverkiChanged:
                                            {
                                                console.log("Good = " + model.modelData.indexProverki);


                                                switch(model.modelData.indexProverki)
                                                {
                                                case "1": radiobutton_proverka_1.checked = false;
                                                    console.log("Model: " + model.modelData.indexProverki +" flag = " + model.modelData.good);
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_1.color = "#99ff99";



                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_1.color = "red";
                                                    }


                                                    break;

                                                }
                                            }

                                        }


                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "1":{
                                                        radiobutton_proverka_1.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_1.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_1.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }








                                        id: radiobutton_proverka_rect_1
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50

                                        //color: "yellow"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1
                                        Text
                                        {
                                            id: numberProverka;
                                            text: "1"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1

                                            //                                        Text
                                            //                                        {
                                            //                                           text: "1"
                                            //                                           color: "black"
                                            //                                           anchors.verticalCenter: parent.verticalCenter
                                            //                                           anchors.horizontalCenter: parent.horizontalCenter
                                            //                                        }
                                        }
                                    }
                                }

                            }
                            RadioButton
                            {
                                id: radiobutton_proverka_2
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        id: radiobutton_proverka_rect_2
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        // color: "red"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {

                                                radiobutton_proverka_2.checked = false;
                                                radiobutton_proverka_rect_2.color =  "white";




                                            }
                                        }

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {
                                                switch(model.modelData.indexProverki)
                                                {
                                                case "2": radiobutton_proverka_2.checked = false;
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_2.color = "#99ff99";
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_2.color = "red";
                                                    }

                                                    break;
                                                }
                                            }

                                        }




                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "2":{
                                                        radiobutton_proverka_2.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_2.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_2.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }



                                        Text
                                        {
                                            text: "2"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1


                                        }
                                    }


                                }

                            }

                            RadioButton
                            {
                                id: radiobutton_proverka_3
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        id: radiobutton_proverka_rect_3
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        //color: "yellow"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {
                                                radiobutton_proverka_3.checked = false;
                                                radiobutton_proverka_rect_3.color =  "white";
                                            }
                                        }
                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {

                                                switch(model.modelData.indexProverki)
                                                {
                                                case "3": radiobutton_proverka_3.checked = false;
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_3.color = "#99ff99";
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_3.color = "red";
                                                    }

                                                    break;
                                                }
                                            }

                                        }


                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "3":{
                                                        radiobutton_proverka_3.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_3.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_3.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }


                                        Text
                                        {
                                            text: "3"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1


                                        }
                                    }
                                }
                            }
                            RadioButton
                            {
                                id: radiobutton_proverka_4
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        id: radiobutton_proverka_rect_4
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        //color: "yellow"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {

                                                radiobutton_proverka_4.checked = false;
                                                radiobutton_proverka_rect_4.color =  "white";


                                            }
                                        }
                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {

                                                switch(model.modelData.indexProverki)
                                                {
                                                case "4": radiobutton_proverka_4.checked = false;
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_4.color = "#99ff99";
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_4.color = "red";
                                                    }
                                                    break;
                                                }
                                            }

                                        }

                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "4":{
                                                        radiobutton_proverka_4.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_4.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_4.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }


                                        Text
                                        {
                                            text: "4"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1


                                        }
                                    }
                                }

                            }
                            RadioButton
                            {
                                id: radiobutton_proverka_5
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        //color: "yellow"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1
                                        id: radiobutton_proverka_rect_5

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {

                                                radiobutton_proverka_5.checked = false;
                                                radiobutton_proverka_rect_5.color =  "white";




                                            }
                                        }
                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {

                                                switch(model.modelData.indexProverki)
                                                {
                                                case "5": radiobutton_proverka_5.checked = false;
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_5.color = "#99ff99";
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_5.color = "red";
                                                    }
                                                    break;
                                                }
                                            }

                                        }

                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "5":{
                                                        radiobutton_proverka_5.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_5.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_5.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }


                                        Text
                                        {
                                            text: "5"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1


                                        }
                                    }
                                }

                            }
                            RadioButton
                            {
                                id: radiobutton_proverka_6
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        //color: "yellow"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1
                                        id: radiobutton_proverka_rect_6

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {

                                                radiobutton_proverka_6.checked = false;
                                                radiobutton_proverka_rect_6.color =  "white";




                                            }
                                        }

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {

                                                switch(model.modelData.indexProverki)
                                                {
                                                case "6": radiobutton_proverka_6.checked = false;
                                                    console.log("LOL radiobutton_proverka_6");
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_6.color = "#99ff99";
                                                        console.log("LOL radiobutton_proverka_6 = #99ff99");
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_6.color = "red";

                                                        console.log("LOL radiobutton_proverka_6 = #red");
                                                    }
                                                    break;
                                                }
                                            }

                                        }

                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "6":{
                                                        radiobutton_proverka_6.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_6.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_6.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }


                                        Text
                                        {
                                            text: "6"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1


                                        }
                                    }
                                }

                            }
                            RadioButton
                            {
                                id: radiobutton_proverka_7
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        //color: "yellow"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1
                                        id: radiobutton_proverka_rect_7

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {

                                                radiobutton_proverka_7.checked = false;
                                                radiobutton_proverka_rect_7.color =  "white";




                                            }
                                        }

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {

                                                switch(model.modelData.indexProverki)
                                                {
                                                case "7": radiobutton_proverka_7.checked = false;
                                                    console.log("LOL radiobutton_proverka_7");
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_7.color = "#99ff99";
                                                        console.log("LOL radiobutton_proverka_7 = #99ff99");
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_7.color = "red";

                                                        console.log("LOL radiobutton_proverka_7 = #red");
                                                    }
                                                    break;
                                                }
                                            }

                                        }

                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "7":{
                                                        radiobutton_proverka_7.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_7.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_7.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }


                                        Text
                                        {
                                            text: "7"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1


                                        }
                                    }
                                }

                            }
                            RadioButton
                            {
                                id: radiobutton_proverka_8
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        //color: "yellow"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1
                                        id: radiobutton_proverka_rect_8

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {

                                                radiobutton_proverka_8.checked = false;
                                                radiobutton_proverka_rect_8.color =  "white";

                                            }
                                        }
                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {

                                                switch(model.modelData.indexProverki)
                                                {
                                                case "8": radiobutton_proverka_8.checked = false;
                                                    console.log("LOL radiobutton_proverka_8");
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_8.color = "#99ff99";
                                                        console.log("LOL radiobutton_proverka_8 = #99ff99");
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_8.color = "red";

                                                        console.log("LOL radiobutton_proverka_8 = #red");
                                                    }
                                                    break;
                                                }
                                            }

                                        }

                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "8":{
                                                        radiobutton_proverka_8.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_8.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_8.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }


                                        Text
                                        {
                                            text: "8"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1

                                        }
                                    }
                                }

                            }
                            RadioButton
                            {
                                id: radiobutton_proverka_9
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        //color: "yellow"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1

                                        id: radiobutton_proverka_rect_9

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {

                                                radiobutton_proverka_9.checked = false;
                                                radiobutton_proverka_rect_9.color =  "white";

                                            }
                                        }
                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {

                                                switch(model.modelData.indexProverki)
                                                {
                                                case "9": radiobutton_proverka_9.checked = false;
                                                    console.log("LOL radiobutton_proverka_9");
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_9.color = "#99ff99";
                                                        console.log("LOL radiobutton_proverka_9 = #99ff99");
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_9.color = "red";

                                                        console.log("LOL radiobutton_proverka_9 = #red");
                                                    }
                                                    break;
                                                }
                                            }

                                        }

                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "9":{
                                                        radiobutton_proverka_9.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_9.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_9.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }


                                        Text
                                        {
                                            text: "9"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1


                                        }
                                    }
                                }

                            }
                            RadioButton
                            {
                                id: radiobutton_proverka_10
                                width: 5
                                height: 12

                                style: RadioButtonStyle {
                                    indicator: Rectangle {
                                        implicitWidth: 18
                                        implicitHeight: 18
                                        radius: 50
                                        //color: "yellow"
                                        //color: "#99ff99"
                                        border.color: control.activeFocus ? "darkblue" : "gray"
                                        border.width: 1

                                        id: radiobutton_proverka_rect_10

                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: proverka // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */



                                            onSignal_clearView:
                                            {

                                                radiobutton_proverka_10.checked = false;
                                                radiobutton_proverka_rect_10.color =  "white";

                                            }
                                        }
                                        /* С помощью объекта Connections
                                             * Устанавливаем соединение с классом ядра приложения
                                             * */
                                        Connections {
                                            target: model.modelData // Указываем целевое соединение
                                            /* Объявляем и реализуем функцию, как параметр
                                                 * объекта и с имененем похожим на название сигнала
                                                 * Разница в том, что добавляем в начале on и далее пишем
                                                 * с заглавной буквы
                                                 * */

                                            onIndexProverkiChanged:
                                            {

                                                switch(model.modelData.indexProverki)
                                                {
                                                case "10": radiobutton_proverka_10.checked = false;
                                                    console.log("LOL radiobutton_proverka_10");
                                                    if(model.modelData.good === true)
                                                    {
                                                        radiobutton_proverka_rect_10.color = "#99ff99";
                                                        console.log("LOL radiobutton_proverka_10 = #99ff99");
                                                    }
                                                    else
                                                    {
                                                        radiobutton_proverka_rect_10.color = "red";

                                                        console.log("LOL radiobutton_proverka_10 = #red");
                                                    }
                                                    break;
                                                }
                                            }

                                        }

                                        Connections {
                                            target: proverka

                                            onQml_good:
                                            {
                                                console.log("onQml_good = ",ok,indexProverka,index);

                                                if(model.modelData.indexNP === index)
                                                {
                                                    switch(indexProverka){
                                                    case "10":{
                                                        radiobutton_proverka_10.checked = false;
                                                        if(ok)
                                                        {
                                                            radiobutton_proverka_rect_10.color = "#99ff99";
                                                        }
                                                        else
                                                        {
                                                            radiobutton_proverka_rect_10.color = "red";
                                                        }
                                                        break;
                                                    }
                                                    }
                                                }
                                            }
                                        }


                                        Text
                                        {
                                            text: "10"
                                            color: "black"
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: control.checked
                                            color: "#ffb961"
                                            radius: 50
                                            anchors.margins: 1


                                        }
                                    }
                                }

                            }








                            Connections {
                                target: proverka

                                onSignal_clearView:
                                {
                                    console.log("signal_clearView");
                                    radiobutton_proverka_1.checked = false;
                                    radiobutton_proverka_2.checked = false;
                                    radiobutton_proverka_3.checked = false;
                                    radiobutton_proverka_4.checked = false;
                                    radiobutton_proverka_5.checked = false;
                                    radiobutton_proverka_6.checked = false;
                                    radiobutton_proverka_7.checked = false;
                                    radiobutton_proverka_8.checked = false;
                                    radiobutton_proverka_9.checked = false;
                                    radiobutton_proverka_10.checked = false;

                                    //radiobutton_proverka_rect_1.color = "white";
                                }
                            }


                        }
                    }

                }



                ChartView
                {
                    id: graph
                    localizeNumbers: true
                    z: 0
                    rotation: 0
                    scale: 1
                    clip: true
                    anchors.right: parent.right
                    anchors.rightMargin: -25
                    anchors.left: parent.left
                    anchors.leftMargin: -25
                    anchors.top: npNameGLONASS2.bottom
                    anchors.topMargin: -28
                    anchors.bottom: idColumnLayer.top
                    anchors.bottomMargin: -40

                    // @disable-check M17
                    legend.visible: false
                    antialiasing: true
                    animationOptions: ChartView.NoAnimation


                    BarSeries{
                        id: mySeries
                        useOpenGL: true



                        axisY:BarCategoryAxis {categories: ["0","1", "2", "3", "4", "5", "6","7" ] }
                        // axisX:ValueAxis{visible: false} BarCategoryAxis {categories: ["1", "2", "3", "4", "5", "6","7", "8", "9", "10", "11", "12","13", "14", "15", "16", "17", "18","19", "20", "21", "22", "23", "24" ] }
                        BarSet {
                            id: bar;
                            values: model.modelData.listSpytnik

                            onValueChanged: {
                                gc();
                            }
                        }   //[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]}

                    }

                    //                    Connections{
                    //                        target: model.modelData

                    //                        onSignal_listSpytnikChanged:
                    //                        {

                    ////                            for(var i = 0;i < bar.count;i++)
                    ////                            {

                    ////                                if(bar.values[i] != model.modelData.listSpytnik[i])
                    ////                                {
                    ////                                    bar.replace(i,model.modelData.listSpytnik[i]);
                    ////                                }
                    ////                            }

                    ////                            console.log(bar.count);

                    //                            gc();

                    //                        }
                    //                    }
                }




                Text {
                    id: npI
                    //text:qsTr("I: ") + model.tok + qsTr(" А")

                    text:qsTr("I: ") + model.modelData.tok + qsTr(" А")

                    //  text:qsTr("I: ") + model.modelData.timeTimer + qsTr(" А")

                    anchors {
                        top: menuRectItemDashboard.bottom
                        topMargin: 10
                        left: parent.left
                        leftMargin: 10
                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Text {
                    id: npV
                    // text:qsTr("   V: ") + model.volt +qsTr(" В")
                    text:qsTr("   V: ") + model.modelData.v +qsTr(" В")
                    anchors.leftMargin: 70

                    anchors {
                        top: menuRectItemDashboard.bottom
                        topMargin: 10
                        right: npP.left
                        rightMargin: 5
                        left: npI.right
                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Text {
                    id: npP
                    //text:qsTr(" P: ") + model.vat +qsTr(" Вт")
                    text:qsTr(" P: ") + model.modelData.power +qsTr(" Вт")

                    anchors {
                        top: menuRectItemDashboard.bottom
                        topMargin: 10
                        right: parent.right
                        rightMargin: 10
                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Text {
                    id: npName
                    //text:qsTr("Количество сгенирированных спутников: ") + sum(model.countSpytnikGPS,model.countSpytnikGLONASS)
                    text:qsTr("Количество сгенирированных спутников: ") + sum(model.modelData.gpsGen,model.modelData.glonassGen)
                    anchors {
                        top: npI.top
                        topMargin: 20
                        right: parent.right
                        rightMargin: 10
                        left: parent.left
                        leftMargin: 10
                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Text {
                    id: npNameGPS
                    //text:qsTr("GPS: ") + model.countSpytnikGPS

                    text:qsTr("GPS: ") + model.modelData.gpsGen

                    anchors {
                        top: npName.top
                        topMargin: 25
                        left: parent.left
                        leftMargin: 50
                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Text {
                    id: npNameGLONASS
                    //text:qsTr("GLONASS: ") + model.countSpytnikGLONASS

                    text:qsTr("GLONASS: ") + model.modelData.glonassGen
                    anchors {
                        top: npName.top
                        topMargin: 25
                        right: parent.right
                        rightMargin: 50

                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }


                Text {
                    id: npName2
                    text:qsTr("Поймано спутников: ") +  sum(model.modelData.gpsFind,model.modelData.glonassFind)
                    anchors {
                        top: npNameGLONASS.top
                        topMargin: 20
                        right: parent.right
                        rightMargin: 10
                        left: parent.left
                        leftMargin: (parent.width - (npI.width + npP.width))/3
                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Text {
                    id: npNameGPS2
                    text:qsTr("GPS: ") + model.modelData.gpsFind
                    anchors {
                        top: npName2.top
                        topMargin: 20
                        left: parent.left
                        leftMargin: 50
                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Text {
                    id: npNameGLONASS2
                    text:qsTr("GLONASS: ") +model.modelData.glonassFind
                    anchors {
                        top: npName2.top
                        topMargin: 20
                        right: parent.right
                        rightMargin: 50
                        bottom: groupBoxGen.top
                        bottomMargin: 10
                    }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }


                MouseArea {
                    enabled: true
                    visible: true
                    anchors.fill: parent
                    onClicked: view.currentIndex = model.index

                }


                Button{
                    id: tableButton

                    height: 23
                    text: qsTr("Подробней")
                    z: 1
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0




                    onClicked: {
                        view.currentIndex = model.index


                        // pageLoader.loaded();

                        dialogNp.show();


                    }

                }

                // Запускаем диалог по нажатию любой из кнопок в главном окне

                Loader {
                    id: pageLoader
                    asynchronous: true
                    sourceComponent: dialogNp

                }

                // Создаём объект диалогового окна
                Window {
                    id: dialogNp

                    /* Когда деплоите под Android-устройства,
                     * обязательно закоментируйте эти две строки,
                     * иначе словите глюки в работе устройства
                     */
                    maximumWidth : 500

                    maximumHeight : 500


                    width: 500 //900  // Задаём ширину диалога, работает на десктопе, но на Android не сработает
                    height: 500 //680 // Задаём высоту диалога, работает на декстопе, но на Android не сработает
                    title: qsTr("ПРМ № " + model.modelData.indexNP)
                    modality: Qt.NonModal

                    // Создаём содержимое диалогового окна
                    // contentItem:
                    Dialog_NP {


                    }

                    Connections
                    {
                        target: proverka


                        onSignal_Finish:
                        {
                            dialogNp.close();
                            gc();
                        }
                    }
                }



                Text {
                    anchors.top: idColumnLayer.top
                    anchors.topMargin: 0
                    anchors.bottomMargin: 0
                    anchors.left: idColumnLayer.right
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.bottom: tableButton.top

                    id: textTimer

                    text: qsTr(model.modelData.timeTimer)
                    verticalAlignment: Text.AlignVCenter
                    transformOrigin: Item.Center
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter


                }

                //                ColumnLayout {

                //                    anchors.top: idColumnLayer.top
                //                    anchors.topMargin: 0
                //                    anchors.bottomMargin: 0
                //                    anchors.left: idColumnLayer.right
                //                    anchors.leftMargin: 0
                //                    anchors.right: parent.right
                //                    anchors.bottom: tableButton.top

                //                  //  ExclusiveGroup { id: tabPositionGroupTimer }


                //                    //                    Connections
                //                    //                    {
                //                    //                        target: proverka

                //                    //                        onSignal_Tick:
                //                    //                        {

                //                    //                            switch(index)
                //                    //                            {
                //                    //                               case"1":
                //                    //                                   textTimer.text = textTimerTick
                //                    //                            }

                //                    //                        }


                //                    //                    }


                //                    Text {
                //                        id: textTimer

                //                        text: qsTr(model.modelData.timeTimer)
                //                        horizontalAlignment: Text.AlignHCenter
                //                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                //                        Layout.fillWidth: false
                //                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter


                //                    }

                //                }






                ColumnLayout {
                    id: idColumnLayer
                    y: 339
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.bottom: tableButton.top
                    anchors.bottomMargin: 0

                    GroupBox
                    {
                        id: groupBoxGen
                        flat: true
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        ColumnLayout {
                            ExclusiveGroup { id: tabPositionGroup }
                            RadioButton {
                                id: radioButtonVnGen
                                exclusiveGroup: tabPositionGroup
                                text: qsTr("Внешний генератор")
                                Layout.fillWidth: true
                                // checked: model.vnGen


                            }

                            RadioButton {
                                id: radioButtonVnytrenGen
                                exclusiveGroup: tabPositionGroup
                                text: qsTr("Внутренний генератор")
                                Layout.fillHeight: false
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                // checked: model.vnytrenGen
                            }

                            MouseArea {
                                anchors.fill: parent
                                visible: true
                                onClicked: view.currentIndex = model.index
                            }

                        }
                    }
                }



            }
        }
    }
}

