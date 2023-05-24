# Librum

Librum is an application, designed to make reading as <b>enjoyable</b> and <b>straightforward</b> as possible for you.<br>

It is more than simply an e-book reader, Librum makes it possible for you, to manage your own library, which you can access from <b>all</b> of your devices at any time.<br>
Librum provides a variety of features to make you as productive as you can be, these features include being able to take <b>notes</b> from within the app, <b>book mark</b> and <b>highlight</b> important sections, while being able to customise the application to look and feel exactly how you want it to. Librum also provides free access to over <b>70,000</b> books, custom <b>plugins</b> and your reading <b>statistics</b>, while remaining <b>free</b> and 100% <b>open source</b>.


Librum's aim is to make reading as <b>frictionless</b> and <b>enjoyable</b> as possible for you.

<br>
<b>Note</b>: Librum is still in development, the servers are <b>not</b> public, and thus Librum is not yet available for public use.
A beta release will be available soon, bringing the servers online and making the application available to end users. 

# Preview

![login_screen](https://user-images.githubusercontent.com/69865187/218166838-c2cad42a-d1dd-4a2a-a110-411368f7ff65.png)
Your entry-point to Librum

<br>

![book_reading](https://user-images.githubusercontent.com/69865187/218166898-9054e579-8cbf-460e-9f35-6178e8666c71.png)
Enjoy a simple and clean look while reading

<br>

![appearance_settings](https://user-images.githubusercontent.com/69865187/218166919-162da3ed-e5da-4db6-9c12-e9913c374c11.png)
Make the application look and feel the way you want

<br>

![about_settings_dark](https://user-images.githubusercontent.com/69865187/218176878-f621669f-9d20-4f92-a9d5-ebb75e877218.png)
Enjoy a relaxing dark mode

<br>

https://user-images.githubusercontent.com/69865187/204820315-4fccb311-8709-486f-8be8-5277a2bc3be5.mp4

Setup and manage your own library

<br>


# Documentation

For documentation go to [Librum's github-wiki](https://github.com/Librum-Reader/Librum/wiki)

<br>

# Donations

If you like the Librum project, consider donating to the opensource developers at https://www.patreon.com/librumreader.<br><br>
As a team of opensource developers we rely on donations from the community to be able to continue working on projects like Librum. Any help is greatly appreciated.

<br>

# Contributing

If you are interested in contributing, feel free to contact us on either:<br>
1. Discord (m_david#0631)
2. Email (contact@librumreader.com)
<br>
We are following a pull request workflow where every contribution is sent as a pull request and merged into the dev/develop branch for testing. <br>
Please make sure to run clang format, keep to the conventions used throughout the application and ensure that all tests pass, before submitting any pull request.
<br>

# Contact


For questions, you can reach us under: help@librumreader.com
<br>
For business related contact, reach out to us here: contact@librumreader.com

<br>

# Details

### Supported platforms
Part of Librum's aim, is to remove the boundary of reading on different devices, no matter where you are or which device you use, you can always continue your book with Librum, since Librum is <b>cross platform</b>.<br>
We support:
- Windows
- GNU/Linux
- MacOS
- IOS
- Android

<br>

### Supported formats
No matter what book you want to read, it will work with Librum, since Librum supports <b>all</b> major book formats,<br>
including:
- PDF
- PS
- Tiff
- CHM
- DjVu
- Images
- DVI
- XPS
- Fiction Book
- Comic Book
- Plucker
- Pub
- Fax
- All plain text formats
- Most image formats

and <b>much more</b>.

<br>

### Features
Librum's objective is to make your reading more <b>productive</b>; to that end, we provide you with a variety of functions that you can access via a <b>simple</b> and <b>straightforward</b> interface.<br>
These functions include:
- A modern e-reader
- Creating your personalized library
- Book meta-data editing
- A free in-app bookstore with more than 60.000 books
- Book syncing across all of your devices
- Highlighting
- Note taking
- Bookmarking
- Searching through
- Tags
- TTS (Text to speech)
- Automated page scrolling
- Syncing with 3rd parties (Google drive, dropbox, nextcloud)
- Custom plugins
- Appearance and behaviour customization
- Your reading statistics
- No-login book reading
- Sharing / Priting pages

If you are missing any feature, feel free to leave a feature request ticket!

<br><br>

# Getting started

Instructions to get Librum up and running in your environment.

<b>Note</b>: Librum currently relies on KDE's infrastructure for okular. This will change soon, however for the time being, Librum and Okular may conflict.
If there is an issue with the Librum installation, consider temporarily uninstalling okular. 
<br>
<br>

## For GNU/Linux
### Foreword
Since Librum is a Qt and KDE application, the Qt and KDE libraries are necessary to run Librum. If you are on a Linux distribution that uses KDE Plasma, you are good to go; else you will need to download the dependencies.

<br>

### Prerequisites for Everyone
- cmake (at least version 3.21)     https://cmake.org/download/
- make (or any other build system)  http://ftp.gnu.org/gnu/make/
- Gcc (or any other c++ compiler)   https://gcc.gnu.org/

<br>

### Prerequisites for KDE Plasma Users
- Extra-cmake-modules               https://github.com/KDE/extra-cmake-modules  (Many package managers list them as `extra-cmake-modules`)

<br>

### Prerequisites for Ubuntu Users
Run the following commands to install all dependencies for Librum

```sh
sudo apt update
```
    
```sh
sudo apt-get install build-essential extra-cmake-modules qtbase5-dev qt5-qmake qtquickcontrols2-5-dev qtdeclarative5-dev qml-module-qtquick-controls2 libkf5parts-dev libkf5crash-dev libkf5textwidgets-dev libkf5archive-dev libkf5threadweaver-dev libkf5activities-dev libqt5svg5-dev libphonon4qt5-dev libqt5texttospeech5-dev zlib1g-dev libkf5pty-dev qtdeclarative5-private-dev qtbase5-private-dev qml-module-qtquick-shapes qml-module-qt-labs-qmlmodels qml-module-qt-labs-settings qml-module-qt-labs-platform libpoppler-qt5-dev qml-module-qtquick-controls qml-module-qtquick-layouts qml-module-qtquick-window2
```
    
<br>

### Prerequisites for other distros
If you are running on another, non-kde-plasma distro which isn't Ubuntu, you will need to find install the dependencies via your distro specific package manager. The packages required should be pretty similar to the ones listed in the "Prerequisites specifically for Ubuntu Users" section, but they might be called differently in your package manager. <br><br>
If you manage to build Librum on a distro that is not listed here, feel free to open a pull request with the instructions to build it on your distro.

<br>

### Installation
Once you have installed the necessary dependencies, go ahead and build Librum by following the steps below.

<br>

1. Clone the repository.
    ```sh
    git clone --recurse-submodules -j4 https://github.com/Etovex/Librum.git
    ```
2. Step into the cloned project folder.
    ```sh
    cd Librum
    ```
3. Create the build folder and step into it.
    ```sh
    mkdir build-Release
    cd build-Release
    ```
4. Run cmake.
    ```sh
    cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF ..
    ```
5. Build the project
    ```sh
    make -j4
    ```
6. Install the files
    ```sh
    sudo make install
    ```
7. Run the app
    ```sh
    ./appLibrum
    ```
<br>


<br>

## For Windows
Not yet officially supported. Support coming soon!

## For macOS
Not yet officially supported. Support coming soon!
