# Wii Bad Apple Player
**Created by Axolat**

An optimized Nintendo Wii Homebrew to play the famous "Bad Apple" animation. This version uses RAM buffering to prevent SD card read conflicts and "ISI Exception" crashes.

## 📥 Installation
1. Download the latest `BadApple_Wii.zip` from the [Releases](https://github.com/YOUR_USERNAME/YOUR_REPO/releases) section.
2. Extract the ZIP file to the root of your SD card.
3. Your SD card should now look like this:
   - `sd:/badapple/` (Contains `audio.mp3` and 6500 images)
   - `sd:/apps/badapple/` (Contains `boot.dol`, `meta.xml`, and `icon.png`)
4. Launch the app via the **Homebrew Channel**.

## 🛠️ Build Requirements
If you want to compile the project yourself:
- **devkitPro** with the **devkitPPC** toolchain.
- **GRRLIB Library**: Essential for the graphics engine.
  - Source: [https://github.com/GRRLIB/GRRLIB](https://github.com/GRRLIB/GRRLIB)

## 🎮 Controls
- **(A)**: Video Only (Safe Mode).
- **(B)**: Video + Audio (Sync Mode - Recommended).
- **(2)**: Audio Only (Diagnostic Mode).
- **(HOME)**: Exit to Homebrew Channel.
