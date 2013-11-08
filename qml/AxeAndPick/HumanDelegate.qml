import QtQuick 2.0
//import Human 1.0

// This is the component that displays each human in the list
Component {
    Rectangle {

        color: "#FFeeeeee"
        height: 36
        anchors.left: parent.left
        anchors.right: parent.right


        border.width: 1
        border.color: "#FFE3E3E3"

        Image {
            id: typeIcon
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter

            source: "images/units/" + profession + ".svg"
            fillMode: Image.PreserveAspectFit
        }

        // Name and Type
        Item {
            id: humanID

            anchors.left: typeIcon.right
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom


            // Name and Profession
            Item
            {
                id: nameAndProfession
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                height: childrenRect.height

                Text {
                    id: nameText
                    text: name

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    font.pointSize: 10
                    clip: true
                }
                Text {
                    id: typeProfession
                    text: profession

                    anchors.top: nameText.bottom
                    anchors.left: nameText.left

                    font.pointSize: 7
                    color: "grey"
                    clip: true
                }
            }
        }

        Image {
            id: deleteButton
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            source: "images/deleteButton.svg"

            MouseArea {
                id: deleteButtonArea
                anchors.fill: parent
                onClicked: {
                    humanModel.remove(id)
                }
            }
            states:
                State { // Pressed
                    when: deleteButtonArea.pressed
                    PropertyChanges {
                        target: deleteButton
                        source: "images/deleteButtonPressed.svg"
                    }
                }
        }

        Image {
            id: infoButton
            anchors.right: deleteButton.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            source: "images/infoButton.svg"

            MouseArea {
                id: infoButtonArea
                anchors.fill: parent
                onClicked: {
                    humanModel.info(id)
                }
            }
            states:
                State { // Pressed
                    when: infoButtonArea.pressed
                    PropertyChanges {
                        target: infoButton
                        source: "images/infoButtonPressed.svg"
                    }
                }
        }
    }
}
