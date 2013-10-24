import QtQuick 2.0

Rectangle {
    property string checkboxText
    property bool checked
    property variant target

    onCheckedChanged: {
        resetValidityMarker();
    }

    onVisibleChanged: {
        resetValidityMarker();
    }

    function resetValidityMarker()
    {
        checkboxCheckedButtonId.visible = (checked == true);
        checkboxUncheckedButtonId.visible = (checked != true);
    }

    Image {
        id: checkboxCheckedButtonId
        source: "images/checkboxCheckedIcon.svg"
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent
        visible: (checked == true)
    }

    Image {
        id: checkboxUncheckedButtonId
        source: "images/checkboxUncheckedIcon.svg"
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent
        visible: (checked != true);
    }

    Text {
        id: checkboxTextId
        anchors {
            left: checkboxCheckedButtonId.right + 32;
            centerIn: parent;
        }

        text: checkboxText;
        font.pointSize: 10
    }
}
