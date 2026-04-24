import QtQuick 2.4
import QtQuick.Dialogs 1.3

Rectangle {
    id: settingsPage
    anchors.fill: parent
    color: previewSettingsBackground

    signal closed()

    function urlToPath(url) {
        var s = url.toString()
        if (s.indexOf("file:///") === 0)
            s = s.substring(8)
        else if (s.indexOf("file://") === 0)
            s = s.substring(7)
        return decodeURIComponent(s).replace(/\//g, '\\')
    }

    function pathToUrl(path) {
        if (!path || path.length === 0)
            return ""
        return "file:///" + encodeURI(path.replace(/\\/g, '/'))
    }

    FileDialog {
        id: metaFolderDialog
        title: "Select Meta Horizon install folder"
        selectFolder: true
        selectMultiple: false
        folder: settingsPage.pathToUrl(previewMetaPath)
        onAccepted: {
            var p = settingsPage.urlToPath(metaFolderDialog.fileUrl)
            previewMetaPath = p
            basePathInput.text = p
        }
    }

    FileDialog {
        id: libFolderDialog
        title: "Select games library folder"
        selectFolder: true
        selectMultiple: false
        folder: settingsPage.pathToUrl(previewLibPath)
        onAccepted: {
            var p = settingsPage.urlToPath(libFolderDialog.fileUrl)
            previewLibPath = p
            libPathInput.text = p
        }
    }

    property string errorMessage: ""

    property string previewRuntime:             AppSettings.runtimePreference === 1 ? "OpenXR" : "OpenVR"
    property string previewMetaPath:            AppSettings.metaHorizonPath.length > 0 ? AppSettings.metaHorizonPath : AppSettings.detectedMetaHorizonPath()
    property string previewLibPath:             AppSettings.gamesLibraryPath.length > 0 ? AppSettings.gamesLibraryPath : AppSettings.detectedGamesLibraryPath()
    property string previewAccent:              AppSettings.accentColor
    property string previewBorder:              AppSettings.borderColor
    property string previewSettingsBackground:  AppSettings.settingsBackgroundColor
    property string previewDashboardBackground: AppSettings.dashboardBackgroundColor

    property color textPrimary:   "#FFFFFF"
    property color textSecondary: "#B4DFF7"
    property color fieldFill:     "#0D1A2B"

    property var accentSwatches: [
        "#F74C4C", "#F79C4C", "#F7F04C", "#4CF77C",
        "#1CC4F7", "#1C7AF7", "#7C1CF7", "#F71CA8"
    ]
    property var borderSwatches: [
        "#FFFFFF", "#E4E4E4", "#B4DFF7",
        "#F7C4E0", "#C4F7D8", "#F7E0A4"
    ]
    property var backgroundSwatches: [
        "#000000", "#0E1A2B", "#101820",
        "#183755", "#1A1A1A", "#1A1028", "#102018"
    ]

    function syncPreviewsFromController() {
        previewRuntime             = AppSettings.runtimePreference === 1 ? "OpenXR" : "OpenVR"
        previewMetaPath            = AppSettings.metaHorizonPath.length > 0 ? AppSettings.metaHorizonPath : AppSettings.detectedMetaHorizonPath()
        previewLibPath             = AppSettings.gamesLibraryPath.length > 0 ? AppSettings.gamesLibraryPath : AppSettings.detectedGamesLibraryPath()
        previewAccent              = AppSettings.accentColor
        previewBorder              = AppSettings.borderColor
        previewSettingsBackground  = AppSettings.settingsBackgroundColor
        previewDashboardBackground = AppSettings.dashboardBackgroundColor
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
            onClicked: metaFolderDialog.open()
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
            onClicked: libFolderDialog.open()
        }
    }

    Text {
        x: 80; y: 520
        color: settingsPage.textPrimary
        font.pixelSize: 28
        text: "Appearance"
    }

    Item {
        id: accentRow
        x: 80; y: 566; width: 1760; height: 88

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
            x: 680; y: 36; width: 220; height: 44
            color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
            TextInput {
                id: accentHex
                anchors.fill: parent; anchors.margins: 8
                color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
                text: previewAccent
                onEditingFinished: {
                    var n = AppSettings.normalizeHex(text)
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
            x: 920; y: 36; width: 44; height: 44; radius: 4
            color: previewAccent
            border.color: "#FFFFFF"; border.width: 1
        }
    }

    Item {
        id: borderRow
        x: 80; y: 646; width: 1760; height: 88

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
            x: 680; y: 36; width: 220; height: 44
            color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
            TextInput {
                id: borderHex
                anchors.fill: parent; anchors.margins: 8
                color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
                text: previewBorder
                onEditingFinished: {
                    var n = AppSettings.normalizeHex(text)
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
            x: 920; y: 36; width: 44; height: 44; radius: 4
            color: previewBorder
            border.color: "#FFFFFF"; border.width: 1
        }
    }

    Item {
        id: settingsBackgroundRow
        x: 80; y: 726; width: 1760; height: 88

        Text { x: 0; y: 4; color: settingsPage.textSecondary; font.pixelSize: 22; text: "Settings background" }

        Row {
            x: 0; y: 36; spacing: 12
            Repeater {
                model: backgroundSwatches
                Rectangle {
                    width: 44; height: 44; radius: 4
                    color: modelData
                    border.color: "#FFFFFF"
                    border.width: previewSettingsBackground.toLowerCase() === modelData.toLowerCase() ? 3 : 0
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { previewSettingsBackground = modelData; settingsBackgroundHex.text = modelData }
                    }
                }
            }
        }

        Rectangle {
            x: 680; y: 36; width: 220; height: 44
            color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
            TextInput {
                id: settingsBackgroundHex
                anchors.fill: parent; anchors.margins: 8
                color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
                text: previewSettingsBackground
                onEditingFinished: {
                    var n = AppSettings.normalizeHex(text)
                    if (n.length > 0) {
                        previewSettingsBackground = n
                        text = n
                        settingsPage.errorMessage = ""
                    } else {
                        settingsPage.errorMessage = "\"" + text + "\" is not a valid hex color."
                        text = previewSettingsBackground
                    }
                }
            }
        }

        Rectangle {
            x: 920; y: 36; width: 44; height: 44; radius: 4
            color: previewSettingsBackground
            border.color: "#FFFFFF"; border.width: 1
        }
    }

    Item {
        id: dashboardBackgroundRow
        x: 80; y: 806; width: 1760; height: 88

        Text { x: 0; y: 4; color: settingsPage.textSecondary; font.pixelSize: 22; text: "Dashboard background" }

        Row {
            x: 0; y: 36; spacing: 12
            Repeater {
                model: backgroundSwatches
                Rectangle {
                    width: 44; height: 44; radius: 4
                    color: modelData
                    border.color: "#FFFFFF"
                    border.width: previewDashboardBackground.toLowerCase() === modelData.toLowerCase() ? 3 : 0
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { previewDashboardBackground = modelData; dashboardBackgroundHex.text = modelData }
                    }
                }
            }
        }

        Rectangle {
            x: 680; y: 36; width: 220; height: 44
            color: settingsPage.fieldFill; border.color: previewBorder; border.width: 1
            TextInput {
                id: dashboardBackgroundHex
                anchors.fill: parent; anchors.margins: 8
                color: settingsPage.textPrimary; font.pixelSize: 22; selectionColor: previewAccent
                text: previewDashboardBackground
                onEditingFinished: {
                    var n = AppSettings.normalizeHex(text)
                    if (n.length > 0) {
                        previewDashboardBackground = n
                        text = n
                        settingsPage.errorMessage = ""
                    } else {
                        settingsPage.errorMessage = "\"" + text + "\" is not a valid hex color."
                        text = previewDashboardBackground
                    }
                }
            }
        }

        Rectangle {
            x: 920; y: 36; width: 44; height: 44; radius: 4
            color: previewDashboardBackground
            border.color: "#FFFFFF"; border.width: 1
        }
    }

    Text {
        x: 80; y: 910
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
                    AppSettings.setRuntimePreference(previewRuntime === "OpenXR" ? 1 : 0)
                    AppSettings.setMetaHorizonPath(previewMetaPath)
                    AppSettings.setGamesLibraryPath(previewLibPath)
                    AppSettings.setAccentColor(previewAccent)
                    AppSettings.setBorderColor(previewBorder)
                    AppSettings.setSettingsBackgroundColor(previewSettingsBackground)
                    AppSettings.setDashboardBackgroundColor(previewDashboardBackground)
                    var err = AppSettings.apply()
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
                    AppSettings.cancel()
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
                    AppSettings.restoreDefaults()
                    syncPreviewsFromController()
                    basePathInput.text = previewMetaPath
                    libPathInput.text = previewLibPath
                    accentHex.text = previewAccent
                    borderHex.text = previewBorder
                    settingsBackgroundHex.text = previewSettingsBackground
                    dashboardBackgroundHex.text = previewDashboardBackground
                    settingsPage.errorMessage = ""
                }
            }
        }
    }
}
