import QtQuick 2.4

Rectangle {
    id: settingsPage
    anchors.fill: parent
    color: "#183755"

    signal closed()

    property string errorMessage: ""

    property var accentSwatches: [
        "#f74c4c", "#f79c4c", "#f7f04c", "#4cf77c",
        "#1cc4f7", "#1c7af7", "#7c1cf7", "#f71ca8", "#1cc4f7"
    ]
    property var swatchLabels: [
        "red", "orange", "yellow", "green",
        "cyan", "blue", "purple", "pink", "default"
    ]

    // Title
    Text {
        x: 80; y: 40
        color: Settings.accentColor
        font.pixelSize: 48
        text: "Settings"
    }

    // ── Runtime ──────────────────────────────────────────────────────────────

    Text {
        x: 80; y: 140
        color: Settings.accentColor
        font.pixelSize: 28
        text: "Runtime"
    }

    Row {
        x: 80; y: 186; spacing: 16
        Rectangle {
            width: 32; height: 32; radius: 16
            border.color: Settings.accentColor; border.width: 3; color: "transparent"
            Rectangle {
                width: 18; height: 18; radius: 9
                anchors.centerIn: parent
                color: Settings.accentColor
                visible: Settings.runtimePreference === 0
            }
            MouseArea { anchors.fill: parent; onClicked: Settings.setRuntimePreference(0) }
        }
        Text {
            text: "OpenVR"; color: "white"; font.pixelSize: 26
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row {
        x: 80; y: 240; spacing: 16
        Rectangle {
            width: 32; height: 32; radius: 16
            border.color: Settings.accentColor; border.width: 3; color: "transparent"
            Rectangle {
                width: 18; height: 18; radius: 9
                anchors.centerIn: parent
                color: Settings.accentColor
                visible: Settings.runtimePreference === 1
            }
            MouseArea { anchors.fill: parent; onClicked: Settings.setRuntimePreference(1) }
        }
        Text {
            text: "OpenXR"; color: "white"; font.pixelSize: 26
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    // ── Paths ────────────────────────────────────────────────────────────────

    Text {
        x: 80; y: 320
        color: Settings.accentColor
        font.pixelSize: 28
        text: "Paths"
    }

    Text { x: 80; y: 366; color: "white"; font.pixelSize: 24; text: "Meta Horizon install" }

    Rectangle {
        x: 80; y: 398; width: 1400; height: 48
        color: "#0d2538"; border.color: Settings.accentColor; border.width: 1
        TextInput {
            id: basePathInput
            anchors.fill: parent; anchors.margins: 8
            color: "white"; font.pixelSize: 24; selectionColor: Settings.accentColor
            text: Settings.metaHorizonPath
            onTextChanged: Settings.setMetaHorizonPath(text)
        }
    }

    Rectangle {
        x: 1496; y: 398; width: 180; height: 48
        color: Settings.accentColor; radius: 4
        Text { anchors.centerIn: parent; text: "Browse\u2026"; color: "#183755"; font.pixelSize: 22 }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                var p = Settings.browseForFolder(Settings.metaHorizonPath)
                if (p.length > 0)
                    Settings.setMetaHorizonPath(p)
            }
        }
    }

    Text { x: 80; y: 466; color: "white"; font.pixelSize: 24; text: "Games library" }

    Rectangle {
        x: 80; y: 498; width: 1400; height: 48
        color: "#0d2538"; border.color: Settings.accentColor; border.width: 1
        TextInput {
            id: libraryPathInput
            anchors.fill: parent; anchors.margins: 8
            color: "white"; font.pixelSize: 24; selectionColor: Settings.accentColor
            text: Settings.gamesLibraryPath
            onTextChanged: Settings.setGamesLibraryPath(text)
        }
    }

    Rectangle {
        x: 1496; y: 498; width: 180; height: 48
        color: Settings.accentColor; radius: 4
        Text { anchors.centerIn: parent; text: "Browse\u2026"; color: "#183755"; font.pixelSize: 22 }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                var p = Settings.browseForFolder(Settings.gamesLibraryPath)
                if (p.length > 0)
                    Settings.setGamesLibraryPath(p)
            }
        }
    }

    // ── Appearance ───────────────────────────────────────────────────────────

    Text {
        x: 80; y: 600
        color: Settings.accentColor
        font.pixelSize: 28
        text: "Appearance"
    }

    Row {
        id: swatchRow
        x: 80; y: 646; spacing: 14

        Repeater {
            model: settingsPage.accentSwatches.length
            Item {
                width: 68; height: 80
                property string swatchColor: settingsPage.accentSwatches[index]

                Rectangle {
                    width: 60; height: 60; radius: 6
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: parent.swatchColor
                    border.color: "white"
                    border.width: Settings.accentColor === parent.parent.swatchColor ? 3 : 0

                    MouseArea {
                        anchors.fill: parent
                        onClicked: Settings.setAccentColor(parent.parent.swatchColor)
                    }
                }

                Text {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: settingsPage.swatchLabels[index]
                    color: "white"; font.pixelSize: 16
                }
            }
        }
    }

    Text { x: 80; y: 760; color: "white"; font.pixelSize: 24; text: "Custom hex" }

    Rectangle {
        x: 80; y: 792; width: 260; height: 48
        color: "#0d2538"; border.color: Settings.accentColor; border.width: 1
        TextInput {
            anchors.fill: parent; anchors.margins: 8
            color: "white"; font.pixelSize: 24; selectionColor: Settings.accentColor
            text: Settings.accentColor
            onTextChanged: {
                if (/^#[0-9a-fA-F]{6}$/.test(text))
                    Settings.setAccentColor(text)
            }
        }
    }

    Rectangle {
        x: 356; y: 792; width: 56; height: 48; radius: 4
        color: Settings.accentColor
    }

    // ── Error ────────────────────────────────────────────────────────────────

    Text {
        x: 80; y: 900
        color: "#f74c4c"
        font.pixelSize: 26
        text: settingsPage.errorMessage
        visible: settingsPage.errorMessage.length > 0
    }

    // ── Buttons ──────────────────────────────────────────────────────────────

    Row {
        x: 80; y: 968; spacing: 24

        Rectangle {
            width: 200; height: 64; radius: 4; color: Settings.accentColor
            Text { anchors.centerIn: parent; text: "Apply"; color: "#183755"; font.pixelSize: 28 }
            MouseArea {
                anchors.fill: parent
                onClicked: settingsPage.errorMessage = Settings.apply()
            }
        }

        Rectangle {
            width: 200; height: 64; radius: 4; color: "transparent"
            border.color: Settings.accentColor; border.width: 2
            Text { anchors.centerIn: parent; text: "Cancel"; color: Settings.accentColor; font.pixelSize: 28 }
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
            width: 320; height: 64; radius: 4; color: "transparent"
            border.color: Settings.accentColor; border.width: 2
            Text { anchors.centerIn: parent; text: "Restore Defaults"; color: Settings.accentColor; font.pixelSize: 28 }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Settings.restoreDefaults()
                    settingsPage.errorMessage = ""
                }
            }
        }
    }
}
