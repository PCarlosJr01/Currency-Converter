import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 900
    height: 600
    title: "Currency Converter"

    Rectangle {
        anchors.fill: parent
        color: "#0f172a"

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "Currency Converter"
                color: "white"
                font.pixelSize: 36
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }

            TextField {
                id: amountInput
                placeholderText: "Enter amount"
                width: 250
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                spacing: 10
                Layout.alignment: Qt.AlignHCenter

                ComboBox {
                    id: fromCurrency
                    model: ["USD", "EUR", "MXN", "GBP"]
                }

                ComboBox {
                    id: toCurrency
                    model: ["USD", "EUR", "MXN", "GBP"]
                }
            }

            Button {
                text: "Convert"
                Layout.alignment: Qt.AlignHCenter

                onClicked: {
                    resultText.text = converterBackend.convert(
                        Number(amountInput.text),
                        fromCurrency.currentText,
                        toCurrency.currentText
                    )
                }
            }

            Text {
                id: resultText
                text: "Result will appear here"
                color: "#94a3b8"
                font.pixelSize: 18
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}