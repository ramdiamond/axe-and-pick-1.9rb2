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
        autoBackupShortCheckboxId.checked = settings.value("TimberAndStone/AutoBackupShort");
        autoBackupFullCheckboxId.checked = settings.value("TimberAndStone/AutoBackupFull");

        directoryText.text = savesAccess.pathIsValid() ? savesAccess.getSavesPath() : "Timber and Stone 'saves.sav' file";
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

                        text: savesAccess.pathIsValid() ? savesAccess.getSavesPath() : "Timber and Stone 'saves.sav' file";
                        onTextChanged: {
                            settings.setValue("TimberAndStone/GameInstallationDirectory",
                                              text);
                            savesAccess.setFilePath(text);

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
                            directoryText.text = savesAccess.getSavesPath();
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

                Rectangle {
                    id: autoBackupCheckboxOutline
                    color: "#FFa0a0a0"
                    height: 24

                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.top: autoBackupTitleBackground.bottom
                    anchors.topMargin: 4

                    Rectangle {
                        id: autoBackupShortCheckboxBackground
                        color: "#FFf8f8f8"
                        height: parent.height-2

                        clip: true
                        anchors.left: parent.left
                        anchors.leftMargin: 1
                        anchors.right: parent.right
                        anchors.rightMargin: 1
                        anchors.verticalCenter: parent.verticalCenter

                        Checkbox {
                            id: autoBackupShortCheckboxId
                            width: autoBackupTitleBackground.width-12
                            anchors {
                                left: parent.left;
                                right: parent.right;
                                centerIn: parent;
                            }

                            checkboxText: "Backup re.sav and un.sav (Recommened)";
                            checked: settings.value("TimberAndStone/AutoBackupShort");
                            target: autoBackupShortCheckboxId

                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: { autoBackupShortCheckboxBackground.color = "#ffc8c8c8" }
                        onExited: { autoBackupShortCheckboxBackground.color = "#FFf8f8f8" }
                        onClicked: {
                            settings.setValue("TimberAndStone/AutoBackupShort", (! settings.value("TimberAndStone/AutoBackupShort")));
                            autoBackupShortCheckboxId.checked = settings.value("TimberAndStone/AutoBackupFull");
                        }
                    }
                }

                Rectangle {
                    id: autoBackupFullCheckboxOutline
                    color: "gray"
                    height: 24

                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.top: autoBackupCheckboxOutline.bottom
                    anchors.topMargin: 4

                    Rectangle {
                        id: autoBackupFullCheckboxBackground
                        color: "#FFf8f8f8"
                        height: parent.height-2

                        clip: true
                        anchors.left: parent.left
                        anchors.leftMargin: 1
                        anchors.right: parent.right
                        anchors.rightMargin: 1
                        anchors.verticalCenter: parent.verticalCenter

                        Checkbox {
                            id: autoBackupFullCheckboxId
                            width: autoBackupTitleBackground.width-12
                            anchors {
                                left: parent.left;
                                right: parent.right;
                                centerIn: parent;
                            }

                            checkboxText: "Backup full save game (Overkill)";
                            checked: settings.value("TimberAndStone/AutoBackupFull");
                            target: autoBackupFullCheckboxId
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: { autoBackupFullCheckboxBackground.color = "#ffc8c8c8" }
                            onExited: { autoBackupFullCheckboxBackground.color = "#FFf8f8f8" }
                            onClicked: {
                                settings.setValue("TimberAndStone/AutoBackupFull", (! settings.value("TimberAndStone/AutoBackupFull")));
                                autoBackupFullCheckboxId.checked = settings.value("TimberAndStone/AutoBackupFull");
                            }
                        }
                    }

                }
            }
        }
    }
}
