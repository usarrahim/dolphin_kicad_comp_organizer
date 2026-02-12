; NSIS Installer Script for KiCad Component Organizer
; Generated for dolphin_kicad_comp_organizer

!define APP_NAME "KiCad Component Organizer"
!define APP_VERSION "1.0.0"
!define APP_PUBLISHER "dolphin_kicad_comp_organizer"
!define APP_EXE "dolphin_kicad_comp_organizer.exe"
!define APP_UNINST "Uninstall.exe"
!define INSTALL_DIR "$PROGRAMFILES\${APP_PUBLISHER}"

; Modern UI
!include "MUI2.nsh"

; Installer attributes
Name "${APP_NAME}"
OutFile "dolphin_kicad_comp_organizer_setup.exe"
InstallDir "${INSTALL_DIR}"
RequestExecutionLevel admin

; Interface Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Languages
!insertmacro MUI_LANGUAGE "English"

; Installer sections
Section "MainSection" SEC01
    SetOutPath "$INSTDIR"
    File "build\windows\x64\release\${APP_EXE}"
    
    ; Create uninstaller
    WriteUninstaller "$INSTDIR\${APP_UNINST}"
    
    ; Create Start Menu shortcuts
    CreateDirectory "$SMPROGRAMS\${APP_PUBLISHER}"
    CreateShortcut "$SMPROGRAMS\${APP_PUBLISHER}\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}"
    CreateShortcut "$SMPROGRAMS\${APP_PUBLISHER}\Uninstall.lnk" "$INSTDIR\${APP_UNINST}"
    
    ; Create Desktop shortcut
    CreateShortcut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}"
    
    ; Registry entries
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_PUBLISHER}" "DisplayName" "${APP_NAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_PUBLISHER}" "UninstallString" "$INSTDIR\${APP_UNINST}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_PUBLISHER}" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_PUBLISHER}" "DisplayVersion" "${APP_VERSION}"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_PUBLISHER}" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_PUBLISHER}" "NoRepair" 1
SectionEnd

; Uninstaller section
Section "Uninstall"
    Delete "$INSTDIR\${APP_EXE}"
    Delete "$INSTDIR\${APP_UNINST}"
    
    ; Remove shortcuts
    Delete "$SMPROGRAMS\${APP_PUBLISHER}\${APP_NAME}.lnk"
    Delete "$SMPROGRAMS\${APP_PUBLISHER}\Uninstall.lnk"
    RMDir "$SMPROGRAMS\${APP_PUBLISHER}"
    Delete "$DESKTOP\${APP_NAME}.lnk"
    
    ; Remove registry entries
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_PUBLISHER}"
    
    ; Remove installation directory
    RMDir "$INSTDIR"
SectionEnd
