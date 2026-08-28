# fltgst

**fltgst** (Flutter + GStreamer) is a cross-platform Flutter project demonstrating how to integrate the **GStreamer** multimedia framework using Dart FFI. It allows you to leverage GStreamer's powerful pipeline capabilities for audio playback, streaming, and processing directly within a Flutter application.

## 🚀 Key Features

* **Cross-Platform Support**: Built to work on Android, iOS, Windows, macOS, and Linux.
* **Dart FFI Integration**: Native C/C++ bindings for high-performance GStreamer pipeline management.
* **Custom Pipelines**: Flexible architecture to define and run GStreamer pipelines from within Flutter.
* **Comprehensive Tutorials**: This repository serves as the companion code for a detailed multi-part blog series on GStreamer + Flutter integration.

## 📦 Prerequisites

Before building the project, ensure you have the following installed on your development machine:

* **Flutter SDK**: [Install Flutter](https://docs.flutter.dev/get-started/install)
* **GStreamer Binaries & Development Files**:
* **Windows**: Install via [GStreamer Website](https://gstreamer.freedesktop.org/download/) (Select MSVC 64-bit runtime and development packages).
* **macOS/iOS**: Install via Homebrew or the official `.pkg` files.
* **Linux**: `sudo apt install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly`.
* **Android**: Download the GStreamer Android binaries.


* **CMake**: (Version 3.5 or higher).

## 🛠️ Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/fengjiongmax/fltgst.git
cd fltgst

```

### 2. Install dependencies

```bash
flutter pub get

```

### 3. Build & Run

Ensure your GStreamer environment variables are set correctly (especially on Windows/macOS), then run:

```bash
# Run on the connected device/emulator
flutter run

```

## 📖 Detailed Guides

The author has provided a comprehensive series of blog posts explaining how this project was built and how to handle platform-specific configurations:

1. **Part 1**: [Creating the C program & CMake setup](https://blog.im404.me/article/using-gstreamer-in-your-flutter-project-pt-1-c-file)
2. **Part 2**: [Integrating with Flutter using FFI](https://blog.im404.me/article/using-gstreamer-in-your-flutter-project-pt-2-linux)
3. **Part 3**: [Windows Platform Configuration](https://blog.im404.me/article/using-gstreamer-in-your-flutter-project-pt-3-windows)
4. **Part 4**: [Android & macOS Specifics](https://blog.im404.me/article/using-gstreamer-in-your-flutter-project-pt-4-android)
5. **Part 5**: [iOS Integration & Initialization](https://blog.im404.me/article/using-gstreamer-in-your-flutter-project-pt-5-ios)

## 📂 Project Structure

* `cfile/`: The simple C program that creates a GStreamer pipeline we'll be referencing in the Flutter project.
* `app/fltgst`: The main Flutter app.
* `app/native_binding`: FFI project that creates bindings to the C library.

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!
Check out the [issues page](https://github.com/fengjiongmax/fltgst/issues) if you want to help improve the project.


---

*Created by [fengjiongmax](https://github.com/fengjiongmax). Check out the [YouTube Playlist](https://www.youtube.com/playlist?list=PLmUt0wXRF1HjsKy2XPPWFvuGA5DKDkpAc) for video demonstrations.*
