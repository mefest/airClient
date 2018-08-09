import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0
import air.control.backend 1.0
import QtQuick.Controls.Material 2.2


//import Theme 1.0
ApplicationWindow {
    id: window
    width: 480
    height: 860
    visible: true

    Client{
        id:client
        host: "127.0.0.1"
        port: 1024
        onConnetError: {
            console.log(client.error)
        }
    }

    FontLoader {
        id: lcd
        source: "qrc:/LCDMN___.TTF"
    }

    Rectangle {
        id: rectangle
        height: 50
        color: "#2E2F30"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Switch {
            id: swON
            text: qsTr("Off")
            checkable: false
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.right: parent.right
            onClicked: {
                if(!client.isConnect){
                    client.tryConnect();
                } else {
                    client.disconnect();
                }

            }
            checked: client.isConnect

            onCheckedChanged: {
                if(!swON.checked){
                    text = 'Off';
                } else {
                    text = 'On'
                }
            }
        }
    }

    Rectangle {
        id: display
        height: 100
        color: "#161616"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: rectangle.bottom
        anchors.topMargin: 0

        Text {
            id: text1
            color: "#ffffff"
            text: qsTr("22 °C")
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            fontSizeMode: Text.Fit
            font.pixelSize: 100
            font.family: lcd.name
        }
    }
    Glow {
        color: "#ffffff"
        anchors.fill: display
        source: text1
        radius: 20
        samples: 22
        spread: 0.2
        fast: false
        cached: true
        transparentBorder: true
    }

    Rectangle {
        id: controls
        color: "#404244"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: display.bottom
        anchors.topMargin: 0

        GridLayout {
            id: grid
            rows: 3
            columns: 2
            anchors.fill: parent

            RoundButton {
                id: roundButton
                Layout.columnSpan: 2
                Layout.fillWidth: true
                Layout.preferredHeight: 100

                text: "Off"
                font.pointSize: 22
                icon.source:"qrc:/img/power.svg"
                icon.color: "red"
                icon.width: 70
                icon.height:70
                checkable: true
                checked: client.power
                onCheckedChanged: {
                    client.power = checked
                    if(checked){
                        text = "On"
                    } else {
                        text = "Off"
                    }
                }

            }

            Slider {
                id: slider
                width: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 100
                Layout.preferredHeight: 300
                Image {
                    width: 50
                    height: 50
                    sourceSize.width: 48
                    sourceSize.height: 48
                    source: "qrc:/img/thermometr.svg"
                }

                orientation: Qt.Vertical

                from: 18
                value: client.temp

                to: 25
                stepSize: 1
                snapMode: Slider.SnapAlways
                onValueChanged: {
                    client.temp = value
                    text1.text = client.temp + " °C"
                }
            }


            Slider {
                id: slider2
                width: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: 100
                Layout.preferredHeight: 300
                value: client.fan
                Image {
                    width: 50
                    height: 50
                    sourceSize.width: 48
                    sourceSize.height: 48
                    source: "qrc:/img/fan.svg"
                }
                Image {
                    width: 50
                    height: 50
                    anchors.bottom: parent.bottom
                    sourceSize.width: 48
                    sourceSize.height: 48
                    source: "qrc:/img/fan-off.svg"
                }

                orientation: Qt.Vertical

                from: 0
                to: 2
                stepSize: 1
                snapMode: Slider.SnapAlways
                onValueChanged: {
                    client.fan = value
                }
            }

            ComboBox {
                id: comboBox
                height: 70
                Layout.preferredHeight: 80
                Layout.preferredWidth: 140
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;

                currentIndex: client.hot?1:0
                model: ListModel {
                    id:listModel
                    ListElement { value: 0; icon: "qrc:/img/sun.svg" }
                    ListElement { value: 1; icon: "qrc:/img/snowflake.svg" }
                }

                delegate: ItemDelegate {
                    width: parent.width
                    contentItem: Image{

                        source: model.icon
                        fillMode: Image.PreserveAspectFit
                    }
                    highlighted: comboBox.highlightedIndex === index
                }
                contentItem: Image {
                    anchors.fill: parent
                    source: listModel.get(comboBox.currentIndex).icon
                    sourceSize.width: 48
                    sourceSize.height: 48
                    anchors.margins: 10
                    fillMode: Image.PreserveAspectFit
                }
                onCurrentIndexChanged: {
                     client.hot = currentIndex
                }

            }

            Button {
                id: button
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 80
                Layout.preferredWidth: 140
                flat: false
                checkable: true
                icon.source: "qrc:/img/wind.svg"
                icon.color:"white"
                icon.width: button.height
                checked: client.swing
                onCheckedChanged: {
                    client.swing = checked
                }
            }
        }

    }
}
