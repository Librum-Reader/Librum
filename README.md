# Librum

Librum is an application designed to make reading <b>enjoyable</b> and <b>straightforward</b> for everyone.

It's not **just** an e-book reader. With Librum, you can manage your own online library and access it from any device anytime, anywhere. It has features like note-taking, bookmarking, and highlighting, while offering customization to make it as personal as you want!
 
Librum also provides free access to over 70,000 books and personal reading statistics while being free and completely open source.

<br>
<b>Note</b>: Librum is still in development, the servers are <b>not</b> public. This means Librum is not yet available for public use.

An alpha release will be available soon, bringing the servers online and making the application available to end users. 

# Preview

The login and starting page of Librum

![login_screen](https://user-images.githubusercontent.com/69865187/218166838-c2cad42a-d1dd-4a2a-a110-411368f7ff65.png)


<br>

A simple and modern interface that removes any distractions and clutter

![book_reading](https://user-images.githubusercontent.com/69865187/218166898-9054e579-8cbf-460e-9f35-6178e8666c71.png)


<br>

Customize Librum to make it personal to you

![appearance_settings](https://user-images.githubusercontent.com/69865187/218166919-162da3ed-e5da-4db6-9c12-e9913c374c11.png)


<br>

Dark mode support. Need we say more?

![about_settings_dark](https://user-images.githubusercontent.com/69865187/218176878-f621669f-9d20-4f92-a9d5-ebb75e877218.png)


<br>

Setup and manage your own library

![HomeScreenDark](https://github.com/Librum-Reader/Librum/assets/69865187/ea94fc68-1bf0-4933-8d80-43a57c6590c5)

<br>

# Documentation

For documentation go to [Librum's GitHub-wiki](https://github.com/Librum-Reader/Librum/wiki)

<br>

# Donations

If you like the Librum project, consider donating to the opensource developers at https://www.patreon.com/librumreader.<br><br>
As a team of opensource developers we rely on donations to continue working on projects like Librum. All help is greatly appreciated.

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
Part of Librum's aim is to work on **any** platform. No matter where you are or which device you use, you can always continue your book with Librum, as it is <b>cross platform</b>.<br>
We support:
- Windows
- GNU/Linux
- MacOS
- IOS
- Android
<br>

### Supported formats
No matter what file type you have, you can read it on Librum, since we support <b>all</b> major book formats,<br>
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
Librum's objective is to make your reading more <b>productive</b>; to that end, we provide you with a variety of features that you can access via a <b>simple</b> and <b>straightforward</b> interface.<br>
These features include:
- A modern e-reader
- Creating a personalized library
- Meta-data editing
- A free in-app bookstore with more than 60.000 books
- Book syncing across all of your devices
- Highlighting
- Note-taking
- Bookmarking
- Extensive search
- Tags
- TTS (Text to speech)
- Automated page scrolling
- Syncing with third parties (Google drive, Dropbox, Nextcloud)
- Custom plugins
- Unlimited customization
- Personalized reading statistics
- No-login book reading
- Sharing / Printing pages

Want a new feature? Feel free to leave a feature request ticket!

<br><br>

# Getting started

Here's how to get Librum up and running in your environment.

<b>Note</b>: Librum currently relies on KDE's infrastructure for Okular. This will change soon, however for now, Librum and Okular may conflict.

If there is an issue with the Librum installation, consider temporarily uninstalling Okular. 
<br>
<br>

## For GNU/Linux
### Foreword
Since Librum is a Qt and KDE application, the Qt and KDE libraries are necessary to run Librum. If you are on a Linux distribution that uses KDE Plasma, you are good to go! If not, you will need to download the dependencies.

<br>

### Prerequisites for Everyone
- cmake (at least version 3.21)     https://cmake.org/download/
- make (or any other build system)  http://ftp.gnu.org/gnu/make/
- Gcc (or any other c++ compiler)   https://gcc.gnu.org/

<br>

### Prerequisites for KDE Plasma Users
If you are using KDE Plasma, simply installing the following will be enough. Jump straight to "Installation" when you are done.
- Extra-cmake-modules               https://github.com/KDE/extra-cmake-modules  (Many package managers list them as `extra-cmake-modules`)

<br>

### Prerequisites for (non KDE) Ubuntu Users
Run the following commands to install all dependencies for Librum

```sh
sudo apt update
```
    
```sh
sudo apt-get install build-essential extra-cmake-modules qtbase5-dev qt5-qmake qtquickcontrols2-5-dev qtdeclarative5-dev qml-module-qtquick-controls2 libkf5parts-dev libkf5crash-dev libkf5textwidgets-dev libkf5archive-dev libkf5threadweaver-dev libkf5activities-dev libqt5svg5-dev libphonon4qt5-dev libqt5texttospeech5-dev zlib1g-dev libkf5pty-dev qtdeclarative5-private-dev qtbase5-private-dev qml-module-qtquick-shapes qml-module-qt-labs-qmlmodels qml-module-qt-labs-settings qml-module-qt-labs-platform libpoppler-qt5-dev qml-module-qtquick-controls qml-module-qtquick-layouts qml-module-qtquick-window2
```
    
<br>

### Prerequisites for (non KDE) Arch Users
Run the following commands to install all dependencies for Librum

```sh
sudo pacman -Syyu
```
    
```sh
sudo pacman -S base-devel extra-cmake-modules qt5-base qt5-quickcontrols2 qt5-quickcontrols2 qt5-declarative kde-dev-utils libarchive threadweaver qt5-svg phonon-qt5 qt5-speech kpty poppler-qt5 kactivities qt5-graphicaleffects ebook-tools
```
    
<br>

### Prerequisites for (non KDE) Fedora
Run the following commands to install all dependencies for Librum
    
```sh
sudo dnf install extra-cmake-modules qt5-qtbase-devel qt5-qtquickcontrols2 qt5-qtdeclarative qt5-qtquickcontrols2 kf5-kparts-devel kf5-threadweaver-devel kf5-kactivities-devel kf5-karchive-devel kf5-kiconthemes-devel kf5-kcrash-devel phonon-qt5-devel zlib-devel kf5-kpty-devel poppler-qt5-devel ebook-tools-devel qt5-qtquickcontrols2-devel qt5-qtsvg-devel qt5-qtbase-private-devel redhat-rpm-config
```

Create a symbolic link for qmake (it is called qmake-qt5 on fedora, but Librum calls it via `qmake`)
```sh
sudo ln -s /usr/bin/qmake-qt5 /usr/bin/qmake
```

Some might get errors like `RPM_* not defined`. If that's the case, run the following:
```sh
export RPM_ARCH=$(uname -m)
export RPM_PACKAGE_VERSION=$(uname -v)
export RPM_PACKAGE_RELEASE=$(uname -v)
```
    
<br>


### Prerequisites for other distros
If you are running on another, non-kde-plasma distro which isn't listed above, you will need to install the dependencies via your distro specific package manager. The packages required should be pretty similar to the ones listed for other non-KDE distros above, but they might be called differently in your package manager. <br><br>
If you manage to build Librum on a distro that is not listed here,  please open a pull request with the instructions to build it on your distro to help others with the same setup.

<br>

### Installation
Once you have installed the necessary dependencies, build Librum by following the steps below:

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
    cmake --build .
    ```
6. Install the files
    ```sh
    sudo cmake --install .
    ```
7. Run the app
    ```sh
    ./appLibrum
    ```
<br>


<br>

## For Windows
Support coming soon!

## For macOS
Support coming soon!
