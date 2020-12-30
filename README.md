# permaplan
Permaculture planning software.  In early stages of development, do not try to use yet.  Shooting to have alpha semi-usable code mid 2021.  Intend to ultimately open-source, but right now is still all rights-reserved with no
license to copy.

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




