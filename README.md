# KiCad Component Organizer

![App Screenshot](app_ss.png)

**KiCad Component Organizer** is a simple Windows application that helps you organize KiCad component files from ZIP archives into your project folders automatically.

## What does it do?

Have you ever downloaded a ZIP file full of KiCad components and spent time manually sorting them into the right folders? This tool does it for you!

Simply:
1. Select your KiCad project folder
2. Drag and drop your ZIP file
3. Watch as it automatically organizes everything

## Features

✨ **Drag & Drop** - Just drag your ZIP file onto the window  
📁 **Smart Organization** - Automatically sorts files by type:
   - Footprints (`.kicad_mod`) → `{ProjectName}.pretty`
   - Symbols (`.kicad_sym`) → `{ProjectName}_Symbols`
   - 3D Models (`.step`, `.stp`) → `{ProjectName}_Models`  
🎯 **Easy to Use** - No complicated settings, just point and click  
⚡ **Fast** - Processes files quickly and efficiently

## Download & Install

1. Download `dolphin_kicad_comp_organizer_setup.exe` from the [Releases](https://github.com/yourusername/dolphin_kicad_comp_organizer/releases) page
2. Run the installer
3. Follow the installation wizard
4. Launch the application from your Start Menu or Desktop

## How to Use

### Step 1: Select Your Project Folder
Click the **"Browse..."** button and navigate to your KiCad project folder, or type the path directly in the text box.

### Step 2: Drop Your ZIP File
Drag and drop your ZIP file containing KiCad components onto the application window.

### Step 3: Done!
The application will automatically:
- Extract the ZIP file
- Sort files by their extension
- Copy them to the correct folders in your project directory

That's it! Your components are now organized and ready to use in KiCad.

## Example

If your project folder is `C:\Projects\MyKiCadProject` and you drop a ZIP file containing:
- `resistor.kicad_mod`
- `capacitor.kicad_sym`
- `component.step`

The application will create:
```
MyKiCadProject/
├── MyKiCadProject.pretty/
│   └── resistor.kicad_mod
├── MyKiCadProject_Symbols/
│   └── capacitor.kicad_sym
└── MyKiCadProject_Models/
    └── component.step
```

## Requirements

- **Windows 10** or later
- A KiCad project folder

## Troubleshooting

**"Error: Please select a ZIP file!"**
- Make sure you're dropping a `.zip` file, not another archive format

**"Error: Please select a project path first!"**
- Click "Browse..." and select your KiCad project folder before dropping files

**Files not appearing in KiCad?**
- Make sure you've selected the correct project folder
- Check that KiCad is configured to look in the right directories for components

## Contributing

Found a bug or have a feature request? Please open an [issue](https://github.com/yourusername/dolphin_kicad_comp_organizer/issues)!

## License

[Add your license here]

---

Made with ❤️ for the KiCad community
