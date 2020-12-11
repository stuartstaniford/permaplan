# permaplan
Open Source permaculture planning software.  In early stages of development, do not try to use yet.

Currently only believed to compile on MacOS Mojave and Catalina.

Installation notes for building the source:

Clone this repository.

Clone Imgui (https://github.com/ocornut/imgui.git) into the subdirectory called imgui.

Clone rapidjson (https://github.com/Tencent/rapidjson.git) into the subdirectory called rapidjson.

You'll need cglm (https://github.com/recp/cglm) installed somewhere (I do "brew install cglm").

Then:

make depend

make



