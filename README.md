
# notepad_win32

A Notepad like Windows application in C++ using win32 API

# Installation

First, clone this repository:

```
git clone https://github.com/S-Sanjith/notepad_win32.git
cd notepad_win32
```
Now use Visual Studio Community, Clang, MinGW (provided with CodeBlocks and Dev-C++) or any other suitable C++ compiler which can handle Win32 API.

Use this compiler to compile the code in main.cpp

If you use MinGW, you can use te following command to compile the code:

`g++ main.cpp -o notepad.exe`

But while using a 64 bit MinGW compiler, you might have to use the -mwindows flag:

`g++ main.cpp -mwindows -o notepad.exe`

Run the executable created, to launch the Notepad application.

# Usage

The application is a basic text editor, visually similar to Windows Notepad. 

It has functionalities including new, save, open, exit and a help dialog box.
