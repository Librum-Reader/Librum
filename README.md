# Librum

Librum is a simple to use, crossplattform, state-saving bookreader with a lot of functionalities. Some of the functionalities include:
- High appearance customization
- A free in app book store
- Book state syncing across devices
- In app file conversion
- Bookmarks / Highlighting
- Note taking functionality

<br><br>

# Getting started

Instructions to get Librum up and running in your environment.

<br>

## For GNU/Linux
### Prerequisites
- cmake (at least 3.22)            https://cmake.org/download/
- make (or any other build system) http://ftp.gnu.org/gnu/make/
- Qt6                              https://www.qt.io/download
- Gcc (or any other c++ compiler)  https://gcc.gnu.org/

### Installation
1. Clone the repository.
    ```sh
    git clone https://github.com/Etovex/Librum.git
    ```
2. Step into the cloned project folder.
    ```sh
    cd Librum
    ```
3. Create build folder and setp into it.
    ```sh
    mkdir build-Debug
    cd build-Debug
    ```
4. Run cmake.
    ```sh
    cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DBUILD_TESTS=On -G "Unix Makefiles" ..
    ```
5. Build the project
    ```sh
    make
    ```
6. Run the app
    ```sh
    ./appLibrum
    ```
<br>
