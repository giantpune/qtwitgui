<s>separate or remove makefiles from the trunk.  makefiles should be generated at compile time by qmake for the desired platform</s>

<s>make sure the project compiles on the current stable QT SDK for each platform</s>

<s>make sure that the project runs correctly on all platforms</s>

<s>remove extraneous files from the svn</s>

<s>save settings and remove hard-coded paths & settings</s>

<s>rearrange stuff and make the layout prettier</s>

<s>get some super cool images for this home page and also an icon for the program</s>

find a pretty, user-friendly way to implement the MIX command

Not supported by wit yet...
  * extract/replace/add/delete single files to/from a game
  * <s>get the IOS from the game as it is loaded into the gui</s>
  * <s>save changes to the same file that is used for input.  ie... open a game, change something, save the game without having to create a complete duplicate of the game ( works with .iso files already )</s>
  * <s>get the offset from files along with the size & name in the tree view</s>
  * <s>VC partitions.  DUMP shows them, but not ILIST.  they are also missing from --psel</s>
  * the fakesign detection in wit is not 100%. ( fails on games made with TheGhost )

<s>implement settings for "recurse" used in "wit LIST"</s>

<s>add static poling functions to WwtHandler like the ones in WitHandler to get the version and other such things.</s>

lots of testing.  current logic for the convert/copy dialog is largely untested.  the gamecube shrinking code worked in testing on its own - tested on 200+ games, all working with WODE ( which is picky about shrunken games ), but putting that code into this project required minor changes, and it has not undergone any testing aside from 1 or 2 games.

<s>re-implement language & svn version support in the gui</s>

<s>the "about" dialogs need to come back, and could use some work.</s>

<s>coverflow is working perfectly, but disabled until the download logic is worked out.</s>

<s>windows version needs everything about "root" and "sudo" removed.  instead use "wmic" to gather FS information.</s>

<s>the unix-filesystem check stuff is only tested on ubuntu.  it probably needs some adjusting to work properly on OSx and other linux distros.</s>  should work now for most situations

again, I need images.  this time, theres lots more.  currently there are just random placeholder icons from free online archives.

possibly correct for errors in WiiTDB.

Maybe add some option to disable the mdi or switch to a tabbed view.  The way windows7 handles this, it looks terrible.

<s>figure out a way to ignore certain folders or let the user decide folders to ignore.  probably add option to disable composing while listing HDDs...  windows version is INSANELY SLOW when listing partitions for sneek and ext3 partitions with 500 games.<br>
</s>