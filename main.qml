import QtQuick 2.5

import QtQuick.Layouts 1.0


import Qt.labs.platform 1.0

import QtQuick.Controls 1.4 as Controls

import QtQuick.Controls 2.0 as Controls2

import QtQuick.Window 2.0

Controls.ApplicationWindow {
    id: window
    visible: true
    width: 1200
    height: 900
    title: qsTr("GPS GLONASS test")


    menuBar: Controls.MenuBar {
        Controls.Menu {
            title: qsTr("Меню")

            Controls.MenuItem {
                text: qsTr("Выход")
                shortcut: "Ctrl+F4"
                onTriggered: Qt.quit()
            }
        }

        Controls.Menu {
            id: menuProverka
            visible: false
            title: qsTr("Проверки")
            Controls.MenuItem {
                id:autoProverka
                text: qsTr("Автоматизированная проверка")
                shortcut: "Ctrl+A"
                onTriggered:  checkable = true;
                checkable: true
                checked: true


            }
        }

        Controls.Menu {
            id: menuTimer
            visible: false
            title: qsTr("Таймер")
            Controls.MenuItem {
                id:menuItemTimer
                text: qsTr("Настройка таймера")
                shortcut: "Ctrl+T"
                onTriggered:  timerDialog.show();


            }
        }

    }


    Shortcut
    {
        sequence: "Ctrl+E"
        onActivated: {
            if(menuProverka.visible)
            {
                menuProverka.visible = false;
            }
            else
            {
               menuProverka.visible = true;
               //autoProverka.enabled = false;
            }
        }

    }



    Window{
        id: timerDialog
        width: 250
        height: 100

        title: qsTr("Таймер")

        maximumWidth : 250

        maximumHeight : 100

        modality: Qt.ApplicationModal

        GridLayout
        {
            columns: 2
            anchors.fill: parent
            Controls2.TextField
            {
                id: textTimer
                placeholderText: qsTr("Время таймера:")
                text:  qsTr(camberTimer.textTimerSet)
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 2
                font.capitalization: Font.MixedCase
                font.bold: true
                font.pixelSize: 20
                horizontalAlignment : TextInput.AlignHCenter
            }

            Controls.Button
            {
                text: "Отмена"
                Layout.fillHeight: true
                Layout.fillWidth: true

                onClicked: {
                    timerDialog.close();
                    gc();

                }
            }

            Controls.Button
            {
                text: "Установить"
                Layout.fillHeight: true
                Layout.fillWidth: true

                onClicked: {
                    camberTimer.textTimerSet = textTimer.text;

                    camberTimer.setTimerConst();

                    timerDialog.close();
                    gc();

                }
            }


        }

    }

    //    Dialog {
    //        id:dialogMode
    //        visible: false
    //        title: "Выбрать режим"
    //        modality: Qt.WindowModal

    //        width: 300
    //        height: 200

    //        maximumWidth : 300

    //        maximumHeight : 200
    //    }


    Rectangle
    {
        z: 3
        id: boxSliver
        color: "white"  //"grey"
        anchors.fill:  parent
        visible: false
    }


    ClockCircle
    {
        z: 4
        anchors.fill: parent
        id: camberTimer
        visible: false
        //asynchronous: true
        // source: "ClockCircle.qml";
    }


    /* С помощью объекта Connections
         * Устанавливаем соединение с классом ядра приложения
         * */
    Connections {
        target: gsg // Указываем целевое соединение
        /* Объявляем и реализуем функцию, как параметр
             * объекта и с имененем похожим на название сигнала
             * Разница в том, что добавляем в начале on и далее пишем
             * с заглавной буквы
             * */
        onSignal_getCONTrolQML: {

            //screenObject.capture("Ждем запуска имитатора.jpg")

            boxSliver.visible = false;


            //Закрыть окна камеры (ждем установки темпиратуры)
            camberTimer.visible = false;
            swipeView.visible = true;

            tabBar.visible = false;
            swipeView.setCurrentIndex(4);

            //показать информацию о камере в проверках
            textCamberInform.visible = true;


            proverka.slot_StartProverka_Next();

        }
    }


    Connections
    {
        target: proverka

        onSignal_stopCamberWorkProverka:
        {
            camber.slot_stopCamberWork();
        }
        // @disable-check M16
        onSignal_startCold:
        {
            autoProverka.enabled = false;

            camber.slot_stopCamberWork();

            camber.slot_SetCold();

            boxSliver.visible = true;

            //Запуск окна камеры (ждем установки темпиратуры)
            camberTimer.visible = true;

            //Запуск окна имитатора (ждем запуска имитатора)
            //messageImitator.visible = true;
            swipeView.visible = false;

            tabBar.visible = false;
            textCamberInform.visible = false;

            //Сначало установить темпиратуру 27 градусов (прграмма номер 3)


            //Затем запустить камеру.
            camber.slot_startCamberWork();

            //Отключение не нужных кнопок для отображения
            start.visible = false;
            showResult.visible = false;
            creatPDF.visible = false;
        }

        // @disable-check M16
        onSignal_startNY:
        {
            camber.slot_stopCamberWork();

            camber.slot_SetNY();

            boxSliver.visible = true;

            //Запуск окна камеры (ждем установки темпиратуры)
            camberTimer.visible = true;

            //Запуск окна имитатора (ждем запуска имитатора)
            //messageImitator.visible = true;
            swipeView.visible = false;

            tabBar.visible = false;
            textCamberInform.visible = false;

            //Сначало установить темпиратуру 27 градусов (прграмма номер 3)


            //Затем запустить камеру.
            camber.slot_startCamberWork();

            //Отключение не нужных кнопок для отображения
            start.visible = false;
            showResult.visible = false;
            creatPDF.visible = false;
        }

        // @disable-check M16
        onSignal_startHord:
        {
            camber.slot_stopCamberWork();

            camber.slot_SetHord();


            boxSliver.visible = true;

            //Запуск окна камеры (ждем установки темпиратуры)
            camberTimer.visible = true;

            //Запуск окна имитатора (ждем запуска имитатора)
            swipeView.visible = false;
            textCamberInform.visible = false;

            tabBar.visible = false;

            //Сначало установить темпиратуру 27 градусов (прграмма номер 3)


            //Затем запустить камеру.
            camber.slot_startCamberWork();

            //Отключение не нужных кнопок для отображения
            start.visible = false;
            showResult.visible = false;
            creatPDF.visible = false;
        }
    }


    toolBar: Controls.ToolBar{

        id: tabHeaderBar

        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5


            Controls.Button {
                id: start
                text: qsTr("Начать проверку")
                visible: false;


                function startCamberWork()
                {
                    //Запустить установку темпиратуры
                    if(camber.slot_TestConnect())
                    {
                        boxSliver.visible = true;

                        //Запуск окна камеры (ждем установки темпиратуры)
                        camberTimer.visible = true;

                        //Запуск окна имитатора (ждем запуска имитатора)

                        swipeView.visible = false;
                        textCamberInform.visible = false;

                        tabBar.visible = false;

                        //Сначало установить темпиратуру 27 градусов (прграмма номер 3)
                        camber.slot_SetNY();

                        //Затем запустить камеру.
                        camber.slot_startCamberWork();

                        //Отключение не нужных кнопок для отображения
                        start.visible = false;
                        showResult.visible = false;
                        creatPDF.visible = false;

                    }
                    else
                    {
                        console.log("EROROROOROROR CONNECT");
                    }
                }

                onClicked: {

                    if(autoProverka.checked)
                    {
                        proverka.autoProverka = true;
                        startCamberWork();
                    }
                    else
                    {
                        proverka.autoProverka = false;
                        swipeView.setCurrentIndex(2);
                        console.log("Запустить не автоматизированную проверку");

                        console.log(proverka.autoProverka);
                    }




                    //proverka.slot_StartProverka();




                    // gsg.setSIGNALtype("START");
                    // gsg.slot_StartTimer();
                    // gsg.slot_Work();

                    // n6700_1.slot_StartTimer();
                    //  gsg.slot_StartTimer();
                    // gsg.slot_Work();
                    // n6700_1.slot_Work();
                }
            }
            Controls.Button {
                id: showResult
                visible: false;


                text: qsTr("Посмотреть результаты")

                onClicked: {
                    tabBar.currentIndex = 3;
                }
            }

        }

        Controls.Button {
            id: creatPDF
            text: qsTr("Сгенирировать отчет")
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            visible: false;

            onClicked: {
                pdf.slot_FindData();
            }
        }


        Connections
        {
            target:camber

            // @disable-check M16
            onSendData:
            {
                textCamberInform.text = "Темпиратура: <b>"+tempSV+"</b> Темпиратура в камере: <b>"+tempPV+"</b>\nРежим камеры: <b>"+current+"</b> нагерватель: <b>"+tempHeater+"</b> охлаждение:  <b>"+refrigeration+"</b>";
                gc();
            }

        }

        Text {
            id: textCamberInform
            width: 500
            //text: qsTr("Темпиратура: <b>42С</b> Темпиратура в камере: <b>40С</b>\nРежим камеры: <b>PROGRAM: RUN</b> нагерватель: <b>5%</b> охлаждение:  <b>STOP</b>")
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            // anchors.rightMargin: 10
            visible: false;
            font.pointSize: 10
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

        }


        Text {
            id: textFioWork
            y: 1
            width: 500
            height: 14
            text: qsTr("")
            font.pointSize: 7
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            anchors.left: parent.left
            anchors.leftMargin: 2
            visible: false
            font.pixelSize: 12
        }

        Text {
            id: textResult
            y: 1
            width: 500
            height: 14
            text: qsTr("")
            font.pointSize: 7
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            anchors.left: parent.left
            anchors.leftMargin: 2
            visible: false
            font.pixelSize: 12
        }

    }

    Controls2.SwipeView {
        id: swipeView
        y: 0
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        //Событие обработки перелистывания
        onCurrentIndexChanged:
        {
            //Отображение определенных кнопок в зависимости от страницы на каторой находится пользователь
            switch(swipeView.currentIndex)
            {
            case 0:
                start.visible = false;
                showResult.visible = false;
                creatPDF.visible = false;

                textResult.visible = false;
                textFioWork.visible = false;

                textCamberInform.visible = false;

                break;
            case 1: //Показывает кнопки на верхней панеле для включения устрйоств
                start.visible = true;
                showResult.visible = true;
                creatPDF.visible = false;

                textResult.visible = false;
                textFioWork.visible = true;
                textCamberInform.visible = false;

                break;
            case 3:
                creatPDF.visible = true;
                textResult.visible = true;

                textFioWork.visible = false;
                start.visible = false;
                showResult.visible = false;
                textCamberInform.visible = false;

                break;
            case 4:
                textFioWork.visible = true;

                textResult.visible = false;
                showResult.visible = false;
                creatPDF.visible = false;
                start.visible = false;
                textCamberInform.visible = true;
                break;

            default:
                creatPDF.visible = false;
                if(start.visible == false || start.visible == false)
                {
                    start.visible = true;
                    showResult.visible = true;


                    textResult.visible = false;
                    textFioWork.visible = true;
                }
                break;
            }
        }

        StartPageEtap
        {


        }

        Page1 {


        }

        Page2 {

        }

        Page3 {

        }

        Page4
        {

        }


    }

    statusBar: Controls.StatusBar
    {

        Controls2.TabBar {
            anchors.fill: parent



            id: tabBar
            visible: false;
            currentIndex: swipeView.currentIndex


            Controls2.TabButton {
                text: qsTr("Режим")

            }
            Controls2.TabButton {
                text: qsTr("Устройства")

            }
            Controls2.TabButton {
                text: qsTr("Проверки")

            }
            Controls2.TabButton {
                text: qsTr("Результаты")

            }
            Controls2.TabButton {
                text: qsTr("Ход выполненения")

            }
        }
    }


    SystemTrayIcon {
        visible: true
        iconSource: "qrc:/img/logo.ico"



        Component.onCompleted: showMessage("Запуск АРМ", "Запуск программы автоматизации испытаний.")


        menu: Menu {
            MenuItem {
                text: qsTr("show")
                onTriggered:{window.show()
                    window.raise()
                    window.requestActivate()
                }
            }
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }

        }
    }
}
