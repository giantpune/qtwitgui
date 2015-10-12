This project is a graphical user interface used to build command-line arguments and send them to wit.  The main goal of this project is to allow a user-friendly interface for the powerful wit tool, and be able to be compiled and run natively on OS-X, linux, and windows.  I am personally targeting Linux, but I will try to ensure most functions work on on other platforms. This program by itself is pretty useless.  You must get wit ( http://wit.wiimm.de/download.html ) and tell this program where you have it stored for anything useful to happen.  I have included many of the features of Wiimms' tools, but there are still many things you can do with these tools that are not supported in the gui.

QtWitGui should also make the tasks that it does much easier and faster than simply using the command line.  For instance, loading a game into the GUI actually issues a couple different commands and parses ( usually ) 5,000 to 50,000 lines of text that would normally not even fix on the screen all at once.  Important values are stored as variables to use for other operations, so the user does not have to dig through all the text to find them for future commands.  There is also less chance of user error & typos if the user is only typing 6 or 10 letters at a time.

**How did QtWitGui come to be?**

> I had an idea for a wii disc that I felt like creating.  However, there were only 1 or 2 tools I knew of for creating a completely homebrew Wii disc.  Of these tools, there was wiiscrubber's partition builder which has never working for me, and wit, which works great, but is cli only.  I made my first few test discs using wit, but quickly I got annoyed by my typos in the terminal.  I came up with a little gui that allowed my to not have to type in any of the parameters needed to create my disc, but instead just click a button or 2.  Over the next month or two, I continued to improve the project, adding new features, and fixing bugs along the way.  Eventually I had something that did what I wanted, and much more.

> This was the first incarnation of QtWitGui ( happened around svn [r96](https://code.google.com/p/qtwitgui/source/detail?r=96) ).  It did what I designed it to do.  I was able to greatly speed up the creating of my discs and eventually I had a disc that needed testing.  And in this testing, I had several times when I needed to add my disc to a WBFS partition.  Just as before, I had only limited choices in Linux.  There are a few different WBFS managers, but none of them work right for me.  And there is only 1 tool that allows building a Wii disc at the same time as adding it to a WBFS partition ( wwt ).  So, this is when I decided it was time to add the necessary stuff into my program to allow me to add games to, and delete them from a WBFS partition.

> I have since added the necessary stuff to QtWitGui to talk to wwt, and support some of its functions.  Along the way, I have added in some other stuff I thought of that I felt would be useful, or just cool to have.  And this is where the project is today.

This is still a work in progress, and should not be considered finished.  Currently, I think it is stable enough for everyday use, but not in a polished & releasable state.

Stuff that is working so far -
  * wit & wwt
    * display information about a game image ( name, ID, IOS, filetree w/ offsets & sizes ) <a href='http://wit.wiimm.de/wit/cmd-dump.html'>( wit DUMP )</a>
    * convert directly between all formats supported by wit <a href='http://wit.wiimm.de/wit/cmd-copy.html'>( wit COPY / wwt ADD )</a>
    * patch parts of iso files without copying the entire file <a href='http://wit.wiimm.de/wit/cmd-edit.html'>( wit INFO & EDIT --id, --name, --modify )</a>
    * keep only files needed for use in SNEEK's DI when converting to FST ( --sneek )
    * keep only certain partitions <a href='http://wit.wiimm.de/options/scrubbing.html#psel'>( --psel= )</a>
    * listing games from folders & partitions <a href='http://wit.wiimm.de/wit/cmd-list-lll.html'>( wit LIST-LLL --sections )</a>
    * support for "cookie-cutter" names when copying games <a href='http://wit.wiimm.de/info/iso-images.html#esc'>( escape sequences in dest. path )</a>
    * format & recover WBFS partitions <a href='http://wit.wiimm.de/wwt/cmd-format.html'>( wwt FORMAT --recover )</a>
    * remove games from WBFS partitions <a href='http://wit.wiimm.de/wwt/cmd-remove.html'>( wwt REMOVE )</a>
    * using names from titles.txt for displaying names & on moving/copying <a href='http://wit.wiimm.de/wit/#opt'>( --titles= )</a>
  * WiiTDB
    * displaying covers for a given partition in a sexy coverflow
    * Downloading covers ( front, 3d, disc, full, full\_HiRes )
    * Locale determined from environmental variable.If the current language is not supported by WiiTDB, it will default to english
    * Insanely powerful WiiTDB searcher.  Including, but not limited to, support for 6 different comparison types in 3 different parameters and regex support when searching ID, Title, and synopsis.  If you can find a more powerful WiiTDB searcher, I'll eat my ballet shoes.
  * Aligning & shrinking gamecube games - tested and working with WODE
  * multilingual support determined by LANG environmental variable
  * embedded and external language files - ( embedded are far from complete, but support is there should somebody with to submit a more complete translation )
  * drag & drop file opening
  * dumping games from DVD+/-r to any supported format
  * open files & partitions via argv ( open with... )
  * support for running processes with root privileges with sudo
  * persistent settings stored in <your home folder>/QtWitGui.ini
  * Made with multitasking in mind - everything ( each instance of wit, parsing game contents, shrinking GC games, loading covers from the HDD, ... ) is done in its own thread, and there is no limit on how many threads can be running.  I leave it up to the user to decide if they are telling the program to do to many things at a time ( if it gets to the point that it starts lagging the PC its probably pushing that limit ).

screnshot from [r222](https://code.google.com/p/qtwitgui/source/detail?r=222) in ubuntu linux
<a href='http://img80.imageshack.us/img80/2439/wholeapp.png'> <img src='http://img80.imageshack.us/img80/2439/wholeapp.png' height='500' /> </a>

[r228](https://code.google.com/p/qtwitgui/source/detail?r=228) in OSx
<a href='http://img87.imageshack.us/img87/2237/228mac.png'> <img src='http://img87.imageshack.us/img87/2237/228mac.png' height='500' /> </a>



This is what the layout used to look like...<br>
<img src='http://img17.imageshack.us/img17/327/witgui90.png' height='375'>
<img src='http://img293.imageshack.us/img293/8944/witgui.png' height='375'>
<img src='http://img706.imageshack.us/img706/2995/witmac44.png' height='325'>
<img src='http://img63.imageshack.us/img63/8447/witxp45.png' height='325'>





wit page <a href='http://wit.wiimm.de/wit/index.html'>http://wit.wiimm.de/wit/index.html</a> <a href='http://wit.wiimm.de/download.html'>http://wit.wiimm.de/download.html</a>

<wiki:gadget url="http://www.ohloh.net/p/486500/widgets/project_cocomo.xml" height="240" border="0"/><br>
<br>
<wiki:gadget url="http://www.ohloh.net/p/486500/widgets/project_languages.xml" height="240" width="440" border="1"/><br>
<br>
<a href='https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=W9FP9ZCBWFQYQ&lc=US&item_name=giantpune&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donate_SM%2egif%3aNonHosted'><img src='https://www.paypal.com/en_US/i/btn/btn_donate_SM.gif' /></a>