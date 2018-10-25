import QtQuick 2.9
import QtQuick.Controls 2.2

import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Controls.Styles 1.4

import Qt.labs.platform 1.0

import QtGraphicalEffects 1.0
import QtWinExtras 1.0

import QtQml 2.2
import ClockCircle 1.0

ApplicationWindow {
    id: mainWindow
    width:1024
    height:768
    visible: true

    minimumHeight: 768
    minimumWidth: 1024

    color: "#ececed"
    property alias planeMain: planeMain
    property alias view: view
    title: qsTr("GPS GLONASS Test stend")


    /////////////////////////////////////////////////////

    flags: Qt.Window | Qt.FramelessWindowHint // Отключаем обрамление окна


    property string stWindow: ""

    onVisibilityChanged:
    {
        console.log("mainWindow.visibility = ",mainWindow.visibility,"  ; "+stWindow);
        if(stWindow === "43" && mainWindow.visibility === 2)
        {
            stWindow == "";
            mainWindow.showMaximized();

            //clockCircle.paint2();
        }
        else
        {
                       if(mainWindow.visibility === 2)
                       {
                           //forePageClockCircle.clockCircleItem.update();



            //               console.log("mainWindow.width  = ",mainWindow.width ,"mainWindow.height = ",mainWindow.height);
            //               mainWindow.width = 1024;
            //               mainWindow.height = 768;
            //               //mainWindow.showMaximized();
            //               console.log("mainWindow.width  = ",mainWindow.width ,"mainWindow.height = ",mainWindow.height);
                        }
        }



    }

    SystemTrayIcon {
        id: sysTryIcon
        visible: true
        iconSource: "qrc:/images/logo_blue.png"

        menu: Menu {
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }
    }

    DwmFeatures {
        id: dwmFeatures
        topGlassMargin: -1
        leftGlassMargin: -1
        rightGlassMargin: -1
        bottomGlassMargin: -1
    }

    TaskbarButton {
        id: taskbarButton
        overlay.iconSource: "qrc:/images/logo_blue.png"
        overlay.accessibleDescription: "qrc:/images/logo_blue.png"
        progress.visible: true
        progress.value: 0

        onActiveFocusChanged: {
            console.log("LOX");
        }

    }



    ThumbnailToolBar {
        ThumbnailToolButton { iconSource: "qrc:/images/closePNG.png"; tooltip: "Exit"; onClicked: Qt.quit() }
    }

    JumpList {
        id: jumpList
        recent.visible: true
        frequent.visible: true
        tasks.visible: true
    }







    // Объявляем свойства, которые будут хранить позицию зажатия курсора мыши
    property int previousX
    property int previousY

    MouseArea {
        id: topArea
        z:2
        height: 2
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        // Устанавливаем форму курсора, чтобы было понятно, что это изменение размера
        cursorShape: Qt.SizeVerCursor

        onPressed: {
            // Запоминаем позицию по оси Y
            previousY = mouseY
        }

        // При изменении позиции делаем пересчёт позиции окна, и его высоты
        onMouseYChanged: {
            var dy = mouseY - previousY;

            var h = mainWindow.height - dy;

            if(h > mainWindow.minimumHeight)
            {
              mainWindow.setY(mainWindow.y + dy)
              mainWindow.setHeight(mainWindow.height - dy)
            }
        }
    }

    // Аналогичные расчёты для остальных трёх областей ресайза
    MouseArea {
        id: bottomArea
        z:1
        height: 2
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        cursorShape: Qt.SizeVerCursor

        onPressed: {
            previousY = mouseY
        }

        onMouseYChanged: {
            var dy = mouseY - previousY

            var h = mainWindow.height + dy;

            if(h > mainWindow.minimumHeight)
            {
              mainWindow.setHeight(mainWindow.height + dy)
            }


        }
    }

    MouseArea {
        id: leftArea
        z:1
        width: 2
        anchors {
            top: topArea.bottom
            bottom: bottomArea.top
            left: parent.left
        }
        cursorShape: Qt.SizeHorCursor

        onPressed: {
            previousX = mouseX
        }

        onMouseXChanged: {
            var dx = mouseX - previousX

            var h = mainWindow.width - dx;

            if(h > mainWindow.minimumWidth)
            {
                mainWindow.setX(mainWindow.x + dx)
                mainWindow.setWidth(mainWindow.width - dx)
            }
        }
    }

    MouseArea {
        id: rightArea
        z:1
        width: 2
        anchors {
            top: topArea.bottom
            bottom: bottomArea.top
            right: parent.right
        }
        cursorShape:  Qt.SizeHorCursor

        onPressed: {
            previousX = mouseX
        }

        onMouseXChanged: {
            var dx = mouseX - previousX

            var h = mainWindow.width + dx;

            if(h > mainWindow.minimumWidth)
            {
                mainWindow.setWidth(mainWindow.width + dx)
            }
        }
    }






    ///////////////////////////////////////////////////



    MyMenu
    {
        id: windowTitle
        x:0
        anchors.top: parent.top
        anchors.topMargin: 0
        z: 1
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        colorBackground: "#565e70"
        colorTittle: "#f0f0f0"

        tittleLabel: title
    }



    // @disable-check M16
    onClosing:
    {
        console.log("CLOSE");
    }

        Rectangle
        {
            id: planeMain
            radius: 10
            width: 1024
            height: 743

            anchors.top: windowTitle.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.topMargin: 0




            color: "#ececed"




    Header {
        id: header
        width: parent.width
        height: 60

        anchors.top: windowTitle.bottom
        anchors.topMargin: 0

        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        z:1
    }


    //    Connections {
    //        target: proverka.listProverka // Указываем целевое соединение


    //        onSignal_proverkaNameChanged: {
    //            console.log("OK проверка ",proverka.listProverka[0].modelData.proverkaName);
    //        }
    //    }


    SwipeView {

        id: view

        width:1024
        height:768


        currentIndex: 0
        anchors.fill: parent
        interactive: false


        Item {
            id: firstPage


            Startwindow
            {
                id: startWindow
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Image {
                id: rightArrow
                source: "images/Arrow_grey.png"
                width: 25
                height: 20
                anchors.left: startWindow.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                transformOrigin: Item.Center

                MouseArea
                {
                    id:mouseArea_rightArrow
                    anchors.fill: parent
                    hoverEnabled : true
                    onClicked: {
                        startWindow.textField_FIO.focus = false;
                        console.log(startWindow.textField_FIO.text,startWindow.etap);

                        if(startWindow.textField_FIO.text != "")
                        {
                            header.exit_label.text = startWindow.textField_FIO.text;
                            header.etap_label.text = "Этап : " + startWindow.etap;
                            view.currentIndex = 1;
                            proverka.modeStart = startWindow.etap;
                            header.autoFlag.visible = true;
                        }
                        else
                        {

                        }
                    }
                    onContainsMouseChanged: {
                        if(this.containsMouse)
                            rightArrow.source = "images/Arrow.png";
                        else
                            rightArrow.source = "images/Arrow_grey.png";


                    }
                }

            }

        }



        Item {
            id: secondPage

            DevicePage
            {
                id: deviceWindow

                anchors.top: parent.top
                anchors.topMargin: 60
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10 // + botomMenu.height
                anchors.left: rightArrow_Left.right
                anchors.leftMargin: 5
                anchors.right: rightArrow_Right.left
                anchors.rightMargin: 5

            }

            Image {
                id: rightArrow_Right
                source: "images/Arrow_grey.png"
                width: 25
                height: 20
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.left: startWindow.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                transformOrigin: Item.Center

                MouseArea
                {
                    id:mouseArea_rightArrowDevice
                    anchors.fill: parent
                    hoverEnabled : true
                    onClicked: {
                        proverka.autoProverka = header.autoFlag.checked;
                        if(header.autoFlag.checked)
                        {
                            proverka.namberProverkaStart = 1;
                            proverka.namberModeCamber = 1;
                            proverka.slot_StartProverka();
                            view.currentIndex = 3;
                            header.autoFlag.enabled = false;
                        }
                        else
                        {
                            header.autoFlag.visible = false;
                            view.currentIndex = 2;
                        }

                    }
                    onContainsMouseChanged: {
                        if(this.containsMouse)
                            rightArrow_Right.source = "images/Arrow.png";
                        else
                            rightArrow_Right.source = "images/Arrow_grey.png";

                    }
                }

            }


            Image {
                id: rightArrow_Left
                source: "images/Arrow_grey.png"
                width: 25
                height: 20
                anchors.right: startWindow.left
                anchors.rightMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                transformOrigin: Item.Center
                rotation: 180

                MouseArea
                {
                    id:mouseArea_leftArrowDevice
                    anchors.fill: parent
                    hoverEnabled : true
                    onClicked: {
                        header.mouseArea_exit2.onClicked(mouse);

                    }
                    onContainsMouseChanged: {
                        if(this.containsMouse)
                            rightArrow_Left.source = "images/Arrow.png";
                        else
                            rightArrow_Left.source = "images/Arrow_grey.png";

                    }
                }

            }
        }


        Item {
            id: thirdPage

            Thirdwindow
            {
                id: thirdWindow

                anchors.top: parent.top
                anchors.topMargin: 60
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10 // + botomMenu.height
                anchors.left: rightArrow_LeftthirdWindow.right
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5

            }

            Image {
                id: rightArrow_LeftthirdWindow
                source: "images/Arrow_grey.png"
                width: 25
                height: 20
                anchors.right: startWindow.left
                anchors.rightMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                transformOrigin: Item.Center
                rotation: 180

                MouseArea
                {
                    id:mouseArea_leftthirdWindow
                    anchors.fill: parent
                    hoverEnabled : true
                    onClicked: {
                        view.currentIndex = 1;
                    }
                    onContainsMouseChanged: {
                        if(this.containsMouse)
                            rightArrow_LeftthirdWindow.source = "images/Arrow.png";
                        else
                            rightArrow_LeftthirdWindow.source = "images/Arrow_grey.png";


                    }
                }

            }
        }


        Item {
            id: forePage

            MyClock
            {
                anchors.fill: parent;
            }

        }



        Item {
            id: fivePage

            Proverkswindow
            {
                id: proverkiStartPage
                anchors.top: parent.top
                anchors.topMargin: 60
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10  + botomMenufivePage.height
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5
            }

            Item {
                id: botomMenufivePage

                height: 20
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0

                // Создаём горизонтальный разделитель с помощью Rectangle
                Rectangle {
                    id: dividerHorizontal2
                    color: "#d7d7d7"
                    height: 2 // Устанавливаем ширину в два пикселя
                    anchors.left: parent.left
                    anchors.right: parent.right
                    // anchors.bottom: row.top
                }


                Label
                {
                    anchors.verticalCenter: parent.verticalCenter

                    //text: this.visible ?  proverkiStartPage.viewProverka.model[0].proverkaName : ""

                    clip: true
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    //visible: proverka.listProverka[0].proverkaName ? 1:0

                }

                //                Label
                //                {
                //                    anchors.right: parent.right
                //                    anchors.rightMargin: 5
                //                    anchors.verticalCenter: parent.verticalCenter
                //                    text: resultatPage.creatPDF
                //                    clip: true
                //                    visible: resultatPage.creatPDF ? 1:0

                //                }

            }
        }


        Item {
            id: sixPage

            Reportwindow
            {
                id:  resultatPage
                anchors.top: parent.top
                anchors.topMargin: 60
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10 + botomMenuSixPage.height
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5
            }


            Item {
                id: botomMenuSixPage

                height: 30
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0

                Label
                {
                    anchors.verticalCenter: parent.verticalCenter

                    text:resultatPage.textResult
                    clip: true
                    anchors.left: parent.left
                    anchors.leftMargin: 10

                }

                Button
                {
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: resultatPage.creatPDF
                    clip: true
                    visible: resultatPage.creatPDF ? 1:0

                    onClicked: pdf.slot_FindData();
                }

            }
        }


    }



    //////


    }



}

/*##^## Designer {
    D{i:13;anchors_y:0}
}
 ##^##*/
