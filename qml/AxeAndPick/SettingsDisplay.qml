import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: settingsDialog

    property int windowWidth
    property string filePath
    property variant settingsObject

    anchors.fill: parent

    onVisibleChanged: {
        resetValidityMarker();
    }

    Rectangle {
        anchors.fill: parent
        color: "#80dddddd"
    }

    MouseArea {
        // This is to disable clicks going through the window.
        anchors.fill: parent
        onWheel: {} // do nothing
        onClicked: {
            settingsDialog.enabled = false;
            settingsDialog.visible = false;
        }
    }

    function resetValidityMarker()
    {
        autoBackupShortCheckboxId.checked = settings.getAutoBackupShort();
        autoBackupLongCheckboxId.checked = settings.getAutoBackupLong();

        directoryText.text = settings.getSavesDirectory();
        directoryTextOutline.color = savesAccess.pathIsValid() ? "green" : "red";
        if( savesAccess.pathIsValid() )
        {
            savesAccess.loadGamesList();
        }
    }

    Rectangle {
        id: fileOpenWindowBackground
        width: windowWidth
        color: "silver"

        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30

        // NOTE: The width is 5 to reflect consistancy with the draggable
        // border between lists.
        border.width: 5
        border.color: "gray"

        MouseArea {
            // This is to disable clicks going through the window.
            anchors.fill: parent
            onClicked: {}
        }
    }


    Item {
        id: fileOpenContents
        anchors.top: fileOpenWindowBackground.top
        anchors.topMargin: 5
        anchors.left: fileOpenWindowBackground.left
        anchors.leftMargin: 5
        anchors.right: fileOpenWindowBackground.right
        anchors.rightMargin: 5
        anchors.bottom: fileOpenWindowBackground.bottom
        anchors.bottomMargin: 5

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
                id: directoryTextOutline
                color: "gray"
                height: 24

                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: directoryTitleOutline.bottom
                anchors.topMargin: 4

                Rectangle {
                    id: directoryTextBackground
                    color: "#FFf8f8f8"
                    height: parent.height-2

                    clip: true
                    anchors.left: parent.left
                    anchors.leftMargin: 1
                    anchors.right: openFileButton.left
                    anchors.rightMargin: 1
                    anchors.verticalCenter: parent.verticalCenter

                    TextInput {
                        id: directoryText
                        width: directoryTextBackground.width-12
                        anchors {
                            left: parent.left;
                            right: parent.right;
                            centerIn: parent;
                        }

                        text: settings.getSavesDirectory();
                        onTextChanged: {
                            settings.setSavesDirectory(text);
                            resetValidityMarker();
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
                            // Open the file dialog
                            savesAccess.openFileDialog();
                            directoryText.text = settings.getSavesDirectory();
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
                anchors.top: autoBackupTitleOutline.bottom;
                anchors.left: parent.left;
                anchors.right: parent.right;
                height: 30;

                //color: "transparent";

                checkboxText: "Backup re.sav and un.sav (Recommened)";
                checked: settings.getAutoBackupShort();

                MouseArea {
                    anchors.fill: parent;
                    drag.filterChildren: true;
                    onClicked: {
                        settings.invertAutoBackupShort();
                        autoBackupShortCheckboxId.checked = settings.getAutoBackupShort();
                    }
                }
            }

            Checkbox {
                id: autoBackupLongCheckboxId;
                anchors.top: autoBackupShortCheckboxId.bottom;
                anchors.left: parent.left;
                anchors.right: parent.right;
                height: 30;

                color: "transparent";

                checkboxText: "Backup saves dir (Not Recommended)";
                checked: settings.getAutoBackupLong();

                MouseArea {
                    anchors.fill: parent;
                    drag.filterChildren: true;
                    onClicked: {
                        settings.invertAutoBackupLong();
                        autoBackupLongCheckboxId.checked = settings.getAutoBackupLong();
                    }
                }
            }
        }
    }
}
