import QtQuick 2.0

Rectangle {
    property string checkboxText;
    property bool checked;

    color: "transparent";

    onCheckedChanged: {
        resetValidityMarker();
    }

    onCheckboxTextChanged: {
        resetValidityMarker();
    }

    onVisibleChanged: {
        resetValidityMarker();
    }

    function resetValidityMarker()
    {
        checkedImage.visible = (checked == true);
        uncheckedImage.visible = (checked != true);
    }

    Rectangle {
        id: outline;
        color: "#ff000000";
        height: 24;

        anchors.fill: parent;
        anchors.topMargin: 4;
        anchors.leftMargin: 0;
        anchors.bottomMargin: 0;
        anchors.rightMargin: 0;

        Rectangle {
            id: background;
            color: "#fff8f8f8";
            clip: true;
            anchors.fill: parent;
            anchors.margins: 2;
            anchors.verticalCenter: parent.verticalCenter;

            Image {
                id: checkedImage;

                source: "images/checkboxCheckedIcon.svg";
                fillMode: Image.PreserveAspectFit;

                anchors.leftMargin: 5;
                anchors.left: parent.left;
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;

                visible: (checked == true);
            }

            Image {
                id: uncheckedImage;

                source: "images/checkboxUncheckedIcon.svg";
                fillMode: Image.PreserveAspectFit;

                anchors.leftMargin: 5;
                anchors.left: parent.left;
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;

                visible: (checked != true);
            }

            Text {
                anchors.margins: 2;
                anchors.left: checkedImage.right;
                anchors.right: parent.right;
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;

                clip: true;

                text: checkboxText;
                font.pointSize: 10;
            }

            MouseArea {
                anchors.fill: parent;
                hoverEnabled: true;
                onEntered: { background.color = "#ffa8a8a8"; }
                onExited: { background.color = "#fff8f8f8"; }
            }
        }
    }
}
