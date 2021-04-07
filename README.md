# permaplan
Permaculture planning software.  In early stages of development, do not try to use yet.  Shooting to have alpha semi-usable code fall of 2021.  Intend to ultimately open-source, but right now is still all rights-reserved with no license to copy.

Currently only believed to compile on MacOS Mojave and Catalina.  Code is in C++, but I speak it with a very strong C accent (I was a C network security guy for over twenty years).

Installation notes for building the source:

Clone this repository.

Clone Imgui (https://github.com/ocornut/imgui.git) into the subdirectory called imgui.

Clone rapidjson (https://github.com/Tencent/rapidjson.git) into the subdirectory called rapidjson.

You'll need cglm (https://github.com/recp/cglm) installed somewhere (I do "brew install cglm").

Then:

make depend

make

Test suites depend on jq (https://stedolan.github.io/jq/ or "brew install jq").

Additional appreciations:

Sean Barrett for stb_image.h and stb_image_write.h.

## Note on target expertise levels

The application interface should be usable by anyone with an interest in gardening or land management who is generally computer literate as a user of modern applications (browers, word processors, etc).  It shouldn't require detailed knowledge of botany, quantitative skills (like spreadsheet generation or data analysis), or ability to edit text files.

Extending the application with new OTDL files to add additional species is a task where the target expertise level is someone with a scientific background in botany, forestry, plant physiology, or something similar (or the ability and willingness to acquire that expertise as they go) as well as the ability to edit text files and maintain a structured data file (ie JSON).  Eg a grad student in ecology, forestry, or similar is the target level expected here.  But OTDL should be manageable without any knowledge of programming, C++, computer graphics, etc.

Extending the source code of the application itself requires a good knowledge of multi-threaded programming in C/C++ as well as a decent knowledge of computer graphics and of botany.




