!define BASE_DIR "..\Release"
!define SRC_DIR "..\Revive"
!define DASH_DIR "..\ReviveOverlay"

Var InstallChoice
Var Dialog
Var RadioRevived
Var RadioRevive
Var RadioCustom
Var CustomPathText

Function .onInit
!ifdef NIGHTLY
    MessageBox MB_YESNO "Looks like you downloaded a beta release, these releases are unstable and only intended for experimentation.$\r$\n$\r$\nDo you want to install a stable release instead?" IDYES nightly_abort IDNO nightly_continue
    nightly_abort:
    ExecShell "open" "https://github.com/cfm-miku-en/Revived/releases/latest"
    Abort
    nightly_continue:
!endif
    StrCpy $INSTDIR "$PROGRAMFILES64\Revived"
    StrCpy $InstallChoice "revived"
FunctionEnd

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "nsDialogs.nsh"
  !include "LogicLib.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Revived"
  OutFile "RevivedInstaller.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\Revived"

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "..\LICENSE"
  
  Page custom InstallChoicePage InstallChoicePageLeave
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Revived"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "Revived"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Custom install-location page

Function InstallChoicePage
  !insertmacro MUI_HEADER_TEXT "Choose Install Location" "Choose where to install Revived."
  
  nsDialogs::Create 1018
  Pop $Dialog
  ${If} $Dialog == error
    Abort
  ${EndIf}
  
  ${NSD_CreateLabel} 0 0 100% 24u "Select an install option:"
  Pop $0
  
  ${NSD_CreateRadioButton} 0 30u 100% 12u "Revived (fresh install to $PROGRAMFILES64\Revived)"
  Pop $RadioRevived
  
  ${NSD_CreateRadioButton} 0 46u 100% 12u "Revive (upgrade existing install; keeps hook.cmd registrations and app paths)"
  Pop $RadioRevive
  
  ${NSD_CreateRadioButton} 0 62u 100% 12u "Custom location"
  Pop $RadioCustom
  
  ${NSD_CreateText} 10u 78u 70% 12u "$PROGRAMFILES64\Revived"
  Pop $CustomPathText
  
  ${If} $InstallChoice == "revive"
    ${NSD_Check} $RadioRevive
  ${ElseIf} $InstallChoice == "custom"
    ${NSD_Check} $RadioCustom
  ${Else}
    ${NSD_Check} $RadioRevived
  ${EndIf}
  
  nsDialogs::Show
FunctionEnd

Function InstallChoicePageLeave
  ${NSD_GetState} $RadioRevived $0
  ${If} $0 == ${BST_CHECKED}
    StrCpy $InstallChoice "revived"
    StrCpy $INSTDIR "$PROGRAMFILES64\Revived"
    Return
  ${EndIf}
  
  ${NSD_GetState} $RadioRevive $0
  ${If} $0 == ${BST_CHECKED}
    StrCpy $InstallChoice "revive"
    ReadRegStr $R0 HKLM "Software\Revive" ""
    ${If} $R0 != ""
      StrCpy $INSTDIR "$R0"
    ${Else}
      StrCpy $INSTDIR "$PROGRAMFILES64\Revive"
    ${EndIf}
    Return
  ${EndIf}
  
  ${NSD_GetState} $RadioCustom $0
  ${If} $0 == ${BST_CHECKED}
    StrCpy $InstallChoice "custom"
    ${NSD_GetText} $CustomPathText $INSTDIR
    ${If} $INSTDIR == ""
      MessageBox MB_OK "Please enter a custom install path."
      Abort
    ${EndIf}
  ${EndIf}
FunctionEnd

;--------------------------------
;Installer Sections

Section "Revive" SecRevive
  DetailPrint "Based on LibreVR/Revive by CrossVR and contributors - https://github.com/LibreVR/Revive"
IfSilent install
  DetailPrint "Terminating dashboard overlay..."
  nsExec::ExecToLog '"taskkill" /F /IM ReviveOverlay.exe'
  Sleep 2000 ; give 2 seconds for the application to finish exiting
  
install:
  SectionIn RO
  
  ; If the directory already exists, use a subfolder
  IfFileExists $INSTDIR\ReviveOverlay.exe +3 0
  IfFileExists $INSTDIR\*.* 0 +2
  StrCpy $INSTDIR "$INSTDIR\Revived"
  
  SetOutPath "$INSTDIR"
  
  ; Main application files
  File "..\LICENSE"
  File "..\hook.cmd"
  File "${DASH_DIR}\app.vrmanifest"
  File "${DASH_DIR}\support.vrmanifest"
  File /r "${BASE_DIR}\*.exe"
  File /r /x "${BASE_DIR}\bearer" "${BASE_DIR}\*.dll"
  File /r "${BASE_DIR}\*.jpg"
  File /r "${BASE_DIR}\Qt*"
  
  SetOutPath "$INSTDIR\Input"
  
  ; Application data
  File /r "${SRC_DIR}\Input\*.json"
  
  ; Create an empty manifest file
  FileOpen $0 "$INSTDIR\revive.vrmanifest" w
  FileWrite $0 ""
  FileClose $0
  
  ; Delete any bearer files already in the install directory
  ; This is a workaround for QTBUG-40332
  RMDir /r "$INSTDIR\bearer"

  ; Delete the Revive local data
  RMDir /r "$LocalAppdata\Revive"

  ; Delete the old Revive injector
  RMDir /r "$INSTDIR\Revive"

  ; Delete the old XInput proxy
  RMDir /r "$INSTDIR\xinput"

  ; Delete outdated dependencies
  Delete "$INSTDIR\LibRXRRT64.dll"
  Delete "$INSTDIR\openxr_loader.dll"
  Delete "$INSTDIR\openxr_loader-1_0.dll"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\ssleay32.dll"

  ; Ensure all users have access to the manifest file
  AccessControl::GrantOnFile \
    "$INSTDIR\revive.vrmanifest" "(S-1-5-32-545)" "GenericRead + GenericWrite"
  Pop $0
  
  ; Install redistributable
  ExecWait '"$INSTDIR\vcredist_x64.exe" /install /quiet'
  
  ; Execute the dashboard to add the application manifest
  ExecWait '"$INSTDIR\ReviveOverlay.exe" -manifest'
  
  ;Store installation folder
  WriteRegStr HKLM "Software\Revive" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  ; Add uninstaller to Programs and Features
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Revive" \
                   "DisplayName" "Revived Dashboard"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Revive" \
                   "UninstallString" "$\"$INSTDIR\Uninstall.exe$\""
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Revive" \
                   "DisplayIcon" "$INSTDIR\ReviveOverlay.exe,0"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ; Use "Revive" for upgrade choice, "Revived" otherwise
    ${If} $InstallChoice == "revive"
      StrCpy $StartMenuFolder "Revive"
    ${EndIf}
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Revived Dashboard.lnk" "$INSTDIR\ReviveOverlay.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END

  ; Execute the dashboard as the user
  Exec '"$WINDIR\explorer.exe" "$INSTDIR\ReviveOverlay.exe"'
SectionEnd
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  DetailPrint "Terminating dashboard overlay..."
  nsExec::ExecToLog '"taskkill" /F /IM ReviveOverlay.exe'
  Sleep 2000 ; give 2 seconds for the application to finish exiting

  RMDir /r "$INSTDIR"

  ; Delete the Revive local data
  RMDir /r "$LocalAppdata\Revive"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Revived Dashboard.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  DeleteRegKey HKLM "Software\Revive"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Revive"

SectionEnd
