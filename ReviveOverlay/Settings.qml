import QtQuick 2.4

Rectangle {
    id: settingsPage
    anchors.fill: parent
    color: "#0E1A2B"

    signal closed()

    property string errorMessage: ""

    property string previewRuntime:    Settings.runtimePreference === 1 ? "OpenXR" : "OpenVR"
    property string previewMetaPath:   Settings.metaHorizonPath
    property string previewLibPath:    Settings.gamesLibraryPath
    property string previewAccent:     Settings.accentColor
    property string previewBorder:     Settings.borderColor
    property string previewBackground: Settings.backgroundColor

    property color textPrimary:   "#FFFFFF"
    property color textSecondary: "#B4DFF7"
    property color fieldFill:     "#0D1A2B"

    property var accentSwatches: [
        "#F74C4C", "#F79C4C", "#F7F04C", "#4CF77C",
        "#1CC4F7", "#1C7AF7", "#7C1CF7", "#F71CA8", "#1CC4F7"
    ]
    property var borderSwatches: [
        "#FFFFFF", "#E4E4E4", "#B4DFF7",
        "#F7C4E0", "#C4F7D8", "#F7E0A4"
    ]
    property var backgroundSwatches: [
        "#000000", "#0E1A2B", "#101820",
        "#1A1A1A", "#1A1028", "#102018"
    ]

    function syncPreviewsFromController() {
        previewRuntime    = Settings.runtimePreference === 1 ? "OpenXR" : "OpenVR"
        previewMetaPath   = Settings.metaHorizonPath
        previewLibPath    = Settings.gamesLibraryPath
        previewAccent     = Settings.accentColor
        previewBorder     = Settings.borderColor
        previewBackground = Settings.backgroundColor
    }

    Text {
        x: 80; y: 40
        color: settingsPage.textPrimary
        font.pixelSize: 48
        text: "Settings"
    }

    Text {
        x: 80; y: 130
        color: settingsPage.textPrimary
        font.pixelSize: 28
        text: "Runtime"
    }

    Row {
        x: 80; y: 176; spacing: 16
        Rectangle {
            width: 32; height: 32; radius: 16
            border.color: previewAccent; border.width: 3; color: "transparent"
            Rectangle {
                width: 18; height: 18; radius: 9
                anchors.centerIn: parent
                color: previewAccent
                visible: previewRuntime === "OpenVR"
            }
            MouseArea { anchors.fill: parent; onClicked: previewRuntime = "OpenVR" }
        }
        Text {
            text: "OpenVR"; color: settingsPage.textPrimary; font.pixelSize: 26
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row {
        x: 80; y: 224; spacing: 16
        Rectangle {
            width: 32; height: 32; radius: 16
            border.color: previewAccent; border.width: 3; color: "transparent"
            Rectangle {
                width: 18; height: 18; radius: 9
                anchors.centerIn: parent
                color: previewAccent
                visible: previewRuntime === "OpenXR"
            }
            MouseArea { anchors.fill: parent; onClicked: previewRuntime = "OpenXR" }
        }
        Text {
            text: "OpenXR"; color: settingsPage.textPrimary; font.pixelSize: 26
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Text {
        x: 80; y: 296
        color: settingsPage.textPrimary
        font.pixelSize: 28
        text: "Paths"
    }

    Text { x: 80; y: 340; color: settingsPage.textSecondary; font.pixelSize: 22; text: "Meta Horizon install" }

    Rectangle {
        x: 80; y: 370; width: 1400; height: 44
        color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
        TextInput {
            id: basePathInput
            anchors.fill: parent; anchors.margins: 8
            color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
            text: previewMetaPath
            onTextChanged: previewMetaPath = text
        }
    }

    Rectangle {
        x: 1496; y: 370; width: 180; height: 44
        color: previewAccent; radius: 4
        Text { anchors.centerIn: parent; text: "Browse…"; color: "#0E1A2B"; font.pixelSize: 20 }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                var p = Settings.browseForFolder(previewMetaPath)
                if (p.length > 0) {
                    previewMetaPath = p
                    basePathInput.text = p
                }
            }
        }
    }

    Text { x: 80; y: 428; color: settingsPage.textSecondary; font.pixelSize: 22; text: "Games library" }

    Rectangle {
        x: 80; y: 458; width: 1400; height: 44
        color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
        TextInput {
            id: libPathInput
            anchors.fill: parent; anchors.margins: 8
            color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
            text: previewLibPath
            onTextChanged: previewLibPath = text
        }
    }

    Rectangle {
        x: 1496; y: 458; width: 180; height: 44
        color: previewAccent; radius: 4
        Text { anchors.centerIn: parent; text: "Browse…"; color: "#0E1A2B"; font.pixelSize: 20 }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                var p = Settings.browseForFolder(previewLibPath)
                if (p.length > 0) {
                    previewLibPath = p
                    libPathInput.text = p
                }
            }
        }
    }

    Text {
        x: 80; y: 528
        color: settingsPage.textPrimary
        font.pixelSize: 28
        text: "Appearance"
    }

    Item {
        id: accentRow
        x: 80; y: 574; width: 1760; height: 88

        Text { x: 0; y: 4; color: settingsPage.textSecondary; font.pixelSize: 22; text: "Accent" }

        Row {
            x: 0; y: 36; spacing: 12
            Repeater {
                model: accentSwatches
                Rectangle {
                    width: 44; height: 44; radius: 4
                    color: modelData
                    border.color: "#FFFFFF"
                    border.width: previewAccent.toLowerCase() === modelData.toLowerCase() ? 3 : 0
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { previewAccent = modelData; accentHex.text = modelData }
                    }
                }
            }
        }

        Rectangle {
            x: 580; y: 36; width: 220; height: 44
            color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
            TextInput {
                id: accentHex
                anchors.fill: parent; anchors.margins: 8
                color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
                text: previewAccent
                onEditingFinished: {
                    var n = Settings.normalizeHex(text)
                    if (n.length > 0) {
                        previewAccent = n
                        text = n
                        settingsPage.errorMessage = ""
                    } else {
                        settingsPage.errorMessage = "\"" + text + "\" is not a valid hex color."
                        text = previewAccent
                    }
                }
            }
        }

        Rectangle {
            x: 820; y: 36; width: 44; height: 44; radius: 4
            color: previewAccent
            border.color: "#FFFFFF"; border.width: 1
        }
    }

    Item {
        id: borderRow
        x: 80; y: 674; width: 1760; height: 88

        Text { x: 0; y: 4; color: settingsPage.textSecondary; font.pixelSize: 22; text: "Border" }

        Row {
            x: 0; y: 36; spacing: 12
            Repeater {
                model: borderSwatches
                Rectangle {
                    width: 44; height: 44; radius: 4
                    color: modelData
                    border.color: "#FFFFFF"
                    border.width: previewBorder.toLowerCase() === modelData.toLowerCase() ? 3 : 0
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { previewBorder = modelData; borderHex.text = modelData }
                    }
                }
            }
        }

        Rectangle {
            x: 580; y: 36; width: 220; height: 44
            color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
            TextInput {
                id: borderHex
                anchors.fill: parent; anchors.margins: 8
                color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
                text: previewBorder
                onEditingFinished: {
                    var n = Settings.normalizeHex(text)
                    if (n.length > 0) {
                        previewBorder = n
                        text = n
                        settingsPage.errorMessage = ""
                    } else {
                        settingsPage.errorMessage = "\"" + text + "\" is not a valid hex color."
                        text = previewBorder
                    }
                }
            }
        }

        Rectangle {
            x: 820; y: 36; width: 44; height: 44; radius: 4
            color: previewBorder
            border.color: "#FFFFFF"; border.width: 1
        }
    }

    Item {
        id: backgroundRow
        x: 80; y: 774; width: 1760; height: 88

        Text { x: 0; y: 4; color: settingsPage.textSecondary; font.pixelSize: 22; text: "Background" }

        Row {
            x: 0; y: 36; spacing: 12
            Repeater {
                model: backgroundSwatches
                Rectangle {
                    width: 44; height: 44; radius: 4
                    color: modelData
                    border.color: "#FFFFFF"
                    border.width: previewBackground.toLowerCase() === modelData.toLowerCase() ? 3 : 0
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { previewBackground = modelData; backgroundHex.text = modelData }
                    }
                }
            }
        }

        Rectangle {
            x: 580; y: 36; width: 220; height: 44
            color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
            TextInput {
                id: backgroundHex
                anchors.fill: parent; anchors.margins: 8
                color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
                text: previewBackground
                onEditingFinished: {
                    var n = Settings.normalizeHex(text)
                    if (n.length > 0) {
                        previewBackground = n
                        text = n
                        settingsPage.errorMessage = ""
                    } else {
                        settingsPage.errorMessage = "\"" + text + "\" is not a valid hex color."
                        text = previewBackground
                    }
                }
            }
        }

        Rectangle {
            x: 820; y: 36; width: 44; height: 44; radius: 4
            color: previewBackground
            border.color: "#FFFFFF"; border.width: 1
        }
    }

    Text {
        x: 80; y: 890
        color: "#F74C4C"
        font.pixelSize: 22
        text: settingsPage.errorMessage
        visible: settingsPage.errorMessage.length > 0
    }

    Row {
        x: 80; y: 960; spacing: 24

        Rectangle {
            width: 200; height: 60; radius: 4; color: previewAccent
            Text { anchors.centerIn: parent; text: "Apply"; color: "#0E1A2B"; font.pixelSize: 26 }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Settings.setRuntimePreference(previewRuntime === "OpenXR" ? 1 : 0)
                    Settings.setMetaHorizonPath(previewMetaPath)
                    Settings.setGamesLibraryPath(previewLibPath)
                    Settings.setAccentColor(previewAccent)
                    Settings.setBorderColor(previewBorder)
                    Settings.setBackgroundColor(previewBackground)
                    var err = Settings.apply()
                    if (err.length > 0) {
                        settingsPage.errorMessage = err
                    } else {
                        settingsPage.errorMessage = ""
                        settingsPage.closed()
                    }
                }
            }
        }

        Rectangle {
            width: 200; height: 60; radius: 4; color: "transparent"
            border.color: previewBorder; border.width: 2
            Text { anchors.centerIn: parent; text: "Cancel"; color: settingsPage.textPrimary; font.pixelSize: 26 }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Settings.cancel()
                    settingsPage.errorMessage = ""
                    settingsPage.closed()
                }
            }
        }

        Rectangle {
            width: 300; height: 60; radius: 4; color: "transparent"
            border.color: previewBorder; border.width: 2
            Text { anchors.centerIn: parent; text: "Restore Defaults"; color: settingsPage.textPrimary; font.pixelSize: 26 }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Settings.restoreDefaults()
                    syncPreviewsFromController()
                    basePathInput.text = previewMetaPath
                    libPathInput.text = previewLibPath
                    accentHex.text = previewAccent
                    borderHex.text = previewBorder
                    backgroundHex.text = previewBackground
                    settingsPage.errorMessage = ""
                }
            }
        }
    }
}
