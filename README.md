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

Instructions to get Librum up and running on your environment.

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
3. Add the Qt path to the CMakeLists.txt<br>
    By default, Qt creator appends the path to Qt to the 'CMAKE_PREFIX_PATH' variable, you have 2 options for building Librum:<br>
    - Use Qt Creator to build the project (Simply open the CMakeLists.txt through Qt Creator, by clicking File -> Open File or Project...)
    - Add the path manually, for that, just copy this line
        ```sh
        list(APPEND CMAKE_PREFIX_PATH "/home/<username>/Qt/<Version>/<compiler>")
        ```
        to the third line of the top-level CMakeLists.txt and replace all contents in angular brackets '<>' with your data, for example:
        ```sh
        list(APPEND CMAKE_PREFIX_PATH "/home/david/Qt/6.2.3/gcc_64")
        ```
4. Create build folder and setp into it.
    ```sh
    mkdir build-Debug
    cd build-Debug
    ```
6. Run cmake.
    ```sh
    cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DBUILD_TESTS=On -G "Unix Makefiles" ..
    ```
6. Build the project
    ```sh
    make
    ```
7. Run the app
    ```sh
    ./appLibrum
    ```
<br>
