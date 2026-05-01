import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic

ApplicationWindow {
    visible: true
    width: 1000
    height: 650
    title: "Currency Converter"

    Rectangle {
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#05070d" }
            GradientStop { position: 1.0; color: "#111827" }
        }

        Rectangle {
            width: 520
            height: 460
            radius: 24
            anchors.centerIn: parent
            color: "#111827"
            border.color: "#c8a951"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 36
                spacing: 18

                Text {
                    text: "Currency Converter"
                    color: "#f5d67b"
                    font.pixelSize: 34
                    font.bold: true
                    font.family: "Georgia"
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: "Live exchange rates • Secure cached pricing"
                    color: "#9ca3af"
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    id: amountInput
                    placeholderText: "Enter amount"
                    Layout.fillWidth: true
                    font.pixelSize: 18
                    color: "#07090a"

                    background: Rectangle {
                        radius: 12
                        color: "#282f3d"
                        border.color: "#374151"
                        border.width: 1
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 14

                    ComboBox {
                        id: fromCurrency
                        model: ["USD", "EUR", "MXN", "GBP"]
                        Layout.fillWidth: true
                    }

                    ComboBox {
                        id: toCurrency
                        model: ["USD", "EUR", "MXN", "GBP"]
                        Layout.fillWidth: true
                    }
                }

                Button {
                    text: "Convert Currency"
                    Layout.fillWidth: true
                    height: 48

                    background: Rectangle {
                        radius: 14
                        color: parent.down ? "#8b6f2f" : parent.hovered ? "#d4af37" : "#c8a951"
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "#0b0e14"
                        font.bold: true
                        font.pixelSize: 16
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        resultText.text = converterBackend.convert(
                            Number(amountInput.text),
                            fromCurrency.currentText,
                            toCurrency.currentText
                        )
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 110
                    radius: 16
                    color: "#0b1220"
                    border.color: "#1f2937"

                    Text {
                        id: resultText
                        text: "Result will appear here"
                        color: "#e5e7eb"
                        font.pixelSize: 18
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
}