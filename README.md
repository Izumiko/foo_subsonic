# foo_subsonic

**This is highly experimental code. Use at your own risk!**


I'm not a C++ programmer and just playing around here, so please be careful.
I expect to have a lot of memory leaks and/or doing bad practices here!

## License
My code is licensed with the [WTFPL](http://www.wtfpl.net).

All used libraries and the foobar SDK are licensed under different license agreements.
Please look at the specific project for further details.

## Requirements:
* [WTL 10.x](http://wtl.sourceforge.net/)
* [TinyXML](http://sourceforge.net/projects/tinyxml/)
* [SQLite3 amalgamation source](https://www.sqlite.org/download.html)
* [SQLiteCPP](https://github.com/SRombauts/SQLiteCpp)
* [Foobar2000 SDK](http://www.foobar2000.org/SDK) (I used 2022-01-04, but newer versions should also be fine)
* MS Visual Studio (at least version 2010, I use 2022 Community Edition, 2022 SDK is for VS2019)

## Build Setup
1. Extract Foobar2000 SDK to SDK folder
2. Extract TinyXML source to SDK folder
3. Extract WTL's Include folder to SDK/wtl/ folder
4. Extract SQLite3 amalgamation source to SDK/sqlite3
5. Extract SQLiteCPP and copy the *.cpp files to SDK/sqlite3. Then copy the files from includes/SQLiteCPP/*.h to SDK/sqlite3/SQLiteCPP
6. Start Visual Studio and open the foo_subsonic.sln
7. Right click the solution in VS and Retarget solution, set target of all projects to your current VS and SDK
8. Add IncludePath `../../wtl/Include` to foobar2000_sdk_helpers; add IncludePath `../wtl/Include` to libPPUI

Directory structure:
```
foo_subsonic/
    SDK/
        foobar2000/
        libPPUI
        pfc/
        sqlite3/
            SQLiteCPP/
        tinyxml/
        wtl/
            Include/
```

Alternative build setup:
1. Extract files as Steps 1-5 above
2. Open cmd at project folder
3. `cmake -A Win32 -B build -S .`
4. `cmake --build build --config Release`
5. The plugin is now in `build/Release/foo_subsonic.dll`

## Help
Usage and FAQ can be found in the [Wiki](https://github.com/hypfvieh/foo_subsonic/wiki)

## Missing Features
* Drag and Drop is not working (any help appreciated)
* List should have a built-in search box (edit control) to do direct filtering
