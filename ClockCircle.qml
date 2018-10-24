import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQml 2.2
// После того, как объект зарегистрирован в C++ слое
// его необходимо подключить в QML
import ClockCircle 1.0
import QtQuick.Layouts 1.0



Item {
    id:clock

    property string textTimerSet: "00:00:05.000";




    function setTimerConst()
    {

        var list = textTimerSet.split(':');


        console.log("List = " + list);

        var mSec = list[2].split('.');

        console.log("mSec = " + mSec);

        //Чтоб посчитать время и установить констунту нужно
        //Если 50 милисекунд это шаг тогда в 1 секунде 20 шагов
        // в 1 минуте 20*60 и т.д
        //Разделить 360 на получившееся число и установить константу в Angle();

        var vremRSec = mSec[0] * 20 ;

        var vremRMin = list[1] * (20 * 60);

        var vremRHour = list[0] * (20*60*60);


        console.log("vremRSec = " + vremRSec );
        console.log("vremRMin = " + vremRMin );
        console.log("vremRHour = " + vremRHour );



        clockCircle.constAngle = 360/(vremRSec + vremRMin + vremRHour);

        console.log("clockCircle.constAngle = " + clockCircle.constAngle);

    }

    Rectangle
    {
        id:statusCamber
        radius: 1
        border.color: "#99d3f0"
        border.width: 4
        x: 8
        y: 20
        width: 250
        height: 100
        color: "#f7f7f7"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenterOffset:  -300
        anchors.verticalCenterOffset: 0


        Label
        {
            id:statusCamberNameText
            x: 4
            y: 3

            //anchors.horizontalCenter: parent.horizontalCenter
           // anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: "Cостояние камеры:"
            font.bold: true
            style: Text.Normal
            font.weight: Font.Normal
            font.capitalization: Font.MixedCase
            font.strikeout: false
            font.underline: false
            font.italic: true
            anchors.topMargin: 100
            font.pointSize: 10
        }

        Label
        {
            id:dataCamberNameText
            x: 5
            y: 79
            text: "Дата:"
            textFormat: Text.RichText

            font.bold: true
            style: Text.Normal
            font.weight: Font.Normal
            font.capitalization: Font.MixedCase
            font.strikeout: false
            font.underline: false
            font.italic: true
            anchors.topMargin: 100
            font.pointSize: 10
        }

        Label
        {
            id:statusCamberText
            x: 48

            textFormat: Text.RichText

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: ""
            clip: false
            horizontalAlignment: Text.AlignLeft
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            anchors.horizontalCenterOffset: -36
            anchors.verticalCenterOffset: 0
            font.pointSize: 15
        }
    }

    Rectangle
    {
        id:hortCamber
        radius: 1
        border.color: "#99d3f0"
        border.width: 4
        x: 8
        y: 20
        width: 250
        height: 100
        color: "#f7f7f7"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenterOffset:  250
        anchors.verticalCenterOffset: 50

        Label
        {
            id:hortCambeNameText
            x: 80
            y: 3

            //anchors.horizontalCenter: parent.horizontalCenter
           // anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: "Нагреватель:"
            font.bold: true
            style: Text.Normal
            font.weight: Font.Normal
            font.capitalization: Font.MixedCase
            font.strikeout: false
            font.underline: false
            font.italic: true
            anchors.topMargin: 100
            font.pointSize: 10
        }

        Label
        {
            id:hortCamberText
            textFormat: Text.RichText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: ""
            font.pointSize: 30
        }
    }

    Rectangle
    {
        id:coldCamber
        radius: 1
        border.color: "#99d3f0"
        border.width: 4
        x: 8
        y: 20
        width: 250
        height: 100
        color: "#f7f7f7"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenterOffset:  250
        anchors.verticalCenterOffset: -50

        Label
        {
            id:coldCamberNameText
            x: 60
            y: 3

            //anchors.horizontalCenter: parent.horizontalCenter
           // anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: "Охлаждение:"
            font.bold: true
            style: Text.Normal
            font.weight: Font.Normal
            font.capitalization: Font.MixedCase
            font.strikeout: false
            font.underline: false
            font.italic: true
            anchors.topMargin: 100
            font.pointSize: 10
        }

        Label
        {
            id:coldCamberText
            x: 70
            width: 117
            height: 38
            textFormat: Text.RichText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: ""
            anchors.horizontalCenterOffset: 10
            anchors.verticalCenterOffset: 0
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pointSize: 20
        }
    }




    Rectangle
    {
        id:timeEndCamber
        radius: 40
        border.color: "#99d3f0"
        border.width: 4
        x: 8
        y: 20
        width: 200
        height: 200
        color: "#f7f7f7"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenterOffset:  150
        anchors.verticalCenterOffset: 200

        Label
        {
            id:timeEndCamberNameText
            x: 100
            y: 30
            width: 129
            height: 40

            //anchors.horizontalCenter: parent.horizontalCenter
           // anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: "Таймер\nзавершения:"
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.bold: true
            style: Text.Normal
            font.weight: Font.Normal
            font.capitalization: Font.MixedCase
            font.strikeout: false
            font.underline: false
            font.italic: true
            anchors.topMargin: 100
            font.pointSize: 10
        }

        Label
        {
            id:timeEndCamberText
            textFormat: Text.RichText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: ""
            font.pointSize: 20
        }
    }

    Rectangle
    {
        id:timeStartCamber
        radius: 40
        border.color: "#99d3f0"
        border.width: 4
        x: 8
        y: 20
        width: 200
        height: 200
        color: "#f7f7f7"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenterOffset:  -150
        anchors.verticalCenterOffset: 200

        Label
        {
            id:timeStartCamberNameText
            x: 20
            y: 30
            width: 129
            height: 40

            //anchors.horizontalCenter: parent.horizontalCenter
           // anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: "Таймер\nкамеры:"
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.bold: true
            style: Text.Normal
            font.weight: Font.Normal
            font.capitalization: Font.MixedCase
            font.strikeout: false
            font.underline: false
            font.italic: true
            anchors.topMargin: 100
            font.pointSize: 10
        }


        Label
        {
            id:timeStartCamberText
            textFormat: Text.RichText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: ""
            font.pointSize: 20
        }
    }

    Rectangle
    {
        id:temratureStayCamber
        radius: 40
        border.color: "#99d3f0"
        border.width: 4
        x: 8
        y: 20
        width: 200
        height: 200
        color: "#f7f7f7"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenterOffset:  -150
        anchors.verticalCenterOffset: -200

        Label
        {
            id:temratureStayCamberNameText
            x: 20
            y: 6

            //anchors.horizontalCenter: parent.horizontalCenter
           // anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: "Заданная температура\nкамеры:"
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.bold: true
            style: Text.Normal
            font.weight: Font.Normal
            font.capitalization: Font.MixedCase
            font.strikeout: false
            font.underline: false
            font.italic: true
            anchors.topMargin: 100
            font.pointSize: 10
        }

        Label
        {
            id:temratureStayCamberText
            textFormat: Text.RichText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: ""
            font.pointSize: 30
        }
    }

    Rectangle
    {
        id:temratureCamber
        radius: 40
        border.color: "#99d3f0"
        border.width: 4
        x: 8
        y: 20
        width: 200
        height: 200
        color: "#f7f7f7"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenterOffset:  150
        anchors.verticalCenterOffset: -200

        Label
        {
            id:temratureCamberNameText
            x: 20
            y: 6

            //anchors.horizontalCenter: parent.horizontalCenter
           // anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: "Температура камеры:"
            font.bold: true
            style: Text.Normal
            font.weight: Font.Normal
            font.capitalization: Font.MixedCase
            font.strikeout: false
            font.underline: false
            font.italic: true
            anchors.topMargin: 100
            font.pointSize: 10
        }


        Label
        {
            id:temratureCamberText
            textFormat: Text.RichText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            //anchors.horizontalCenterOffset:  -10
            text: ""
            font.pointSize: 30
        }
    }



    // А теперь добавляем объект
    ClockCircle {
        id: clockCircle
        objectName: "MyClock"
        // позиционируем его и задаём размеры
        //  anchors.top: parent.top
        // anchors.topMargin: 50
        // anchors.horizontalCenter: parent.horizontalCenter
        // width: 200
        // height: 200

        // Определяем его свойства, которые Q_PROPERTY
        name: "clock"
        width: 400
        height: 400
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        backgroundColor: "whiteSmoke"
        borderActiveColor: "LightSlateGray"
        borderNonActiveColor: "LightSteelBlue"

        // Добавляем текст, на который будет выставляться время таймера
        Text {
            id: textTimer
            anchors.centerIn: parent
            font.bold: true
            font.pixelSize: 24
        }


        ColumnLayout {
            id:busyIndicatorLayout
            visible: true
            x: 0
            y: 0
            width: 56
            height: 78
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Label {
                id: label
                //text: qsTr("Загрузка имитатора на имитацию сигнала...")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                Layout.fillHeight: false
                Layout.fillWidth: true
                font.pixelSize: 24
                font.bold: true
            }

            BusyIndicator {
                id: busyIndicator
                spacing: 10

                running: true

            }
        }



        Connections
        {
            target: proverka

            // @disable-check M16
            onQml_text:
            {
                label.text = qsTr(str);
            }

            // @disable-check M16
            onSignal_StartProverka:
            {
                view.currentIndex = 3;
            }
        }



        // При изменении времени, помещаем это время на таймер
        onCircleTimeChanged: {
            textTimer.text = Qt.formatTime(circleTime, "hh:mm:ss.zzz");
            //В дальнейшем установить время переменной для изменения.
            //Сейчас таймер стоит на 3 часа
            //console.log(textTimer.text );
            if(textTimer.text == textTimerSet)//"00:00:02.000")
            {
                clockCircle.clear();
                clockCircle.stop();
                textTimer.text = "";


                //label.text = qsTr("Загрузка имитатора на имитацию сигнала...");

                //Показать что запускается имитатор
                busyIndicatorLayout.visible = true;

                //Запускаем проверку
                clockCircle.slot_Finish();


                view.currentIndex = 4;

//                if(proverka.autoProverka && ( proverka.modeStart == "Регулировка" || proverka.modeStart == "ПСИ"))
//                {
//                    proverka.slot_StartProverka();
//                }
//                else
//                {

//                    if(proverka.autoProverka && ( proverka.modeStart == "После технической тренировки в НУ" || proverka.modeStart == "Проверка неприрывной работы"))
//                    {
//                        proverka.slot_StartProverka2();
//                    }
//                    else
//                    {
//                        //написать код который будет запускать разные првоерки
//                        //относительно той которую выбрали.
//                        proverka.slot_StartProverka3();
//                    }

//                }

            }
            gc();
        }
    }

    Connections {
        target: gsg // Указываем целевое соединение
        /* Объявляем и реализуем функцию, как параметр
             * объекта и с имененем похожим на название сигнала
             * Разница в том, что добавляем в начале on и далее пишем
             * с заглавной буквы
             * */
        // @disable-check M16
        onSignal_getCONTrolQML: {

            label.text = "";

        }
    }

    Connections
    {
        target:camber

        // @disable-check M16
        onSendData:
        {
            //editText.text = dataHTML;


            statusCamberText.text = current;
            dataCamberNameText.text = "Дата: " + dataTime;
            timeStartCamberText.text = totalTime;
            timeEndCamberText.text = stepTimeLeft;
            temratureStayCamberText.text = tempSV;
            temratureCamberText.text = tempPV;
            hortCamberText.text = tempHeater;
            coldCamberText.text = refrigeration;



            if(tempPV == tempSV && busyIndicatorLayout.visible == true && label.text !== qsTr("Загрузка имитатора на имитацию сигнала..."))
            {
                busyIndicatorLayout.visible = false;
                clockCircle.start();
            }

            if(busyIndicatorLayout.visible == true && label.text !== qsTr("Загрузка имитатора на имитацию сигнала..."))
            {
                label.text = qsTr("Ждем заданную температуру камеры "+temratureStayCamberText.text+"...");
            }

            gc();
        }

        // @disable-check M16
        onSignal_startTimer:
        {
            busyIndicatorLayout.visible = false;
            clockCircle.start();
            gc();
        }

        // @disable-check M16
        onSignal_startCamberWork:
        {

            busyIndicatorLayout.visible = true;
        }
    }


//    Button {
//        id: start
//        text: "Start"
//        onClicked: {clockCircle.start(),camber.slot_startCamberWork();} // Запуск таймера
//        anchors {
//            left: parent.left
//            leftMargin: 20
//            bottom: parent.bottom
//            bottomMargin: 20
//        }
//    }

//    Button {
//        id: connect
//        text: "Connect"
//        onClicked: {camber.slot_connectCamber();} // Запуск таймера
//        anchors {
//            left: parent.left
//            leftMargin: 300
//            bottom: parent.bottom
//            bottomMargin: 20
//        }
//    }


//    Button {
//        id: stop
//        text: "Stop"
//        onClicked:  {clockCircle.stop();camber.slot_stopCamberWork();} // остановка таймера
//        anchors {
//            horizontalCenter: parent.horizontalCenter
//            bottom: parent.bottom
//            bottomMargin: 20
//        }
//    }

//    Button {
//        id: clear
//        text: "Clear"
//        onClicked: clockCircle.clear(); // очистка таймера
//        anchors {
//            right: parent.right
//            rightMargin: 20
//            bottom: parent.bottom
//            bottomMargin: 20
//        }
//    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
