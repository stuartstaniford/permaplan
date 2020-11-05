#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'pmod'
# 'make clean'  removes all .o and executable files
#

# define the C++ compiler to use
CPP = clang++

# define any compile-time flags
CFLAGS = -Wall -g -std=c++11

# define any directories containing header files other than /usr/include
#
INCLUDES = -I include -I imgui -I imgui/examples

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -framework OpenGL -lglfw -lGLEW

# define the source files
SRCS = $(wildcard src/*.c) $(wildcard src/*.cpp)


# define the C object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.cpp=.o)

# define the executable file
MAIN = pmod

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
				@echo $(MAIN) has been compiled.

$(MAIN): $(OBJS)
			$(CPP) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.cpp.o:
				$(CPP) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
				$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
				makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

src/Arrow.o: include/Arrow.h include/VisualObject.h include/BoundingBox.h
src/Arrow.o: include/Shader.h include/VertexBufferCombo.h
src/Arrow.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Arrow.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/Arrow.o: include/HttpDebug.h include/TriangleBuffer.h
src/Arrow.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/BezierPatch.o: include/BezierPatch.h include/LandSurfaceRegion.h
src/BezierPatch.o: include/VisualObject.h include/BoundingBox.h
src/BezierPatch.o: include/Shader.h include/VertexBufferCombo.h
src/BezierPatch.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/BezierPatch.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/BezierPatch.o: include/HttpDebug.h include/TriangleBuffer.h
src/BezierPatch.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/BezierPatch.o: include/Quadtree.h include/DisplayList.h
src/BezierPatch.o: include/LandSurfaceRegionPlanar.h include/HeightMarker.h
src/BezierPatch.o: include/Arrow.h
src/BoundingBox.o: include/BoundingBox.h include/Shader.h
src/BoundingBox.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/BoundingBox.o: include/VertexBufferObject.h include/GlobalMacros.h
src/BoundingBox.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/Box.o: include/Box.h include/VisualObject.h include/BoundingBox.h
src/Box.o: include/Shader.h include/VertexBufferCombo.h
src/Box.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Box.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/Box.o: include/HttpDebug.h include/TriangleBuffer.h
src/Box.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/Camera.o: include/Camera.h include/Shader.h include/HttpDebug.h
src/ColoredAxes.o: include/ColoredAxes.h include/LineStripList.h
src/ColoredAxes.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/ColoredAxes.o: include/VertexBufferObject.h include/GlobalMacros.h
src/ColoredAxes.o: include/Logging.h include/Vertex.h include/Shader.h
src/ColoredAxes.o: include/HttpDebug.h
src/DisplayList.o: include/DisplayList.h include/VisualObject.h
src/DisplayList.o: include/BoundingBox.h include/Shader.h
src/DisplayList.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/DisplayList.o: include/VertexBufferObject.h include/GlobalMacros.h
src/DisplayList.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/DisplayList.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/DisplayList.o: include/ElementBufferObject.h include/HeightMarker.h
src/ElementBufferCombo.o: include/ElementBufferCombo.h
src/ElementBufferCombo.o: include/VertexArrayObject.h
src/ElementBufferCombo.o: include/VertexBufferObject.h include/GlobalMacros.h
src/ElementBufferCombo.o: include/Logging.h include/Vertex.h
src/ElementBufferCombo.o: include/ElementBufferObject.h
src/ElementBufferObject.o: include/ElementBufferObject.h include/Shader.h
src/Grid.o: include/Grid.h include/Shader.h include/VertexArrayObject.h
src/Grid.o: include/VertexBufferObject.h include/GlobalMacros.h
src/Grid.o: include/Logging.h include/Vertex.h include/LandSurface.h
src/Grid.o: include/PmodDesign.h include/rapidjson/document.h
src/Grid.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/Grid.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/Grid.o: include/rapidjson/encodings.h include/rapidjson/encodedstream.h
src/Grid.o: include/rapidjson/memorystream.h
src/Grid.o: include/rapidjson/internal/clzll.h include/rapidjson/rapidjson.h
src/Grid.o: include/rapidjson/internal/meta.h
src/Grid.o: include/rapidjson/internal/stack.h include/rapidjson/allocators.h
src/Grid.o: include/rapidjson/internal/swap.h
src/Grid.o: include/rapidjson/internal/strtod.h
src/Grid.o: include/rapidjson/internal/ieee754.h
src/Grid.o: include/rapidjson/internal/biginteger.h
src/Grid.o: include/rapidjson/internal/diyfp.h
src/Grid.o: include/rapidjson/internal/clzll.h
src/Grid.o: include/rapidjson/internal/pow10.h
src/Grid.o: include/rapidjson/error/error.h
src/Grid.o: include/rapidjson/internal/strfunc.h include/rapidjson/stream.h
src/Grid.o: include/rapidjson/writer.h include/rapidjson/internal/dtoa.h
src/Grid.o: include/rapidjson/internal/itoa.h
src/Grid.o: include/rapidjson/internal/itoa.h
src/Grid.o: include/rapidjson/stringbuffer.h include/rapidjson/stringbuffer.h
src/Grid.o: include/rapidjson/error/en.h include/rapidjson/error/error.h
src/Grid.o: include/PmodConfig.h include/TexturedRect.h include/Texture.h
src/Grid.o: include/Quadtree.h include/DisplayList.h include/VisualObject.h
src/Grid.o: include/BoundingBox.h include/VertexBufferCombo.h
src/Grid.o: include/HttpDebug.h include/TriangleBuffer.h
src/Grid.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/Grid.o: include/LandSurfaceRegionPlanar.h include/LandSurfaceRegion.h
src/Grid.o: include/Camera.h include/HeightMarker.h include/BezierPatch.h
src/Grid.o: include/LineStripList.h
src/HeightMarker.o: include/HeightMarker.h include/VisualObject.h
src/HeightMarker.o: include/BoundingBox.h include/Shader.h
src/HeightMarker.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/HeightMarker.o: include/VertexBufferObject.h include/GlobalMacros.h
src/HeightMarker.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/HeightMarker.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/HeightMarker.o: include/ElementBufferObject.h
src/HttpDebug.o: include/HttpDebug.h include/Scene.h include/Shader.h
src/HttpDebug.o: include/ColoredAxes.h include/LineStripList.h
src/HttpDebug.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/HttpDebug.o: include/VertexBufferObject.h include/GlobalMacros.h
src/HttpDebug.o: include/Logging.h include/Vertex.h include/PmodConfig.h
src/HttpDebug.o: include/LandSurface.h include/PmodDesign.h
src/HttpDebug.o: include/rapidjson/document.h include/rapidjson/reader.h
src/HttpDebug.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/HttpDebug.o: include/rapidjson/stream.h include/rapidjson/encodings.h
src/HttpDebug.o: include/rapidjson/encodedstream.h
src/HttpDebug.o: include/rapidjson/memorystream.h
src/HttpDebug.o: include/rapidjson/internal/clzll.h
src/HttpDebug.o: include/rapidjson/rapidjson.h
src/HttpDebug.o: include/rapidjson/internal/meta.h
src/HttpDebug.o: include/rapidjson/internal/stack.h
src/HttpDebug.o: include/rapidjson/allocators.h
src/HttpDebug.o: include/rapidjson/internal/swap.h
src/HttpDebug.o: include/rapidjson/internal/strtod.h
src/HttpDebug.o: include/rapidjson/internal/ieee754.h
src/HttpDebug.o: include/rapidjson/internal/biginteger.h
src/HttpDebug.o: include/rapidjson/internal/diyfp.h
src/HttpDebug.o: include/rapidjson/internal/clzll.h
src/HttpDebug.o: include/rapidjson/internal/pow10.h
src/HttpDebug.o: include/rapidjson/error/error.h
src/HttpDebug.o: include/rapidjson/internal/strfunc.h
src/HttpDebug.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/HttpDebug.o: include/rapidjson/internal/dtoa.h
src/HttpDebug.o: include/rapidjson/internal/itoa.h
src/HttpDebug.o: include/rapidjson/internal/itoa.h
src/HttpDebug.o: include/rapidjson/stringbuffer.h
src/HttpDebug.o: include/rapidjson/stringbuffer.h
src/HttpDebug.o: include/rapidjson/error/en.h include/rapidjson/error/error.h
src/HttpDebug.o: include/TexturedRect.h include/Texture.h include/Quadtree.h
src/HttpDebug.o: include/DisplayList.h include/VisualObject.h
src/HttpDebug.o: include/BoundingBox.h include/TriangleBuffer.h
src/HttpDebug.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/HttpDebug.o: include/LandSurfaceRegionPlanar.h
src/HttpDebug.o: include/LandSurfaceRegion.h include/Camera.h
src/HttpDebug.o: include/HeightMarker.h include/BezierPatch.h include/Grid.h
src/HttpDebug.o: include/Material.h include/LightingModel.h
src/LandSurface.o: include/Camera.h include/Shader.h include/HttpDebug.h
src/LandSurface.o: include/LandSurface.h include/PmodDesign.h
src/LandSurface.o: include/rapidjson/document.h include/rapidjson/reader.h
src/LandSurface.o: include/rapidjson/allocators.h
src/LandSurface.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/LandSurface.o: include/rapidjson/encodings.h
src/LandSurface.o: include/rapidjson/encodedstream.h
src/LandSurface.o: include/rapidjson/memorystream.h
src/LandSurface.o: include/rapidjson/internal/clzll.h
src/LandSurface.o: include/rapidjson/rapidjson.h
src/LandSurface.o: include/rapidjson/internal/meta.h
src/LandSurface.o: include/rapidjson/internal/stack.h
src/LandSurface.o: include/rapidjson/allocators.h
src/LandSurface.o: include/rapidjson/internal/swap.h
src/LandSurface.o: include/rapidjson/internal/strtod.h
src/LandSurface.o: include/rapidjson/internal/ieee754.h
src/LandSurface.o: include/rapidjson/internal/biginteger.h
src/LandSurface.o: include/rapidjson/internal/diyfp.h
src/LandSurface.o: include/rapidjson/internal/clzll.h
src/LandSurface.o: include/rapidjson/internal/pow10.h
src/LandSurface.o: include/rapidjson/error/error.h
src/LandSurface.o: include/rapidjson/internal/strfunc.h
src/LandSurface.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/LandSurface.o: include/rapidjson/internal/dtoa.h
src/LandSurface.o: include/rapidjson/internal/itoa.h
src/LandSurface.o: include/rapidjson/internal/itoa.h
src/LandSurface.o: include/rapidjson/stringbuffer.h
src/LandSurface.o: include/rapidjson/stringbuffer.h
src/LandSurface.o: include/rapidjson/error/en.h
src/LandSurface.o: include/rapidjson/error/error.h include/PmodConfig.h
src/LandSurface.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/LandSurface.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/LandSurface.o: include/TexturedRect.h include/Texture.h
src/LandSurface.o: include/Quadtree.h include/DisplayList.h
src/LandSurface.o: include/VisualObject.h include/BoundingBox.h
src/LandSurface.o: include/VertexBufferCombo.h include/TriangleBuffer.h
src/LandSurface.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/LandSurface.o: include/LandSurfaceRegionPlanar.h
src/LandSurface.o: include/LandSurfaceRegion.h include/HeightMarker.h
src/LandSurface.o: include/BezierPatch.h
src/LandSurfaceRegion.o: include/LandSurfaceRegion.h include/VisualObject.h
src/LandSurfaceRegion.o: include/BoundingBox.h include/Shader.h
src/LandSurfaceRegion.o: include/VertexBufferCombo.h
src/LandSurfaceRegion.o: include/VertexArrayObject.h
src/LandSurfaceRegion.o: include/VertexBufferObject.h include/GlobalMacros.h
src/LandSurfaceRegion.o: include/Logging.h include/Vertex.h
src/LandSurfaceRegion.o: include/HttpDebug.h include/TriangleBuffer.h
src/LandSurfaceRegion.o: include/ElementBufferCombo.h
src/LandSurfaceRegion.o: include/ElementBufferObject.h
src/LandSurfaceRegionPlanar.o: include/LandSurfaceRegionPlanar.h
src/LandSurfaceRegionPlanar.o: include/LandSurfaceRegion.h
src/LandSurfaceRegionPlanar.o: include/VisualObject.h include/BoundingBox.h
src/LandSurfaceRegionPlanar.o: include/Shader.h include/VertexBufferCombo.h
src/LandSurfaceRegionPlanar.o: include/VertexArrayObject.h
src/LandSurfaceRegionPlanar.o: include/VertexBufferObject.h
src/LandSurfaceRegionPlanar.o: include/GlobalMacros.h include/Logging.h
src/LandSurfaceRegionPlanar.o: include/Vertex.h include/HttpDebug.h
src/LandSurfaceRegionPlanar.o: include/TriangleBuffer.h
src/LandSurfaceRegionPlanar.o: include/ElementBufferCombo.h
src/LandSurfaceRegionPlanar.o: include/ElementBufferObject.h
src/LightingModel.o: include/LightingModel.h include/Shader.h
src/LineStripList.o: include/LineStripList.h include/VertexBufferCombo.h
src/LineStripList.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/LineStripList.o: include/GlobalMacros.h include/Logging.h
src/LineStripList.o: include/Vertex.h include/Shader.h include/HttpDebug.h
src/Logging.o: include/Logging.h include/Vertex.h include/GlobalMacros.h
src/Material.o: include/Material.h include/Texture.h include/Shader.h
src/Material.o: include/HttpDebug.h
src/PmodConfig.o: include/PmodConfig.h
src/PmodDesign.o: include/PmodDesign.h include/rapidjson/document.h
src/PmodDesign.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/PmodDesign.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/PmodDesign.o: include/rapidjson/encodings.h
src/PmodDesign.o: include/rapidjson/encodedstream.h
src/PmodDesign.o: include/rapidjson/memorystream.h
src/PmodDesign.o: include/rapidjson/internal/clzll.h
src/PmodDesign.o: include/rapidjson/rapidjson.h
src/PmodDesign.o: include/rapidjson/internal/meta.h
src/PmodDesign.o: include/rapidjson/internal/stack.h
src/PmodDesign.o: include/rapidjson/allocators.h
src/PmodDesign.o: include/rapidjson/internal/swap.h
src/PmodDesign.o: include/rapidjson/internal/strtod.h
src/PmodDesign.o: include/rapidjson/internal/ieee754.h
src/PmodDesign.o: include/rapidjson/internal/biginteger.h
src/PmodDesign.o: include/rapidjson/internal/diyfp.h
src/PmodDesign.o: include/rapidjson/internal/clzll.h
src/PmodDesign.o: include/rapidjson/internal/pow10.h
src/PmodDesign.o: include/rapidjson/error/error.h
src/PmodDesign.o: include/rapidjson/internal/strfunc.h
src/PmodDesign.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/PmodDesign.o: include/rapidjson/internal/dtoa.h
src/PmodDesign.o: include/rapidjson/internal/itoa.h
src/PmodDesign.o: include/rapidjson/internal/itoa.h
src/PmodDesign.o: include/rapidjson/stringbuffer.h
src/PmodDesign.o: include/rapidjson/stringbuffer.h
src/PmodDesign.o: include/rapidjson/error/en.h
src/PmodDesign.o: include/rapidjson/error/error.h include/PmodConfig.h
src/PmodDesign.o: include/loadFileToBuf.h
src/Quadtree.o: include/Quadtree.h include/Shader.h
src/Quadtree.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Quadtree.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/Quadtree.o: include/DisplayList.h include/VisualObject.h
src/Quadtree.o: include/BoundingBox.h include/VertexBufferCombo.h
src/Quadtree.o: include/HttpDebug.h include/TriangleBuffer.h
src/Quadtree.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/Quadtree.o: include/LandSurfaceRegionPlanar.h include/LandSurfaceRegion.h
src/Quadtree.o: include/Scene.h include/ColoredAxes.h include/LineStripList.h
src/Quadtree.o: include/PmodConfig.h include/LandSurface.h
src/Quadtree.o: include/PmodDesign.h include/rapidjson/document.h
src/Quadtree.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/Quadtree.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/Quadtree.o: include/rapidjson/encodings.h
src/Quadtree.o: include/rapidjson/encodedstream.h
src/Quadtree.o: include/rapidjson/memorystream.h
src/Quadtree.o: include/rapidjson/internal/clzll.h
src/Quadtree.o: include/rapidjson/rapidjson.h
src/Quadtree.o: include/rapidjson/internal/meta.h
src/Quadtree.o: include/rapidjson/internal/stack.h
src/Quadtree.o: include/rapidjson/allocators.h
src/Quadtree.o: include/rapidjson/internal/swap.h
src/Quadtree.o: include/rapidjson/internal/strtod.h
src/Quadtree.o: include/rapidjson/internal/ieee754.h
src/Quadtree.o: include/rapidjson/internal/biginteger.h
src/Quadtree.o: include/rapidjson/internal/diyfp.h
src/Quadtree.o: include/rapidjson/internal/clzll.h
src/Quadtree.o: include/rapidjson/internal/pow10.h
src/Quadtree.o: include/rapidjson/error/error.h
src/Quadtree.o: include/rapidjson/internal/strfunc.h
src/Quadtree.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/Quadtree.o: include/rapidjson/internal/dtoa.h
src/Quadtree.o: include/rapidjson/internal/itoa.h
src/Quadtree.o: include/rapidjson/internal/itoa.h
src/Quadtree.o: include/rapidjson/stringbuffer.h
src/Quadtree.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Quadtree.o: include/rapidjson/error/error.h include/TexturedRect.h
src/Quadtree.o: include/Texture.h include/Camera.h include/HeightMarker.h
src/Quadtree.o: include/BezierPatch.h include/Grid.h include/Material.h
src/Quadtree.o: include/LightingModel.h
src/Scene.o: include/Scene.h include/Shader.h include/ColoredAxes.h
src/Scene.o: include/LineStripList.h include/VertexBufferCombo.h
src/Scene.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Scene.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/Scene.o: include/HttpDebug.h include/PmodConfig.h include/LandSurface.h
src/Scene.o: include/PmodDesign.h include/rapidjson/document.h
src/Scene.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/Scene.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/Scene.o: include/rapidjson/encodings.h include/rapidjson/encodedstream.h
src/Scene.o: include/rapidjson/memorystream.h
src/Scene.o: include/rapidjson/internal/clzll.h include/rapidjson/rapidjson.h
src/Scene.o: include/rapidjson/internal/meta.h
src/Scene.o: include/rapidjson/internal/stack.h
src/Scene.o: include/rapidjson/allocators.h include/rapidjson/internal/swap.h
src/Scene.o: include/rapidjson/internal/strtod.h
src/Scene.o: include/rapidjson/internal/ieee754.h
src/Scene.o: include/rapidjson/internal/biginteger.h
src/Scene.o: include/rapidjson/internal/diyfp.h
src/Scene.o: include/rapidjson/internal/clzll.h
src/Scene.o: include/rapidjson/internal/pow10.h
src/Scene.o: include/rapidjson/error/error.h
src/Scene.o: include/rapidjson/internal/strfunc.h include/rapidjson/stream.h
src/Scene.o: include/rapidjson/writer.h include/rapidjson/internal/dtoa.h
src/Scene.o: include/rapidjson/internal/itoa.h
src/Scene.o: include/rapidjson/internal/itoa.h
src/Scene.o: include/rapidjson/stringbuffer.h
src/Scene.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Scene.o: include/rapidjson/error/error.h include/TexturedRect.h
src/Scene.o: include/Texture.h include/Quadtree.h include/DisplayList.h
src/Scene.o: include/VisualObject.h include/BoundingBox.h
src/Scene.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/Scene.o: include/ElementBufferObject.h include/LandSurfaceRegionPlanar.h
src/Scene.o: include/LandSurfaceRegion.h include/Camera.h
src/Scene.o: include/HeightMarker.h include/BezierPatch.h include/Grid.h
src/Scene.o: include/Material.h include/LightingModel.h include/Box.h
src/ScriptController.o: include/VertexBufferObject.h include/GlobalMacros.h
src/ScriptController.o: include/Logging.h include/Vertex.h
src/ScriptController.o: include/ScriptController.h include/HttpDebug.h
src/ScriptController.o: include/PmodDesign.h include/rapidjson/document.h
src/ScriptController.o: include/rapidjson/reader.h
src/ScriptController.o: include/rapidjson/allocators.h
src/ScriptController.o: include/rapidjson/rapidjson.h
src/ScriptController.o: include/rapidjson/stream.h
src/ScriptController.o: include/rapidjson/encodings.h
src/ScriptController.o: include/rapidjson/encodedstream.h
src/ScriptController.o: include/rapidjson/memorystream.h
src/ScriptController.o: include/rapidjson/internal/clzll.h
src/ScriptController.o: include/rapidjson/rapidjson.h
src/ScriptController.o: include/rapidjson/internal/meta.h
src/ScriptController.o: include/rapidjson/internal/stack.h
src/ScriptController.o: include/rapidjson/allocators.h
src/ScriptController.o: include/rapidjson/internal/swap.h
src/ScriptController.o: include/rapidjson/internal/strtod.h
src/ScriptController.o: include/rapidjson/internal/ieee754.h
src/ScriptController.o: include/rapidjson/internal/biginteger.h
src/ScriptController.o: include/rapidjson/internal/diyfp.h
src/ScriptController.o: include/rapidjson/internal/clzll.h
src/ScriptController.o: include/rapidjson/internal/pow10.h
src/ScriptController.o: include/rapidjson/error/error.h
src/ScriptController.o: include/rapidjson/internal/strfunc.h
src/ScriptController.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/ScriptController.o: include/rapidjson/internal/dtoa.h
src/ScriptController.o: include/rapidjson/internal/itoa.h
src/ScriptController.o: include/rapidjson/internal/itoa.h
src/ScriptController.o: include/rapidjson/stringbuffer.h
src/ScriptController.o: include/rapidjson/stringbuffer.h
src/ScriptController.o: include/rapidjson/error/en.h
src/ScriptController.o: include/rapidjson/error/error.h include/PmodConfig.h
src/ScriptController.o: include/Camera.h include/Shader.h
src/Shader.o: include/loadFileToBuf.h include/Shader.h
src/Texture.o: include/Texture.h include/Shader.h include/stb_image.h
src/TextureAtlas.o: include/TextureAtlas.h include/Texture.h include/Shader.h
src/TextureAtlas.o: include/Logging.h include/GlobalMacros.h
src/TexturedRect.o: include/TexturedRect.h include/Shader.h
src/TexturedRect.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/TexturedRect.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/TexturedRect.o: include/Texture.h
src/Timeval.o: include/Timeval.h
src/TriangleBuffer.o: include/Shader.h include/TriangleBuffer.h
src/TriangleBuffer.o: include/ElementBufferCombo.h
src/TriangleBuffer.o: include/VertexArrayObject.h
src/TriangleBuffer.o: include/VertexBufferObject.h include/GlobalMacros.h
src/TriangleBuffer.o: include/Logging.h include/Vertex.h
src/TriangleBuffer.o: include/ElementBufferObject.h include/HttpDebug.h
src/Vertex.o: include/Vertex.h include/GlobalMacros.h include/Logging.h
src/Vertex.o: include/Shader.h
src/VertexArrayObject.o: include/VertexArrayObject.h include/Shader.h
src/VertexBufferCombo.o: include/VertexBufferCombo.h
src/VertexBufferCombo.o: include/VertexArrayObject.h
src/VertexBufferCombo.o: include/VertexBufferObject.h include/GlobalMacros.h
src/VertexBufferCombo.o: include/Logging.h include/Vertex.h
src/VertexBufferObject.o: include/VertexBufferObject.h include/GlobalMacros.h
src/VertexBufferObject.o: include/Logging.h include/Vertex.h include/Shader.h
src/VisualObject.o: include/VisualObject.h include/BoundingBox.h
src/VisualObject.o: include/Shader.h include/VertexBufferCombo.h
src/VisualObject.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/VisualObject.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/VisualObject.o: include/HttpDebug.h include/TriangleBuffer.h
src/VisualObject.o: include/ElementBufferCombo.h
src/VisualObject.o: include/ElementBufferObject.h
src/Window3D.o: imgui/examples/imgui_impl_opengl3.h imgui/imgui.h
src/Window3D.o: imgui/imconfig.h imgui/examples/imgui_impl_glfw.h
src/Window3D.o: include/Window3D.h include/Scene.h include/Shader.h
src/Window3D.o: include/ColoredAxes.h include/LineStripList.h
src/Window3D.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/Window3D.o: include/VertexBufferObject.h include/GlobalMacros.h
src/Window3D.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/Window3D.o: include/PmodConfig.h include/LandSurface.h
src/Window3D.o: include/PmodDesign.h include/rapidjson/document.h
src/Window3D.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/Window3D.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/Window3D.o: include/rapidjson/encodings.h
src/Window3D.o: include/rapidjson/encodedstream.h
src/Window3D.o: include/rapidjson/memorystream.h
src/Window3D.o: include/rapidjson/internal/clzll.h
src/Window3D.o: include/rapidjson/rapidjson.h
src/Window3D.o: include/rapidjson/internal/meta.h
src/Window3D.o: include/rapidjson/internal/stack.h
src/Window3D.o: include/rapidjson/allocators.h
src/Window3D.o: include/rapidjson/internal/swap.h
src/Window3D.o: include/rapidjson/internal/strtod.h
src/Window3D.o: include/rapidjson/internal/ieee754.h
src/Window3D.o: include/rapidjson/internal/biginteger.h
src/Window3D.o: include/rapidjson/internal/diyfp.h
src/Window3D.o: include/rapidjson/internal/clzll.h
src/Window3D.o: include/rapidjson/internal/pow10.h
src/Window3D.o: include/rapidjson/error/error.h
src/Window3D.o: include/rapidjson/internal/strfunc.h
src/Window3D.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/Window3D.o: include/rapidjson/internal/dtoa.h
src/Window3D.o: include/rapidjson/internal/itoa.h
src/Window3D.o: include/rapidjson/internal/itoa.h
src/Window3D.o: include/rapidjson/stringbuffer.h
src/Window3D.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Window3D.o: include/rapidjson/error/error.h include/TexturedRect.h
src/Window3D.o: include/Texture.h include/Quadtree.h include/DisplayList.h
src/Window3D.o: include/VisualObject.h include/BoundingBox.h
src/Window3D.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/Window3D.o: include/ElementBufferObject.h
src/Window3D.o: include/LandSurfaceRegionPlanar.h include/LandSurfaceRegion.h
src/Window3D.o: include/Camera.h include/HeightMarker.h include/BezierPatch.h
src/Window3D.o: include/Grid.h include/Material.h include/LightingModel.h
src/Window3D.o: include/ScriptController.h include/Timeval.h
src/Window3D.o: include/PmodException.h
src/imgui.o: imgui/imgui.h imgui/imconfig.h imgui/imgui_internal.h
src/imgui.o: imgui/imstb_textedit.h
src/imgui_demo.o: imgui/imgui.h imgui/imconfig.h
src/imgui_draw.o: imgui/imgui.h imgui/imconfig.h imgui/imgui_internal.h
src/imgui_draw.o: imgui/imstb_textedit.h imgui/imstb_rectpack.h
src/imgui_draw.o: imgui/imstb_truetype.h
src/imgui_impl_glfw.o: imgui/imgui.h imgui/imconfig.h
src/imgui_impl_glfw.o: imgui/examples/imgui_impl_glfw.h
src/imgui_impl_opengl3.o: imgui/imgui.h imgui/imconfig.h
src/imgui_impl_opengl3.o: imgui/examples/imgui_impl_opengl3.h
src/imgui_widgets.o: imgui/imgui.h imgui/imconfig.h imgui/imgui_internal.h
src/imgui_widgets.o: imgui/imstb_textedit.h
src/loadFileToBuf.o: include/loadFileToBuf.h
src/main.o: include/Window3D.h include/Scene.h include/Shader.h
src/main.o: include/ColoredAxes.h include/LineStripList.h
src/main.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/main.o: include/VertexBufferObject.h include/GlobalMacros.h
src/main.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/main.o: include/PmodConfig.h include/LandSurface.h include/PmodDesign.h
src/main.o: include/rapidjson/document.h include/rapidjson/reader.h
src/main.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/main.o: include/rapidjson/stream.h include/rapidjson/encodings.h
src/main.o: include/rapidjson/encodedstream.h
src/main.o: include/rapidjson/memorystream.h
src/main.o: include/rapidjson/internal/clzll.h include/rapidjson/rapidjson.h
src/main.o: include/rapidjson/internal/meta.h
src/main.o: include/rapidjson/internal/stack.h include/rapidjson/allocators.h
src/main.o: include/rapidjson/internal/swap.h
src/main.o: include/rapidjson/internal/strtod.h
src/main.o: include/rapidjson/internal/ieee754.h
src/main.o: include/rapidjson/internal/biginteger.h
src/main.o: include/rapidjson/internal/diyfp.h
src/main.o: include/rapidjson/internal/clzll.h
src/main.o: include/rapidjson/internal/pow10.h
src/main.o: include/rapidjson/error/error.h
src/main.o: include/rapidjson/internal/strfunc.h include/rapidjson/stream.h
src/main.o: include/rapidjson/writer.h include/rapidjson/internal/dtoa.h
src/main.o: include/rapidjson/internal/itoa.h
src/main.o: include/rapidjson/internal/itoa.h
src/main.o: include/rapidjson/stringbuffer.h include/rapidjson/stringbuffer.h
src/main.o: include/rapidjson/error/en.h include/rapidjson/error/error.h
src/main.o: include/TexturedRect.h include/Texture.h include/Quadtree.h
src/main.o: include/DisplayList.h include/VisualObject.h
src/main.o: include/BoundingBox.h include/TriangleBuffer.h
src/main.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/main.o: include/LandSurfaceRegionPlanar.h include/LandSurfaceRegion.h
src/main.o: include/Camera.h include/HeightMarker.h include/BezierPatch.h
src/main.o: include/Grid.h include/Material.h include/LightingModel.h
src/main.o: imgui/imgui.h imgui/imconfig.h include/ScriptController.h
src/main.o: include/Timeval.h include/TextureAtlas.h
