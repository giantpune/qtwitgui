

# Usage #

## General ##
  * This program works by getting input from the user, then building a list of commands and runs several different programs as a process and passes it those commands.  Then it parses the text output and turns the text output into elements in the GUI. Due to this, changes in the formatting of the output text from from these programs, as well as different formatting from different versions and locales can break this program easily.  Wherever possible, I try to compensate for this.  In some cases, I have added settings for the user to enter specific strings to match ( ie, the messages sudo outputs ).

  * Why not just incorporate code from wit/wwt into this program and forget the external process?  As it is right now, wit is still a work in progress.  Bugs are getting fixed and features are being added.  It would be pointless to borrow code from that project now only to have the borrowed code be improved and obsoleted in a week.

  * This program uses a multiple document interface.  Each of the game and partition windows has its own set of processes.  This means that each window can perform a different operation at the same time.  In most cases, this should pose no problem, except to use more resources.  However, I don't think 2 processes writing to the same WBFS partition at the same time is a good idea.  I have taken measures to prevent his from happening in this program.  In the even that I made an error, and you do find a loophole that allows this, I strongly suggest you don't try to do it.
<a href='http://img266.imageshack.us/img266/1405/multipledocument.png'><img src='http://img266.imageshack.us/img266/1405/multipledocument.th.png' border='0' /></a>


  * The term "partition" is not used in the common sense in QtWitGui.  I handle common folders used for wii games as a separate partition.  Such folders are "wbfs", "games", and "iso".  So, It is possible to have 1 partition in the normal sense of the word with 3 partitions as i treat them.  The logic here is that people tend to keep different types of games in the different folders.  Treating the "games" folder differently than the "wbfs" folder allows to save settings for each folder differently ( ie, default to extracting games in sneek's format for the "games" folder ).

  * The basic logic behind the program is that each window keeps a copy of the known partitions and games.  The Main window keeps a "master list".  Each time A subwindow gets information about the available games/partitions, it sends this information to the main window, who then sends updated lists to the subwindows.

  * Most of the operations in this program are performed asynchronously through signals and slots.  When an action need to happen, the window sends a request for that action to some thread and sets a "busy" cursor.  Once the performed action is done, the object that was doing the work sends a message to the window that asked for the action.

## Usage ##
  * When the program is started, it checks for the external processes it uses.  If it cannot find them, menus and actions will be disabled.  You will need to go to the settings and enter the necessary paths before it will allow all the features to be used.

  * Once the settings are adjusted, there are 3 ways to proceed.  You can either open a single game in the gameWindow, you can open a partition in a partitionWindow, or you can let the program go ahead and build a list of all the partitions & games in the "open partition" dialog.

  * Using one of these 3 options, you can jump right to patching a single game, converting several games, or going ahead and letting the program cache all the gamelists for later use.

  * Opening partitions and games can be done several different ways.  Drag & drop, argv[.md](.md), the "File" menu, and through right-click menus.

  * To let the QtWitGui get all the gamelists, use the upper left button in the "open partition" dialog.  This will take longer than just reading 1 partition's games, but it will ensure that all available partitions will be flagged and added to the lists used in the "copy & convert" dialog and the cover manager.

### Partition Select Dialog ###
<a href='http://img5.imageshack.us/img5/6967/partitionselect.png'><img src='http://img5.imageshack.us/img5/6967/partitionselect.th.png' border='0' /></a>
  * To get here, press File->Open Partition ( ctrl + P ).
  * The easiest thing to do here is to just press the upper left button and let the program search for partitions.  The program will check the common locations for wii games and try to add those folders to the partition list.  The common locations are...
    * Windows - DriveLetter:\games, wbfs, iso
    * OSx - /Volumes/.../games, wbfs, iso
    * Linux - /media/.../games, wbfs, iso
  * After these common locations, wwt is asked for to check for WBFS formatted drives.  Then wit is asked to check and partition for games.

  * Each partition has a set of flags.  These include the number of games, amount of space used, whether it is busy or not, the filesystem, whether or not to split large files when writing to, etc.

  * The "rescan" button doesnt look for and drives that are not already known.  It just clears some of the known flags from partitions already on the list, and rechecks them.

  * The "manual" button lets you select some path that is not included in the common locations.

  * Logic for the automatic partition flags:
    * If the partition came from wwt, it has a WBFS partition
    * If the partition has a FAT FS and ends with "/games" flag it as "SNEEK".  Otherwise, set the "split games" flag.

  * While the automatic flags should be correct, it is possible that you may want to change them.  Just right-click a partition, or select multiple items and right click one to edit flags for more than 1 at a time.

  * Changing partition flags has no affect on the partition itself.  They are only used as guides before writing.  ( changing a partition's filesystem flag does not reformat it )


### Partition Window ###
<a href='http://img253.imageshack.us/img253/8485/partitionwindow.png'><img src='http://img253.imageshack.us/img253/8485/partitionwindow.th.png' border='0' /></a>
  * Shows information about all the games in a folder -  ID, title, size, container, partitions, path...
  * From this window you can select 1 or more games and copy & convert them to some other partition or format.

### Game Window ###
<a href='http://img841.imageshack.us/img841/2513/gamewindow.png'><img src='http://img841.imageshack.us/img841/2513/gamewindow.th.png' border='0' /></a>
  * Shows detailed information about 1 game...  IOS, file layout, signature status...
  * You can select certain parts of a game and patch them - make a game use a different location on the wii's nand to save its data, change a game's name...
  * For most containers, you can simply patch a game ( File->save ).  This will present a window where you can view changes that will be applied to the game.  Saving here will overwrite the curent file.
<a href='http://img545.imageshack.us/img545/2581/gamesavedetails.png'><img src='http://img545.imageshack.us/img545/2581/gamesavedetails.th.png' border='0' /></a>
  * Selecting "File->save as..." will bring up a copy/convert dialog.


### Copy & Convert Dialog ###
<a href='http://img5.imageshack.us/img5/9923/copyconvertdialog.png'><img src='http://img5.imageshack.us/img5/9923/copyconvertdialog.th.png' border='0' /></a>
  * This dialog is the ticket to converting games from one container/partition to another.
  * Whatever partitions are known to the main window when this dalog is called will be added to the list of choices.  And whatever flags these partitions have will affect what the dialog does when you select this partition from the list.  ( ie, If a partition is flagged as "SNEEK", the ui will fill in the necessary choices to copy a game to the SNEEK format whenever you select this partition in the list )
  * The main idea here is that you can select a set of rules that will be applied to all games when converting.  There is example text at the bottom of the screen to show what the output games will be.
  * If there is only 1 game selected when this dialog is called, there will be a choice to offer a specific filename.
  * For more information about the different containers/formats, check Wiimms' site or google
  * This dialog has a memory.  It will do its best to load up the last accepted settings.

## WiiTDB ##
<a href='http://img835.imageshack.us/img835/9105/wiitdbwindow.png'><img src='http://img835.imageshack.us/img835/9105/wiitdbwindow.th.png' border='0' /></a>  <a href='http://img243.imageshack.us/img243/6852/searchwiitdb.png'><img src='http://img243.imageshack.us/img243/6852/searchwiitdb.th.png' border='0' /></a>

  * You can choose "view->wiitdb" and get a window that shows information about your games.  This window should sync automagically with any open partitions.
  * The window is created with "stay on top" flags.  Usually right-clicking the window titlebar will allow to turn off this setting.
  * There is a copy of wiitdb.xml built into the program.  You can also specify a different copy in the settings.  Due to incompatibilities in the zip libraries, wiitdb must be unzipped on OSx.
  * There is a "search" tab that allows searching through all the games in the currently loaded WiiTDB.  This tab should be pretty self-explanatory.

  * The ID, Title, and Synopsis fields not only accept input as one would expect, but also work with regex expressions ( case-insensitive ).  This makes this it officially the most advanced WiiTDB searcher in the world.  For more information on how to use the qregex see http://www.regular-expressions.info/ and http://doc.qt.nokia.com/4.7/qregexp.html

  * Possible searches:  "^[0-9a-z]{3}E" in the ID field will return all games with "E" as the 4th character.  "metallica|ozzy" in the synopsis filter will show only games that have either "metallica" or "ozzy" in it's synopsis.

  * There are several checkboxes to add accessories to the search filter.  These are tristate checkboxes.  If you simi-check an accessory, the search will only list games that support that item.  If you fully check an accessory, the icon will turn blue, and the search results will only include games that require that item.

  * There are 2 filters for the number of players.  The first is for the amount of players the game supports locally, and the second is the amount of players the game supports online.  You can search "less than", "less than or equal", "equal", "greater than", "greater than or equal", and "not equal".

  * The rating filter allows to search for games based on their rating ( duh ).  Ratings from different regions are converted ( roughly ) to the ration of the search filter.  This filter accepts the same 6 comparison types as the player filters.


## Settings ##
  * As the name says, this is the place to adjust parameters that affect the way the program behaves.
  * This is still a work in progress, so theres not much to look at, as some of what is there is not actually functional.

### Paths ###
  * Some of there are necessary for the program to work and some are optional
  * wit - path to the wit binary ( necessary )
  * wwt - path to the wwt binary ( necessary )
  * covers - basepath for covers ( optional - unused )
  * titles.txt - even though there is wiitdb support in this program, wit does not support it.  instead it uses titles.txt ( optional - unused )

### Root settings ###
  * linux and OSx only
  * allows running wit, wwt, and file with sudo for more access
  * The fields here are the the expected output from sudo as it asks the password and delivers failure messages.  This text must be exact or else the program will hang as sudo has asked a question and since the text doesnt match so, it doesn't know to respond.  If you are unsure of the proper text, just try "sudo ls" in a terminal and enter the wrong password a few times.
  * Using sudo with an empty password in not supported.  It should be at least 1 character.
  * For security reasons, the password is not stored in any file.  A dialog is presented to you and you must enter it.  The password will be stored in ram while the program is running, and reused each time it is needed.


## Cover Window ##
<a href='http://img168.imageshack.us/img168/6786/coversk.png'><img src='http://img168.imageshack.us/img168/6786/coversk.th.png' border='0' /></a>
  * To get this window, set a cover path in the settings screen.  Then Click view -> covers.

  * The logic behind the window is pretty straight forward...

  * The window gets a copy of all the known partitions and gamelists from the main window.

  * The window deals with covers for 1 partition at a time.  To change partitions, right-click -> "sync with partition" -> select a partition from the list.

  * If you dont see a partition available, then it has not been scanned since the program was started.  Either use the partition selector window or a partition browser window to scan the partition.  Its game list will be sent to the main window, and then to the cover window.

  * There are different subdirectories used inside the cover path set in the settings.  Flat & Front 160x224 = "2d"; Fake 3D 176x248 = "3d"; Full 512X340 = "full"; Full HQ 1024x680 = "full\_HiRes"; disc 160x160 = "disc"

  * The cover window takes the list of games for a given partition, adds unique IDs to a list, and tries to find a cover from the above mentioned paths.  It checks the 2d, then full, then full HQ in that order.    It will only use the front part of whatever cover it finds, and it will be scaled down.  So, for this program, a 2d cover is just as good as a HQ one.

  * To download covers from wiitdb, right-click -> "download missing covers" -> and select the type you want to get.  There is a number out to the right of how many covers there are missing of the given type from the current partition.

  * If you choose to download either of the full covers and there is not one available, it will check if you have the 2d version already, and if not, it will try to download that one instead.

  * This window uses a modified version of http://code.google.com/p/pictureflow/ to display and animate the covers.  Most of the credit goes to its creator(s), and a big thanks for sharing their work.






## Compiling ##
  * This project needs wiitdb.xml in the directory with all the source.  I do not supply this file.  Instructions in the .pro file use curl to grab the zipped file from online.  Then it uses unzip to grab the xml from inside the zip archive.  If you have trouble with either of these, you can simply get the xml by any means necessary and put it in the project directory.

  * Get your meathooks on the Qt SDK ( http://qt.nokia.com/downloads/ ).  Then just open the .pro file from this svn in Qt Creator ( IDE ) and click the picture of a hammer in the lower-left.

---



## Creating and using language files ##

  * This program determines the appropriate language file to use by checking the environmental variable at startup.

  * I have added a bit of debug output viewable if running this program from a terminal to display the region variable.

  * open the .ts file from this svn in Qt Linguist.  Select your country and language.  Select a string and enter new text for your language.  Then do "translation -> Done and Next" (ctrl+Enter).  It will give a green check mark and move on to the next string. Once you are done, do file->release and save a .qm file.
> > <a href='http://img808.imageshack.us/img808/9990/qtlinguist.png'><img src='http://img808.imageshack.us/img808/9990/qtlinguist.th.png' border='0' /></a>

  * Strings may contain arguments, which begin with a %, and are numbered.  The string "My name is Sally and I like pizza on Tuesdays." may look like "My name is %1 and I like %2 on %3."  So, it may be translated and the arguments are not required to be in the same order as in the original sentence.

  * Since I am using USA/english (en\_US )  on my PC, the program will first try to load "witGuiLang\_en\_US.qm".  Failing that, it will try "witGuiLang\_en.qm".  If there still is no language file found, it will fall back to the version compiled within the program itself.  This makes it possible for the program to have a French translation embedded within itself and the user is still able to load their own translation if they disagree with the translation.

  * The qm files embedded in this program are given generic names ( language only, no country specification ).  The same French will be used for French-Canada and French-France.

  * There is no guarantee that the .ts file will be up-to-date.  There is an instruction in the project file to update the ts files, but this instruction only gets executed when qmake is run, not every time the program is compiled.  Because of this, there may be strings used in the project that are not in the current .ts file.  This file in no automatically updated.  You can run "lupdate -verbose ./witGui.pro".  This will grab the current strings used in the program and slap them in the .ts file.  Then you can use that to create a .qm file.

  * You can submit to me a filled out .ts file and I may include it in the project.  This will allow myself and others to keep all the translated strings and add new ones to be translated when needed, making updating translations much easier.

---





# Platform Specific Stuff #

## linux ##
  * right out of the box, there is no access to WBFS partitions.  Either run this program as root ( NOT RECOMMENDED ), make use of the sudo stuff in the settings, or add an exception rule in the sudoers file for wit & wwt
  * Results of "ldd ./QtWitGui" ( from [r112](https://code.google.com/p/qtwitgui/source/detail?r=112) )
```
	linux-vdso.so.1 =>  (0x00007ffff57ff000)
	libQtGui.so.4 => /usr/lib/libQtGui.so.4 (0x00007fc81b9c7000)
	libQtNetwork.so.4 => /usr/lib/libQtNetwork.so.4 (0x00007fc81b6ac000)
	libQtCore.so.4 => /usr/lib/libQtCore.so.4 (0x00007fc81b275000)
	libpthread.so.0 => /lib/libpthread.so.0 (0x00007fc81b059000)
	libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x00007fc81ad49000)
	libm.so.6 => /lib/libm.so.6 (0x00007fc81aac5000)
	libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00007fc81a8ae000)
	libc.so.6 => /lib/libc.so.6 (0x00007fc81a53f000)
	libaudio.so.2 => /usr/lib/libaudio.so.2 (0x00007fc81a326000)
	libpng12.so.0 => /usr/lib/libpng12.so.0 (0x00007fc81a100000)
	libfreetype.so.6 => /usr/lib/libfreetype.so.6 (0x00007fc819e7b000)
	libgobject-2.0.so.0 => /usr/lib/libgobject-2.0.so.0 (0x00007fc819c34000)
	libSM.so.6 => /usr/lib/libSM.so.6 (0x00007fc819a2b000)
	libICE.so.6 => /usr/lib/libICE.so.6 (0x00007fc819810000)
	libz.so.1 => /lib/libz.so.1 (0x00007fc8195f9000)
	libglib-2.0.so.0 => /lib/libglib-2.0.so.0 (0x00007fc819332000)
	libXrender.so.1 => /usr/lib/libXrender.so.1 (0x00007fc819128000)
	libfontconfig.so.1 => /usr/lib/libfontconfig.so.1 (0x00007fc818ef6000)
	libXext.so.6 => /usr/lib/libXext.so.6 (0x00007fc818ce4000)
	libX11.so.6 => /usr/lib/libX11.so.6 (0x00007fc8189ae000)
	libgthread-2.0.so.0 => /usr/lib/libgthread-2.0.so.0 (0x00007fc8187a9000)
	librt.so.1 => /lib/librt.so.1 (0x00007fc8185a1000)
	libdl.so.2 => /lib/libdl.so.2 (0x00007fc81839d000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fc81c57b000)
	libXt.so.6 => /usr/lib/libXt.so.6 (0x00007fc818137000)
	libXau.so.6 => /usr/lib/libXau.so.6 (0x00007fc817f34000)
	libpcre.so.3 => /lib/libpcre.so.3 (0x00007fc817d06000)
	libuuid.so.1 => /lib/libuuid.so.1 (0x00007fc817b01000)
	libexpat.so.1 => /lib/libexpat.so.1 (0x00007fc8178d8000)
	libxcb.so.1 => /usr/lib/libxcb.so.1 (0x00007fc8176bc000)
	libXdmcp.so.6 => /usr/lib/libXdmcp.so.6 (0x00007fc8174b7000)
```

  * apparently all these libs ( except the Qt ones ) can be found in these packages for ubuntu 10.0.4 AMD64.
```
	ia32-libs
	lib32z1
	libc6
	libstdc++6
	libgcc1
```

---




## windows ##
  * It is possible to run this exe without having the Qt SDK installed, but it requires at least 6 .dll files:
<pre>
QtCore4.dll<br>
QtGui4.dll<br>
QtNetwork4.dll<br>
QtXml.dll<br>
libgcc_s_dw2-1.dll<br>
mingwm10.dll<br>
</pre>
  * Since Wiimm's tools rely on cygwin, this program uses cygpath.exe to convert windows paths to cygwin paths and back again.  And cygpath.exe relies on the cygwin1.dll.  So, required files include the above dlls, wit, wit's dependencies, and cygpath.exe ( which shares a dependency on cygwin1.dll with wit.  both programs must have access to it )

  * In order to support using games on DVD-r as source files, I have created a little helper program called "listDvd.exe" which simply lists dvd drives as the paths that cygwin & Wiimms' tools expect.  This exe should be in the same path as qtwitgui, as well as cygwin1.dll.  You can test that this exe is working properly by inserting a DVd into one of your drives and running the exe from a cmd window.  If it says something like "/dev/sr0", then it is working properly, and you should be able to copy games from DVD-r to any format supported by the wiimms toolset.

  * wmic is required.  This SHOULD be already present on your system.  If you get errors about not being able to start wmic, open a cmd windows and run "wmic /output:stdout /interactive:off /locale:ms\_409 OS GET caption".  This will tell you if you have the program or not.  And if you don't, check google for how to get it back from wherever it went.

  * There is no support for the UAC in this program.  If you want wit/wwt to be able to access WBFS partitions on windows version that have the UAC, run this program as administrator and the external processes will inherit the privileges.

  * Im not sure if it is just my PC, or what, but this program runs pretty terribly in windows7.  I have tested it in virtual machines running XP, and a high-end real PC running win7.  The VM-XP runs perfectly, while the PC-win7 was littered with graphical artifacts.  I don't know who to blame it on, or if this only happens for my setup.  In any case, there is not much I can think of to fix it.  This is just how win7's window manager handles stuff, and how Qt talks to the window manager.

---





## OS-X ##
  * no .zip support for wiitdb
  * Root support doesnt appear to be needed for WBFS access, but it was already there for linux, so i left it in for OSx.
  * The DVD menu is disabled.  My DVD drives are not working on my test MAC, so I have no way to test & debug this stuff.  If somebody is willing to take a loot at the source and submit a patch, I will enable this menu.
  * results of "otool -L ./QtWitGui" ( from [r112](https://code.google.com/p/qtwitgui/source/detail?r=112) )
```
/Users/noskill/c/qtWitGui/_69_/QtWitGui.app/Contents/MacOS/QtWitGui:
	QtGui.framework/Versions/4/QtGui (compatibility version 4.7.0, current version 4.7.0)
	QtCore.framework/Versions/4/QtCore (compatibility version 4.7.0, current version 4.7.0)
	/usr/lib/libstdc++.6.dylib (compatibility version 7.0.0, current version 7.9.0)
	/usr/lib/libgcc_s.1.dylib (compatibility version 1.0.0, current version 246.0.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 125.0.1)
```

## Big Endian Machines ##
  * The code used to align gamecube games does some endian conversions.  I do not have a big endian machine to test this stuff out.  So, I have simply added a bit that that will cause compile errors on purpose.  This is to make sure that anybody using that will know to be careful.  you can simply delete these forced errors and it should compile fine.  Please do not share these compiled binaries with anybody.  I want to make it obvious to anybody using this program that it is possible that this code is completely broken.

  * After I have a reasonable amount of reports that this code is working properly, I will remove these compile errors.