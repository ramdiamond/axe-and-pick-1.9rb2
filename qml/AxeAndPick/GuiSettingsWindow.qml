import QtQuick 2.0
//import QtGraphicalEffects 1.0

Item {
    width: 700
    height: 600
    id: rootWindow

    onVisibleChanged: {
        directoryText.text = myHandler.getSavesDirectory();
        directoryTextOutline.color = myHandler.isSavesDirectoryValid() ? "green" : "red";
    }

    Rectangle {
        anchors.fill: parent
        color: "#ffdddddd"
    }

    Rectangle {
        id: rootWindowBackground
        color: "#ffdddddd"

        anchors.fill: parent;
        anchors.margins: 5;
        border.width: 5
        border.color: "#ff888888"
    }


    Item {
        id: fileOpenContents
        x: 5
        y: 5
        width: 250
        height: 530
        anchors.fill: rootWindowBackground;
        anchors.margins: 5;

        Rectangle {
            id: savesFileDirectoryBackground
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            height: 64
            color: "gray"

            Rectangle {
                id: directoryTitleOutline
                color: "gray"
                height: 24

                anchors.left: parent.left
                anchors.leftMargin: 1
                anchors.right: parent.right
                anchors.rightMargin: 1
                anchors.top: parent.top
                anchors.topMargin: 4

                Rectangle {
                    id: directoryTitleBackground
                    color: "#FFf8f8f8"
                    height: parent.height-2

                    clip: true
                    anchors.left: parent.left
                    anchors.leftMargin: 1
                    anchors.right: parent.right
                    anchors.rightMargin: 1
                    anchors.verticalCenter: parent.verticalCenter

                    Text {
                        id: directoryTitle
                        width: directoryTitleBackground.width-12
                        anchors {
                            left: parent.left;
                            right: parent.right;
                            centerIn: parent;
                        }

                        text: "Timber and Stone 'saves.sav' file:";
                        font.pointSize: 10
                    }
                }
            }

            Rectangle {
                id: directoryTextBackground
                color: "#FFf8f8f8"
                height: 24

                border.color: "#ff888888";
                border.width: 1;

                clip: true
                anchors.left: parent.left
                anchors.leftMargin: 1
                anchors.right: openFileButton.left
                anchors.rightMargin: 1
                anchors.verticalCenter: parent.verticalCenter

                TextInput {
                    id: directoryText
                    objectName: "SavesDirectoryText";
                    width: directoryTextBackground.width-12
                    anchors {
                        left: parent.left;
                        right: parent.right;
                        centerIn: parent;
                    }

                    text: "";
                    onTextChanged: {
                        myHandler.onTextChangedSavesDirectory(text);
                    }

                    autoScroll: true
                    selectByMouse: true
                    font.pointSize: 10
                }
            }
            Image {
                id: openFileButton
                anchors.right: parent.right
                anchors.rightMargin: 1
                anchors.verticalCenter: parent.verticalCenter
                source: "images/openButton.svg"
                MouseArea {
                    id: openFileButtonArea
                    anchors.fill: parent
                    onClicked: {
                        myHandler.onClickOpenSavesDirectoryFile();
                    }
                }
                states:
                    State { // Pressed
                        when: openFileButtonArea.pressed
                        PropertyChanges {
                            target: openFileButton
                            source: "images/openButtonPressed.svg"
                        }
                    }
            }
        }

        Rectangle {
            id: autoBackupBackground
            anchors.top: savesFileDirectoryBackground.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            height: 96
            color: "gray"

            Rectangle {
                id: autoBackupTitleOutline
                color: "gray"
                height: 24

                anchors.left: parent.left
                anchors.leftMargin: 1
                anchors.right: parent.right
                anchors.rightMargin: 1
                anchors.top: parent.top
                anchors.topMargin: 4

                Rectangle {
                    id: autoBackupTitleBackground
                    color: "#FFf8f8f8"
                    height: parent.height-2

                    clip: true
                    anchors.left: parent.left
                    anchors.leftMargin: 1
                    anchors.right: parent.right
                    anchors.rightMargin: 1
                    anchors.verticalCenter: parent.verticalCenter

                    Text {
                        id: autoBackupTitle
                        width: autoBackupTitleBackground.width-12
                        anchors {
                            left: parent.left;
                            right: parent.right;
                            centerIn: parent;
                        }

                        text: "Auto-backups:";
                        font.pointSize: 10
                    }
                }
            }

            Checkbox {
                id: autoBackupShortCheckboxId;
                objectName: "AutoBackupShortCheckboxId";
                anchors.top: autoBackupTitleOutline.bottom;
                anchors.left: parent.left;
                anchors.right: parent.right;
                height: 30;

                checkboxText: "Backup re.sav and un.sav (Recommened)";
                checked: false;

                MouseArea {
                    anchors.fill: parent;
                    drag.filterChildren: true;
                    onClicked: {
                        myHandler.onClickAutoBackupShort();
                        //autoBackupShortCheckboxId.checked = myHandler.getAutoBackupShort();
                    }
                }
            }

            Checkbox {
                id: autoBackupLongCheckboxId;
                objectName: "AutoBackupLongCheckboxId";
                anchors.top: autoBackupShortCheckboxId.bottom;
                anchors.left: parent.left;
                anchors.right: parent.right;
                height: 30;

                color: "transparent";

                checkboxText: "Backup saves dir (Not Recommended)";
                checked: false;

                MouseArea {
                    anchors.fill: parent;
                    drag.filterChildren: true;
                    onClicked: {
                        myHandler.onClickAutoBackupLong();
                        //autoBackupLongCheckboxId.checked = myHandler.getAutoBackupLong();
                    }
                }
            }
        }
    }
}
