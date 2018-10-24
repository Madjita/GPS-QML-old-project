import QtQuick 2.11
import QtQuick.Controls 2.2

import QtQuick.Layouts 1.3

import QtGraphicalEffects 1.0



Item{
    id: topMenu
    height: 25;

    property string tittleLabel: qsTr("Карты рабочих режимов")
    property string urlTitleIcon: sysTryIcon.iconSource

    property string colorBackground: "#2d2d30"
    property string colorTittle: "#999999"

    Rectangle {
        id: rectangle
        color: colorBackground
        anchors.fill: parent

        Image {
            id: titleIcon

            source: urlTitleIcon
            width: 16
            height: 16
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.Stretch

            visible: true


        }
        //        ColorOverlay{
        //            anchors.fill: titleIcon
        //            source:titleIcon
        //            color:"#bfbfc0"
        //            transform:rotation
        //            antialiasing: true
        //        }


        Label {
            text: tittleLabel
            anchors.left: titleIcon.right
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
            color: colorTittle
            font.pointSize: 8
        }

        Rectangle
        {
            id: minPNG
            anchors.right: maxPNG.left
            anchors.rightMargin: 1


            property bool isClose: false

            width: 40
            color: isClose ? "#3f3f41" : "transparent"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Canvas {
                id: minPNG_convas

                anchors.centerIn: parent
                width: 10; height: 10
                property string colorStrokeStyle: "#bfbfc0"

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.beginPath();
                    ctx.strokeStyle = colorStrokeStyle;
                    ctx.moveTo(0,height/2);
                    ctx.lineTo(width,height/2);
                    ctx.stroke();
                    ctx.closePath();
                }

                layer.enabled: true
                layer.smooth: true
                layer.samples: 8
                visible: true

            }

            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                onClicked:
                {
                    if( mainWindow.stWindow === "4")
                        mainWindow.stWindow += "3";

                    mainWindow.visibility = 3 ;
                }

                onHoveredChanged:
                {
                    minPNG.isClose = !minPNG.isClose
                    minPNG_convas.colorStrokeStyle = minPNG.isClose ? "white" : "#bfbfc0";
                    minPNG_convas.requestPaint();
                }
            }
        }

        Rectangle
        {
            id: maxPNG
            anchors.right: closePNG.left
            anchors.rightMargin: 1


            property bool isClose: false
            width: 40
            color: isClose ? "#3f3f41" : "transparent"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Canvas {
                id: maxPNG_convas

                anchors.centerIn: parent
                width: 10; height: 10
                property string colorStrokeStyle: "#bfbfc0"

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.strokeStyle = "white";
                    ctx.strokeStyle = colorStrokeStyle;
                    ctx.strokeRect(0, 0, width, height);
                    ctx.closePath();
                }

                layer.enabled: true
                layer.smooth: true
                layer.samples: 8
                visible: true

            }

            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                onClicked:
                {
                    if(mainWindow.visibility !== ApplicationWindow.Maximized )
                    {
                        mainWindow.stWindow = "";
                        mainWindow.stWindow += "4";
                        mainWindow.showMaximized();

                        topArea.visible = !topArea.visible;
                        bottomArea.visible = !bottomArea.visible;
                        leftArea.visible = !leftArea.visible;
                        rightArea.visible = !rightArea.visible;
                        //arelMouse.visible = !arelMouse.visible;
                    }
                    else
                    {
                        mainWindow.stWindow = "";
                        mainWindow.showNormal();
                    }
                }

                onHoveredChanged:
                {
                    maxPNG.isClose = !maxPNG.isClose

                    maxPNG_convas.colorStrokeStyle = maxPNG.isClose ?  "white" : "#bfbfc0";
                    maxPNG_convas.requestPaint();
                }
            }
        }


        Rectangle
        {
            id: closePNG
            anchors.right: parent.right
            anchors.rightMargin: 0


            property bool isClose: false

            width: 40
            color: isClose ? "#e81123" : "transparent"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Canvas {
                id:closePNG_convas
                anchors.centerIn: parent
                width: 10; height: 10

                property string colorStrokeStyle: "#bfbfc0"

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.lineWidth=1;

                    ctx.beginPath();
                    ctx.strokeStyle = colorStrokeStyle;
                    ctx.moveTo(0,0);
                    ctx.lineTo(width,height);
                    ctx.moveTo(0,height);
                    ctx.lineTo(width,0);
                    ctx.stroke();
                    ctx.fill();
                    ctx.closePath();
                }

                layer.enabled: true
                layer.smooth: true
                layer.samples: 8

            }

            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                onClicked:
                {
                    Qt.quit();
                }

                onHoveredChanged:
                {
                    closePNG.isClose = !closePNG.isClose;
                    closePNG_convas.colorStrokeStyle = closePNG.isClose ? "white" : "#bfbfc0";
                    closePNG_convas.requestPaint();
                }
            }
        }



    //////////////////////////

        /////////////


        // Центральная область для перемещения окна приложения
        // Здесь уже потребуется использовать положение как по оси X, так и по оси Y
        MouseArea {
            z:1
            id:arelMouse
            property variant clickPos: "1,1"
            anchors.right: minPNG.left
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.left: titleIcon.right
            anchors.top: parent.top


            onPressed: {
                clickPos  = Qt.point(mouse.x,mouse.y)
            }

            onPositionChanged: {

                if(mainWindow.visibility === ApplicationWindow.Maximized )
                {
                    mainWindow.showNormal();

                    topArea.visible = !topArea.visible;
                    bottomArea.visible = !bottomArea.visible;
                    leftArea.visible = !leftArea.visible;
                    rightArea.visible = !rightArea.visible;
                }
                else
                {

                    var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                    mainWindow.x += delta.x;
                    mainWindow.y += delta.y;
                }

            }

            onDoubleClicked: {

                if(mainWindow.visibility !== ApplicationWindow.Maximized )
                {
                    mainWindow.stWindow = "";
                    mainWindow.stWindow += "4";
                    mainWindow.showMaximized();

                    topArea.visible = !topArea.visible;
                    bottomArea.visible = !bottomArea.visible;
                    leftArea.visible = !leftArea.visible;
                    rightArea.visible = !rightArea.visible;
                }
                else
                {
                    mainWindow.stWindow = "";
                    mainWindow.showNormal();
                }

            }
        }

    }

}
