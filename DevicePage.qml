import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

import QtQuick.Window 2.2

import QtQuick.Controls.Universal 2.1
import QtQuick.Dialogs 1.2

import QtQml 2.2

Item {
    id: item1


    // Пересчёт независимых от плотности пикселей в физические пиксели устройства
    readonly property int dpi: Screen.pixelDensity * 25.4
    width:1024
    height:768
    function dp(x){ return (dpi < 120) ? x : x*(dpi/160); }

    Rectangle {
        id: skyblue
        color: "skyblue"
        z: -1
    }

    Rectangle {
        id: skyblue2
        color: "skyblue"
        z: -1
    }

    Window
    {
        id: messageDialog_button_FindIP_1

        width: 100; height: 100
        maximumWidth: 400;
        minimumWidth: 400;
        maximumHeight: 300;
        minimumHeight: 300;

        visible: false;
        x:mainWindow.width/2
        y:mainWindow.height/2

        property int plata : 0

        //modality: StandardButton.Discard

        title: qsTr("Поиск устройств")


        // @disable-check M16
        onClosing:
        {
            switch(plata)
            {
            case 1:  modelIp.text = find.listDevice[tableFind.currentRow].ip; break;
            case 2:  modelIp1.text = find.listDevice[tableFind.currentRow].ip; break;
            case 3:  modelIp2.text = find.listDevice[tableFind.currentRow].ip; break;
            case 4:  modelIp3.text = find.listDevice[tableFind.currentRow].ip; break;
            case 5:  modelIp4.text = find.listDevice[tableFind.currentRow].ip; break;
            case 6:  modelIp7.text = find.listDevice[tableFind.currentRow].ip; break;
            }

        }


        Component.onCompleted: {
            console.log("Открыли ");
        }



        TableView {
            id: tableFind
            anchors.bottom: button.top
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top



            onDoubleClicked:messageDialog_button_FindIP_1.close();

            model: find.listDevice


            onModelChanged:
            {
                console.log(find.listDevice.length);

                if(find.listDevice.length >=4)
                {

                for(var i=0;i < find.listDevice.length;i++)
                {
                    var item = find.listDevice[i];

                    if(item.content === "Agilent Technologies: N6700B: MY54009013")
                    {
                        if(n6700_1.getConnect() === false)
                        {
                            n6700_1.connectDevice(item.ip);
                        }
                    }

                    if(item.content === "KEYSIGHT TECHNOLOGIES: MSO-X 3022T: MY56310164")
                    {
                        if(os.slot_connectOk() === false)
                        {
                            os.connectDevice(item.ip);
                        }
                    }

                    if(item.content === "SPECTRACOM: GSG-64E: 201312")
                    {
                        if(gsg.slot_connectOk() === false)
                        {
                            gsg.connectDevice(item.ip);
                        }
                    }

                    var tpItem = item.content.split(':');

                    console.log(tpItem[tpItem.length]);
                    if(tpItem[tpItem.length-1] === " "+item.ip)
                    {
                        if(tp.get_flagConnect() === false)
                        {
                            tp.slot_ConnectTp(item.ip);
                            port1.openPort("COM1");
                            port3.openPort("COM4");
                        }
                    }
                }
                }

            }

            TableViewColumn {
                width: 100
                horizontalAlignment: Text.AlignHCenter
                title: "IP"
                role: "ip"
            }
            TableViewColumn {
                width: 296
                horizontalAlignment: Text.AlignHCenter
                title: "Прибор"
                role: "content"
            }
        }

        Button {
            id: button
            text: qsTr("Поиск устроств")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            onClicked: find.work();

        }

        Component.onDestruction: console.log("разрушили ");

    }



    GridLayout {
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        columnSpacing: 10
        rowSpacing: 10
        anchors.fill: parent
        rows: 2
        columns: 3


        Rectangle {
            id: rectangle1
            radius: 0
            transformOrigin: Item.Center
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.minimumHeight: 200
            Layout.minimumWidth: 200
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200
            border.color: "#000000"
            border.width: 1



            function bluBox()
            {
                skyblue3.visible = false
                skyblue.visible = true
                skyblue.x = rectangle1.x
                skyblue.y = rectangle1.y
                skyblue.width = rectangle1.width+20
                skyblue.height = rectangle1.height+20
                skyblue2.visible = false

                rectangle1.focus = true
                rectangle2.focus = false
                rectangle3.focus = false
                rectangle4.focus = false
                rectangle5.focus = false
                rectangle6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboard
                width: 600
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName
                    color: "#ffffff"
                    text: "Источник питания №1"
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


            GridView
            {
                id: gridIP
                z: 1
                anchors.horizontalCenterOffset: -40
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: label
                    x: -26
                    y: 0
                    text: qsTr("IP:")
                    font.pointSize: 11
                    width: 20
                    height: 20
                }

                TextField
                {
                    id: modelIp
                    width: 102
                    height: 20
                    text: "10.12.0.137"
                    z: 1
                    placeholderText: "10.12.0.139"


                    validator: RegExpValidator
                    {
                        regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }



                    onSelectionStartChanged:
                    {

                        if(rectangle1.focus == false)
                        {

                            skyblue3.visible = false;
                            skyblue.visible = true;
                            skyblue.x = rectangle1.x;
                            skyblue.y = rectangle1.y;
                            skyblue.width = rectangle1.width+20;
                            skyblue.height = rectangle1.height+20;
                            skyblue2.visible = false;


                            rectangle2.focus = false;
                            rectangle3.focus = false;
                            rectangle4.focus = false;
                            rectangle5.focus = false;
                            rectangle6.focus = false;
                        }

                    }


                }









                Button {
                    id: button_FindIP_1
                    x: 101.5
                    y: -1
                    width: 20
                    height: 22


                    onClicked:
                    {

                        messageDialog_button_FindIP_1.plata = 1;
                        messageDialog_button_FindIP_1.show();

                    }




                    iconSource: "images/Find.PNG"
                }
            }



            Button {
                id: mdelButton
                height: 23
                text: if (modelName.text == "Поиск")
                      {
                          qsTr("Найти")
                      }
                      else
                      {
                          qsTr("Подключить")
                      }
                anchors.left: parent.left
                z: 1
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.bottomMargin: 0
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0

                BusyIndicator {
                    id: busyIndicator
                    //spacing: 10

                    anchors.fill: parent
                    running: false

                    visible: false
                }

                onClicked: {
                    rectangle1.bluBox();
                    //  MyDevice.setGSG2(mdelButton2,modelIp2,gsg,modelDevice3,menuRectItemDashboard2)

                    if(mdelButton.text == "Подключить")
                    {
                        busyIndicator.visible = true;
                        busyIndicator.running = true;

                        n6700_1.slot_connectDevice(modelIp.text);
                        mdelButton.text = qsTr("");

                    }
                    else
                    {

                        n6700_1.slot_DisConnect();
                        mdelButton.text = qsTr("Подключить");
                        modelDevice.text = qsTr("");

                        menuRectItemDashboard.color = "#7e827f"
                    }


                }
            }



            Connections{
                target: n6700_1

                // @disable-check M16
                onSignal_Ok :
                {

                    console.log("dsafdasfdasf = " + good);
                    if(good)
                    {
                        busyIndicator.visible = false;
                        busyIndicator.running = false;

                        mdelButton.text = qsTr("Отключить");

                        modelDevice.text = n6700_1.name;

                        menuRectItemDashboard.color = "green"


                    }
                    else
                    {
                        busyIndicator.visible = false;
                        busyIndicator.running = false;

                        mdelButton.text = qsTr("Подключить");

                        modelDevice.text = n6700_1.name;

                        menuRectItemDashboard.color = "red"

                    }


                }
            }




            MouseArea {
                visible: true
                anchors.fill: parent

                onClicked: rectangle1.bluBox()


                onHeightChanged: {
                    if(skyblue2.visible == false && rectangle1.focus)
                    {
                        skyblue.height = rectangle1.height+20;
                        skyblue.x = rectangle1.x;
                        skyblue.y = rectangle1.y;
                    }
                }
                onWidthChanged:  {
                    if(skyblue2.visible == false && rectangle1.focus)
                    {
                        skyblue.width = rectangle1.width+20;
                        skyblue.x = rectangle1.x;
                        skyblue.y = rectangle1.y
                    }
                }
            }



            Text {
                id: modelDevice
                height: 50
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                horizontalAlignment: Text.AlignHCenter

                anchors.leftMargin: 8

            }


        }

        Rectangle {
            id: rectangle2
            Layout.fillHeight: true
            Layout.minimumHeight: 200
            Layout.minimumWidth: 200
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200



            function bluBox()
            {
                skyblue3.visible = false
                skyblue.visible = true

                skyblue.x = rectangle2.x
                skyblue.y = rectangle2.y

                skyblue.width = rectangle2.width+20
                skyblue.height = rectangle2.height+20

                skyblue2.visible = false
                rectangle1.focus = false
                rectangle2.focus = true
                rectangle3.focus = false
                rectangle4.focus = false
                rectangle5.focus = false
                rectangle6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboard1
                width: 600
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName1
                    color: "#ffffff"
                    text: "Источник питания №2"
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

            GridView
            {
                id: gridIP1
                z: 1
                anchors.horizontalCenterOffset: -40
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: label1
                    x: -26
                    y: 0
                    text: qsTr("IP:")
                    font.pointSize: 11
                    width: 20
                    height: 20
                }

                TextField
                {
                    id: modelIp1
                    width: 102
                    height: 20
                    text: ""
                    z: 1
                    placeholderText: "10.12.0.178 "

                    validator: RegExpValidator
                    {
                        regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }

                    onSelectionStartChanged:
                    {

                        if(rectangle2.focus == false)
                        {

                            skyblue3.visible = false;
                            skyblue.visible = true;
                            skyblue.x = rectangle2.x;
                            skyblue.y = rectangle2.y;
                            skyblue.width = rectangle2.width+20;
                            skyblue.height = rectangle2.height+20;
                            skyblue2.visible = false;


                            rectangle1.focus = false;
                            rectangle3.focus = false;
                            rectangle4.focus = false;
                            rectangle5.focus = false;
                            rectangle6.focus = false;
                        }

                    }
                }

                Button {
                    id: button_FindIP_2
                    x: 101.5
                    y: -1
                    width: 20
                    height: 22

                    onClicked:
                    {
                        messageDialog_button_FindIP_1.plata = 2;

                        messageDialog_button_FindIP_1.x = mouse.x;
                        messageDialog_button_FindIP_1.y = mouse.y;


                        messageDialog_button_FindIP_1.show();

                    }

                    iconSource: "images/Find.PNG"
                }
            }


            Button {
                id: mdelButton1
                height: 23
                text: if (modelName1.text == "Поиск")
                      {
                          qsTr("Найти")
                      }
                      else
                      {
                          qsTr("Подключить")
                      }
                anchors.left: parent.left
                z: 1
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.bottomMargin: 0
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0

                onClicked: {
                    rectangle2.bluBox();

                    if(mdelButton1.text == "Подключить")
                    {
                        busyIndicator1.visible = true;
                        busyIndicator1.running = true;

                        n6700_2.slot_connectDevice(modelIp1.text);
                        mdelButton1.text = qsTr("");
                    }
                    else
                    {

                        n6700_2.slot_DisConnect();
                        mdelButton1.text = qsTr("Подключить");
                        modelDevice1.text = qsTr("");

                        menuRectItemDashboard1.color = "#7e827f"
                    }


                }

                BusyIndicator {
                    id: busyIndicator1
                    //spacing: 10

                    anchors.fill: parent
                    running: false

                    visible: false
                }

                Connections{
                    target: n6700_2

                    // @disable-check M16
                    onSignal_Ok :
                    {

                        console.log("dsafdasfdasf = " + good);
                        if(good)
                        {
                            busyIndicator1.visible = false;
                            busyIndicator1.running = false;

                            mdelButton1.text = qsTr("Отключить");

                            modelDevice1.text = n6700_2.name;

                            menuRectItemDashboard1.color = "green"

                        }
                        else
                        {
                            busyIndicator1.visible = false;
                            busyIndicator1.running = false;

                            mdelButton1.text = qsTr("Подключить");

                            modelDevice1.text = n6700_2.name;

                            menuRectItemDashboard1.color = "red"
                        }


                    }
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                onClicked: rectangle2.bluBox()

                onHeightChanged: {
                    if(skyblue2.visible == false && rectangle2.focus)
                    {
                        skyblue.height = rectangle2.height+20;
                        skyblue.x = rectangle2.x;
                        skyblue.y = rectangle2.y;
                    }
                }
                onWidthChanged:  {
                    if(skyblue2.visible == false && rectangle2.focus)
                    {
                        skyblue.width = rectangle2.width+20;
                        skyblue.x = rectangle2.x;
                        skyblue.y = rectangle2.y;
                    }
                }

            }



            Text {
                id: modelDevice1
                height: 50
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                horizontalAlignment: Text.AlignHCenter

                anchors.leftMargin: 8
            }


            border.color: "#000000"
            border.width: 1
        }

        Rectangle {
            id: rectangle7
            Layout.fillHeight: true
            Layout.minimumHeight: 200
            Layout.minimumWidth: 200
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200



            function bluBox()
            {
                skyblue3.visible = false
                skyblue.visible = true

                skyblue.x = rectangle7.x
                skyblue.y = rectangle7.y

                skyblue.width = rectangle7.width+20
                skyblue.height = rectangle7.height+20

                skyblue2.visible = false
                rectangle1.focus = false
                rectangle2.focus = false
                rectangle3.focus = false
                rectangle4.focus = false
                rectangle5.focus = false
                rectangle6.focus = false
                rectangle7.focus = true
            }

            Rectangle {
                id: menuRectItemDashboard7
                width: 600
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName7
                    color: "#ffffff"
                    text: "Климатическая камера PG-2J"
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

            GridView
            {
                id: gridIP7
                z: 1
                anchors.horizontalCenterOffset: -40
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: label7
                    x: -26
                    y: 0
                    text: qsTr("IP:")
                    font.pointSize: 11
                    width: 20
                    height: 20
                }

                TextField
                {
                    id: modelIp7
                    width: 102
                    height: 20
                    text: ""
                    z: 1
                    placeholderText: "10.12.0.178"

                    validator: RegExpValidator
                    {
                        regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }

                    onSelectionStartChanged:
                    {

                        if(rectangle7.focus == false)
                        {

                            skyblue3.visible = false
                            skyblue.visible = true
                            skyblue.x = rectangle7.x
                            skyblue.y = rectangle7.y
                            skyblue.width = rectangle7.width+20
                            skyblue.height = rectangle7.height+20
                            skyblue3.visible = false


                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = false

                        }

                    }
                }

                Button {
                    id: button_FindIP_7
                    x: 101.5
                    y: -1
                    width: 20
                    height: 22

                    onClicked:
                    {
                        messageDialog_button_FindIP_1.plata = 6;
                        messageDialog_button_FindIP_1.show();

                    }

                    iconSource: "images/Find.PNG"
                }
            }


            Button {
                id: mdelButton7
                height: 23
                text: if (modelName1.text == "Поиск")
                      {
                          qsTr("Найти")
                      }
                      else
                      {
                          qsTr("Подключить")
                      }
                anchors.left: parent.left
                z: 1
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.bottomMargin: 0
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0

                onClicked: {
                    rectangle7.bluBox();

                    if(mdelButton7.text == "Подключить")
                    {
                        busyIndicator7.visible = true;
                        busyIndicator7.running = true;

                        camber.slot_connectCamber(modelIp7.text)

                        mdelButton7.text = qsTr("");
                    }
                    else
                    {

                        n6700_2.slot_DisConnect();
                        mdelButton7.text = qsTr("Подключить");
                        modelDevice7.text = qsTr("");

                        menuRectItemDashboard7.color = "#7e827f"
                    }


                }

                BusyIndicator {
                    id: busyIndicator7
                    //spacing: 10

                    anchors.fill: parent
                    running: false

                    visible: false
                }

                Connections{
                    target: camber

                    // @disable-check M16
                    onSignal_Ok :
                    {

                        console.log("dsafdasfdasf = " + good);
                        if(good)
                        {
                            busyIndicator7.visible = false;
                            busyIndicator7.running = false;

                            mdelButton7.text = qsTr("Отключить");

                            modelDevice7.text = camber.name;

                            menuRectItemDashboard7.color = "green"

                        }
                        else
                        {
                            busyIndicator7.visible = false;
                            busyIndicator7.running = false;

                            mdelButton7.text = qsTr("Подключить");

                            modelDevice7.text = camber.name;

                            menuRectItemDashboard7.color = "red"
                        }


                    }
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                onClicked: rectangle7.bluBox()

                onHeightChanged: {
                    if(skyblue2.visible == false && rectangle7.focus)
                    {
                        skyblue.height = rectangle7.height+20;
                        skyblue.x = rectangle7.x;
                        skyblue.y = rectangle7.y;
                    }
                }
                onWidthChanged:  {
                    if(skyblue2.visible == false && rectangle7.focus)
                    {
                        skyblue.width = rectangle7.width+20;
                        skyblue.x = rectangle7.x;
                        skyblue.y = rectangle7.y;
                    }
                }

            }


            Text {
                id: modelDevice7
                height: 50
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                horizontalAlignment: Text.AlignHCenter

                anchors.leftMargin: 8
            }


            border.color: "#000000"
            border.width: 1
        }

        Rectangle {
            id: rectangle3
            Layout.fillHeight: true
            Layout.minimumHeight: 200
            Layout.minimumWidth: 200
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200

            function bluBox()
            {
                skyblue3.visible = false
                skyblue.visible = true
                skyblue.x = rectangle3.x
                skyblue.y = rectangle3.y
                skyblue.width = rectangle3.width+20
                skyblue.height = rectangle3.height+20
                skyblue2.visible = false

                rectangle1.focus = false
                rectangle2.focus = false
                rectangle3.focus = true
                rectangle4.focus = false
                rectangle5.focus = false
                rectangle6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboard2
                width: 600
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName2
                    color: "#ffffff"
                    text: "Имитатор GPS/GLONASS"
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


            GridView
            {
                id: gridIP2
                z: 1
                anchors.horizontalCenterOffset: -40
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: label2
                    x: -26
                    y: 0
                    text: qsTr("IP:")
                    font.pointSize: 11
                    width: 20
                    height: 20
                }

                TextField
                {
                    id: modelIp2
                    width: 102
                    height: 20
                    text: "10.12.1.46"
                    z: 1
                    placeholderText: "10.12.1.46"

                    validator: RegExpValidator
                    {
                        regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }

                    onSelectionStartChanged:
                    {

                        if(rectangle3.focus == false)
                        {

                            skyblue3.visible = false;
                            skyblue.visible = true;
                            skyblue.x = rectangle3.x;
                            skyblue.y = rectangle3.y;
                            skyblue.width = rectangle3.width+20;
                            skyblue.height = rectangle3.height+20;
                            skyblue2.visible = false;


                            rectangle1.focus = false;
                            rectangle2.focus = false;
                            rectangle4.focus = false;
                            rectangle5.focus = false;
                            rectangle6.focus = false;
                        }

                    }
                }

                Button {
                    id: button_FindIP_3
                    x: 101.5
                    y: -1
                    width: 20
                    height: 22

                    onClicked:
                    {
                        messageDialog_button_FindIP_1.plata = 3;
                        messageDialog_button_FindIP_1.show();

                    }

                    iconSource: "images/Find.PNG"
                }
            }


            Button {
                id: mdelButton2
                height: 23
                text: if (modelName2.text == "Поиск")
                      {
                          qsTr("Найти")
                      }
                      else
                      {
                          qsTr("Подключить")
                      }
                anchors.left: parent.left
                z: 1
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.bottomMargin: 0
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0

                onClicked: {
                    rectangle3.bluBox();
                    //  MyDevice.setGSG2(mdelButton2,modelIp2,gsg,modelDevice3,menuRectItemDashboard2)

                    if(mdelButton2.text == "Подключить")
                    {
                        busyIndicator2.visible = true;
                        busyIndicator2.running = true;

                        gsg.slot_connectDevice(modelIp2.text);
                        mdelButton2.text = qsTr("");

                    }
                    else
                    {

                        gsg.slot_DisConnect();
                        mdelButton2.text = qsTr("Подключить");
                        modelDevice3.text = qsTr("");

                        menuRectItemDashboard2.color = "#7e827f";
                    }


                }


                BusyIndicator {
                    id: busyIndicator2
                    //spacing: 10

                    anchors.fill: parent
                    running: false

                    visible: false
                }

                Connections{
                    target: gsg

                    // @disable-check M16
                    onSignal_Ok :
                    {

                        console.log("dsafdasfdasf = " + good);
                        if(good)
                        {
                            busyIndicator2.visible = false;
                            busyIndicator2.running = false;

                            mdelButton2.text = qsTr("Отключить");

                            modelDevice3.text = gsg.name;

                            menuRectItemDashboard2.color = "green"



                        }
                        else
                        {
                            busyIndicator2.visible = false;
                            busyIndicator2.running = false;

                            mdelButton2.text = qsTr("Подключить");

                            modelDevice3.text = gsg.name;

                            menuRectItemDashboard2.color = "red"


                        }


                    }
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                onClicked: rectangle3.bluBox()


                onHeightChanged: {
                    if(skyblue2.visible == false && rectangle3.focus)
                    {
                        skyblue.height = rectangle3.height+20;
                        skyblue.x = rectangle3.x;
                        skyblue.y = rectangle3.y;
                    }
                }
                onWidthChanged:  {
                    if(skyblue2.visible == false && rectangle3.focus)
                    {
                        skyblue.width = rectangle3.width+20;
                        skyblue.x = rectangle3.x;
                        skyblue.y = rectangle3.y;
                    }
                }
            }



            Text {
                id: modelDevice3
                height: 50
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                horizontalAlignment: Text.AlignHCenter

                anchors.leftMargin: 8
            }
            border.color: "#000000"
            border.width: 1
        }

        Rectangle {
            id: rectangle4
            Layout.minimumHeight: 200
            Layout.minimumWidth: 200
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200

            function bluBox()
            {
                skyblue3.visible = false
                skyblue2.visible = true
                skyblue2.x = rectangle4.x
                skyblue2.y = rectangle4.y
                skyblue2.width = rectangle4.width+20
                skyblue2.height = rectangle4.height+20
                skyblue.visible = false

                rectangle1.focus = false
                rectangle2.focus = false
                rectangle3.focus = false
                rectangle4.focus = true
                rectangle5.focus = false
                rectangle6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboard3
                width: 600
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName3
                    color: "#ffffff"
                    text: "Осцилограф"
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

            GridView
            {
                id: gridIP3
                z: 1
                anchors.horizontalCenterOffset: -40
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: label3
                    x: -26
                    y: 0
                    text: qsTr("IP:")
                    font.pointSize: 11
                    width: 20
                    height: 20
                }

                TextField
                {
                    id: modelIp3
                    width: 102
                    height: 20
                    text: "10.12.0.139"
                    z: 1
                    placeholderText: "10.12.0.139"

                    validator: RegExpValidator
                    {
                        regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }

                    onSelectionStartChanged:
                    {

                        if(rectangle4.focus == false)
                        {

                            skyblue3.visible = false
                            skyblue2.visible = true
                            skyblue2.x = rectangle4.x
                            skyblue2.y = rectangle4.y
                            skyblue2.width = rectangle4.width+20
                            skyblue2.height = rectangle4.height+20
                            skyblue.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = false
                        }

                    }

                }

                Button {
                    id: button_FindIP_4
                    x: 101.5
                    y: -1
                    width: 20
                    height: 22

                    onClicked:
                    {
                        messageDialog_button_FindIP_1.plata = 4
                        messageDialog_button_FindIP_1.show()
                    }

                    iconSource: "images/Find.PNG"


                }
            }

            Button {
                id: mdelButton3
                height: 23
                text: if (modelName3.text == "Поиск")
                      {
                          qsTr("Найти")
                      }
                      else
                      {
                          qsTr("Подключить")
                      }
                anchors.left: parent.left
                z: 1
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.bottomMargin: 0
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0

                onClicked: {
                    rectangle4.bluBox();

                    if(mdelButton3.text == "Подключить")
                    {
                        busyIndicator3.visible = true;
                        busyIndicator3.running = true;

                        os.slot_connectDevice(modelIp3.text);
                        mdelButton3.text = qsTr("");

                    }
                    else
                    {

                        gsg.slot_DisConnect();
                        mdelButton3.text = qsTr("Подключить");
                        modelDevice4.text = qsTr("");

                        menuRectItemDashboard3.color = "#7e827f"
                    }


                }


                BusyIndicator {
                    id: busyIndicator3
                    //spacing: 10

                    anchors.fill: parent
                    running: false

                    visible: false
                }

                Connections{
                    target: os

                    // @disable-check M16
                    onSignal_Ok :
                    {

                        console.log("dsafdasfdasf = " + good);
                        if(good)
                        {
                            busyIndicator3.visible = false;
                            busyIndicator3.running = false;

                            mdelButton3.text = qsTr("Отключить");

                            modelDevice4.text = os.name;

                            menuRectItemDashboard3.color = "green"


                        }
                        else
                        {
                            busyIndicator3.visible = false;
                            busyIndicator3.running = false;

                            mdelButton3.text = qsTr("Подключить");

                            modelDevice4.text = os.name;

                            menuRectItemDashboard3.color = "red"

                        }


                    }
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                onClicked: rectangle4.bluBox()


                onHeightChanged: {
                    if(skyblue.visible == false && rectangle4.focus)
                    {
                        skyblue2.height = rectangle4.height+20;
                        skyblue2.x = rectangle4.x;
                        skyblue2.y = rectangle4.y
                    }
                }
                onWidthChanged:  {
                    if(skyblue.visible == false && rectangle4.focus)
                    {
                        skyblue2.width = rectangle4.width+20;
                        skyblue2.x = rectangle4.x;
                        skyblue2.y = rectangle4.y
                    }
                }

            }



            Text {
                id: modelDevice4
                height: 50
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                horizontalAlignment: Text.AlignHCenter

                anchors.leftMargin: 8
            }

            border.color: "#000000"
            border.width: 1
        }

        Rectangle {
            id: rectangle5
            Layout.minimumHeight: 200
            Layout.minimumWidth: 200
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200

            function bluBox()
            {
                skyblue3.visible = false
                skyblue2.visible = true
                skyblue2.x = rectangle5.x
                skyblue2.y = rectangle5.y
                skyblue2.width = rectangle5.width+20
                skyblue2.height = rectangle5.height+20
                skyblue.visible = false

                rectangle1.focus = false
                rectangle2.focus = false
                rectangle3.focus = false
                rectangle4.focus = false
                rectangle5.focus = true
                rectangle6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboard4
                width: 600
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName4
                    color: "#ffffff"
                    text: "Пульт"
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

            GridView
            {
                id: gridIP4
                z: 1
                anchors.horizontalCenterOffset: -40
                anchors.verticalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: label4
                    x: -26
                    y: 0
                    text: qsTr("IP:")
                    font.pointSize: 11
                    width: 20
                    height: 20
                }

                TextField
                {
                    id: modelIp4
                    width: 102
                    height: 20
                    text: ""
                    z: 1
                    placeholderText: "10.12.0.178"

                    validator: RegExpValidator
                    {
                        regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }

                    onSelectionStartChanged:
                    {

                        if(rectangle5.focus == false)
                        {

                            skyblue3.visible = false
                            skyblue2.visible = true
                            skyblue2.x = rectangle5.x
                            skyblue2.y = rectangle5.y
                            skyblue2.width = rectangle5.width+20
                            skyblue2.height = rectangle5.height+20
                            skyblue.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle6.focus = false
                        }

                    }

                }

                Button {
                    id: button_FindIP_5
                    x: 101.5
                    y: -1
                    width: 20
                    height: 22

                    onClicked:
                    {
                        messageDialog_button_FindIP_1.plata = 5;
                        messageDialog_button_FindIP_1.show();

                    }

                    iconSource: "images/Find.PNG"
                }
            }



            Button {
                id: mdelButton4
                height: 23
                text: if (modelName4.text == "Поиск")
                      {
                          qsTr("Найти")
                      }
                      else
                      {
                          qsTr("Подключить")
                      }
                anchors.left: parent.left
                z: 1
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.bottomMargin: 0
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0

                onClicked: {
                    rectangle5.bluBox();

                    if(mdelButton4.text == "Подключить")
                    {
                        busyIndicator4.visible = true;
                        busyIndicator4.running = true;

                        tp.slot_ConnectTp(modelIp4.text);
                        mdelButton4.text = qsTr("");

                        // screenObject.capture("Подключение Осцилографа.jpg")
                    }
                    else
                    {

                        gsg.slot_DisConnect();
                        mdelButton4.text = qsTr("Подключить");
                        modelDevice5.text = qsTr("");

                        menuRectItemDashboard4.color = "#7e827f"
                    }

                    //  MyDevice.setN6700_12(mdelButton4,modelIp4,n6700_1,modelDevice5,menuRectItemDashboard4)


                }

                BusyIndicator {
                    id: busyIndicator4
                    //spacing: 10

                    anchors.fill: parent
                    running: false

                    visible: false
                }

                Connections{
                    target: tp

                    // @disable-check M16
                    onSignal_Ok :
                    {

                        if(ok)
                        {
                            busyIndicator4.visible = false;
                            busyIndicator4.running = false;
                            mdelButton4.text = qsTr("Отключить");
                            modelDevice5.text = "Пульт ТП8";
                            menuRectItemDashboard4.color = "green"
                        }
                        else
                        {
                            busyIndicator4.visible = false;
                            busyIndicator4.running = false;
                            mdelButton4.text = qsTr("Подключить");
                            modelDevice5.text = "Пульт ТП8";
                            menuRectItemDashboard4.color = "red"

                        }


                    }
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                onClicked: rectangle5.bluBox()


                onHeightChanged: {
                    if(skyblue.visible == false && rectangle5.focus)
                    {
                        skyblue2.height = rectangle5.height+20;
                        skyblue2.x = rectangle5.x;
                        skyblue2.y = rectangle5.y;
                    }
                }
                onWidthChanged:  {
                    if(skyblue.visible == false && rectangle5.focus)
                    {
                        skyblue2.width = rectangle5.width+20;
                        skyblue2.x = rectangle5.x;
                        skyblue2.y = rectangle5.y;
                    }
                }
            }


            Text {
                id: modelDevice5
                height: 50
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                horizontalAlignment: Text.AlignHCenter

                anchors.leftMargin: 8
            }
            border.color: "#000000"
            border.width: 1
        }

        Rectangle {
            id: rectangle6
            Layout.columnSpan: 3
            Layout.minimumHeight: 200
            Layout.minimumWidth: 200
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 200
            Layout.preferredWidth: 200

            function bluBox()
            {
                skyblue2.visible = true
                skyblue2.x = rectangle6.x
                skyblue2.y = rectangle6.y
                skyblue2.width = rectangle6.width+20
                skyblue2.height = rectangle6.height+20
                skyblue.visible = false

                rectangle1.focus = false
                rectangle2.focus = false
                rectangle3.focus = false
                rectangle4.focus = false
                rectangle5.focus = false
                rectangle6.focus = true
            }

            Rectangle {
                id: menuRectItemDashboard5
                width: 600
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName5
                    color: "#ffffff"
                    text: "Настройка COM порта (RS232)"
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

            Rectangle {
                id: skyblue3
                color: "skyblue"
                z: 0

            }


            GridView
            {
                id: gridIP5
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.bottomMargin: 20
                anchors.topMargin: 15
                anchors.fill: parent
                z: 1

                MouseArea {
                    z: 0
                    visible: true
                    anchors.fill: parent

                    onClicked: rectangle6.bluBox()


                    onHeightChanged: {
                        if(skyblue.visible == false && rectangle6.focus)
                        {

                            skyblue2.height = rectangle6.height+20;
                            skyblue2.x = rectangle6.x;
                            skyblue2.y = rectangle6.y;
                        }
                    }
                    onWidthChanged:  {
                        if(skyblue.visible == false && rectangle6.focus)
                        {
                            skyblue2.width = rectangle6.width+20;
                            skyblue2.x = rectangle6.x;
                            skyblue2.y = rectangle6.y;
                        }
                    }
                }




                GridLayout {
                    id: mrkList
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    anchors.bottomMargin: 10
                    anchors.topMargin: 10
                    columnSpacing: 10
                    rowSpacing: 10
                    anchors.fill: parent
                    rows: 2
                    columns: 8


                    Rectangle {
                        id: rectangleNP1
                        radius: 0
                        transformOrigin: Item.Center
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.minimumHeight: 100
                        Layout.minimumWidth: 100
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        scale: 1
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 100
                        border.color: "#000000"
                        border.width: 1

                        function bluBox() {
                            skyblue3.visible = true
                            skyblue3.x = rectangleNP1.x+10
                            skyblue3.y = rectangleNP1.y+20
                            skyblue3.width = rectangleNP1.width+10
                            skyblue3.height = rectangleNP1.height+10



                            rectangleNP1.focus = true
                            rectangleNP2.focus = false
                            rectangleNP3.focus = false
                            rectangleNP4.focus = false
                            rectangleNP5.focus = false
                            rectangleNP6.focus = false


                            skyblue.visible = true
                            skyblue.x = rectangle6.x
                            skyblue.y = rectangle6.y
                            skyblue.width = rectangle6.width + 20
                            skyblue.height = rectangle6.height + 20
                            skyblue2.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = true
                        }

                        Rectangle {
                            id: menuRectItemDashboardNP
                            width: 600
                            height: dp(24)
                            color: "#7e827f"
                            anchors.left: parent.left
                            Text {
                                id: modelNameNP
                                color: "#ffffff"
                                text: "НП №1"
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

                        GridView {
                            id: gridIPNP
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            z: 1
                            anchors.verticalCenterOffset: -10
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                id: labelNP
                                y: 0
                                width: 35
                                text: qsTr("COM:")
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                font.pointSize: 11
                                height: 20
                            }

                            ComboBox {
                                id: modelIpNP
                                y: 0
                                height: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 4
                                anchors.left: labelNP.right
                                anchors.leftMargin: 4
                                z: 1
                                model: port1.listCOM
                                currentIndex: 0

                                onPressedChanged:
                                {
                                    rectangleNP1.bluBox()
                                }
                            }
                        }

                        Button {
                            id: mdelButtonNP
                            height: 23
                            text: if (modelName.text == "Поиск")
                                  {
                                      qsTr("Найти")
                                  }
                                  else
                                  {
                                      qsTr("Подключить")
                                  }
                            anchors.left: parent.left
                            z: 1
                            anchors.rightMargin: 0
                            anchors.right: parent.right
                            anchors.bottomMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0




                            BusyIndicator {
                                id: busyIndicator_ComboBox_1
                                //spacing: 10

                                anchors.fill: parent
                                running: false

                                visible: false
                            }


                            onClicked: {
                                rectangleNP1.bluBox();

                                if(mdelButtonNP.text === qsTr("Подключить"))
                                {
                                    port1.openPort(modelIpNP.currentText);

                                    busyIndicator_ComboBox_1.visible = true;
                                    busyIndicator_ComboBox_1.running = true;

                                    mdelButtonNP.text = qsTr("");


                                }
                                else
                                {
                                    mdelButtonNP.text = qsTr("Подключить");
                                    menuRectItemDashboardNP.color = "#7e827f";
                                    port1.closePort();
                                    port1.slot_endWork();
                                }


                            }



                            Connections{
                                target: port1

                                // @disable-check M16
                                onSignal_Ok :
                                {

                                    console.log("dsafdasfdasf = " + good);
                                    if(good)
                                    {
                                        busyIndicator_ComboBox_1.visible = false;
                                        busyIndicator_ComboBox_1.running = false;

                                        mdelButtonNP.text = qsTr("Отключить");
                                        mdelButton5.text = mdelButtonNP.text;
                                        menuRectItemDashboardNP.color = "green"


                                    }
                                    else
                                    {
                                        busyIndicator_ComboBox_1.visible = false;
                                        busyIndicator_ComboBox_1.running = false;

                                        mdelButtonNP.text = qsTr("Подключить");
                                        menuRectItemDashboardNP.color = "red"
                                    }


                                }
                            }
                        }

                        MouseArea {
                            visible: true
                            anchors.fill: parent

                            onClicked: rectangleNP1.bluBox()

                            onHeightChanged: if (rectangleNP1.focus) rectangleNP1.bluBox()

                            onWidthChanged: if (rectangleNP1.focus) rectangleNP1.bluBox()
                        }

                        Text {
                            id: modelDeviceNP
                            height: 50
                            anchors.top: parent.top
                            anchors.topMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            horizontalAlignment: Text.AlignHCenter

                            anchors.leftMargin: 8
                        }
                    }

                    Rectangle {
                        id: rectangleNP2
                        radius: 0
                        transformOrigin: Item.Center
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.minimumHeight: 100
                        Layout.minimumWidth: 100
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        scale: 1
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 100
                        border.color: "#000000"
                        border.width: 1

                        function bluBox() {
                            skyblue3.visible = true
                            skyblue3.x = rectangleNP2.x+10
                            skyblue3.y = rectangleNP2.y+20
                            skyblue3.width = rectangleNP2.width+10
                            skyblue3.height = rectangleNP2.height+10



                            rectangleNP1.focus = false
                            rectangleNP2.focus = true
                            rectangleNP3.focus = false
                            rectangleNP4.focus = false
                            rectangleNP5.focus = false
                            rectangleNP6.focus = false


                            skyblue.visible = true
                            skyblue.x = rectangle6.x
                            skyblue.y = rectangle6.y
                            skyblue.width = rectangle6.width + 20
                            skyblue.height = rectangle6.height + 20
                            skyblue2.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = true
                        }


                        Rectangle {
                            id: menuRectItemDashboardNP2
                            width: 600
                            height: dp(24)
                            color: "#7e827f"
                            anchors.left: parent.left
                            Text {
                                id: modelNameNP2
                                color: "#ffffff"
                                text: "НП №2"
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

                        GridView {
                            id: gridIPNP2
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            z: 1
                            anchors.verticalCenterOffset: -10
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                id: labelNP2
                                y: 0
                                width: 35
                                text: qsTr("COM:")
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                font.pointSize: 11
                                height: 20
                            }

                            ComboBox {
                                id: modelIpNP2
                                y: 0
                                height: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 4
                                anchors.left: labelNP2.right
                                anchors.leftMargin: 4
                                z: 1

                                model: port2.listCOM
                                currentIndex: 1

                                onPressedChanged:
                                {
                                    rectangleNP2.bluBox();
                                }
                            }
                        }

                        Button {
                            id: mdelButtonNP2
                            height: 23
                            text: if (modelName.text == "Поиск")
                                  {
                                      qsTr("Найти")
                                  }
                                  else
                                  {
                                      qsTr("Подключить")
                                  }
                            anchors.left: parent.left
                            z: 1
                            anchors.rightMargin: 0
                            anchors.right: parent.right
                            anchors.bottomMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0


                            BusyIndicator {
                                id: busyIndicator_ComboBox_2
                                //spacing: 10

                                anchors.fill: parent
                                running: false

                                visible: false
                            }


                            onClicked: {
                                rectangleNP2.bluBox();

                                if(mdelButtonNP2.text === qsTr("Подключить"))
                                {
                                    port2.openPort(modelIpNP2.currentText);

                                    busyIndicator_ComboBox_2.visible = true;
                                    busyIndicator_ComboBox_2.running = true;

                                    mdelButtonNP2.text = qsTr("");

                                }
                                else
                                {
                                    mdelButtonNP2.text = qsTr("Подключить");
                                    menuRectItemDashboardNP2.color = "#7e827f";
                                    port2.closePort();
                                    port2.slot_endWork();
                                }


                            }



                            Connections{
                                target: port2

                                // @disable-check M16
                                onSignal_Ok :
                                {

                                    console.log("dsafdasfdasf = " + good);
                                    if(good)
                                    {
                                        busyIndicator_ComboBox_2.visible = false;
                                        busyIndicator_ComboBox_2.running = false;

                                        mdelButtonNP2.text = qsTr("Отключить");
                                        mdelButton5.text = mdelButtonNP2.text;
                                        menuRectItemDashboardNP2.color = "green"

                                    }
                                    else
                                    {
                                        busyIndicator_ComboBox_2.visible = false;
                                        busyIndicator_ComboBox_2.running = false;

                                        mdelButtonNP2.text = qsTr("Подключить");
                                        menuRectItemDashboardNP2.color = "red"
                                    }


                                }
                            }
                        }

                        MouseArea {
                            visible: true
                            anchors.fill: parent

                            onClicked: rectangleNP2.bluBox()

                            onHeightChanged: if (rectangleNP2.focus) rectangleNP2.bluBox()
                            onWidthChanged: if (rectangleNP2.focus) rectangleNP2.bluBox()
                        }

                        Text {
                            id: modelDeviceNP2
                            height: 50
                            anchors.top: parent.top
                            anchors.topMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            horizontalAlignment: Text.AlignHCenter

                            anchors.leftMargin: 8
                        }
                    }

                    Rectangle {
                        id: rectangleNP3
                        radius: 0
                        transformOrigin: Item.Center
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.minimumHeight: 100
                        Layout.minimumWidth: 100
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        scale: 1
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 100
                        border.color: "#000000"
                        border.width: 1

                        function bluBox() {
                            skyblue3.visible = true
                            skyblue3.x = rectangleNP3.x+10
                            skyblue3.y = rectangleNP3.y+20
                            skyblue3.width = rectangleNP3.width+10
                            skyblue3.height = rectangleNP3.height+10



                            rectangleNP1.focus = false
                            rectangleNP2.focus = false
                            rectangleNP3.focus = true
                            rectangleNP4.focus = false
                            rectangleNP5.focus = false
                            rectangleNP6.focus = false


                            skyblue.visible = true
                            skyblue.x = rectangle6.x
                            skyblue.y = rectangle6.y
                            skyblue.width = rectangle6.width + 20
                            skyblue.height = rectangle6.height + 20
                            skyblue2.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = true
                        }

                        Rectangle {
                            id: menuRectItemDashboardNP3
                            width: 600
                            height: dp(24)
                            color: "#7e827f"
                            anchors.left: parent.left
                            Text {
                                id: modelNameNP3
                                color: "#ffffff"
                                text: "НП №3"
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

                        GridView {
                            id: gridIPNP3
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            z: 1
                            anchors.verticalCenterOffset: -10
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                id: labelNP3
                                y: 0
                                width: 35
                                text: qsTr("COM:")
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                font.pointSize: 11
                                height: 20
                            }

                            ComboBox {
                                id: modelIpNP3
                                y: 0
                                height: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 4
                                anchors.left: labelNP3.right
                                anchors.leftMargin: 4
                                z: 1
                                model: port1.listCOM
                                currentIndex: 2

                                onPressedChanged:
                                {
                                    rectangleNP3.bluBox();

                                }
                            }
                        }

                        Button {
                            id: mdelButtonNP3
                            height: 23
                            text: if (modelName.text == "Поиск")
                                  {
                                      qsTr("Найти")
                                  }
                                  else
                                  {
                                      qsTr("Подключить")
                                  }
                            anchors.left: parent.left
                            z: 1
                            anchors.rightMargin: 0
                            anchors.right: parent.right
                            anchors.bottomMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0

                            BusyIndicator {
                                id: busyIndicator_ComboBox_3
                                //spacing: 10

                                anchors.fill: parent
                                running: false

                                visible: false
                            }


                            onClicked: {
                                rectangleNP3.bluBox();

                                if(mdelButtonNP3.text === qsTr("Подключить"))
                                {
                                    port3.openPort(modelIpNP3.currentText);

                                    busyIndicator_ComboBox_3.visible = true;
                                    busyIndicator_ComboBox_3.running = true;

                                    mdelButtonNP3.text = qsTr("");

                                }
                                else
                                {
                                    mdelButtonNP3.text = qsTr("Подключить");
                                    menuRectItemDashboardNP3.color = "#7e827f";
                                    port3.closePort();
                                    port3.slot_endWork();
                                }


                            }



                            Connections{
                                target: port3

                                // @disable-check M16
                                onSignal_Ok :
                                {

                                    console.log("dsafdasfdasf = " + good);
                                    if(good)
                                    {
                                        busyIndicator_ComboBox_3.visible = false;
                                        busyIndicator_ComboBox_3.running = false;

                                        mdelButtonNP3.text = qsTr("Отключить");
                                        mdelButton5.text = mdelButtonNP3.text;
                                        menuRectItemDashboardNP3.color = "green"

                                    }
                                    else
                                    {
                                        busyIndicator_ComboBox_3.visible = false;
                                        busyIndicator_ComboBox_3.running = false;

                                        mdelButtonNP3.text = qsTr("Подключить");
                                        menuRectItemDashboardNP3.color = "red"
                                    }


                                }
                            }
                        }

                        MouseArea {
                            visible: true
                            anchors.fill: parent

                            onClicked: rectangleNP3.bluBox()

                            onHeightChanged: if (rectangleNP3.focus) rectangleNP3.bluBox()
                            onWidthChanged: if (rectangleNP3.focus) rectangleNP3.bluBox()
                        }

                        Text {
                            id: modelDeviceNP3
                            height: 50
                            anchors.top: parent.top
                            anchors.topMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            horizontalAlignment: Text.AlignHCenter

                            anchors.leftMargin: 8
                        }
                    }

                    Rectangle {
                        id: rectangleNP4
                        radius: 0
                        transformOrigin: Item.Center
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.minimumHeight: 100
                        Layout.minimumWidth: 100
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        scale: 1
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 100
                        border.color: "#000000"
                        border.width: 1

                        function bluBox() {
                            skyblue3.visible = true
                            skyblue3.x = rectangleNP4.x+10
                            skyblue3.y = rectangleNP4.y+20
                            skyblue3.width = rectangleNP4.width+10
                            skyblue3.height = rectangleNP4.height+10



                            rectangleNP1.focus = false
                            rectangleNP2.focus = false
                            rectangleNP3.focus = false
                            rectangleNP4.focus = true
                            rectangleNP5.focus = false
                            rectangleNP6.focus = false


                            skyblue.visible = true
                            skyblue.x = rectangle6.x
                            skyblue.y = rectangle6.y
                            skyblue.width = rectangle6.width + 20
                            skyblue.height = rectangle6.height + 20
                            skyblue2.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = true
                        }

                        Rectangle {
                            id: menuRectItemDashboardNP4
                            width: 600
                            height: dp(24)
                            color: "#7e827f"
                            anchors.left: parent.left
                            Text {
                                id: modelNameNP4
                                color: "#ffffff"
                                text: "НП №4"
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

                        GridView {
                            id: gridIPNP4
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            z: 1
                            anchors.verticalCenterOffset: -10
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                id: labelNP4
                                y: 0
                                width: 35
                                text: qsTr("COM:")
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                font.pointSize: 11
                                height: 20
                            }

                            ComboBox {
                                id: modelIpNP4
                                y: 0
                                height: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 4
                                anchors.left: labelNP4.right
                                anchors.leftMargin: 4
                                z: 1
                                model: port1.listCOM
                                currentIndex: 3

                                onPressedChanged:
                                {
                                    rectangleNP4.bluBox();

                                }
                            }
                        }

                        Button {
                            id: mdelButtonNP4
                            height: 23
                            text: if (modelName.text == "Поиск")
                                  {
                                      qsTr("Найти")
                                  }
                                  else
                                  {
                                      qsTr("Подключить")
                                  }
                            anchors.left: parent.left
                            z: 1
                            anchors.rightMargin: 0
                            anchors.right: parent.right
                            anchors.bottomMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0

                            BusyIndicator {
                                id: busyIndicator_ComboBox_4
                                //spacing: 10

                                anchors.fill: parent
                                running: false

                                visible: false
                            }


                            onClicked: {
                                rectangleNP4.bluBox();

                                if(mdelButtonNP4.text === qsTr("Подключить"))
                                {
                                    port4.openPort(modelIpNP4.currentText);

                                    busyIndicator_ComboBox_4.visible = true;
                                    busyIndicator_ComboBox_4.running = true;

                                    mdelButtonNP4.text = qsTr("");

                                }
                                else
                                {
                                    mdelButtonNP4.text = qsTr("Подключить");
                                    menuRectItemDashboardNP4.color = "#7e827f";
                                    port4.closePort();
                                    port4.slot_endWork();
                                }


                            }



                            Connections{
                                target: port4

                                // @disable-check M16
                                onSignal_Ok :
                                {

                                    console.log("dsafdasfdasf = " + good);
                                    if(good)
                                    {
                                        busyIndicator_ComboBox_4.visible = false;
                                        busyIndicator_ComboBox_4.running = false;

                                        mdelButtonNP4.text = qsTr("Отключить");
                                        mdelButton5.text = mdelButtonNP4.text;
                                        menuRectItemDashboardNP4.color = "green"

                                    }
                                    else
                                    {
                                        busyIndicator_ComboBox_4.visible = false;
                                        busyIndicator_ComboBox_4.running = false;

                                        mdelButtonNP4.text = qsTr("Подключить");
                                        menuRectItemDashboardNP4.color = "red"
                                    }


                                }
                            }
                        }

                        MouseArea {
                            visible: true
                            anchors.fill: parent

                            onClicked: rectangleNP4.bluBox()

                            onHeightChanged: if (rectangleNP4.focus) rectangleNP4.bluBox()
                            onWidthChanged: if (rectangleNP4.focus) rectangleNP4.bluBox()
                        }

                        Text {
                            id: modelDeviceNP4
                            height: 50
                            anchors.top: parent.top
                            anchors.topMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            horizontalAlignment: Text.AlignHCenter

                            anchors.leftMargin: 8
                        }
                    }

                    Rectangle {
                        id: rectangleNP5
                        radius: 0
                        transformOrigin: Item.Center
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.minimumHeight: 100
                        Layout.minimumWidth: 100
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        scale: 1
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 100
                        border.color: "#000000"
                        border.width: 1

                        function bluBox() {
                            skyblue3.visible = true
                            skyblue3.x = rectangleNP5.x+10
                            skyblue3.y = rectangleNP5.y+20
                            skyblue3.width = rectangleNP5.width+10
                            skyblue3.height = rectangleNP5.height+10



                            rectangleNP1.focus = false
                            rectangleNP2.focus = false
                            rectangleNP3.focus = false
                            rectangleNP4.focus = false
                            rectangleNP5.focus = true
                            rectangleNP6.focus = false


                            skyblue.visible = true
                            skyblue.x = rectangle6.x
                            skyblue.y = rectangle6.y
                            skyblue.width = rectangle6.width + 20
                            skyblue.height = rectangle6.height + 20
                            skyblue2.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = true
                        }

                        Rectangle {
                            id: menuRectItemDashboardNP5
                            width: 600
                            height: dp(24)
                            color: "#7e827f"
                            anchors.left: parent.left
                            Text {
                                id: modelNameNP5
                                color: "#ffffff"
                                text: "НП №5"
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

                        GridView {
                            id: gridIPNP5
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            z: 1
                            anchors.verticalCenterOffset: -10
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                id: labelNP5
                                y: 0
                                width: 35
                                text: qsTr("COM:")
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                font.pointSize: 11
                                height: 20
                            }

                            ComboBox {
                                id: modelIpNP5
                                y: 0
                                height: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 4
                                anchors.left: labelNP5.right
                                anchors.leftMargin: 4
                                z: 1
                                model: port1.listCOM
                                currentIndex: 4

                                onPressedChanged:
                                {
                                    rectangleNP5.bluBox();
                                }
                            }
                        }

                        Button {
                            id: mdelButtonNP5
                            height: 23
                            text: if (modelName.text == "Поиск")
                                  {
                                      qsTr("Найти")
                                  }
                                  else
                                  {
                                      qsTr("Подключить")
                                  }
                            anchors.left: parent.left
                            z: 1
                            anchors.rightMargin: 0
                            anchors.right: parent.right
                            anchors.bottomMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0

                            BusyIndicator {
                                id: busyIndicator_ComboBox_5
                                //spacing: 10

                                anchors.fill: parent
                                running: false

                                visible: false
                            }



                            onClicked: {
                                rectangleNP5.bluBox();

                                if(mdelButtonNP5.text === qsTr("Подключить"))
                                {
                                    port5.openPort(modelIpNP5.currentText);

                                    busyIndicator_ComboBox_5.visible = true;
                                    busyIndicator_ComboBox_5.running = true;

                                    mdelButtonNP5.text = qsTr("");

                                }
                                else
                                {
                                    mdelButtonNP5.text = qsTr("Подключить");
                                    menuRectItemDashboardNP5.color = "#7e827f";
                                    port5.closePort();
                                    port5.slot_endWork();
                                }


                            }



                            Connections{
                                target: port5

                                // @disable-check M16
                                onSignal_Ok :
                                {

                                    console.log("dsafdasfdasf = " + good);
                                    if(good)
                                    {
                                        busyIndicator_ComboBox_5.visible = false;
                                        busyIndicator_ComboBox_5.running = false;

                                        mdelButtonNP5.text = qsTr("Отключить");
                                        mdelButton5.text = mdelButtonNP5.text;
                                        menuRectItemDashboardNP5.color = "green"

                                    }
                                    else
                                    {
                                        busyIndicator_ComboBox_5.visible = false;
                                        busyIndicator_ComboBox_5.running = false;

                                        mdelButtonNP5.text = qsTr("Подключить");
                                        menuRectItemDashboardNP5.color = "red"
                                    }


                                }
                            }
                        }

                        MouseArea {
                            visible: true
                            anchors.fill: parent

                            onClicked: rectangleNP5.bluBox()

                            onHeightChanged: if (rectangleNP5.focus) rectangleNP5.bluBox()
                            onWidthChanged: if (rectangleNP5.focus) rectangleNP5.bluBox()
                        }

                        Text {
                            id: modelDeviceNP5
                            height: 50
                            anchors.top: parent.top
                            anchors.topMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            horizontalAlignment: Text.AlignHCenter

                            anchors.leftMargin: 8
                        }
                    }

                    Rectangle {
                        id: rectangleNP6
                        radius: 0
                        transformOrigin: Item.Center
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.minimumHeight: 100
                        Layout.minimumWidth: 100
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        scale: 1
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 100
                        border.color: "#000000"
                        border.width: 1

                        function bluBox() {
                            skyblue3.visible = true
                            skyblue3.x = rectangleNP6.x+10
                            skyblue3.y = rectangleNP6.y+20
                            skyblue3.width = rectangleNP6.width+10
                            skyblue3.height = rectangleNP6.height+10



                            rectangleNP1.focus = false
                            rectangleNP2.focus = false
                            rectangleNP3.focus = false
                            rectangleNP4.focus = false
                            rectangleNP5.focus = false
                            rectangleNP6.focus = true


                            skyblue.visible = true
                            skyblue.x = rectangle6.x
                            skyblue.y = rectangle6.y
                            skyblue.width = rectangle6.width + 20
                            skyblue.height = rectangle6.height + 20
                            skyblue2.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = true
                        }

                        Rectangle {
                            id: menuRectItemDashboardNP6
                            width: 600
                            height: dp(24)
                            color: "#7e827f"
                            anchors.left: parent.left
                            Text {
                                id: modelNameNP6
                                color: "#ffffff"
                                text: "НП №6"
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

                        GridView {
                            id: gridIPNP6
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            z: 1
                            anchors.verticalCenterOffset: -10
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                id: labelNP6
                                y: 0
                                width: 35
                                text: qsTr("COM:")
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                font.pointSize: 11
                                height: 20
                            }

                            ComboBox {
                                id: modelIpNP6
                                y: 0
                                height: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 4
                                anchors.left: labelNP6.right
                                anchors.leftMargin: 4
                                z: 1
                                model: port1.listCOM
                                currentIndex: 5

                                onPressedChanged:
                                {
                                    rectangleNP6.bluBox()
                                }
                            }
                        }

                        Button {
                            id: mdelButtonNP6
                            height: 23
                            text: if (modelName.text == "Поиск")
                                  {
                                      qsTr("Найти")
                                  }
                                  else
                                  {
                                      qsTr("Подключить")
                                  }
                            anchors.left: parent.left
                            z: 1
                            anchors.rightMargin: 0
                            anchors.right: parent.right
                            anchors.bottomMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0

                            BusyIndicator {
                                id: busyIndicator_ComboBox_6
                                //spacing: 10

                                anchors.fill: parent
                                running: false

                                visible: false
                            }



                            onClicked: {
                                rectangleNP6.bluBox();

                                if(mdelButtonNP6.text === qsTr("Подключить"))
                                {
                                    port6.openPort(modelIpNP6.currentText);

                                    busyIndicator_ComboBox_6.visible = true;
                                    busyIndicator_ComboBox_6.running = true;

                                    mdelButtonNP6.text = qsTr("");

                                }
                                else
                                {
                                    mdelButtonNP6.text = qsTr("Подключить");
                                    menuRectItemDashboardNP6.color = "#7e827f";
                                    port6.closePort();
                                    port6.slot_endWork();
                                }


                            }



                            Connections{
                                target: port6

                                // @disable-check M16
                                onSignal_Ok :
                                {

                                    console.log("dsafdasfdasf = " + good);
                                    if(good)
                                    {
                                        busyIndicator_ComboBox_6.visible = false;
                                        busyIndicator_ComboBox_6.running = false;

                                        mdelButtonNP6.text = qsTr("Отключить");
                                        mdelButton5.text = mdelButtonNP6.text;
                                        menuRectItemDashboardNP6.color = "green"

                                    }
                                    else
                                    {
                                        busyIndicator_ComboBox_6.visible = false;
                                        busyIndicator_ComboBox_6.running = false;

                                        mdelButtonNP6.text = qsTr("Подключить");
                                        menuRectItemDashboardNP6.color = "red"
                                    }


                                }
                            }
                        }

                        MouseArea {
                            visible: true
                            anchors.fill: parent

                            onClicked: rectangleNP6.bluBox()

                            onHeightChanged: if (rectangleNP6.focus) rectangleNP6.bluBox()
                            onWidthChanged: if (rectangleNP6.focus) rectangleNP6.bluBox()
                        }

                        Text {
                            id: modelDeviceNP6
                            height: 50
                            anchors.top: parent.top
                            anchors.topMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            horizontalAlignment: Text.AlignHCenter

                            anchors.leftMargin: 8
                        }
                    }

                    Rectangle {
                        id: rectangleNP7
                        radius: 0
                        transformOrigin: Item.Center
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.minimumHeight: 100
                        Layout.minimumWidth: 100
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        scale: 1
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 100
                        border.color: "#000000"
                        border.width: 1

                        function bluBox() {
                            skyblue3.visible = true
                            skyblue3.x = rectangleNP7.x+10
                            skyblue3.y = rectangleNP7.y+20
                            skyblue3.width = rectangleNP7.width+10
                            skyblue3.height = rectangleNP7.height+10



                            rectangleNP1.focus = false
                            rectangleNP2.focus = false
                            rectangleNP3.focus = false
                            rectangleNP4.focus = false
                            rectangleNP5.focus = false
                            rectangleNP6.focus = false
                            rectangleNP7.focus = true


                            skyblue.visible = true
                            skyblue.x = rectangle6.x
                            skyblue.y = rectangle6.y
                            skyblue.width = rectangle6.width + 20
                            skyblue.height = rectangle6.height + 20
                            skyblue2.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = true
                        }

                        Rectangle {
                            id: menuRectItemDashboardNP7
                            width: 600
                            height: dp(24)
                            color: "#7e827f"
                            anchors.left: parent.left
                            Text {
                                id: modelNameNP7
                                color: "#ffffff"
                                text: "НП №7"
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

                        GridView {
                            id: gridIPNP7
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            z: 1
                            anchors.verticalCenterOffset: -10
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                id: labelNP7
                                y: 0
                                width: 35
                                text: qsTr("COM:")
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                font.pointSize: 11
                                height: 20
                            }

                            ComboBox {
                                id: modelIpNP7
                                y: 0
                                height: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 4
                                anchors.left: labelNP7.right
                                anchors.leftMargin: 4
                                z: 1
                                model: port1.listCOM
                                currentIndex: 6

                                onPressedChanged:
                                {
                                    rectangleNP7.bluBox();
                                }
                            }
                        }

                        Button {
                            id: mdelButtonNP7
                            height: 23
                            text: if (modelName.text == "Поиск")
                                  {
                                      qsTr("Найти")
                                  }
                                  else
                                  {
                                      qsTr("Подключить")
                                  }
                            anchors.left: parent.left
                            z: 1
                            anchors.rightMargin: 0
                            anchors.right: parent.right
                            anchors.bottomMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0

                            BusyIndicator {
                                id: busyIndicator_ComboBox_7
                                //spacing: 10

                                anchors.fill: parent
                                running: false

                                visible: false
                            }


                            onClicked: {
                                rectangleNP7.bluBox();

                                if(mdelButtonNP7.text === qsTr("Подключить"))
                                {
                                    port7.openPort(modelIpNP7.currentText);

                                    busyIndicator_ComboBox_7.visible = true;
                                    busyIndicator_ComboBox_7.running = true;

                                    mdelButtonNP7.text = qsTr("");

                                }
                                else
                                {
                                    mdelButtonNP7.text = qsTr("Подключить");
                                    menuRectItemDashboardNP7.color = "#7e827f";
                                    port7.closePort();
                                    port7.slot_endWork();
                                }


                            }



                            Connections{
                                target: port7

                                // @disable-check M16
                                onSignal_Ok :
                                {

                                    console.log("dsafdasfdasf = " + good);
                                    if(good)
                                    {
                                        busyIndicator_ComboBox_7.visible = false;
                                        busyIndicator_ComboBox_7.running = false;

                                        mdelButtonNP7.text = qsTr("Отключить");
                                        mdelButton5.text = mdelButtonNP7.text;
                                        menuRectItemDashboardNP7.color = "green"

                                    }
                                    else
                                    {
                                        busyIndicator_ComboBox_7.visible = false;
                                        busyIndicator_ComboBox_7.running = false;

                                        mdelButtonNP7.text = qsTr("Подключить");
                                        menuRectItemDashboardNP7.color = "red"
                                    }


                                }
                            }
                        }

                        MouseArea {
                            visible: true
                            anchors.fill: parent

                            onClicked: rectangleNP7.bluBox()

                            onHeightChanged: if (rectangleNP7.focus) rectangleNP7.bluBox()
                            onWidthChanged: if (rectangleNP7.focus) rectangleNP7.bluBox()
                        }

                        Text {
                            id: modelDeviceNP7
                            height: 50
                            anchors.top: parent.top
                            anchors.topMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            horizontalAlignment: Text.AlignHCenter

                            anchors.leftMargin: 8
                        }
                    }

                    Rectangle {
                        id: rectangleNP8
                        radius: 0
                        transformOrigin: Item.Center
                        Layout.columnSpan: 1
                        Layout.rowSpan: 1
                        Layout.minimumHeight: 100
                        Layout.minimumWidth: 100
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        scale: 1
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.preferredHeight: 100
                        Layout.preferredWidth: 100
                        border.color: "#000000"
                        border.width: 1

                        function bluBox() {
                            skyblue3.visible = true
                            skyblue3.x = rectangleNP8.x+10
                            skyblue3.y = rectangleNP8.y+20
                            skyblue3.width = rectangleNP8.width+10
                            skyblue3.height = rectangleNP8.height+10



                            rectangleNP1.focus = false
                            rectangleNP2.focus = false
                            rectangleNP3.focus = false
                            rectangleNP4.focus = false
                            rectangleNP5.focus = false
                            rectangleNP6.focus = false
                            rectangleNP7.focus = false
                            rectangleNP8.focus = true


                            skyblue.visible = true
                            skyblue.x = rectangle6.x
                            skyblue.y = rectangle6.y
                            skyblue.width = rectangle6.width + 20
                            skyblue.height = rectangle6.height + 20
                            skyblue2.visible = false

                            rectangle1.focus = false
                            rectangle2.focus = false
                            rectangle3.focus = false
                            rectangle4.focus = false
                            rectangle5.focus = false
                            rectangle6.focus = true
                        }
                        Rectangle {
                            id: menuRectItemDashboardNP8
                            width: 600
                            height: dp(24)
                            color: "#7e827f"
                            anchors.left: parent.left
                            Text {
                                id: modelNameNP8
                                color: "#ffffff"
                                text: "НП №8"
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

                        GridView {
                            id: gridIPNP8
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            z: 1
                            anchors.verticalCenterOffset: -10
                            anchors.verticalCenter: parent.verticalCenter
                            Label {
                                id: labelNP8
                                y: 0
                                width: 35
                                text: qsTr("COM:")
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                font.pointSize: 11
                                height: 20
                            }

                            ComboBox {
                                id: modelIpNP8
                                y: 0
                                height: 20
                                anchors.right: parent.right
                                anchors.rightMargin: 4
                                anchors.left: labelNP8.right
                                anchors.leftMargin: 4
                                z: 1
                                model: port1.listCOM

                                currentIndex: 7

                                onPressedChanged:
                                {
                                    rectangleNP8.bluBox()
                                }
                            }
                        }

                        Button {
                            id: mdelButtonNP8
                            height: 23
                            text: if (modelName.text == "Поиск")
                                  {
                                      qsTr("Найти")
                                  }
                                  else
                                  {
                                      qsTr("Подключить")
                                  }
                            anchors.left: parent.left
                            z: 1
                            anchors.rightMargin: 0
                            anchors.right: parent.right
                            anchors.bottomMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0

                            BusyIndicator {
                                id: busyIndicator_ComboBox_8
                                //spacing: 10

                                anchors.fill: parent
                                running: false

                                visible: false
                            }


                            onClicked: {
                                rectangleNP8.bluBox();

                                if(mdelButtonNP8.text === qsTr("Подключить"))
                                {
                                    port8.openPort(modelIpNP8.currentText);

                                    busyIndicator_ComboBox_8.visible = true;
                                    busyIndicator_ComboBox_8.running = true;

                                    mdelButtonNP8.text = qsTr("");

                                }
                                else
                                {
                                    mdelButtonNP8.text = qsTr("Подключить");
                                    menuRectItemDashboardNP8.color = "#7e827f";
                                    port8.closePort();
                                    port8.slot_endWork();
                                }


                            }



                            Connections{
                                target: port8

                                // @disable-check M16
                                onSignal_Ok :
                                {

                                    console.log("dsafdasfdasf = " + good);
                                    if(good)
                                    {
                                        busyIndicator_ComboBox_8.visible = false;
                                        busyIndicator_ComboBox_8.running = false;


                                        mdelButtonNP8.text = qsTr("Отключить");
                                        mdelButton5.text = mdelButtonNP8.text;
                                        menuRectItemDashboardNP8.color = "green"

                                    }
                                    else
                                    {
                                        busyIndicator_ComboBox_8.visible = false;
                                        busyIndicator_ComboBox_8.running = false;

                                        mdelButtonNP8.text = qsTr("Подключить");
                                        menuRectItemDashboardNP8.color = "red"
                                    }


                                }
                            }
                        }

                        MouseArea {
                            visible: true
                            anchors.fill: parent

                            onClicked: rectangleNP8.bluBox()

                            onHeightChanged: if (rectangleNP8.focus) rectangleNP8.bluBox()
                            onWidthChanged: if (rectangleNP8.focus) rectangleNP8.bluBox()
                        }

                        Text {
                            id: modelDeviceNP8
                            height: 50
                            anchors.top: parent.top
                            anchors.topMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.left: parent.left
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            horizontalAlignment: Text.AlignHCenter

                            anchors.leftMargin: 8
                        }
                    }


                }

                //                Label {
                //                    id: label5
                //                    x: -26
                //                    y: 0
                //                    text: qsTr("COM:")
                //                    font.pointSize: 11
                //                    width: 20
                //                    height: 20
                //                }

                //                ComboBox
                //                {
                //                    id: modelIp5
                //                    x: 13
                //                    y: 0
                //                    width: 89
                //                    height: 20
                //                    z: 1

                //                }
            }


            Button {
                id: mdelButton5
                height: 23
                text: if (modelName5.text === qsTr("Подключить"))
                      {
                          qsTr("Найти")
                      }
                      else
                      {
                          qsTr("Подключить")
                      }
                anchors.left: parent.left
                z: 1
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.bottomMargin: 0
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0

                onClicked:{
                    rectangle6.bluBox()

                    if(mdelButton5.text === qsTr("Подключить"))
                    {
                        port1.openPort(modelIpNP.currentText);
                        port2.openPort(modelIpNP2.currentText);
                        port3.openPort(modelIpNP3.currentText);
                        port4.openPort(modelIpNP4.currentText);
                        port5.openPort(modelIpNP5.currentText);
                        port6.openPort(modelIpNP6.currentText);
                        port7.openPort(modelIpNP7.currentText);
                        port8.openPort(modelIpNP8.currentText);

                        busyIndicator_ComboBox_1.visible = true;
                        busyIndicator_ComboBox_1.running = true;
                        busyIndicator_ComboBox_2.visible = true;
                        busyIndicator_ComboBox_2.running = true;
                        busyIndicator_ComboBox_3.visible = true;
                        busyIndicator_ComboBox_3.running = true;
                        busyIndicator_ComboBox_4.visible = true;
                        busyIndicator_ComboBox_4.running = true;
                        busyIndicator_ComboBox_5.visible = true;
                        busyIndicator_ComboBox_5.running = true;
                        busyIndicator_ComboBox_6.visible = true;
                        busyIndicator_ComboBox_6.running = true;
                        busyIndicator_ComboBox_7.visible = true;
                        busyIndicator_ComboBox_7.running = true;
                        busyIndicator_ComboBox_8.visible = true;
                        busyIndicator_ComboBox_8.running = true;

                        mdelButton5.text = qsTr("Отключить");
                    }
                    else
                    {
                        port1.closePort();
                        port1.slot_endWork();
                        port2.closePort();
                        port2.slot_endWork();
                        port3.closePort();
                        port3.slot_endWork();
                        port4.closePort();
                        port4.slot_endWork();
                        port5.closePort();
                        port5.slot_endWork();
                        port6.closePort();
                        port6.slot_endWork();
                        port7.closePort();
                        port7.slot_endWork();
                        port8.closePort();
                        port8.slot_endWork();


                        mdelButtonNP.text = qsTr("Подключить");
                        menuRectItemDashboardNP.color = "#7e827f";
                        mdelButtonNP2.text = qsTr("Подключить");
                        menuRectItemDashboardNP2.color = "#7e827f";
                        mdelButtonNP3.text = qsTr("Подключить");
                        menuRectItemDashboardNP3.color = "#7e827f";
                        mdelButtonNP4.text = qsTr("Подключить");
                        menuRectItemDashboardNP4.color = "#7e827f";
                        mdelButtonNP5.text = qsTr("Подключить");
                        menuRectItemDashboardNP5.color = "#7e827f";
                        mdelButtonNP6.text = qsTr("Подключить");
                        menuRectItemDashboardNP6.color = "#7e827f";
                        mdelButtonNP7.text = qsTr("Подключить");
                        menuRectItemDashboardNP7.color = "#7e827f";
                        mdelButtonNP8.text = qsTr("Подключить");
                        menuRectItemDashboardNP8.color = "#7e827f";



                        mdelButton5.text = qsTr("Подключить");
                    }
                }


                onHeightChanged: {
                    if(skyblue.visible == false && rectangle6.focus)
                    {
                        skyblue2.height = rectangle6.height+20;
                        skyblue2.x = rectangle6.x;
                        skyblue2.y = rectangle6.y
                    }
                }
                onWidthChanged:  {
                    if(skyblue.visible == false && rectangle6.focus)
                    {
                        skyblue2.width = rectangle6.width+20;
                        skyblue2.x = rectangle6.x;
                        skyblue2.y = rectangle6.y;
                    }
                }
            }

            border.color: "#000000"
            border.width: 1
        }
    }

    //    /* С помощью объекта Connections
    //        * Устанавливаем соединение с классом ядра приложения
    //        * */
    //       Connections {
    //           target: gsg // Указываем целевое соединение
    //           /* Объявляем и реализуем функцию, как параметр
    //            * объекта и с имененем похожим на название сигнала
    //            * Разница в том, что добавляем в начале on и далее пишем
    //            * с заглавной буквы
    //            * */
    //           onConnectOk: {

    //                    radioButtonVKl.checked = true//qsTr("Вкл") // Устанавливаем счётчик в текстовый лейбл
    //           }
    //       }


}
