/*
import QtQuick 2.4
import QtDataVisualization 1.2

Item {
    width: 400
    height: 400

    Surface3D {
        id: surface3D1
        x: 59
        y: 70
        width: 300
        height: 300
        Surface3DSeries {
            ItemModelSurfaceDataProxy {
                columnRole: "column"
                yPosRole: "y"
                itemModel: ListModel {
                    ListElement {
                        y: "1"
                        column: "1"
                        row: "1"
                    }

                    ListElement {
                        y: "2"
                        column: "2"
                        row: "1"
                    }

                    ListElement {
                        y: "3"
                        column: "1"
                        row: "2"
                    }

                    ListElement {
                        y: "4"
                        column: "2"
                        row: "2"
                    }
                }
                rowRole: "row"
            }
        }
    }
}
*/
import QtQuick 2.0

Rectangle {
    id: page
    width: 320; height: 480
    color: "lightgray"

    Text {
        id: helloText
        text: "Hello world!"
        y: 30
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }
}
