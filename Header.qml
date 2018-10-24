import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import "js/style.js" as MyStyle


Item {
    id: header


    property string colorLabelApp: "#ab4d36"
    property string colorHover: "#595e6f"
    property string rectHeader: "images/base_copy_grey.png"
    property string imgLogo: "images/logo_white.png"
    property Label exit_label: exit_label
    property Label etap_label: etap_label
    property MouseArea mouseArea_exit2: mouseArea_exit2
    property CheckBox autoFlag: autoFlag

    Component.onCompleted: MyStyle.setBar_grey();

    Image {
        id: header_rect
        source: rectHeader
        height: 60
        anchors.top: parent.top
        anchors.topMargin: -10
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        opacity: 1

//        Image {
//            id: label_App
//            source: imgLogo
//            //            height: 41
//            width: 30
//            visible: true


//            anchors.left: parent.left
//            anchors.leftMargin: 21
//            verticalAlignment: Text.AlignVCenter
//            anchors.bottom: parent.bottom
//            anchors.bottomMargin: 15
//            anchors.top: parent.top
//            anchors.topMargin: 15
//            horizontalAlignment: Text.AlignHCenter

//            MouseArea
//            {
//                id: mouseArea_label_App
//                hoverEnabled : true
//                anchors.fill: parent
//                onContainsMouseChanged: {
//                    if(this.containsMouse)
//                        MyStyle.setBar_orange();
//                    else
//                        MyStyle.setBar_grey(); }
//            }
//        }

        Label
        {
            id: etap_label
            y: 20
            color: "#ffffff"
            text: qsTr("")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: parent.left//label_App.right
            anchors.leftMargin: 5
            Layout.fillHeight: true

            font.pixelSize: 14
            font.family: "Lato"
        }


        CheckBox
        {
            id: autoFlag
            visible: false
            text: qsTr("Автоматизированная проверка")
            checked: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: etap_label.right
            anchors.leftMargin: 5
            anchors.right: rightArrow_Right.left
            anchors.rightMargin: 5


            contentItem: Text {
                leftPadding: autoFlag.indicator.width + 4
                text: autoFlag.text
                color: "#ffffff"
                elide: Text.ElideRight
                visible: autoFlag.text
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.family: "Lato"
                font.pixelSize: 14
            }

        }




        Item {
            id: item_in_out
            width: 100

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 23
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 10

            Image {
                id: in_out
                visible: false
                x: 70
                y: -6
                width: 0
                height: 30
                anchors.right: parent.right
                anchors.rightMargin: 0
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "images/out.png"

                MouseArea
                {
                    id: mouseArea_exit2
                    anchors.fill: parent
                    hoverEnabled : true
                    onClicked:
                    {
                        if(exit_label.text == MyStyle.sigIN_default)
                            mainWindow.close();
                        else
                        {
                            view.currentIndex = 0;
                            in_out.visible = false;
                            exit_label.text = MyStyle.sigIN_default;
                            etap_label.text = "";
                            autoFlag.visible = false;
                        }

                    }
                    onContainsMouseChanged: {

                        if(this.containsMouse)
                            exit_label.color = colorHover;
                        else
                            exit_label.color =  "#ffffff";}
                }
            }

            Label {
                id: exit_label
                color: "#ffffff"
                text: qsTr(MyStyle.sigIN_default)
                anchors.right: in_out.left
                anchors.rightMargin: 5
                Layout.fillHeight: true

                font.pixelSize: 14
                font.family: "Lato"

                onTextChanged: {
                    if(exit_label.text != MyStyle.sigIN_default)
                    {
                        in_out.visible = true;
                        in_out.width = 30;
                    }
                    else
                    {
                        in_out.width = 0;
                    }
                }

                MouseArea
                {
                    id: mouseArea_exit
                    anchors.fill: parent
                    hoverEnabled : true
                    onClicked:
                    {
                        if(exit_label.text == MyStyle.sigIN_default)
                            mainWindow.close();
                        else
                        {
                            view.currentIndex = 0;
                            in_out.visible = false;
                            exit_label.text = MyStyle.sigIN_default;
                            etap_label.text = "";
                            autoFlag.visible = false;
                        }

                    }
                    onContainsMouseChanged: {

                        if(this.containsMouse)
                            exit_label.color = colorHover;
                        else
                            exit_label.color =  "#ffffff";}
                }

            }


        }

    }

}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
