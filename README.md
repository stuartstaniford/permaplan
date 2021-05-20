# permaplan
Permaculture planning software.  In early stages of development, do not try to use yet.  Shooting to have alpha semi-usable code late in 2021 (so I can use it for design work over the winter before the 2022 growing season). I intend to ultimately open-source it, but right now it is still all rights-reserved with no license to copy.

Code is in C++, but I speak it with a very strong C accent (I was a C network security guy for over twenty years).  Some of this is a matter of not having learnt better yet, and some of it is conscious choice for performance reasons.  So my coding style is evolving at the moment.  Test and other admin scripts are in Perl.  The system is currently only believed to compile on MacOS Mojave, Catalina, and Big Sur.  Test scripts will not work below Big Sur (ancient Perl versions on prior MacOS's).  However, it's written in a manner intended to be fairly portable and my intent is to port it to Windows and Linux in due course.  

Installation notes for building the source:

Clone this repository.

Clone Imgui (https://github.com/ocornut/imgui.git) into the subdirectory called imgui.

Clone rapidjson (https://github.com/Tencent/rapidjson.git) into the subdirectory called rapidjson.

You'll need cglm (https://github.com/recp/cglm) installed somewhere (I do "brew install cglm").

Then:

make depend

make

Additional appreciations:

Sean Barrett for stb_image.h and stb_image_write.h.

## Note on target expertise levels

The application interface should be usable by anyone with an interest in gardening or land management who is generally computer literate as a user of modern applications (browers, word processors, etc).  It shouldn't require detailed knowledge of botany, quantitative skills (like spreadsheet generation or data analysis), or ability to edit text files.

Extending the application with new OTDL files to add additional species is a task where the target expertise level is someone with a scientific background in botany, forestry, plant physiology, or something similar (or the ability and willingness to acquire that expertise as they go) as well as the ability to edit text files and maintain a structured data file (ie JSON).  Eg a grad student in ecology, forestry, or similar is the target level expected here.  But OTDL should be manageable without any knowledge of programming, C++, computer graphics, etc.

Extending the source code of the application itself requires a good knowledge of multi-threaded programming in C/C++ as well as a decent knowledge of computer graphics and of botany.




