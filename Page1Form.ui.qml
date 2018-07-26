import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

import QtQuick.Window 2.1

Item {
    id: mrkList
    // Пересчёт независимых от плотности пикселей в физические пиксели устройства
    readonly property int dpi: Screen.pixelDensity * 25.4
    // @disable-check M222
    function dp(x) {
        return (dpi < 120) ? x : x * (dpi / 160)
    }

    GridLayout {
        // @disable-check M15
        id: mrkList
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
            id: rectangleNP1
            radius: 0
            transformOrigin: Item.Center
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.minimumHeight: 100
            Layout.minimumWidth: 100
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            border.color: "#000000"
            border.width: 1

            // @disable-check M222
            function bluBox() {
                skyblue.visible = true
                skyblue.x = rectangleNP1.x
                skyblue.y = rectangleNP1.y
                skyblue.width = rectangleNP1.width + 20
                skyblue.height = rectangleNP1.height + 20
                skyblue2.visible = false

                rectangleNP1.focus = true
                rectangleNP2.focus = false
                rectangleNP3.focus = false
                rectangleNP4.focus = false
                rectangleNP5.focus = false
                rectangleNP6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboardNP
                width: 600
                // @disable-check M222
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName
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
                }
            }

            Button {
                id: mdelButtonNP
                height: 23
                text: if (modelName.text == "Поиск")
                          // @disable-check M223
                      {
                          qsTr("Найти")
                      } else
                          // @disable-check M223
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

                // @disable-check M223
                onClicked: {
                    // @disable-check M222
                    rectangleNP1.bluBox()
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                // @disable-check M222
                onClicked: rectangleNP1.bluBox()

                onHeightChanged: if (skyblue2.visible == false
                                         && rectangleNP1.focus)
                                     skyblue.height = rectangleNP1.height + 20,skyblue.x
                                             = rectangleNP1.xskyblue.y = rectangleNP1.y
                onWidthChanged: if (skyblue2.visible == false
                                        && rectangleNP1.focus)
                                    skyblue.width = rectangleNP1.width + 20,skyblue.x
                                            = rectangleNP1.xskyblue.y = rectangleNP1.y
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            border.color: "#000000"
            border.width: 1

            // @disable-check M222
            function bluBox() {
                skyblue.visible = true
                skyblue.x = rectangleNP2.x
                skyblue.y = rectangleNP2.y
                skyblue.width = rectangleNP2.width + 20
                skyblue.height = rectangleNP2.height + 20
                skyblue2.visible = false

                rectangleNP1.focus = false
                rectangleNP2.focus = true
                rectangleNP3.focus = false
                rectangleNP4.focus = false
                rectangleNP5.focus = false
                rectangleNP6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboardNP2
                width: 600
                // @disable-check M222
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName2
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
                    anchors.left: labelNP.right
                    anchors.leftMargin: 4
                    z: 1
                }
            }

            Button {
                id: mdelButtonNP2
                height: 23
                text: if (modelName.text == "Поиск")
                          // @disable-check M223
                      {
                          qsTr("Найти")
                      } else
                          // @disable-check M223
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

                // @disable-check M223
                onClicked: {
                    // @disable-check M222
                    rectangleNP2.bluBox()
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                // @disable-check M222
                onClicked: rectangleNP2.bluBox()

                onHeightChanged: if (skyblue2.visible == false
                                         && rectangleNP2.focus)
                                     skyblue.height = rectangleNP2.height + 20,skyblue.x
                                             = rectangleNP2.xskyblue.y = rectangleNP2.y
                onWidthChanged: if (skyblue2.visible == false
                                        && rectangleNP2.focus)
                                    skyblue.width = rectangleNP2.width + 20,skyblue.x
                                            = rectangleNP2.xskyblue.y = rectangleNP2.y
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            border.color: "#000000"
            border.width: 1

            // @disable-check M222
            function bluBox() {
                skyblue.visible = true
                skyblue.x = rectangleNP3.x
                skyblue.y = rectangleNP3.y
                skyblue.width = rectangleNP3.width + 20
                skyblue.height = rectangleNP3.height + 20
                skyblue2.visible = false

                rectangleNP1.focus = false
                rectangleNP2.focus = false
                rectangleNP3.focus = true
                rectangleNP4.focus = false
                rectangleNP5.focus = false
                rectangleNP6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboardNP3
                width: 600
                // @disable-check M222
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName3
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
                    anchors.left: labelNP.right
                    anchors.leftMargin: 4
                    z: 1
                }
            }

            Button {
                id: mdelButtonNP3
                height: 23
                text: if (modelName.text == "Поиск")
                          // @disable-check M223
                      {
                          qsTr("Найти")
                      } else
                          // @disable-check M223
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

                // @disable-check M223
                onClicked: {
                    // @disable-check M222
                    rectangleNP2.bluBox()
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                // @disable-check M222
                onClicked: rectangleNP3.bluBox()

                onHeightChanged: if (skyblue2.visible == false
                                         && rectangleNP3.focus)
                                     skyblue.height = rectangleNP3.height + 20,skyblue.x
                                             = rectangleNP3.xskyblue.y = rectangleNP3.y
                onWidthChanged: if (skyblue2.visible == false
                                        && rectangleNP3.focus)
                                    skyblue.width = rectangleNP3.width + 20,skyblue.x
                                            = rectangleNP3.xskyblue.y = rectangleNP3.y
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            border.color: "#000000"
            border.width: 1

            // @disable-check M222
            function bluBox() {
                skyblue.visible = true
                skyblue.x = rectangleNP4.x
                skyblue.y = rectangleNP4.y
                skyblue.width = rectangleNP4.width + 20
                skyblue.height = rectangleNP4.height + 20
                skyblue2.visible = false

                rectangleNP1.focus = false
                rectangleNP2.focus = false
                rectangleNP3.focus = false
                rectangleNP4.focus = true
                rectangleNP5.focus = false
                rectangleNP6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboardNP4
                width: 600
                // @disable-check M222
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName4
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
                    anchors.left: labelNP.right
                    anchors.leftMargin: 4
                    z: 1
                }
            }

            Button {
                id: mdelButtonNP4
                height: 23
                text: if (modelName.text == "Поиск")
                          // @disable-check M223
                      {
                          qsTr("Найти")
                      } else
                          // @disable-check M223
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

                // @disable-check M223
                onClicked: {
                    // @disable-check M222
                    rectangleNP2.bluBox()
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                // @disable-check M222
                onClicked: rectangleNP3.bluBox()

                onHeightChanged: if (skyblue2.visible == false
                                         && rectangleNP4.focus)
                                     skyblue.height = rectangleNP4.height + 20,skyblue.x
                                             = rectangleNP4.xskyblue.y = rectangleNP4.y
                onWidthChanged: if (skyblue2.visible == false
                                        && rectangleNP4.focus)
                                    skyblue.width = rectangleNP4.width + 20,skyblue.x
                                            = rectangleNP4.xskyblue.y = rectangleNP4.y
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            border.color: "#000000"
            border.width: 1

            // @disable-check M222
            function bluBox() {
                skyblue.visible = true
                skyblue.x = rectangleNP5.x
                skyblue.y = rectangleNP5.y
                skyblue.width = rectangleNP5.width + 20
                skyblue.height = rectangleNP5.height + 20
                skyblue2.visible = false

                rectangleNP1.focus = false
                rectangleNP2.focus = false
                rectangleNP3.focus = false
                rectangleNP4.focus = false
                rectangleNP5.focus = true
                rectangleNP6.focus = false
            }

            Rectangle {
                id: menuRectItemDashboardNP5
                width: 600
                // @disable-check M222
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName5
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
                    anchors.left: labelNP.right
                    anchors.leftMargin: 4
                    z: 1
                }
            }

            Button {
                id: mdelButtonNP5
                height: 23
                text: if (modelName.text == "Поиск")
                          // @disable-check M223
                      {
                          qsTr("Найти")
                      } else
                          // @disable-check M223
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

                // @disable-check M223
                onClicked: {
                    // @disable-check M222
                    rectangleNP2.bluBox()
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                // @disable-check M222
                onClicked: rectangleNP3.bluBox()

                onHeightChanged: if (skyblue2.visible == false
                                         && rectangleNP5.focus)
                                     skyblue.height = rectangleNP5.height + 20,skyblue.x
                                             = rectangleNP5.xskyblue.y = rectangleNP5.y
                onWidthChanged: if (skyblue2.visible == false
                                        && rectangleNP5.focus)
                                    skyblue.width = rectangleNP5.width + 20,skyblue.x
                                            = rectangleNP5.xskyblue.y = rectangleNP5.y
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            border.color: "#000000"
            border.width: 1

            // @disable-check M222
            function bluBox() {
                skyblue.visible = true
                skyblue.x = rectangleNP6.x
                skyblue.y = rectangleNP6.y
                skyblue.width = rectangleNP6.width + 20
                skyblue.height = rectangleNP6.height + 20
                skyblue2.visible = false

                rectangleNP1.focus = false
                rectangleNP2.focus = false
                rectangleNP3.focus = false
                rectangleNP4.focus = false
                rectangleNP5.focus = false
                rectangleNP6.focus = true
            }

            Rectangle {
                id: menuRectItemDashboardNP6
                width: 600
                // @disable-check M222
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName6
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
                    anchors.left: labelNP.right
                    anchors.leftMargin: 4
                    z: 1
                }
            }

            Button {
                id: mdelButtonNP6
                height: 23
                text: if (modelName.text == "Поиск")
                          // @disable-check M223
                      {
                          qsTr("Найти")
                      } else
                          // @disable-check M223
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

                // @disable-check M223
                onClicked: {
                    // @disable-check M222
                    rectangleNP2.bluBox()
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                // @disable-check M222
                onClicked: rectangleNP3.bluBox()

                onHeightChanged: if (skyblue2.visible == false
                                         && rectangleNP6.focus)
                                     skyblue.height = rectangleNP6.height + 20,skyblue.x
                                             = rectangleNP6.xskyblue.y = rectangleNP6.y
                onWidthChanged: if (skyblue2.visible == false
                                        && rectangleNP6.focus)
                                    skyblue.width = rectangleNP6.width + 20,skyblue.x
                                            = rectangleNP6.xskyblue.y = rectangleNP6.y
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            border.color: "#000000"
            border.width: 1

            // @disable-check M222
            function bluBox() {
                skyblue.visible = true
                skyblue.x = rectangleNP7.x
                skyblue.y = rectangleNP7.y
                skyblue.width = rectangleNP7.width + 20
                skyblue.height = rectangleNP7.height + 20
                skyblue2.visible = false

                rectangleNP1.focus = false
                rectangleNP2.focus = false
                rectangleNP3.focus = false
                rectangleNP4.focus = false
                rectangleNP5.focus = false
                rectangleNP6.focus = false
                rectangleNP7.focus = true
            }

            Rectangle {
                id: menuRectItemDashboardNP7
                width: 600
                // @disable-check M222
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName7
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
                    anchors.left: labelNP.right
                    anchors.leftMargin: 4
                    z: 1
                }
            }

            Button {
                id: mdelButtonNP7
                height: 23
                text: if (modelName.text == "Поиск")
                          // @disable-check M223
                      {
                          qsTr("Найти")
                      } else
                          // @disable-check M223
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

                // @disable-check M223
                onClicked: {
                    // @disable-check M222
                    rectangleNP2.bluBox()
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                // @disable-check M222
                onClicked: rectangleNP3.bluBox()

                onHeightChanged: if (skyblue2.visible == false
                                         && rectangleNP7.focus)
                                     skyblue.height = rectangleNP7.height + 20,skyblue.x
                                             = rectangleNP7.xskyblue.y = rectangleNP7.y
                onWidthChanged: if (skyblue2.visible == false
                                        && rectangleNP7.focus)
                                    skyblue.width = rectangleNP7.width + 20,skyblue.x
                                            = rectangleNP7.xskyblue.y = rectangleNP7.y
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            scale: 1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            border.color: "#000000"
            border.width: 1

            // @disable-check M222
            function bluBox() {
                skyblue.visible = true
                skyblue.x = rectangleNP7.x
                skyblue.y = rectangleNP7.y
                skyblue.width = rectangleNP7.width + 20
                skyblue.height = rectangleNP7.height + 20
                skyblue2.visible = false

                rectangleNP1.focus = false
                rectangleNP2.focus = false
                rectangleNP3.focus = false
                rectangleNP4.focus = false
                rectangleNP5.focus = false
                rectangleNP6.focus = false
                rectangleNP7.focus = true
            }

            Rectangle {
                id: menuRectItemDashboardNP8
                width: 600
                // @disable-check M222
                height: dp(24)
                color: "#7e827f"
                anchors.left: parent.left
                Text {
                    id: modelName8
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
                    anchors.left: labelNP.right
                    anchors.leftMargin: 4
                    z: 1
                }
            }

            Button {
                id: mdelButtonNP8
                height: 23
                text: if (modelName.text == "Поиск")
                          // @disable-check M223
                      {
                          qsTr("Найти")
                      } else
                          // @disable-check M223
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

                // @disable-check M223
                onClicked: {
                    // @disable-check M222
                    rectangleNP2.bluBox()
                }
            }

            MouseArea {
                visible: true
                anchors.fill: parent

                // @disable-check M222
                onClicked: rectangleNP3.bluBox()

                onHeightChanged: if (skyblue2.visible == false
                                         && rectangleNP8.focus)
                                     skyblue.height = rectangleNP8.height + 20,skyblue.x
                                             = rectangleNP8.xskyblue.y = rectangleNP8.y
                onWidthChanged: if (skyblue2.visible == false
                                        && rectangleNP8.focus)
                                    skyblue.width = rectangleNP8.width + 20,skyblue.x
                                            = rectangleNP8.xskyblue.y = rectangleNP8.y
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
}
