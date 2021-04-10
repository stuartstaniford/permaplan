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
INCLUDES = -I include -I imgui -I imgui/backends -I /usr/local/include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -framework OpenGL -lglfw -lGLEW

# define the source files
SRCS = $(wildcard src/*.cpp)


# define the C++ object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.cpp=.o)

# define the executable file
MAIN = permaplan

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
				$(RM) src/*.o *~ $(MAIN)

depend: $(SRCS)
				makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

src/Arrow.o: include/Cylinder.h include/AxialElement.h
src/Arrow.o: include/VisualElement.h include/TriangleBuffer.h
src/Arrow.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/Arrow.o: include/VertexBufferObject.h /usr/local/include/cglm/cglm.h
src/Arrow.o: /usr/local/include/cglm/common.h /usr/local/include/cglm/types.h
src/Arrow.o: /usr/local/include/cglm/simd/intrin.h
src/Arrow.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Arrow.o: /usr/local/include/cglm/vec2-ext.h
src/Arrow.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Arrow.o: /usr/local/include/cglm/vec4-ext.h
src/Arrow.o: /usr/local/include/cglm/vec3-ext.h
src/Arrow.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Arrow.o: /usr/local/include/cglm/mat2.h /usr/local/include/cglm/affine.h
src/Arrow.o: /usr/local/include/cglm/affine-mat.h
src/Arrow.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Arrow.o: /usr/local/include/cglm/frustum.h /usr/local/include/cglm/quat.h
src/Arrow.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Arrow.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Arrow.o: /usr/local/include/cglm/project.h
src/Arrow.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Arrow.o: /usr/local/include/cglm/curve.h /usr/local/include/cglm/bezier.h
src/Arrow.o: /usr/local/include/cglm/ray.h /usr/local/include/cglm/affine2d.h
src/Arrow.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Arrow.o: include/Global.h include/Logging.h include/MemoryTracker.h
src/Arrow.o: include/HttpDebug.h include/Vertex.h
src/Arrow.o: include/ElementBufferObject.h include/BoundingBox.h
src/Arrow.o: include/Shader.h include/VertexBufferCombo.h
src/Arrow.o: include/TruncatedCone.h include/Arrow.h include/VisualObject.h
src/Arrow.o: include/Lockable.h
src/AxialElement.o: include/AxialElement.h include/VisualElement.h
src/AxialElement.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/AxialElement.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/AxialElement.o: /usr/local/include/cglm/cglm.h
src/AxialElement.o: /usr/local/include/cglm/common.h
src/AxialElement.o: /usr/local/include/cglm/types.h
src/AxialElement.o: /usr/local/include/cglm/simd/intrin.h
src/AxialElement.o: /usr/local/include/cglm/vec2.h
src/AxialElement.o: /usr/local/include/cglm/util.h
src/AxialElement.o: /usr/local/include/cglm/vec2-ext.h
src/AxialElement.o: /usr/local/include/cglm/vec3.h
src/AxialElement.o: /usr/local/include/cglm/vec4.h
src/AxialElement.o: /usr/local/include/cglm/vec4-ext.h
src/AxialElement.o: /usr/local/include/cglm/vec3-ext.h
src/AxialElement.o: /usr/local/include/cglm/mat4.h
src/AxialElement.o: /usr/local/include/cglm/mat3.h
src/AxialElement.o: /usr/local/include/cglm/mat2.h
src/AxialElement.o: /usr/local/include/cglm/affine.h
src/AxialElement.o: /usr/local/include/cglm/affine-mat.h
src/AxialElement.o: /usr/local/include/cglm/cam.h
src/AxialElement.o: /usr/local/include/cglm/plane.h
src/AxialElement.o: /usr/local/include/cglm/frustum.h
src/AxialElement.o: /usr/local/include/cglm/quat.h
src/AxialElement.o: /usr/local/include/cglm/euler.h
src/AxialElement.o: /usr/local/include/cglm/box.h
src/AxialElement.o: /usr/local/include/cglm/color.h
src/AxialElement.o: /usr/local/include/cglm/io.h
src/AxialElement.o: /usr/local/include/cglm/project.h
src/AxialElement.o: /usr/local/include/cglm/sphere.h
src/AxialElement.o: /usr/local/include/cglm/ease.h
src/AxialElement.o: /usr/local/include/cglm/curve.h
src/AxialElement.o: /usr/local/include/cglm/bezier.h
src/AxialElement.o: /usr/local/include/cglm/ray.h
src/AxialElement.o: /usr/local/include/cglm/affine2d.h
src/AxialElement.o: /usr/local/include/GL/glew.h
src/AxialElement.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/AxialElement.o: include/Logging.h include/MemoryTracker.h
src/AxialElement.o: include/HttpDebug.h include/Vertex.h
src/AxialElement.o: include/ElementBufferObject.h include/BoundingBox.h
src/AxialElement.o: include/Shader.h include/VertexBufferCombo.h
src/BezierPatch.o: include/BezierPatch.h include/LandSurfaceRegion.h
src/BezierPatch.o: include/VisualObject.h include/VisualElement.h
src/BezierPatch.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/BezierPatch.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/BezierPatch.o: /usr/local/include/cglm/cglm.h
src/BezierPatch.o: /usr/local/include/cglm/common.h
src/BezierPatch.o: /usr/local/include/cglm/types.h
src/BezierPatch.o: /usr/local/include/cglm/simd/intrin.h
src/BezierPatch.o: /usr/local/include/cglm/vec2.h
src/BezierPatch.o: /usr/local/include/cglm/util.h
src/BezierPatch.o: /usr/local/include/cglm/vec2-ext.h
src/BezierPatch.o: /usr/local/include/cglm/vec3.h
src/BezierPatch.o: /usr/local/include/cglm/vec4.h
src/BezierPatch.o: /usr/local/include/cglm/vec4-ext.h
src/BezierPatch.o: /usr/local/include/cglm/vec3-ext.h
src/BezierPatch.o: /usr/local/include/cglm/mat4.h
src/BezierPatch.o: /usr/local/include/cglm/mat3.h
src/BezierPatch.o: /usr/local/include/cglm/mat2.h
src/BezierPatch.o: /usr/local/include/cglm/affine.h
src/BezierPatch.o: /usr/local/include/cglm/affine-mat.h
src/BezierPatch.o: /usr/local/include/cglm/cam.h
src/BezierPatch.o: /usr/local/include/cglm/plane.h
src/BezierPatch.o: /usr/local/include/cglm/frustum.h
src/BezierPatch.o: /usr/local/include/cglm/quat.h
src/BezierPatch.o: /usr/local/include/cglm/euler.h
src/BezierPatch.o: /usr/local/include/cglm/box.h
src/BezierPatch.o: /usr/local/include/cglm/color.h
src/BezierPatch.o: /usr/local/include/cglm/io.h
src/BezierPatch.o: /usr/local/include/cglm/project.h
src/BezierPatch.o: /usr/local/include/cglm/sphere.h
src/BezierPatch.o: /usr/local/include/cglm/ease.h
src/BezierPatch.o: /usr/local/include/cglm/curve.h
src/BezierPatch.o: /usr/local/include/cglm/bezier.h
src/BezierPatch.o: /usr/local/include/cglm/ray.h
src/BezierPatch.o: /usr/local/include/cglm/affine2d.h
src/BezierPatch.o: /usr/local/include/GL/glew.h
src/BezierPatch.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/BezierPatch.o: include/Logging.h include/MemoryTracker.h
src/BezierPatch.o: include/HttpDebug.h include/Vertex.h
src/BezierPatch.o: include/ElementBufferObject.h include/BoundingBox.h
src/BezierPatch.o: include/Shader.h include/VertexBufferCombo.h
src/BezierPatch.o: include/Lockable.h include/Quadtree.h
src/BezierPatch.o: include/DisplayList.h include/LandSurfaceRegionPlanar.h
src/BezierPatch.o: include/HeightMarker.h include/Arrow.h
src/Boundary.o: include/Boundary.h /usr/local/include/cglm/cglm.h
src/Boundary.o: /usr/local/include/cglm/common.h
src/Boundary.o: /usr/local/include/cglm/types.h
src/Boundary.o: /usr/local/include/cglm/simd/intrin.h
src/Boundary.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Boundary.o: /usr/local/include/cglm/vec2-ext.h
src/Boundary.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Boundary.o: /usr/local/include/cglm/vec4-ext.h
src/Boundary.o: /usr/local/include/cglm/vec3-ext.h
src/Boundary.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Boundary.o: /usr/local/include/cglm/mat2.h
src/Boundary.o: /usr/local/include/cglm/affine.h
src/Boundary.o: /usr/local/include/cglm/affine-mat.h
src/Boundary.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Boundary.o: /usr/local/include/cglm/frustum.h
src/Boundary.o: /usr/local/include/cglm/quat.h
src/Boundary.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Boundary.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Boundary.o: /usr/local/include/cglm/project.h
src/Boundary.o: /usr/local/include/cglm/sphere.h
src/Boundary.o: /usr/local/include/cglm/ease.h
src/Boundary.o: /usr/local/include/cglm/curve.h
src/Boundary.o: /usr/local/include/cglm/bezier.h
src/Boundary.o: /usr/local/include/cglm/ray.h
src/Boundary.o: /usr/local/include/cglm/affine2d.h include/HttpDebug.h
src/Boundary.o: include/rapidjson/document.h include/rapidjson/reader.h
src/Boundary.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/Boundary.o: include/rapidjson/stream.h include/rapidjson/encodings.h
src/Boundary.o: include/rapidjson/encodedstream.h
src/Boundary.o: include/rapidjson/memorystream.h
src/Boundary.o: include/rapidjson/internal/clzll.h
src/Boundary.o: include/rapidjson/rapidjson.h
src/Boundary.o: include/rapidjson/internal/meta.h
src/Boundary.o: include/rapidjson/internal/stack.h
src/Boundary.o: include/rapidjson/allocators.h
src/Boundary.o: include/rapidjson/internal/swap.h
src/Boundary.o: include/rapidjson/internal/strtod.h
src/Boundary.o: include/rapidjson/internal/ieee754.h
src/Boundary.o: include/rapidjson/internal/biginteger.h
src/Boundary.o: include/rapidjson/internal/diyfp.h
src/Boundary.o: include/rapidjson/internal/clzll.h
src/Boundary.o: include/rapidjson/internal/pow10.h
src/Boundary.o: include/rapidjson/error/error.h
src/Boundary.o: include/rapidjson/internal/strfunc.h
src/Boundary.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/Boundary.o: include/rapidjson/internal/dtoa.h
src/Boundary.o: include/rapidjson/internal/itoa.h
src/Boundary.o: include/rapidjson/internal/itoa.h
src/Boundary.o: include/rapidjson/stringbuffer.h
src/Boundary.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Boundary.o: include/rapidjson/error/error.h include/PmodConfig.h
src/Boundary.o: include/Logging.h include/MemoryTracker.h include/Global.h
src/BoundingBox.o: /usr/local/include/GL/glew.h
src/BoundingBox.o: /usr/local/include/GLFW/glfw3.h include/BoundingBox.h
src/BoundingBox.o: /usr/local/include/cglm/cglm.h
src/BoundingBox.o: /usr/local/include/cglm/common.h
src/BoundingBox.o: /usr/local/include/cglm/types.h
src/BoundingBox.o: /usr/local/include/cglm/simd/intrin.h
src/BoundingBox.o: /usr/local/include/cglm/vec2.h
src/BoundingBox.o: /usr/local/include/cglm/util.h
src/BoundingBox.o: /usr/local/include/cglm/vec2-ext.h
src/BoundingBox.o: /usr/local/include/cglm/vec3.h
src/BoundingBox.o: /usr/local/include/cglm/vec4.h
src/BoundingBox.o: /usr/local/include/cglm/vec4-ext.h
src/BoundingBox.o: /usr/local/include/cglm/vec3-ext.h
src/BoundingBox.o: /usr/local/include/cglm/mat4.h
src/BoundingBox.o: /usr/local/include/cglm/mat3.h
src/BoundingBox.o: /usr/local/include/cglm/mat2.h
src/BoundingBox.o: /usr/local/include/cglm/affine.h
src/BoundingBox.o: /usr/local/include/cglm/affine-mat.h
src/BoundingBox.o: /usr/local/include/cglm/cam.h
src/BoundingBox.o: /usr/local/include/cglm/plane.h
src/BoundingBox.o: /usr/local/include/cglm/frustum.h
src/BoundingBox.o: /usr/local/include/cglm/quat.h
src/BoundingBox.o: /usr/local/include/cglm/euler.h
src/BoundingBox.o: /usr/local/include/cglm/box.h
src/BoundingBox.o: /usr/local/include/cglm/color.h
src/BoundingBox.o: /usr/local/include/cglm/io.h
src/BoundingBox.o: /usr/local/include/cglm/project.h
src/BoundingBox.o: /usr/local/include/cglm/sphere.h
src/BoundingBox.o: /usr/local/include/cglm/ease.h
src/BoundingBox.o: /usr/local/include/cglm/curve.h
src/BoundingBox.o: /usr/local/include/cglm/bezier.h
src/BoundingBox.o: /usr/local/include/cglm/ray.h
src/BoundingBox.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/BoundingBox.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/BoundingBox.o: include/VertexBufferObject.h include/Global.h
src/BoundingBox.o: include/Logging.h include/MemoryTracker.h
src/BoundingBox.o: include/HttpDebug.h include/Vertex.h
src/Box.o: include/Box.h include/VisualObject.h include/VisualElement.h
src/Box.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/Box.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Box.o: /usr/local/include/cglm/cglm.h /usr/local/include/cglm/common.h
src/Box.o: /usr/local/include/cglm/types.h
src/Box.o: /usr/local/include/cglm/simd/intrin.h
src/Box.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Box.o: /usr/local/include/cglm/vec2-ext.h /usr/local/include/cglm/vec3.h
src/Box.o: /usr/local/include/cglm/vec4.h /usr/local/include/cglm/vec4-ext.h
src/Box.o: /usr/local/include/cglm/vec3-ext.h /usr/local/include/cglm/mat4.h
src/Box.o: /usr/local/include/cglm/mat3.h /usr/local/include/cglm/mat2.h
src/Box.o: /usr/local/include/cglm/affine.h
src/Box.o: /usr/local/include/cglm/affine-mat.h /usr/local/include/cglm/cam.h
src/Box.o: /usr/local/include/cglm/plane.h /usr/local/include/cglm/frustum.h
src/Box.o: /usr/local/include/cglm/quat.h /usr/local/include/cglm/euler.h
src/Box.o: /usr/local/include/cglm/box.h /usr/local/include/cglm/color.h
src/Box.o: /usr/local/include/cglm/io.h /usr/local/include/cglm/project.h
src/Box.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Box.o: /usr/local/include/cglm/curve.h /usr/local/include/cglm/bezier.h
src/Box.o: /usr/local/include/cglm/ray.h /usr/local/include/cglm/affine2d.h
src/Box.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Box.o: include/Global.h include/Logging.h include/MemoryTracker.h
src/Box.o: include/HttpDebug.h include/Vertex.h include/ElementBufferObject.h
src/Box.o: include/BoundingBox.h include/Shader.h include/VertexBufferCombo.h
src/Box.o: include/Lockable.h
src/CO2Scenario.o: include/CO2Scenario.h include/rapidjson/document.h
src/CO2Scenario.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/CO2Scenario.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/CO2Scenario.o: include/rapidjson/encodings.h
src/CO2Scenario.o: include/rapidjson/encodedstream.h
src/CO2Scenario.o: include/rapidjson/memorystream.h
src/CO2Scenario.o: include/rapidjson/internal/clzll.h
src/CO2Scenario.o: include/rapidjson/rapidjson.h
src/CO2Scenario.o: include/rapidjson/internal/meta.h
src/CO2Scenario.o: include/rapidjson/internal/stack.h
src/CO2Scenario.o: include/rapidjson/allocators.h
src/CO2Scenario.o: include/rapidjson/internal/swap.h
src/CO2Scenario.o: include/rapidjson/internal/strtod.h
src/CO2Scenario.o: include/rapidjson/internal/ieee754.h
src/CO2Scenario.o: include/rapidjson/internal/biginteger.h
src/CO2Scenario.o: include/rapidjson/internal/diyfp.h
src/CO2Scenario.o: include/rapidjson/internal/clzll.h
src/CO2Scenario.o: include/rapidjson/internal/pow10.h
src/CO2Scenario.o: include/rapidjson/error/error.h
src/CO2Scenario.o: include/rapidjson/internal/strfunc.h
src/CO2Scenario.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/CO2Scenario.o: include/rapidjson/internal/dtoa.h
src/CO2Scenario.o: include/rapidjson/internal/itoa.h
src/CO2Scenario.o: include/rapidjson/internal/itoa.h
src/CO2Scenario.o: include/rapidjson/stringbuffer.h
src/CO2Scenario.o: include/rapidjson/stringbuffer.h
src/CO2Scenario.o: include/rapidjson/error/en.h
src/CO2Scenario.o: include/rapidjson/error/error.h include/Logging.h
src/CO2Scenario.o: include/MemoryTracker.h include/HttpDebug.h
src/CO2Scenario.o: include/Global.h include/loadFileToBuf.h
src/Camera.o: /usr/local/include/GL/glew.h /usr/local/include/cglm/cglm.h
src/Camera.o: /usr/local/include/cglm/common.h
src/Camera.o: /usr/local/include/cglm/types.h
src/Camera.o: /usr/local/include/cglm/simd/intrin.h
src/Camera.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Camera.o: /usr/local/include/cglm/vec2-ext.h
src/Camera.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Camera.o: /usr/local/include/cglm/vec4-ext.h
src/Camera.o: /usr/local/include/cglm/vec3-ext.h
src/Camera.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Camera.o: /usr/local/include/cglm/mat2.h /usr/local/include/cglm/affine.h
src/Camera.o: /usr/local/include/cglm/affine-mat.h
src/Camera.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Camera.o: /usr/local/include/cglm/frustum.h
src/Camera.o: /usr/local/include/cglm/quat.h /usr/local/include/cglm/euler.h
src/Camera.o: /usr/local/include/cglm/box.h /usr/local/include/cglm/color.h
src/Camera.o: /usr/local/include/cglm/io.h /usr/local/include/cglm/project.h
src/Camera.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Camera.o: /usr/local/include/cglm/curve.h
src/Camera.o: /usr/local/include/cglm/bezier.h /usr/local/include/cglm/ray.h
src/Camera.o: /usr/local/include/cglm/affine2d.h include/Camera.h
src/Camera.o: include/Shader.h include/HttpDebug.h include/Global.h
src/ColoredAxes.o: /usr/local/include/GL/glew.h
src/ColoredAxes.o: /usr/local/include/GLFW/glfw3.h include/ColoredAxes.h
src/ColoredAxes.o: include/LineStripList.h /usr/local/include/cglm/cglm.h
src/ColoredAxes.o: /usr/local/include/cglm/common.h
src/ColoredAxes.o: /usr/local/include/cglm/types.h
src/ColoredAxes.o: /usr/local/include/cglm/simd/intrin.h
src/ColoredAxes.o: /usr/local/include/cglm/vec2.h
src/ColoredAxes.o: /usr/local/include/cglm/util.h
src/ColoredAxes.o: /usr/local/include/cglm/vec2-ext.h
src/ColoredAxes.o: /usr/local/include/cglm/vec3.h
src/ColoredAxes.o: /usr/local/include/cglm/vec4.h
src/ColoredAxes.o: /usr/local/include/cglm/vec4-ext.h
src/ColoredAxes.o: /usr/local/include/cglm/vec3-ext.h
src/ColoredAxes.o: /usr/local/include/cglm/mat4.h
src/ColoredAxes.o: /usr/local/include/cglm/mat3.h
src/ColoredAxes.o: /usr/local/include/cglm/mat2.h
src/ColoredAxes.o: /usr/local/include/cglm/affine.h
src/ColoredAxes.o: /usr/local/include/cglm/affine-mat.h
src/ColoredAxes.o: /usr/local/include/cglm/cam.h
src/ColoredAxes.o: /usr/local/include/cglm/plane.h
src/ColoredAxes.o: /usr/local/include/cglm/frustum.h
src/ColoredAxes.o: /usr/local/include/cglm/quat.h
src/ColoredAxes.o: /usr/local/include/cglm/euler.h
src/ColoredAxes.o: /usr/local/include/cglm/box.h
src/ColoredAxes.o: /usr/local/include/cglm/color.h
src/ColoredAxes.o: /usr/local/include/cglm/io.h
src/ColoredAxes.o: /usr/local/include/cglm/project.h
src/ColoredAxes.o: /usr/local/include/cglm/sphere.h
src/ColoredAxes.o: /usr/local/include/cglm/ease.h
src/ColoredAxes.o: /usr/local/include/cglm/curve.h
src/ColoredAxes.o: /usr/local/include/cglm/bezier.h
src/ColoredAxes.o: /usr/local/include/cglm/ray.h
src/ColoredAxes.o: /usr/local/include/cglm/affine2d.h
src/ColoredAxes.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/ColoredAxes.o: include/VertexBufferObject.h include/Global.h
src/ColoredAxes.o: include/Logging.h include/MemoryTracker.h
src/ColoredAxes.o: include/HttpDebug.h include/Vertex.h include/Shader.h
src/ControlGroup.o: include/ControlGroup.h include/ObjectGroup.h
src/ControlGroup.o: include/VisualObject.h include/VisualElement.h
src/ControlGroup.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/ControlGroup.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/ControlGroup.o: /usr/local/include/cglm/cglm.h
src/ControlGroup.o: /usr/local/include/cglm/common.h
src/ControlGroup.o: /usr/local/include/cglm/types.h
src/ControlGroup.o: /usr/local/include/cglm/simd/intrin.h
src/ControlGroup.o: /usr/local/include/cglm/vec2.h
src/ControlGroup.o: /usr/local/include/cglm/util.h
src/ControlGroup.o: /usr/local/include/cglm/vec2-ext.h
src/ControlGroup.o: /usr/local/include/cglm/vec3.h
src/ControlGroup.o: /usr/local/include/cglm/vec4.h
src/ControlGroup.o: /usr/local/include/cglm/vec4-ext.h
src/ControlGroup.o: /usr/local/include/cglm/vec3-ext.h
src/ControlGroup.o: /usr/local/include/cglm/mat4.h
src/ControlGroup.o: /usr/local/include/cglm/mat3.h
src/ControlGroup.o: /usr/local/include/cglm/mat2.h
src/ControlGroup.o: /usr/local/include/cglm/affine.h
src/ControlGroup.o: /usr/local/include/cglm/affine-mat.h
src/ControlGroup.o: /usr/local/include/cglm/cam.h
src/ControlGroup.o: /usr/local/include/cglm/plane.h
src/ControlGroup.o: /usr/local/include/cglm/frustum.h
src/ControlGroup.o: /usr/local/include/cglm/quat.h
src/ControlGroup.o: /usr/local/include/cglm/euler.h
src/ControlGroup.o: /usr/local/include/cglm/box.h
src/ControlGroup.o: /usr/local/include/cglm/color.h
src/ControlGroup.o: /usr/local/include/cglm/io.h
src/ControlGroup.o: /usr/local/include/cglm/project.h
src/ControlGroup.o: /usr/local/include/cglm/sphere.h
src/ControlGroup.o: /usr/local/include/cglm/ease.h
src/ControlGroup.o: /usr/local/include/cglm/curve.h
src/ControlGroup.o: /usr/local/include/cglm/bezier.h
src/ControlGroup.o: /usr/local/include/cglm/ray.h
src/ControlGroup.o: /usr/local/include/cglm/affine2d.h
src/ControlGroup.o: /usr/local/include/GL/glew.h
src/ControlGroup.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/ControlGroup.o: include/Logging.h include/MemoryTracker.h
src/ControlGroup.o: include/HttpDebug.h include/Vertex.h
src/ControlGroup.o: include/ElementBufferObject.h include/BoundingBox.h
src/ControlGroup.o: include/Shader.h include/VertexBufferCombo.h
src/ControlGroup.o: include/Lockable.h include/DisplayList.h
src/ControlGroup.o: include/LandSurfaceRegion.h
src/Copse.o: include/Copse.h include/DisplayList.h include/VisualObject.h
src/Copse.o: include/VisualElement.h include/TriangleBuffer.h
src/Copse.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/Copse.o: include/VertexBufferObject.h /usr/local/include/cglm/cglm.h
src/Copse.o: /usr/local/include/cglm/common.h /usr/local/include/cglm/types.h
src/Copse.o: /usr/local/include/cglm/simd/intrin.h
src/Copse.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Copse.o: /usr/local/include/cglm/vec2-ext.h
src/Copse.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Copse.o: /usr/local/include/cglm/vec4-ext.h
src/Copse.o: /usr/local/include/cglm/vec3-ext.h
src/Copse.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Copse.o: /usr/local/include/cglm/mat2.h /usr/local/include/cglm/affine.h
src/Copse.o: /usr/local/include/cglm/affine-mat.h
src/Copse.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Copse.o: /usr/local/include/cglm/frustum.h /usr/local/include/cglm/quat.h
src/Copse.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Copse.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Copse.o: /usr/local/include/cglm/project.h
src/Copse.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Copse.o: /usr/local/include/cglm/curve.h /usr/local/include/cglm/bezier.h
src/Copse.o: /usr/local/include/cglm/ray.h /usr/local/include/cglm/affine2d.h
src/Copse.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Copse.o: include/Global.h include/Logging.h include/MemoryTracker.h
src/Copse.o: include/HttpDebug.h include/Vertex.h
src/Copse.o: include/ElementBufferObject.h include/BoundingBox.h
src/Copse.o: include/Shader.h include/VertexBufferCombo.h include/Lockable.h
src/Copse.o: include/LandSurfaceRegion.h
src/Cylinder.o: include/Cylinder.h include/AxialElement.h
src/Cylinder.o: include/VisualElement.h include/TriangleBuffer.h
src/Cylinder.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/Cylinder.o: include/VertexBufferObject.h /usr/local/include/cglm/cglm.h
src/Cylinder.o: /usr/local/include/cglm/common.h
src/Cylinder.o: /usr/local/include/cglm/types.h
src/Cylinder.o: /usr/local/include/cglm/simd/intrin.h
src/Cylinder.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Cylinder.o: /usr/local/include/cglm/vec2-ext.h
src/Cylinder.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Cylinder.o: /usr/local/include/cglm/vec4-ext.h
src/Cylinder.o: /usr/local/include/cglm/vec3-ext.h
src/Cylinder.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Cylinder.o: /usr/local/include/cglm/mat2.h
src/Cylinder.o: /usr/local/include/cglm/affine.h
src/Cylinder.o: /usr/local/include/cglm/affine-mat.h
src/Cylinder.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Cylinder.o: /usr/local/include/cglm/frustum.h
src/Cylinder.o: /usr/local/include/cglm/quat.h
src/Cylinder.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Cylinder.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Cylinder.o: /usr/local/include/cglm/project.h
src/Cylinder.o: /usr/local/include/cglm/sphere.h
src/Cylinder.o: /usr/local/include/cglm/ease.h
src/Cylinder.o: /usr/local/include/cglm/curve.h
src/Cylinder.o: /usr/local/include/cglm/bezier.h
src/Cylinder.o: /usr/local/include/cglm/ray.h
src/Cylinder.o: /usr/local/include/cglm/affine2d.h
src/Cylinder.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Cylinder.o: include/Global.h include/Logging.h include/MemoryTracker.h
src/Cylinder.o: include/HttpDebug.h include/Vertex.h
src/Cylinder.o: include/ElementBufferObject.h include/BoundingBox.h
src/Cylinder.o: include/Shader.h include/VertexBufferCombo.h
src/Cylinder.o: include/TruncatedCone.h
src/DisplayList.o: include/DisplayList.h include/VisualObject.h
src/DisplayList.o: include/VisualElement.h include/TriangleBuffer.h
src/DisplayList.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/DisplayList.o: include/VertexBufferObject.h
src/DisplayList.o: /usr/local/include/cglm/cglm.h
src/DisplayList.o: /usr/local/include/cglm/common.h
src/DisplayList.o: /usr/local/include/cglm/types.h
src/DisplayList.o: /usr/local/include/cglm/simd/intrin.h
src/DisplayList.o: /usr/local/include/cglm/vec2.h
src/DisplayList.o: /usr/local/include/cglm/util.h
src/DisplayList.o: /usr/local/include/cglm/vec2-ext.h
src/DisplayList.o: /usr/local/include/cglm/vec3.h
src/DisplayList.o: /usr/local/include/cglm/vec4.h
src/DisplayList.o: /usr/local/include/cglm/vec4-ext.h
src/DisplayList.o: /usr/local/include/cglm/vec3-ext.h
src/DisplayList.o: /usr/local/include/cglm/mat4.h
src/DisplayList.o: /usr/local/include/cglm/mat3.h
src/DisplayList.o: /usr/local/include/cglm/mat2.h
src/DisplayList.o: /usr/local/include/cglm/affine.h
src/DisplayList.o: /usr/local/include/cglm/affine-mat.h
src/DisplayList.o: /usr/local/include/cglm/cam.h
src/DisplayList.o: /usr/local/include/cglm/plane.h
src/DisplayList.o: /usr/local/include/cglm/frustum.h
src/DisplayList.o: /usr/local/include/cglm/quat.h
src/DisplayList.o: /usr/local/include/cglm/euler.h
src/DisplayList.o: /usr/local/include/cglm/box.h
src/DisplayList.o: /usr/local/include/cglm/color.h
src/DisplayList.o: /usr/local/include/cglm/io.h
src/DisplayList.o: /usr/local/include/cglm/project.h
src/DisplayList.o: /usr/local/include/cglm/sphere.h
src/DisplayList.o: /usr/local/include/cglm/ease.h
src/DisplayList.o: /usr/local/include/cglm/curve.h
src/DisplayList.o: /usr/local/include/cglm/bezier.h
src/DisplayList.o: /usr/local/include/cglm/ray.h
src/DisplayList.o: /usr/local/include/cglm/affine2d.h
src/DisplayList.o: /usr/local/include/GL/glew.h
src/DisplayList.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/DisplayList.o: include/Logging.h include/MemoryTracker.h
src/DisplayList.o: include/HttpDebug.h include/Vertex.h
src/DisplayList.o: include/ElementBufferObject.h include/BoundingBox.h
src/DisplayList.o: include/Shader.h include/VertexBufferCombo.h
src/DisplayList.o: include/Lockable.h include/LandSurfaceRegion.h
src/DisplayList.o: include/HeightMarker.h
src/ElementBufferCombo.o: /usr/local/include/GL/glew.h
src/ElementBufferCombo.o: /usr/local/include/GLFW/glfw3.h
src/ElementBufferCombo.o: include/ElementBufferCombo.h
src/ElementBufferCombo.o: include/VertexArrayObject.h
src/ElementBufferCombo.o: include/VertexBufferObject.h
src/ElementBufferCombo.o: /usr/local/include/cglm/cglm.h
src/ElementBufferCombo.o: /usr/local/include/cglm/common.h
src/ElementBufferCombo.o: /usr/local/include/cglm/types.h
src/ElementBufferCombo.o: /usr/local/include/cglm/simd/intrin.h
src/ElementBufferCombo.o: /usr/local/include/cglm/vec2.h
src/ElementBufferCombo.o: /usr/local/include/cglm/util.h
src/ElementBufferCombo.o: /usr/local/include/cglm/vec2-ext.h
src/ElementBufferCombo.o: /usr/local/include/cglm/vec3.h
src/ElementBufferCombo.o: /usr/local/include/cglm/vec4.h
src/ElementBufferCombo.o: /usr/local/include/cglm/vec4-ext.h
src/ElementBufferCombo.o: /usr/local/include/cglm/vec3-ext.h
src/ElementBufferCombo.o: /usr/local/include/cglm/mat4.h
src/ElementBufferCombo.o: /usr/local/include/cglm/mat3.h
src/ElementBufferCombo.o: /usr/local/include/cglm/mat2.h
src/ElementBufferCombo.o: /usr/local/include/cglm/affine.h
src/ElementBufferCombo.o: /usr/local/include/cglm/affine-mat.h
src/ElementBufferCombo.o: /usr/local/include/cglm/cam.h
src/ElementBufferCombo.o: /usr/local/include/cglm/plane.h
src/ElementBufferCombo.o: /usr/local/include/cglm/frustum.h
src/ElementBufferCombo.o: /usr/local/include/cglm/quat.h
src/ElementBufferCombo.o: /usr/local/include/cglm/euler.h
src/ElementBufferCombo.o: /usr/local/include/cglm/box.h
src/ElementBufferCombo.o: /usr/local/include/cglm/color.h
src/ElementBufferCombo.o: /usr/local/include/cglm/io.h
src/ElementBufferCombo.o: /usr/local/include/cglm/project.h
src/ElementBufferCombo.o: /usr/local/include/cglm/sphere.h
src/ElementBufferCombo.o: /usr/local/include/cglm/ease.h
src/ElementBufferCombo.o: /usr/local/include/cglm/curve.h
src/ElementBufferCombo.o: /usr/local/include/cglm/bezier.h
src/ElementBufferCombo.o: /usr/local/include/cglm/ray.h
src/ElementBufferCombo.o: /usr/local/include/cglm/affine2d.h include/Global.h
src/ElementBufferCombo.o: include/Logging.h include/MemoryTracker.h
src/ElementBufferCombo.o: include/HttpDebug.h include/Vertex.h
src/ElementBufferCombo.o: include/ElementBufferObject.h
src/ElementBufferObject.o: /usr/local/include/GL/glew.h
src/ElementBufferObject.o: /usr/local/include/GLFW/glfw3.h
src/ElementBufferObject.o: include/ElementBufferObject.h include/Shader.h
src/ElementBufferObject.o: /usr/local/include/cglm/cglm.h
src/ElementBufferObject.o: /usr/local/include/cglm/common.h
src/ElementBufferObject.o: /usr/local/include/cglm/types.h
src/ElementBufferObject.o: /usr/local/include/cglm/simd/intrin.h
src/ElementBufferObject.o: /usr/local/include/cglm/vec2.h
src/ElementBufferObject.o: /usr/local/include/cglm/util.h
src/ElementBufferObject.o: /usr/local/include/cglm/vec2-ext.h
src/ElementBufferObject.o: /usr/local/include/cglm/vec3.h
src/ElementBufferObject.o: /usr/local/include/cglm/vec4.h
src/ElementBufferObject.o: /usr/local/include/cglm/vec4-ext.h
src/ElementBufferObject.o: /usr/local/include/cglm/vec3-ext.h
src/ElementBufferObject.o: /usr/local/include/cglm/mat4.h
src/ElementBufferObject.o: /usr/local/include/cglm/mat3.h
src/ElementBufferObject.o: /usr/local/include/cglm/mat2.h
src/ElementBufferObject.o: /usr/local/include/cglm/affine.h
src/ElementBufferObject.o: /usr/local/include/cglm/affine-mat.h
src/ElementBufferObject.o: /usr/local/include/cglm/cam.h
src/ElementBufferObject.o: /usr/local/include/cglm/plane.h
src/ElementBufferObject.o: /usr/local/include/cglm/frustum.h
src/ElementBufferObject.o: /usr/local/include/cglm/quat.h
src/ElementBufferObject.o: /usr/local/include/cglm/euler.h
src/ElementBufferObject.o: /usr/local/include/cglm/box.h
src/ElementBufferObject.o: /usr/local/include/cglm/color.h
src/ElementBufferObject.o: /usr/local/include/cglm/io.h
src/ElementBufferObject.o: /usr/local/include/cglm/project.h
src/ElementBufferObject.o: /usr/local/include/cglm/sphere.h
src/ElementBufferObject.o: /usr/local/include/cglm/ease.h
src/ElementBufferObject.o: /usr/local/include/cglm/curve.h
src/ElementBufferObject.o: /usr/local/include/cglm/bezier.h
src/ElementBufferObject.o: /usr/local/include/cglm/ray.h
src/ElementBufferObject.o: /usr/local/include/cglm/affine2d.h
src/EmptyClass.o: include/EmptyClass.h
src/Global.o: include/Global.h include/TaskQueueFarm.h include/TaskQueue.h
src/Global.o: include/Lockable.h include/HttpDebug.h
src/Grid.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Grid.o: include/Grid.h /usr/local/include/cglm/cglm.h
src/Grid.o: /usr/local/include/cglm/common.h /usr/local/include/cglm/types.h
src/Grid.o: /usr/local/include/cglm/simd/intrin.h
src/Grid.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Grid.o: /usr/local/include/cglm/vec2-ext.h /usr/local/include/cglm/vec3.h
src/Grid.o: /usr/local/include/cglm/vec4.h /usr/local/include/cglm/vec4-ext.h
src/Grid.o: /usr/local/include/cglm/vec3-ext.h /usr/local/include/cglm/mat4.h
src/Grid.o: /usr/local/include/cglm/mat3.h /usr/local/include/cglm/mat2.h
src/Grid.o: /usr/local/include/cglm/affine.h
src/Grid.o: /usr/local/include/cglm/affine-mat.h
src/Grid.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Grid.o: /usr/local/include/cglm/frustum.h /usr/local/include/cglm/quat.h
src/Grid.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Grid.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Grid.o: /usr/local/include/cglm/project.h
src/Grid.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Grid.o: /usr/local/include/cglm/curve.h /usr/local/include/cglm/bezier.h
src/Grid.o: /usr/local/include/cglm/ray.h /usr/local/include/cglm/affine2d.h
src/Grid.o: include/Shader.h include/VertexArrayObject.h
src/Grid.o: include/VertexBufferObject.h include/Global.h include/Logging.h
src/Grid.o: include/MemoryTracker.h include/HttpDebug.h include/Vertex.h
src/Grid.o: include/LandSurface.h include/TexturedRect.h include/Texture.h
src/Grid.o: include/Quadtree.h include/DisplayList.h include/VisualObject.h
src/Grid.o: include/VisualElement.h include/TriangleBuffer.h
src/Grid.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/Grid.o: include/BoundingBox.h include/VertexBufferCombo.h
src/Grid.o: include/Lockable.h include/LandSurfaceRegion.h
src/Grid.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/Grid.o: include/HeightMarker.h include/BezierPatch.h
src/Grid.o: include/rapidjson/document.h include/rapidjson/reader.h
src/Grid.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/Grid.o: include/rapidjson/stream.h include/rapidjson/encodings.h
src/Grid.o: include/rapidjson/encodedstream.h
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
src/Grid.o: include/LineStripList.h
src/HeightMarker.o: include/HeightMarker.h include/VisualObject.h
src/HeightMarker.o: include/VisualElement.h include/TriangleBuffer.h
src/HeightMarker.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/HeightMarker.o: include/VertexBufferObject.h
src/HeightMarker.o: /usr/local/include/cglm/cglm.h
src/HeightMarker.o: /usr/local/include/cglm/common.h
src/HeightMarker.o: /usr/local/include/cglm/types.h
src/HeightMarker.o: /usr/local/include/cglm/simd/intrin.h
src/HeightMarker.o: /usr/local/include/cglm/vec2.h
src/HeightMarker.o: /usr/local/include/cglm/util.h
src/HeightMarker.o: /usr/local/include/cglm/vec2-ext.h
src/HeightMarker.o: /usr/local/include/cglm/vec3.h
src/HeightMarker.o: /usr/local/include/cglm/vec4.h
src/HeightMarker.o: /usr/local/include/cglm/vec4-ext.h
src/HeightMarker.o: /usr/local/include/cglm/vec3-ext.h
src/HeightMarker.o: /usr/local/include/cglm/mat4.h
src/HeightMarker.o: /usr/local/include/cglm/mat3.h
src/HeightMarker.o: /usr/local/include/cglm/mat2.h
src/HeightMarker.o: /usr/local/include/cglm/affine.h
src/HeightMarker.o: /usr/local/include/cglm/affine-mat.h
src/HeightMarker.o: /usr/local/include/cglm/cam.h
src/HeightMarker.o: /usr/local/include/cglm/plane.h
src/HeightMarker.o: /usr/local/include/cglm/frustum.h
src/HeightMarker.o: /usr/local/include/cglm/quat.h
src/HeightMarker.o: /usr/local/include/cglm/euler.h
src/HeightMarker.o: /usr/local/include/cglm/box.h
src/HeightMarker.o: /usr/local/include/cglm/color.h
src/HeightMarker.o: /usr/local/include/cglm/io.h
src/HeightMarker.o: /usr/local/include/cglm/project.h
src/HeightMarker.o: /usr/local/include/cglm/sphere.h
src/HeightMarker.o: /usr/local/include/cglm/ease.h
src/HeightMarker.o: /usr/local/include/cglm/curve.h
src/HeightMarker.o: /usr/local/include/cglm/bezier.h
src/HeightMarker.o: /usr/local/include/cglm/ray.h
src/HeightMarker.o: /usr/local/include/cglm/affine2d.h
src/HeightMarker.o: /usr/local/include/GL/glew.h
src/HeightMarker.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/HeightMarker.o: include/Logging.h include/MemoryTracker.h
src/HeightMarker.o: include/HttpDebug.h include/Vertex.h
src/HeightMarker.o: include/ElementBufferObject.h include/BoundingBox.h
src/HeightMarker.o: include/Shader.h include/VertexBufferCombo.h
src/HeightMarker.o: include/Lockable.h
src/HttpDebug.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/HttpDebug.o: include/HttpDebug.h include/Scene.h include/Shader.h
src/HttpDebug.o: /usr/local/include/cglm/cglm.h
src/HttpDebug.o: /usr/local/include/cglm/common.h
src/HttpDebug.o: /usr/local/include/cglm/types.h
src/HttpDebug.o: /usr/local/include/cglm/simd/intrin.h
src/HttpDebug.o: /usr/local/include/cglm/vec2.h
src/HttpDebug.o: /usr/local/include/cglm/util.h
src/HttpDebug.o: /usr/local/include/cglm/vec2-ext.h
src/HttpDebug.o: /usr/local/include/cglm/vec3.h
src/HttpDebug.o: /usr/local/include/cglm/vec4.h
src/HttpDebug.o: /usr/local/include/cglm/vec4-ext.h
src/HttpDebug.o: /usr/local/include/cglm/vec3-ext.h
src/HttpDebug.o: /usr/local/include/cglm/mat4.h
src/HttpDebug.o: /usr/local/include/cglm/mat3.h
src/HttpDebug.o: /usr/local/include/cglm/mat2.h
src/HttpDebug.o: /usr/local/include/cglm/affine.h
src/HttpDebug.o: /usr/local/include/cglm/affine-mat.h
src/HttpDebug.o: /usr/local/include/cglm/cam.h
src/HttpDebug.o: /usr/local/include/cglm/plane.h
src/HttpDebug.o: /usr/local/include/cglm/frustum.h
src/HttpDebug.o: /usr/local/include/cglm/quat.h
src/HttpDebug.o: /usr/local/include/cglm/euler.h
src/HttpDebug.o: /usr/local/include/cglm/box.h
src/HttpDebug.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/HttpDebug.o: /usr/local/include/cglm/project.h
src/HttpDebug.o: /usr/local/include/cglm/sphere.h
src/HttpDebug.o: /usr/local/include/cglm/ease.h
src/HttpDebug.o: /usr/local/include/cglm/curve.h
src/HttpDebug.o: /usr/local/include/cglm/bezier.h
src/HttpDebug.o: /usr/local/include/cglm/ray.h
src/HttpDebug.o: /usr/local/include/cglm/affine2d.h include/ColoredAxes.h
src/HttpDebug.o: include/LineStripList.h include/VertexBufferCombo.h
src/HttpDebug.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/HttpDebug.o: include/Global.h include/Logging.h include/MemoryTracker.h
src/HttpDebug.o: include/Vertex.h include/PmodConfig.h include/LandSurface.h
src/HttpDebug.o: include/TexturedRect.h include/Texture.h include/Quadtree.h
src/HttpDebug.o: include/DisplayList.h include/VisualObject.h
src/HttpDebug.o: include/VisualElement.h include/TriangleBuffer.h
src/HttpDebug.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/HttpDebug.o: include/BoundingBox.h include/Lockable.h
src/HttpDebug.o: include/LandSurfaceRegion.h
src/HttpDebug.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/HttpDebug.o: include/HeightMarker.h include/BezierPatch.h
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
src/HttpDebug.o: include/rapidjson/stream.h include/PmodDesign.h
src/HttpDebug.o: include/rapidjson/writer.h include/rapidjson/internal/dtoa.h
src/HttpDebug.o: include/rapidjson/internal/itoa.h
src/HttpDebug.o: include/rapidjson/internal/itoa.h
src/HttpDebug.o: include/rapidjson/stringbuffer.h
src/HttpDebug.o: include/rapidjson/stringbuffer.h
src/HttpDebug.o: include/rapidjson/error/en.h include/rapidjson/error/error.h
src/HttpDebug.o: include/Timeval.h include/JSONStructureChecker.h
src/HttpDebug.o: include/Version.h include/Boundary.h include/Grid.h
src/HttpDebug.o: include/Material.h include/TextureAtlas.h
src/HttpDebug.o: include/LightingModel.h include/Species.h
src/HttpDebug.o: include/LeafModel.h include/CO2Scenario.h
src/HttpDebug.o: include/TaskQueue.h include/Tree.h include/TreePart.h
src/HttpDebug.o: include/TaskQueueFarm.h include/SkySampleModel.h
src/JSONStructureChecker.o: include/PmodDesign.h include/rapidjson/document.h
src/JSONStructureChecker.o: include/rapidjson/reader.h
src/JSONStructureChecker.o: include/rapidjson/allocators.h
src/JSONStructureChecker.o: include/rapidjson/rapidjson.h
src/JSONStructureChecker.o: include/rapidjson/stream.h
src/JSONStructureChecker.o: include/rapidjson/encodings.h
src/JSONStructureChecker.o: include/rapidjson/encodedstream.h
src/JSONStructureChecker.o: include/rapidjson/memorystream.h
src/JSONStructureChecker.o: include/rapidjson/internal/clzll.h
src/JSONStructureChecker.o: include/rapidjson/rapidjson.h
src/JSONStructureChecker.o: include/rapidjson/internal/meta.h
src/JSONStructureChecker.o: include/rapidjson/internal/stack.h
src/JSONStructureChecker.o: include/rapidjson/allocators.h
src/JSONStructureChecker.o: include/rapidjson/internal/swap.h
src/JSONStructureChecker.o: include/rapidjson/internal/strtod.h
src/JSONStructureChecker.o: include/rapidjson/internal/ieee754.h
src/JSONStructureChecker.o: include/rapidjson/internal/biginteger.h
src/JSONStructureChecker.o: include/rapidjson/internal/diyfp.h
src/JSONStructureChecker.o: include/rapidjson/internal/clzll.h
src/JSONStructureChecker.o: include/rapidjson/internal/pow10.h
src/JSONStructureChecker.o: include/rapidjson/error/error.h
src/JSONStructureChecker.o: include/rapidjson/internal/strfunc.h
src/JSONStructureChecker.o: include/rapidjson/stream.h
src/JSONStructureChecker.o: include/rapidjson/writer.h
src/JSONStructureChecker.o: include/rapidjson/internal/dtoa.h
src/JSONStructureChecker.o: include/rapidjson/internal/itoa.h
src/JSONStructureChecker.o: include/rapidjson/internal/itoa.h
src/JSONStructureChecker.o: include/rapidjson/stringbuffer.h
src/JSONStructureChecker.o: include/rapidjson/stringbuffer.h
src/JSONStructureChecker.o: include/rapidjson/error/en.h
src/JSONStructureChecker.o: include/rapidjson/error/error.h
src/JSONStructureChecker.o: include/LandSurface.h
src/JSONStructureChecker.o: /usr/local/include/cglm/cglm.h
src/JSONStructureChecker.o: /usr/local/include/cglm/common.h
src/JSONStructureChecker.o: /usr/local/include/cglm/types.h
src/JSONStructureChecker.o: /usr/local/include/cglm/simd/intrin.h
src/JSONStructureChecker.o: /usr/local/include/cglm/vec2.h
src/JSONStructureChecker.o: /usr/local/include/cglm/util.h
src/JSONStructureChecker.o: /usr/local/include/cglm/vec2-ext.h
src/JSONStructureChecker.o: /usr/local/include/cglm/vec3.h
src/JSONStructureChecker.o: /usr/local/include/cglm/vec4.h
src/JSONStructureChecker.o: /usr/local/include/cglm/vec4-ext.h
src/JSONStructureChecker.o: /usr/local/include/cglm/vec3-ext.h
src/JSONStructureChecker.o: /usr/local/include/cglm/mat4.h
src/JSONStructureChecker.o: /usr/local/include/cglm/mat3.h
src/JSONStructureChecker.o: /usr/local/include/cglm/mat2.h
src/JSONStructureChecker.o: /usr/local/include/cglm/affine.h
src/JSONStructureChecker.o: /usr/local/include/cglm/affine-mat.h
src/JSONStructureChecker.o: /usr/local/include/cglm/cam.h
src/JSONStructureChecker.o: /usr/local/include/cglm/plane.h
src/JSONStructureChecker.o: /usr/local/include/cglm/frustum.h
src/JSONStructureChecker.o: /usr/local/include/cglm/quat.h
src/JSONStructureChecker.o: /usr/local/include/cglm/euler.h
src/JSONStructureChecker.o: /usr/local/include/cglm/box.h
src/JSONStructureChecker.o: /usr/local/include/cglm/color.h
src/JSONStructureChecker.o: /usr/local/include/cglm/io.h
src/JSONStructureChecker.o: /usr/local/include/cglm/project.h
src/JSONStructureChecker.o: /usr/local/include/cglm/sphere.h
src/JSONStructureChecker.o: /usr/local/include/cglm/ease.h
src/JSONStructureChecker.o: /usr/local/include/cglm/curve.h
src/JSONStructureChecker.o: /usr/local/include/cglm/bezier.h
src/JSONStructureChecker.o: /usr/local/include/cglm/ray.h
src/JSONStructureChecker.o: /usr/local/include/cglm/affine2d.h
src/JSONStructureChecker.o: include/Shader.h include/VertexArrayObject.h
src/JSONStructureChecker.o: include/VertexBufferObject.h
src/JSONStructureChecker.o: /usr/local/include/GL/glew.h
src/JSONStructureChecker.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/JSONStructureChecker.o: include/Logging.h include/MemoryTracker.h
src/JSONStructureChecker.o: include/HttpDebug.h include/Vertex.h
src/JSONStructureChecker.o: include/TexturedRect.h include/Texture.h
src/JSONStructureChecker.o: include/Quadtree.h include/DisplayList.h
src/JSONStructureChecker.o: include/VisualObject.h include/VisualElement.h
src/JSONStructureChecker.o: include/TriangleBuffer.h
src/JSONStructureChecker.o: include/ElementBufferCombo.h
src/JSONStructureChecker.o: include/ElementBufferObject.h
src/JSONStructureChecker.o: include/BoundingBox.h include/VertexBufferCombo.h
src/JSONStructureChecker.o: include/Lockable.h include/LandSurfaceRegion.h
src/JSONStructureChecker.o: include/LandSurfaceRegionPlanar.h
src/JSONStructureChecker.o: include/Camera.h include/HeightMarker.h
src/JSONStructureChecker.o: include/BezierPatch.h include/PmodConfig.h
src/JSONStructureChecker.o: include/Timeval.h include/JSONStructureChecker.h
src/JSONStructureChecker.o: include/Version.h include/Boundary.h
src/JSONStructureChecker.o: include/loadFileToBuf.h
src/LandSurface.o: /usr/local/include/GL/glew.h
src/LandSurface.o: /usr/local/include/GLFW/glfw3.h include/Camera.h
src/LandSurface.o: /usr/local/include/cglm/cglm.h
src/LandSurface.o: /usr/local/include/cglm/common.h
src/LandSurface.o: /usr/local/include/cglm/types.h
src/LandSurface.o: /usr/local/include/cglm/simd/intrin.h
src/LandSurface.o: /usr/local/include/cglm/vec2.h
src/LandSurface.o: /usr/local/include/cglm/util.h
src/LandSurface.o: /usr/local/include/cglm/vec2-ext.h
src/LandSurface.o: /usr/local/include/cglm/vec3.h
src/LandSurface.o: /usr/local/include/cglm/vec4.h
src/LandSurface.o: /usr/local/include/cglm/vec4-ext.h
src/LandSurface.o: /usr/local/include/cglm/vec3-ext.h
src/LandSurface.o: /usr/local/include/cglm/mat4.h
src/LandSurface.o: /usr/local/include/cglm/mat3.h
src/LandSurface.o: /usr/local/include/cglm/mat2.h
src/LandSurface.o: /usr/local/include/cglm/affine.h
src/LandSurface.o: /usr/local/include/cglm/affine-mat.h
src/LandSurface.o: /usr/local/include/cglm/cam.h
src/LandSurface.o: /usr/local/include/cglm/plane.h
src/LandSurface.o: /usr/local/include/cglm/frustum.h
src/LandSurface.o: /usr/local/include/cglm/quat.h
src/LandSurface.o: /usr/local/include/cglm/euler.h
src/LandSurface.o: /usr/local/include/cglm/box.h
src/LandSurface.o: /usr/local/include/cglm/color.h
src/LandSurface.o: /usr/local/include/cglm/io.h
src/LandSurface.o: /usr/local/include/cglm/project.h
src/LandSurface.o: /usr/local/include/cglm/sphere.h
src/LandSurface.o: /usr/local/include/cglm/ease.h
src/LandSurface.o: /usr/local/include/cglm/curve.h
src/LandSurface.o: /usr/local/include/cglm/bezier.h
src/LandSurface.o: /usr/local/include/cglm/ray.h
src/LandSurface.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/LandSurface.o: include/HttpDebug.h include/Global.h include/LandSurface.h
src/LandSurface.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/LandSurface.o: include/Logging.h include/MemoryTracker.h include/Vertex.h
src/LandSurface.o: include/TexturedRect.h include/Texture.h
src/LandSurface.o: include/Quadtree.h include/DisplayList.h
src/LandSurface.o: include/VisualObject.h include/VisualElement.h
src/LandSurface.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/LandSurface.o: include/ElementBufferObject.h include/BoundingBox.h
src/LandSurface.o: include/VertexBufferCombo.h include/Lockable.h
src/LandSurface.o: include/LandSurfaceRegion.h
src/LandSurface.o: include/LandSurfaceRegionPlanar.h include/HeightMarker.h
src/LandSurface.o: include/BezierPatch.h include/rapidjson/document.h
src/LandSurface.o: include/rapidjson/reader.h include/rapidjson/allocators.h
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
src/LandSurface.o: include/rapidjson/stream.h include/PmodDesign.h
src/LandSurface.o: include/rapidjson/writer.h
src/LandSurface.o: include/rapidjson/internal/dtoa.h
src/LandSurface.o: include/rapidjson/internal/itoa.h
src/LandSurface.o: include/rapidjson/internal/itoa.h
src/LandSurface.o: include/rapidjson/stringbuffer.h
src/LandSurface.o: include/rapidjson/stringbuffer.h
src/LandSurface.o: include/rapidjson/error/en.h
src/LandSurface.o: include/rapidjson/error/error.h include/PmodConfig.h
src/LandSurface.o: include/Timeval.h include/JSONStructureChecker.h
src/LandSurface.o: include/Version.h include/Boundary.h
src/LandSurfaceRegion.o: include/LandSurfaceRegion.h include/VisualObject.h
src/LandSurfaceRegion.o: include/VisualElement.h include/TriangleBuffer.h
src/LandSurfaceRegion.o: include/ElementBufferCombo.h
src/LandSurfaceRegion.o: include/VertexArrayObject.h
src/LandSurfaceRegion.o: include/VertexBufferObject.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/cglm.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/common.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/types.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/simd/intrin.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/vec2.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/util.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/vec2-ext.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/vec3.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/vec4.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/vec4-ext.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/vec3-ext.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/mat4.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/mat3.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/mat2.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/affine.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/affine-mat.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/cam.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/plane.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/frustum.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/quat.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/euler.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/box.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/color.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/io.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/project.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/sphere.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/ease.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/curve.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/bezier.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/ray.h
src/LandSurfaceRegion.o: /usr/local/include/cglm/affine2d.h
src/LandSurfaceRegion.o: /usr/local/include/GL/glew.h
src/LandSurfaceRegion.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/LandSurfaceRegion.o: include/Logging.h include/MemoryTracker.h
src/LandSurfaceRegion.o: include/HttpDebug.h include/Vertex.h
src/LandSurfaceRegion.o: include/ElementBufferObject.h include/BoundingBox.h
src/LandSurfaceRegion.o: include/Shader.h include/VertexBufferCombo.h
src/LandSurfaceRegion.o: include/Lockable.h
src/LandSurfaceRegionPlanar.o: include/LandSurfaceRegionPlanar.h
src/LandSurfaceRegionPlanar.o: include/LandSurfaceRegion.h
src/LandSurfaceRegionPlanar.o: include/VisualObject.h include/VisualElement.h
src/LandSurfaceRegionPlanar.o: include/TriangleBuffer.h
src/LandSurfaceRegionPlanar.o: include/ElementBufferCombo.h
src/LandSurfaceRegionPlanar.o: include/VertexArrayObject.h
src/LandSurfaceRegionPlanar.o: include/VertexBufferObject.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/cglm.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/common.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/types.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/simd/intrin.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/vec2.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/util.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/vec2-ext.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/vec3.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/vec4.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/vec4-ext.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/vec3-ext.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/mat4.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/mat3.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/mat2.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/affine.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/affine-mat.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/cam.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/plane.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/frustum.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/quat.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/euler.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/box.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/color.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/io.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/project.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/sphere.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/ease.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/curve.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/bezier.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/ray.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/cglm/affine2d.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/GL/glew.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/GLFW/glfw3.h
src/LandSurfaceRegionPlanar.o: include/Global.h include/Logging.h
src/LandSurfaceRegionPlanar.o: include/MemoryTracker.h include/HttpDebug.h
src/LandSurfaceRegionPlanar.o: include/Vertex.h include/ElementBufferObject.h
src/LandSurfaceRegionPlanar.o: include/BoundingBox.h include/Shader.h
src/LandSurfaceRegionPlanar.o: include/VertexBufferCombo.h include/Lockable.h
src/LeafModel.o: include/LeafModel.h include/JSONStructureChecker.h
src/LeafModel.o: include/rapidjson/document.h include/rapidjson/reader.h
src/LeafModel.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/LeafModel.o: include/rapidjson/stream.h include/rapidjson/encodings.h
src/LeafModel.o: include/rapidjson/encodedstream.h
src/LeafModel.o: include/rapidjson/memorystream.h
src/LeafModel.o: include/rapidjson/internal/clzll.h
src/LeafModel.o: include/rapidjson/rapidjson.h
src/LeafModel.o: include/rapidjson/internal/meta.h
src/LeafModel.o: include/rapidjson/internal/stack.h
src/LeafModel.o: include/rapidjson/allocators.h
src/LeafModel.o: include/rapidjson/internal/swap.h
src/LeafModel.o: include/rapidjson/internal/strtod.h
src/LeafModel.o: include/rapidjson/internal/ieee754.h
src/LeafModel.o: include/rapidjson/internal/biginteger.h
src/LeafModel.o: include/rapidjson/internal/diyfp.h
src/LeafModel.o: include/rapidjson/internal/clzll.h
src/LeafModel.o: include/rapidjson/internal/pow10.h
src/LeafModel.o: include/rapidjson/error/error.h
src/LeafModel.o: include/rapidjson/internal/strfunc.h
src/LeafModel.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/LeafModel.o: include/rapidjson/internal/dtoa.h
src/LeafModel.o: include/rapidjson/internal/itoa.h
src/LeafModel.o: include/rapidjson/internal/itoa.h
src/LeafModel.o: include/rapidjson/stringbuffer.h
src/LeafModel.o: include/rapidjson/stringbuffer.h
src/LeafModel.o: include/rapidjson/error/en.h include/rapidjson/error/error.h
src/LeafModel.o: include/Timeval.h include/Version.h
src/LeafModel.o: /usr/local/include/cglm/cglm.h
src/LeafModel.o: /usr/local/include/cglm/common.h
src/LeafModel.o: /usr/local/include/cglm/types.h
src/LeafModel.o: /usr/local/include/cglm/simd/intrin.h
src/LeafModel.o: /usr/local/include/cglm/vec2.h
src/LeafModel.o: /usr/local/include/cglm/util.h
src/LeafModel.o: /usr/local/include/cglm/vec2-ext.h
src/LeafModel.o: /usr/local/include/cglm/vec3.h
src/LeafModel.o: /usr/local/include/cglm/vec4.h
src/LeafModel.o: /usr/local/include/cglm/vec4-ext.h
src/LeafModel.o: /usr/local/include/cglm/vec3-ext.h
src/LeafModel.o: /usr/local/include/cglm/mat4.h
src/LeafModel.o: /usr/local/include/cglm/mat3.h
src/LeafModel.o: /usr/local/include/cglm/mat2.h
src/LeafModel.o: /usr/local/include/cglm/affine.h
src/LeafModel.o: /usr/local/include/cglm/affine-mat.h
src/LeafModel.o: /usr/local/include/cglm/cam.h
src/LeafModel.o: /usr/local/include/cglm/plane.h
src/LeafModel.o: /usr/local/include/cglm/frustum.h
src/LeafModel.o: /usr/local/include/cglm/quat.h
src/LeafModel.o: /usr/local/include/cglm/euler.h
src/LeafModel.o: /usr/local/include/cglm/box.h
src/LeafModel.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/LeafModel.o: /usr/local/include/cglm/project.h
src/LeafModel.o: /usr/local/include/cglm/sphere.h
src/LeafModel.o: /usr/local/include/cglm/ease.h
src/LeafModel.o: /usr/local/include/cglm/curve.h
src/LeafModel.o: /usr/local/include/cglm/bezier.h
src/LeafModel.o: /usr/local/include/cglm/ray.h
src/LeafModel.o: /usr/local/include/cglm/affine2d.h include/Global.h
src/LeafModel.o: include/Logging.h include/MemoryTracker.h
src/LeafModel.o: include/HttpDebug.h
src/LightingModel.o: include/LightingModel.h /usr/local/include/cglm/cglm.h
src/LightingModel.o: /usr/local/include/cglm/common.h
src/LightingModel.o: /usr/local/include/cglm/types.h
src/LightingModel.o: /usr/local/include/cglm/simd/intrin.h
src/LightingModel.o: /usr/local/include/cglm/vec2.h
src/LightingModel.o: /usr/local/include/cglm/util.h
src/LightingModel.o: /usr/local/include/cglm/vec2-ext.h
src/LightingModel.o: /usr/local/include/cglm/vec3.h
src/LightingModel.o: /usr/local/include/cglm/vec4.h
src/LightingModel.o: /usr/local/include/cglm/vec4-ext.h
src/LightingModel.o: /usr/local/include/cglm/vec3-ext.h
src/LightingModel.o: /usr/local/include/cglm/mat4.h
src/LightingModel.o: /usr/local/include/cglm/mat3.h
src/LightingModel.o: /usr/local/include/cglm/mat2.h
src/LightingModel.o: /usr/local/include/cglm/affine.h
src/LightingModel.o: /usr/local/include/cglm/affine-mat.h
src/LightingModel.o: /usr/local/include/cglm/cam.h
src/LightingModel.o: /usr/local/include/cglm/plane.h
src/LightingModel.o: /usr/local/include/cglm/frustum.h
src/LightingModel.o: /usr/local/include/cglm/quat.h
src/LightingModel.o: /usr/local/include/cglm/euler.h
src/LightingModel.o: /usr/local/include/cglm/box.h
src/LightingModel.o: /usr/local/include/cglm/color.h
src/LightingModel.o: /usr/local/include/cglm/io.h
src/LightingModel.o: /usr/local/include/cglm/project.h
src/LightingModel.o: /usr/local/include/cglm/sphere.h
src/LightingModel.o: /usr/local/include/cglm/ease.h
src/LightingModel.o: /usr/local/include/cglm/curve.h
src/LightingModel.o: /usr/local/include/cglm/bezier.h
src/LightingModel.o: /usr/local/include/cglm/ray.h
src/LightingModel.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/LineStripList.o: include/LineStripList.h /usr/local/include/cglm/cglm.h
src/LineStripList.o: /usr/local/include/cglm/common.h
src/LineStripList.o: /usr/local/include/cglm/types.h
src/LineStripList.o: /usr/local/include/cglm/simd/intrin.h
src/LineStripList.o: /usr/local/include/cglm/vec2.h
src/LineStripList.o: /usr/local/include/cglm/util.h
src/LineStripList.o: /usr/local/include/cglm/vec2-ext.h
src/LineStripList.o: /usr/local/include/cglm/vec3.h
src/LineStripList.o: /usr/local/include/cglm/vec4.h
src/LineStripList.o: /usr/local/include/cglm/vec4-ext.h
src/LineStripList.o: /usr/local/include/cglm/vec3-ext.h
src/LineStripList.o: /usr/local/include/cglm/mat4.h
src/LineStripList.o: /usr/local/include/cglm/mat3.h
src/LineStripList.o: /usr/local/include/cglm/mat2.h
src/LineStripList.o: /usr/local/include/cglm/affine.h
src/LineStripList.o: /usr/local/include/cglm/affine-mat.h
src/LineStripList.o: /usr/local/include/cglm/cam.h
src/LineStripList.o: /usr/local/include/cglm/plane.h
src/LineStripList.o: /usr/local/include/cglm/frustum.h
src/LineStripList.o: /usr/local/include/cglm/quat.h
src/LineStripList.o: /usr/local/include/cglm/euler.h
src/LineStripList.o: /usr/local/include/cglm/box.h
src/LineStripList.o: /usr/local/include/cglm/color.h
src/LineStripList.o: /usr/local/include/cglm/io.h
src/LineStripList.o: /usr/local/include/cglm/project.h
src/LineStripList.o: /usr/local/include/cglm/sphere.h
src/LineStripList.o: /usr/local/include/cglm/ease.h
src/LineStripList.o: /usr/local/include/cglm/curve.h
src/LineStripList.o: /usr/local/include/cglm/bezier.h
src/LineStripList.o: /usr/local/include/cglm/ray.h
src/LineStripList.o: /usr/local/include/cglm/affine2d.h
src/LineStripList.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/LineStripList.o: include/VertexBufferObject.h
src/LineStripList.o: /usr/local/include/GL/glew.h
src/LineStripList.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/LineStripList.o: include/Logging.h include/MemoryTracker.h
src/LineStripList.o: include/HttpDebug.h include/Vertex.h include/Shader.h
src/Lockable.o: include/Lockable.h include/Global.h
src/Logging.o: include/Logging.h include/MemoryTracker.h include/HttpDebug.h
src/Logging.o: include/Vertex.h /usr/local/include/cglm/cglm.h
src/Logging.o: /usr/local/include/cglm/common.h
src/Logging.o: /usr/local/include/cglm/types.h
src/Logging.o: /usr/local/include/cglm/simd/intrin.h
src/Logging.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Logging.o: /usr/local/include/cglm/vec2-ext.h
src/Logging.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Logging.o: /usr/local/include/cglm/vec4-ext.h
src/Logging.o: /usr/local/include/cglm/vec3-ext.h
src/Logging.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Logging.o: /usr/local/include/cglm/mat2.h
src/Logging.o: /usr/local/include/cglm/affine.h
src/Logging.o: /usr/local/include/cglm/affine-mat.h
src/Logging.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Logging.o: /usr/local/include/cglm/frustum.h
src/Logging.o: /usr/local/include/cglm/quat.h /usr/local/include/cglm/euler.h
src/Logging.o: /usr/local/include/cglm/box.h /usr/local/include/cglm/color.h
src/Logging.o: /usr/local/include/cglm/io.h /usr/local/include/cglm/project.h
src/Logging.o: /usr/local/include/cglm/sphere.h
src/Logging.o: /usr/local/include/cglm/ease.h /usr/local/include/cglm/curve.h
src/Logging.o: /usr/local/include/cglm/bezier.h /usr/local/include/cglm/ray.h
src/Logging.o: /usr/local/include/cglm/affine2d.h
src/Logging.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Logging.o: include/Global.h
src/Material.o: include/Material.h /usr/local/include/cglm/cglm.h
src/Material.o: /usr/local/include/cglm/common.h
src/Material.o: /usr/local/include/cglm/types.h
src/Material.o: /usr/local/include/cglm/simd/intrin.h
src/Material.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Material.o: /usr/local/include/cglm/vec2-ext.h
src/Material.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Material.o: /usr/local/include/cglm/vec4-ext.h
src/Material.o: /usr/local/include/cglm/vec3-ext.h
src/Material.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Material.o: /usr/local/include/cglm/mat2.h
src/Material.o: /usr/local/include/cglm/affine.h
src/Material.o: /usr/local/include/cglm/affine-mat.h
src/Material.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Material.o: /usr/local/include/cglm/frustum.h
src/Material.o: /usr/local/include/cglm/quat.h
src/Material.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Material.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Material.o: /usr/local/include/cglm/project.h
src/Material.o: /usr/local/include/cglm/sphere.h
src/Material.o: /usr/local/include/cglm/ease.h
src/Material.o: /usr/local/include/cglm/curve.h
src/Material.o: /usr/local/include/cglm/bezier.h
src/Material.o: /usr/local/include/cglm/ray.h
src/Material.o: /usr/local/include/cglm/affine2d.h include/TextureAtlas.h
src/Material.o: include/Texture.h /usr/local/include/GL/glew.h
src/Material.o: include/Shader.h include/HttpDebug.h include/Logging.h
src/Material.o: include/MemoryTracker.h
src/MemoryTracker.o: include/MemoryTracker.h include/HttpDebug.h
src/MenuInterface.o: imgui/backends/imgui_impl_opengl3.h imgui/imgui.h
src/MenuInterface.o: imgui/imconfig.h imgui/backends/imgui_impl_glfw.h
src/MenuInterface.o: include/MenuInterface.h /usr/local/include/cglm/cglm.h
src/MenuInterface.o: /usr/local/include/cglm/common.h
src/MenuInterface.o: /usr/local/include/cglm/types.h
src/MenuInterface.o: /usr/local/include/cglm/simd/intrin.h
src/MenuInterface.o: /usr/local/include/cglm/vec2.h
src/MenuInterface.o: /usr/local/include/cglm/util.h
src/MenuInterface.o: /usr/local/include/cglm/vec2-ext.h
src/MenuInterface.o: /usr/local/include/cglm/vec3.h
src/MenuInterface.o: /usr/local/include/cglm/vec4.h
src/MenuInterface.o: /usr/local/include/cglm/vec4-ext.h
src/MenuInterface.o: /usr/local/include/cglm/vec3-ext.h
src/MenuInterface.o: /usr/local/include/cglm/mat4.h
src/MenuInterface.o: /usr/local/include/cglm/mat3.h
src/MenuInterface.o: /usr/local/include/cglm/mat2.h
src/MenuInterface.o: /usr/local/include/cglm/affine.h
src/MenuInterface.o: /usr/local/include/cglm/affine-mat.h
src/MenuInterface.o: /usr/local/include/cglm/cam.h
src/MenuInterface.o: /usr/local/include/cglm/plane.h
src/MenuInterface.o: /usr/local/include/cglm/frustum.h
src/MenuInterface.o: /usr/local/include/cglm/quat.h
src/MenuInterface.o: /usr/local/include/cglm/euler.h
src/MenuInterface.o: /usr/local/include/cglm/box.h
src/MenuInterface.o: /usr/local/include/cglm/color.h
src/MenuInterface.o: /usr/local/include/cglm/io.h
src/MenuInterface.o: /usr/local/include/cglm/project.h
src/MenuInterface.o: /usr/local/include/cglm/sphere.h
src/MenuInterface.o: /usr/local/include/cglm/ease.h
src/MenuInterface.o: /usr/local/include/cglm/curve.h
src/MenuInterface.o: /usr/local/include/cglm/bezier.h
src/MenuInterface.o: /usr/local/include/cglm/ray.h
src/MenuInterface.o: /usr/local/include/cglm/affine2d.h
src/MenuInterface.o: /usr/local/include/GL/glew.h
src/MenuInterface.o: /usr/local/include/GLFW/glfw3.h
src/MenuInterface.o: include/ScriptController.h include/HttpDebug.h
src/MenuInterface.o: include/PmodDesign.h include/rapidjson/document.h
src/MenuInterface.o: include/rapidjson/reader.h
src/MenuInterface.o: include/rapidjson/allocators.h
src/MenuInterface.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/MenuInterface.o: include/rapidjson/encodings.h
src/MenuInterface.o: include/rapidjson/encodedstream.h
src/MenuInterface.o: include/rapidjson/memorystream.h
src/MenuInterface.o: include/rapidjson/internal/clzll.h
src/MenuInterface.o: include/rapidjson/rapidjson.h
src/MenuInterface.o: include/rapidjson/internal/meta.h
src/MenuInterface.o: include/rapidjson/internal/stack.h
src/MenuInterface.o: include/rapidjson/allocators.h
src/MenuInterface.o: include/rapidjson/internal/swap.h
src/MenuInterface.o: include/rapidjson/internal/strtod.h
src/MenuInterface.o: include/rapidjson/internal/ieee754.h
src/MenuInterface.o: include/rapidjson/internal/biginteger.h
src/MenuInterface.o: include/rapidjson/internal/diyfp.h
src/MenuInterface.o: include/rapidjson/internal/clzll.h
src/MenuInterface.o: include/rapidjson/internal/pow10.h
src/MenuInterface.o: include/rapidjson/error/error.h
src/MenuInterface.o: include/rapidjson/internal/strfunc.h
src/MenuInterface.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/MenuInterface.o: include/rapidjson/internal/dtoa.h
src/MenuInterface.o: include/rapidjson/internal/itoa.h
src/MenuInterface.o: include/rapidjson/internal/itoa.h
src/MenuInterface.o: include/rapidjson/stringbuffer.h
src/MenuInterface.o: include/rapidjson/stringbuffer.h
src/MenuInterface.o: include/rapidjson/error/en.h
src/MenuInterface.o: include/rapidjson/error/error.h include/LandSurface.h
src/MenuInterface.o: include/Shader.h include/VertexArrayObject.h
src/MenuInterface.o: include/VertexBufferObject.h include/Global.h
src/MenuInterface.o: include/Logging.h include/MemoryTracker.h
src/MenuInterface.o: include/Vertex.h include/TexturedRect.h
src/MenuInterface.o: include/Texture.h include/Quadtree.h
src/MenuInterface.o: include/DisplayList.h include/VisualObject.h
src/MenuInterface.o: include/VisualElement.h include/TriangleBuffer.h
src/MenuInterface.o: include/ElementBufferCombo.h
src/MenuInterface.o: include/ElementBufferObject.h include/BoundingBox.h
src/MenuInterface.o: include/VertexBufferCombo.h include/Lockable.h
src/MenuInterface.o: include/LandSurfaceRegion.h
src/MenuInterface.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/MenuInterface.o: include/HeightMarker.h include/BezierPatch.h
src/MenuInterface.o: include/PmodConfig.h include/Timeval.h
src/MenuInterface.o: include/JSONStructureChecker.h include/Version.h
src/MenuInterface.o: include/Boundary.h include/Scene.h include/ColoredAxes.h
src/MenuInterface.o: include/LineStripList.h include/Grid.h
src/MenuInterface.o: include/Material.h include/TextureAtlas.h
src/MenuInterface.o: include/LightingModel.h include/Species.h
src/MenuInterface.o: include/LeafModel.h include/CO2Scenario.h
src/MenuInterface.o: include/TaskQueue.h include/Window3D.h
src/MenuInterface.o: include/loadFileToBuf.h
src/ObjectGroup.o: include/ObjectGroup.h include/VisualObject.h
src/ObjectGroup.o: include/VisualElement.h include/TriangleBuffer.h
src/ObjectGroup.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/ObjectGroup.o: include/VertexBufferObject.h
src/ObjectGroup.o: /usr/local/include/cglm/cglm.h
src/ObjectGroup.o: /usr/local/include/cglm/common.h
src/ObjectGroup.o: /usr/local/include/cglm/types.h
src/ObjectGroup.o: /usr/local/include/cglm/simd/intrin.h
src/ObjectGroup.o: /usr/local/include/cglm/vec2.h
src/ObjectGroup.o: /usr/local/include/cglm/util.h
src/ObjectGroup.o: /usr/local/include/cglm/vec2-ext.h
src/ObjectGroup.o: /usr/local/include/cglm/vec3.h
src/ObjectGroup.o: /usr/local/include/cglm/vec4.h
src/ObjectGroup.o: /usr/local/include/cglm/vec4-ext.h
src/ObjectGroup.o: /usr/local/include/cglm/vec3-ext.h
src/ObjectGroup.o: /usr/local/include/cglm/mat4.h
src/ObjectGroup.o: /usr/local/include/cglm/mat3.h
src/ObjectGroup.o: /usr/local/include/cglm/mat2.h
src/ObjectGroup.o: /usr/local/include/cglm/affine.h
src/ObjectGroup.o: /usr/local/include/cglm/affine-mat.h
src/ObjectGroup.o: /usr/local/include/cglm/cam.h
src/ObjectGroup.o: /usr/local/include/cglm/plane.h
src/ObjectGroup.o: /usr/local/include/cglm/frustum.h
src/ObjectGroup.o: /usr/local/include/cglm/quat.h
src/ObjectGroup.o: /usr/local/include/cglm/euler.h
src/ObjectGroup.o: /usr/local/include/cglm/box.h
src/ObjectGroup.o: /usr/local/include/cglm/color.h
src/ObjectGroup.o: /usr/local/include/cglm/io.h
src/ObjectGroup.o: /usr/local/include/cglm/project.h
src/ObjectGroup.o: /usr/local/include/cglm/sphere.h
src/ObjectGroup.o: /usr/local/include/cglm/ease.h
src/ObjectGroup.o: /usr/local/include/cglm/curve.h
src/ObjectGroup.o: /usr/local/include/cglm/bezier.h
src/ObjectGroup.o: /usr/local/include/cglm/ray.h
src/ObjectGroup.o: /usr/local/include/cglm/affine2d.h
src/ObjectGroup.o: /usr/local/include/GL/glew.h
src/ObjectGroup.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/ObjectGroup.o: include/Logging.h include/MemoryTracker.h
src/ObjectGroup.o: include/HttpDebug.h include/Vertex.h
src/ObjectGroup.o: include/ElementBufferObject.h include/BoundingBox.h
src/ObjectGroup.o: include/Shader.h include/VertexBufferCombo.h
src/ObjectGroup.o: include/Lockable.h include/DisplayList.h
src/ObjectGroup.o: include/LandSurfaceRegion.h
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
src/PmodDesign.o: include/rapidjson/error/error.h include/LandSurface.h
src/PmodDesign.o: /usr/local/include/cglm/cglm.h
src/PmodDesign.o: /usr/local/include/cglm/common.h
src/PmodDesign.o: /usr/local/include/cglm/types.h
src/PmodDesign.o: /usr/local/include/cglm/simd/intrin.h
src/PmodDesign.o: /usr/local/include/cglm/vec2.h
src/PmodDesign.o: /usr/local/include/cglm/util.h
src/PmodDesign.o: /usr/local/include/cglm/vec2-ext.h
src/PmodDesign.o: /usr/local/include/cglm/vec3.h
src/PmodDesign.o: /usr/local/include/cglm/vec4.h
src/PmodDesign.o: /usr/local/include/cglm/vec4-ext.h
src/PmodDesign.o: /usr/local/include/cglm/vec3-ext.h
src/PmodDesign.o: /usr/local/include/cglm/mat4.h
src/PmodDesign.o: /usr/local/include/cglm/mat3.h
src/PmodDesign.o: /usr/local/include/cglm/mat2.h
src/PmodDesign.o: /usr/local/include/cglm/affine.h
src/PmodDesign.o: /usr/local/include/cglm/affine-mat.h
src/PmodDesign.o: /usr/local/include/cglm/cam.h
src/PmodDesign.o: /usr/local/include/cglm/plane.h
src/PmodDesign.o: /usr/local/include/cglm/frustum.h
src/PmodDesign.o: /usr/local/include/cglm/quat.h
src/PmodDesign.o: /usr/local/include/cglm/euler.h
src/PmodDesign.o: /usr/local/include/cglm/box.h
src/PmodDesign.o: /usr/local/include/cglm/color.h
src/PmodDesign.o: /usr/local/include/cglm/io.h
src/PmodDesign.o: /usr/local/include/cglm/project.h
src/PmodDesign.o: /usr/local/include/cglm/sphere.h
src/PmodDesign.o: /usr/local/include/cglm/ease.h
src/PmodDesign.o: /usr/local/include/cglm/curve.h
src/PmodDesign.o: /usr/local/include/cglm/bezier.h
src/PmodDesign.o: /usr/local/include/cglm/ray.h
src/PmodDesign.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/PmodDesign.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/PmodDesign.o: /usr/local/include/GL/glew.h
src/PmodDesign.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/PmodDesign.o: include/Logging.h include/MemoryTracker.h
src/PmodDesign.o: include/HttpDebug.h include/Vertex.h include/TexturedRect.h
src/PmodDesign.o: include/Texture.h include/Quadtree.h include/DisplayList.h
src/PmodDesign.o: include/VisualObject.h include/VisualElement.h
src/PmodDesign.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/PmodDesign.o: include/ElementBufferObject.h include/BoundingBox.h
src/PmodDesign.o: include/VertexBufferCombo.h include/Lockable.h
src/PmodDesign.o: include/LandSurfaceRegion.h
src/PmodDesign.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/PmodDesign.o: include/HeightMarker.h include/BezierPatch.h
src/PmodDesign.o: include/PmodConfig.h include/Timeval.h
src/PmodDesign.o: include/JSONStructureChecker.h include/Version.h
src/PmodDesign.o: include/Boundary.h include/loadFileToBuf.h include/Tree.h
src/PmodDesign.o: include/Species.h include/LeafModel.h include/TreePart.h
src/Quadtree.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Quadtree.o: include/Quadtree.h include/Shader.h
src/Quadtree.o: /usr/local/include/cglm/cglm.h
src/Quadtree.o: /usr/local/include/cglm/common.h
src/Quadtree.o: /usr/local/include/cglm/types.h
src/Quadtree.o: /usr/local/include/cglm/simd/intrin.h
src/Quadtree.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Quadtree.o: /usr/local/include/cglm/vec2-ext.h
src/Quadtree.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Quadtree.o: /usr/local/include/cglm/vec4-ext.h
src/Quadtree.o: /usr/local/include/cglm/vec3-ext.h
src/Quadtree.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Quadtree.o: /usr/local/include/cglm/mat2.h
src/Quadtree.o: /usr/local/include/cglm/affine.h
src/Quadtree.o: /usr/local/include/cglm/affine-mat.h
src/Quadtree.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Quadtree.o: /usr/local/include/cglm/frustum.h
src/Quadtree.o: /usr/local/include/cglm/quat.h
src/Quadtree.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Quadtree.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Quadtree.o: /usr/local/include/cglm/project.h
src/Quadtree.o: /usr/local/include/cglm/sphere.h
src/Quadtree.o: /usr/local/include/cglm/ease.h
src/Quadtree.o: /usr/local/include/cglm/curve.h
src/Quadtree.o: /usr/local/include/cglm/bezier.h
src/Quadtree.o: /usr/local/include/cglm/ray.h
src/Quadtree.o: /usr/local/include/cglm/affine2d.h
src/Quadtree.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Quadtree.o: include/Global.h include/Logging.h include/MemoryTracker.h
src/Quadtree.o: include/HttpDebug.h include/Vertex.h include/DisplayList.h
src/Quadtree.o: include/VisualObject.h include/VisualElement.h
src/Quadtree.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/Quadtree.o: include/ElementBufferObject.h include/BoundingBox.h
src/Quadtree.o: include/VertexBufferCombo.h include/Lockable.h
src/Quadtree.o: include/LandSurfaceRegion.h include/LandSurfaceRegionPlanar.h
src/Quadtree.o: include/Scene.h include/ColoredAxes.h include/LineStripList.h
src/Quadtree.o: include/PmodConfig.h include/LandSurface.h
src/Quadtree.o: include/TexturedRect.h include/Texture.h include/Camera.h
src/Quadtree.o: include/HeightMarker.h include/BezierPatch.h
src/Quadtree.o: include/rapidjson/document.h include/rapidjson/reader.h
src/Quadtree.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/Quadtree.o: include/rapidjson/stream.h include/rapidjson/encodings.h
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
src/Quadtree.o: include/rapidjson/stream.h include/PmodDesign.h
src/Quadtree.o: include/rapidjson/writer.h include/rapidjson/internal/dtoa.h
src/Quadtree.o: include/rapidjson/internal/itoa.h
src/Quadtree.o: include/rapidjson/internal/itoa.h
src/Quadtree.o: include/rapidjson/stringbuffer.h
src/Quadtree.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Quadtree.o: include/rapidjson/error/error.h include/Timeval.h
src/Quadtree.o: include/JSONStructureChecker.h include/Version.h
src/Quadtree.o: include/Boundary.h include/Grid.h include/Material.h
src/Quadtree.o: include/TextureAtlas.h include/LightingModel.h
src/Quadtree.o: include/Species.h include/LeafModel.h include/CO2Scenario.h
src/Quadtree.o: include/TaskQueue.h
src/Scene.o: include/ControlGroup.h include/ObjectGroup.h
src/Scene.o: include/VisualObject.h include/VisualElement.h
src/Scene.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/Scene.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Scene.o: /usr/local/include/cglm/cglm.h /usr/local/include/cglm/common.h
src/Scene.o: /usr/local/include/cglm/types.h
src/Scene.o: /usr/local/include/cglm/simd/intrin.h
src/Scene.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Scene.o: /usr/local/include/cglm/vec2-ext.h
src/Scene.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Scene.o: /usr/local/include/cglm/vec4-ext.h
src/Scene.o: /usr/local/include/cglm/vec3-ext.h
src/Scene.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Scene.o: /usr/local/include/cglm/mat2.h /usr/local/include/cglm/affine.h
src/Scene.o: /usr/local/include/cglm/affine-mat.h
src/Scene.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Scene.o: /usr/local/include/cglm/frustum.h /usr/local/include/cglm/quat.h
src/Scene.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Scene.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Scene.o: /usr/local/include/cglm/project.h
src/Scene.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Scene.o: /usr/local/include/cglm/curve.h /usr/local/include/cglm/bezier.h
src/Scene.o: /usr/local/include/cglm/ray.h /usr/local/include/cglm/affine2d.h
src/Scene.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Scene.o: include/Global.h include/Logging.h include/MemoryTracker.h
src/Scene.o: include/HttpDebug.h include/Vertex.h
src/Scene.o: include/ElementBufferObject.h include/BoundingBox.h
src/Scene.o: include/Shader.h include/VertexBufferCombo.h include/Lockable.h
src/Scene.o: include/DisplayList.h include/LandSurfaceRegion.h
src/Scene.o: include/Scene.h include/ColoredAxes.h include/LineStripList.h
src/Scene.o: include/PmodConfig.h include/LandSurface.h
src/Scene.o: include/TexturedRect.h include/Texture.h include/Quadtree.h
src/Scene.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/Scene.o: include/HeightMarker.h include/BezierPatch.h
src/Scene.o: include/rapidjson/document.h include/rapidjson/reader.h
src/Scene.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/Scene.o: include/rapidjson/stream.h include/rapidjson/encodings.h
src/Scene.o: include/rapidjson/encodedstream.h
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
src/Scene.o: include/PmodDesign.h include/rapidjson/writer.h
src/Scene.o: include/rapidjson/internal/dtoa.h
src/Scene.o: include/rapidjson/internal/itoa.h
src/Scene.o: include/rapidjson/internal/itoa.h
src/Scene.o: include/rapidjson/stringbuffer.h
src/Scene.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Scene.o: include/rapidjson/error/error.h include/Timeval.h
src/Scene.o: include/JSONStructureChecker.h include/Version.h
src/Scene.o: include/Boundary.h include/Grid.h include/Material.h
src/Scene.o: include/TextureAtlas.h include/LightingModel.h include/Species.h
src/Scene.o: include/LeafModel.h include/CO2Scenario.h include/TaskQueue.h
src/Scene.o: include/Box.h include/Tree.h include/TreePart.h
src/Scene.o: include/loadFileToBuf.h
src/ScriptController.o: include/VertexBufferObject.h
src/ScriptController.o: /usr/local/include/cglm/cglm.h
src/ScriptController.o: /usr/local/include/cglm/common.h
src/ScriptController.o: /usr/local/include/cglm/types.h
src/ScriptController.o: /usr/local/include/cglm/simd/intrin.h
src/ScriptController.o: /usr/local/include/cglm/vec2.h
src/ScriptController.o: /usr/local/include/cglm/util.h
src/ScriptController.o: /usr/local/include/cglm/vec2-ext.h
src/ScriptController.o: /usr/local/include/cglm/vec3.h
src/ScriptController.o: /usr/local/include/cglm/vec4.h
src/ScriptController.o: /usr/local/include/cglm/vec4-ext.h
src/ScriptController.o: /usr/local/include/cglm/vec3-ext.h
src/ScriptController.o: /usr/local/include/cglm/mat4.h
src/ScriptController.o: /usr/local/include/cglm/mat3.h
src/ScriptController.o: /usr/local/include/cglm/mat2.h
src/ScriptController.o: /usr/local/include/cglm/affine.h
src/ScriptController.o: /usr/local/include/cglm/affine-mat.h
src/ScriptController.o: /usr/local/include/cglm/cam.h
src/ScriptController.o: /usr/local/include/cglm/plane.h
src/ScriptController.o: /usr/local/include/cglm/frustum.h
src/ScriptController.o: /usr/local/include/cglm/quat.h
src/ScriptController.o: /usr/local/include/cglm/euler.h
src/ScriptController.o: /usr/local/include/cglm/box.h
src/ScriptController.o: /usr/local/include/cglm/color.h
src/ScriptController.o: /usr/local/include/cglm/io.h
src/ScriptController.o: /usr/local/include/cglm/project.h
src/ScriptController.o: /usr/local/include/cglm/sphere.h
src/ScriptController.o: /usr/local/include/cglm/ease.h
src/ScriptController.o: /usr/local/include/cglm/curve.h
src/ScriptController.o: /usr/local/include/cglm/bezier.h
src/ScriptController.o: /usr/local/include/cglm/ray.h
src/ScriptController.o: /usr/local/include/cglm/affine2d.h
src/ScriptController.o: /usr/local/include/GL/glew.h
src/ScriptController.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/ScriptController.o: include/Logging.h include/MemoryTracker.h
src/ScriptController.o: include/HttpDebug.h include/Vertex.h
src/ScriptController.o: include/ScriptController.h include/PmodDesign.h
src/ScriptController.o: include/rapidjson/document.h
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
src/ScriptController.o: include/rapidjson/error/error.h include/LandSurface.h
src/ScriptController.o: include/Shader.h include/VertexArrayObject.h
src/ScriptController.o: include/TexturedRect.h include/Texture.h
src/ScriptController.o: include/Quadtree.h include/DisplayList.h
src/ScriptController.o: include/VisualObject.h include/VisualElement.h
src/ScriptController.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/ScriptController.o: include/ElementBufferObject.h include/BoundingBox.h
src/ScriptController.o: include/VertexBufferCombo.h include/Lockable.h
src/ScriptController.o: include/LandSurfaceRegion.h
src/ScriptController.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/ScriptController.o: include/HeightMarker.h include/BezierPatch.h
src/ScriptController.o: include/PmodConfig.h include/Timeval.h
src/ScriptController.o: include/JSONStructureChecker.h include/Version.h
src/ScriptController.o: include/Boundary.h
src/Shader.o: /usr/local/include/GL/glew.h include/loadFileToBuf.h
src/Shader.o: include/Shader.h /usr/local/include/cglm/cglm.h
src/Shader.o: /usr/local/include/cglm/common.h
src/Shader.o: /usr/local/include/cglm/types.h
src/Shader.o: /usr/local/include/cglm/simd/intrin.h
src/Shader.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Shader.o: /usr/local/include/cglm/vec2-ext.h
src/Shader.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Shader.o: /usr/local/include/cglm/vec4-ext.h
src/Shader.o: /usr/local/include/cglm/vec3-ext.h
src/Shader.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Shader.o: /usr/local/include/cglm/mat2.h /usr/local/include/cglm/affine.h
src/Shader.o: /usr/local/include/cglm/affine-mat.h
src/Shader.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Shader.o: /usr/local/include/cglm/frustum.h
src/Shader.o: /usr/local/include/cglm/quat.h /usr/local/include/cglm/euler.h
src/Shader.o: /usr/local/include/cglm/box.h /usr/local/include/cglm/color.h
src/Shader.o: /usr/local/include/cglm/io.h /usr/local/include/cglm/project.h
src/Shader.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Shader.o: /usr/local/include/cglm/curve.h
src/Shader.o: /usr/local/include/cglm/bezier.h /usr/local/include/cglm/ray.h
src/Shader.o: /usr/local/include/cglm/affine2d.h
src/SkySampleModel.o: include/Logging.h include/MemoryTracker.h
src/SkySampleModel.o: include/HttpDebug.h include/SkySampleModel.h
src/SkySampleModel.o: /usr/local/include/cglm/cglm.h
src/SkySampleModel.o: /usr/local/include/cglm/common.h
src/SkySampleModel.o: /usr/local/include/cglm/types.h
src/SkySampleModel.o: /usr/local/include/cglm/simd/intrin.h
src/SkySampleModel.o: /usr/local/include/cglm/vec2.h
src/SkySampleModel.o: /usr/local/include/cglm/util.h
src/SkySampleModel.o: /usr/local/include/cglm/vec2-ext.h
src/SkySampleModel.o: /usr/local/include/cglm/vec3.h
src/SkySampleModel.o: /usr/local/include/cglm/vec4.h
src/SkySampleModel.o: /usr/local/include/cglm/vec4-ext.h
src/SkySampleModel.o: /usr/local/include/cglm/vec3-ext.h
src/SkySampleModel.o: /usr/local/include/cglm/mat4.h
src/SkySampleModel.o: /usr/local/include/cglm/mat3.h
src/SkySampleModel.o: /usr/local/include/cglm/mat2.h
src/SkySampleModel.o: /usr/local/include/cglm/affine.h
src/SkySampleModel.o: /usr/local/include/cglm/affine-mat.h
src/SkySampleModel.o: /usr/local/include/cglm/cam.h
src/SkySampleModel.o: /usr/local/include/cglm/plane.h
src/SkySampleModel.o: /usr/local/include/cglm/frustum.h
src/SkySampleModel.o: /usr/local/include/cglm/quat.h
src/SkySampleModel.o: /usr/local/include/cglm/euler.h
src/SkySampleModel.o: /usr/local/include/cglm/box.h
src/SkySampleModel.o: /usr/local/include/cglm/color.h
src/SkySampleModel.o: /usr/local/include/cglm/io.h
src/SkySampleModel.o: /usr/local/include/cglm/project.h
src/SkySampleModel.o: /usr/local/include/cglm/sphere.h
src/SkySampleModel.o: /usr/local/include/cglm/ease.h
src/SkySampleModel.o: /usr/local/include/cglm/curve.h
src/SkySampleModel.o: /usr/local/include/cglm/bezier.h
src/SkySampleModel.o: /usr/local/include/cglm/ray.h
src/SkySampleModel.o: /usr/local/include/cglm/affine2d.h
src/SkySampleModel.o: include/BoundingBox.h include/Shader.h
src/SkySampleModel.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/SkySampleModel.o: include/VertexBufferObject.h
src/SkySampleModel.o: /usr/local/include/GL/glew.h
src/SkySampleModel.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/SkySampleModel.o: include/Vertex.h
src/Species.o: include/Species.h include/HttpDebug.h
src/Species.o: include/rapidjson/document.h include/rapidjson/reader.h
src/Species.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/Species.o: include/rapidjson/stream.h include/rapidjson/encodings.h
src/Species.o: include/rapidjson/encodedstream.h
src/Species.o: include/rapidjson/memorystream.h
src/Species.o: include/rapidjson/internal/clzll.h
src/Species.o: include/rapidjson/rapidjson.h
src/Species.o: include/rapidjson/internal/meta.h
src/Species.o: include/rapidjson/internal/stack.h
src/Species.o: include/rapidjson/allocators.h
src/Species.o: include/rapidjson/internal/swap.h
src/Species.o: include/rapidjson/internal/strtod.h
src/Species.o: include/rapidjson/internal/ieee754.h
src/Species.o: include/rapidjson/internal/biginteger.h
src/Species.o: include/rapidjson/internal/diyfp.h
src/Species.o: include/rapidjson/internal/clzll.h
src/Species.o: include/rapidjson/internal/pow10.h
src/Species.o: include/rapidjson/error/error.h
src/Species.o: include/rapidjson/internal/strfunc.h
src/Species.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/Species.o: include/rapidjson/internal/dtoa.h
src/Species.o: include/rapidjson/internal/itoa.h
src/Species.o: include/rapidjson/internal/itoa.h
src/Species.o: include/rapidjson/stringbuffer.h
src/Species.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Species.o: include/rapidjson/error/error.h include/LeafModel.h
src/Species.o: include/JSONStructureChecker.h include/Timeval.h
src/Species.o: include/Version.h /usr/local/include/cglm/cglm.h
src/Species.o: /usr/local/include/cglm/common.h
src/Species.o: /usr/local/include/cglm/types.h
src/Species.o: /usr/local/include/cglm/simd/intrin.h
src/Species.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Species.o: /usr/local/include/cglm/vec2-ext.h
src/Species.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Species.o: /usr/local/include/cglm/vec4-ext.h
src/Species.o: /usr/local/include/cglm/vec3-ext.h
src/Species.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Species.o: /usr/local/include/cglm/mat2.h
src/Species.o: /usr/local/include/cglm/affine.h
src/Species.o: /usr/local/include/cglm/affine-mat.h
src/Species.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Species.o: /usr/local/include/cglm/frustum.h
src/Species.o: /usr/local/include/cglm/quat.h /usr/local/include/cglm/euler.h
src/Species.o: /usr/local/include/cglm/box.h /usr/local/include/cglm/color.h
src/Species.o: /usr/local/include/cglm/io.h /usr/local/include/cglm/project.h
src/Species.o: /usr/local/include/cglm/sphere.h
src/Species.o: /usr/local/include/cglm/ease.h /usr/local/include/cglm/curve.h
src/Species.o: /usr/local/include/cglm/bezier.h /usr/local/include/cglm/ray.h
src/Species.o: /usr/local/include/cglm/affine2d.h include/loadFileToBuf.h
src/Species.o: include/Global.h include/PmodConfig.h include/PmodDesign.h
src/Species.o: include/LandSurface.h include/Shader.h
src/Species.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Species.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Species.o: include/Logging.h include/MemoryTracker.h include/Vertex.h
src/Species.o: include/TexturedRect.h include/Texture.h include/Quadtree.h
src/Species.o: include/DisplayList.h include/VisualObject.h
src/Species.o: include/VisualElement.h include/TriangleBuffer.h
src/Species.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/Species.o: include/BoundingBox.h include/VertexBufferCombo.h
src/Species.o: include/Lockable.h include/LandSurfaceRegion.h
src/Species.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/Species.o: include/HeightMarker.h include/BezierPatch.h
src/Species.o: include/Boundary.h
src/TaskQueue.o: include/TaskQueue.h include/Lockable.h include/Global.h
src/TaskQueue.o: include/Logging.h include/MemoryTracker.h
src/TaskQueue.o: include/HttpDebug.h
src/TaskQueueFarm.o: include/TaskQueueFarm.h include/TaskQueue.h
src/TaskQueueFarm.o: include/Lockable.h include/Global.h include/HttpDebug.h
src/Texture.o: /usr/local/include/GL/glew.h include/Texture.h
src/Texture.o: include/Shader.h /usr/local/include/cglm/cglm.h
src/Texture.o: /usr/local/include/cglm/common.h
src/Texture.o: /usr/local/include/cglm/types.h
src/Texture.o: /usr/local/include/cglm/simd/intrin.h
src/Texture.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Texture.o: /usr/local/include/cglm/vec2-ext.h
src/Texture.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Texture.o: /usr/local/include/cglm/vec4-ext.h
src/Texture.o: /usr/local/include/cglm/vec3-ext.h
src/Texture.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Texture.o: /usr/local/include/cglm/mat2.h
src/Texture.o: /usr/local/include/cglm/affine.h
src/Texture.o: /usr/local/include/cglm/affine-mat.h
src/Texture.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Texture.o: /usr/local/include/cglm/frustum.h
src/Texture.o: /usr/local/include/cglm/quat.h /usr/local/include/cglm/euler.h
src/Texture.o: /usr/local/include/cglm/box.h /usr/local/include/cglm/color.h
src/Texture.o: /usr/local/include/cglm/io.h /usr/local/include/cglm/project.h
src/Texture.o: /usr/local/include/cglm/sphere.h
src/Texture.o: /usr/local/include/cglm/ease.h /usr/local/include/cglm/curve.h
src/Texture.o: /usr/local/include/cglm/bezier.h /usr/local/include/cglm/ray.h
src/Texture.o: /usr/local/include/cglm/affine2d.h include/stb_image.h
src/TextureAtlas.o: include/TextureAtlas.h include/Texture.h
src/TextureAtlas.o: /usr/local/include/GL/glew.h include/Shader.h
src/TextureAtlas.o: /usr/local/include/cglm/cglm.h
src/TextureAtlas.o: /usr/local/include/cglm/common.h
src/TextureAtlas.o: /usr/local/include/cglm/types.h
src/TextureAtlas.o: /usr/local/include/cglm/simd/intrin.h
src/TextureAtlas.o: /usr/local/include/cglm/vec2.h
src/TextureAtlas.o: /usr/local/include/cglm/util.h
src/TextureAtlas.o: /usr/local/include/cglm/vec2-ext.h
src/TextureAtlas.o: /usr/local/include/cglm/vec3.h
src/TextureAtlas.o: /usr/local/include/cglm/vec4.h
src/TextureAtlas.o: /usr/local/include/cglm/vec4-ext.h
src/TextureAtlas.o: /usr/local/include/cglm/vec3-ext.h
src/TextureAtlas.o: /usr/local/include/cglm/mat4.h
src/TextureAtlas.o: /usr/local/include/cglm/mat3.h
src/TextureAtlas.o: /usr/local/include/cglm/mat2.h
src/TextureAtlas.o: /usr/local/include/cglm/affine.h
src/TextureAtlas.o: /usr/local/include/cglm/affine-mat.h
src/TextureAtlas.o: /usr/local/include/cglm/cam.h
src/TextureAtlas.o: /usr/local/include/cglm/plane.h
src/TextureAtlas.o: /usr/local/include/cglm/frustum.h
src/TextureAtlas.o: /usr/local/include/cglm/quat.h
src/TextureAtlas.o: /usr/local/include/cglm/euler.h
src/TextureAtlas.o: /usr/local/include/cglm/box.h
src/TextureAtlas.o: /usr/local/include/cglm/color.h
src/TextureAtlas.o: /usr/local/include/cglm/io.h
src/TextureAtlas.o: /usr/local/include/cglm/project.h
src/TextureAtlas.o: /usr/local/include/cglm/sphere.h
src/TextureAtlas.o: /usr/local/include/cglm/ease.h
src/TextureAtlas.o: /usr/local/include/cglm/curve.h
src/TextureAtlas.o: /usr/local/include/cglm/bezier.h
src/TextureAtlas.o: /usr/local/include/cglm/ray.h
src/TextureAtlas.o: /usr/local/include/cglm/affine2d.h include/Logging.h
src/TextureAtlas.o: include/MemoryTracker.h include/HttpDebug.h
src/TextureAtlas.o: include/Global.h include/stb_image_write.h
src/TexturedRect.o: /usr/local/include/GL/glew.h
src/TexturedRect.o: /usr/local/include/GLFW/glfw3.h include/TexturedRect.h
src/TexturedRect.o: /usr/local/include/cglm/cglm.h
src/TexturedRect.o: /usr/local/include/cglm/common.h
src/TexturedRect.o: /usr/local/include/cglm/types.h
src/TexturedRect.o: /usr/local/include/cglm/simd/intrin.h
src/TexturedRect.o: /usr/local/include/cglm/vec2.h
src/TexturedRect.o: /usr/local/include/cglm/util.h
src/TexturedRect.o: /usr/local/include/cglm/vec2-ext.h
src/TexturedRect.o: /usr/local/include/cglm/vec3.h
src/TexturedRect.o: /usr/local/include/cglm/vec4.h
src/TexturedRect.o: /usr/local/include/cglm/vec4-ext.h
src/TexturedRect.o: /usr/local/include/cglm/vec3-ext.h
src/TexturedRect.o: /usr/local/include/cglm/mat4.h
src/TexturedRect.o: /usr/local/include/cglm/mat3.h
src/TexturedRect.o: /usr/local/include/cglm/mat2.h
src/TexturedRect.o: /usr/local/include/cglm/affine.h
src/TexturedRect.o: /usr/local/include/cglm/affine-mat.h
src/TexturedRect.o: /usr/local/include/cglm/cam.h
src/TexturedRect.o: /usr/local/include/cglm/plane.h
src/TexturedRect.o: /usr/local/include/cglm/frustum.h
src/TexturedRect.o: /usr/local/include/cglm/quat.h
src/TexturedRect.o: /usr/local/include/cglm/euler.h
src/TexturedRect.o: /usr/local/include/cglm/box.h
src/TexturedRect.o: /usr/local/include/cglm/color.h
src/TexturedRect.o: /usr/local/include/cglm/io.h
src/TexturedRect.o: /usr/local/include/cglm/project.h
src/TexturedRect.o: /usr/local/include/cglm/sphere.h
src/TexturedRect.o: /usr/local/include/cglm/ease.h
src/TexturedRect.o: /usr/local/include/cglm/curve.h
src/TexturedRect.o: /usr/local/include/cglm/bezier.h
src/TexturedRect.o: /usr/local/include/cglm/ray.h
src/TexturedRect.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/TexturedRect.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/TexturedRect.o: include/Global.h include/Logging.h
src/TexturedRect.o: include/MemoryTracker.h include/HttpDebug.h
src/TexturedRect.o: include/Vertex.h include/Texture.h
src/Timeval.o: include/Timeval.h include/Global.h
src/Tree.o: include/Tree.h include/VisualObject.h include/VisualElement.h
src/Tree.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/Tree.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Tree.o: /usr/local/include/cglm/cglm.h /usr/local/include/cglm/common.h
src/Tree.o: /usr/local/include/cglm/types.h
src/Tree.o: /usr/local/include/cglm/simd/intrin.h
src/Tree.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Tree.o: /usr/local/include/cglm/vec2-ext.h /usr/local/include/cglm/vec3.h
src/Tree.o: /usr/local/include/cglm/vec4.h /usr/local/include/cglm/vec4-ext.h
src/Tree.o: /usr/local/include/cglm/vec3-ext.h /usr/local/include/cglm/mat4.h
src/Tree.o: /usr/local/include/cglm/mat3.h /usr/local/include/cglm/mat2.h
src/Tree.o: /usr/local/include/cglm/affine.h
src/Tree.o: /usr/local/include/cglm/affine-mat.h
src/Tree.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Tree.o: /usr/local/include/cglm/frustum.h /usr/local/include/cglm/quat.h
src/Tree.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Tree.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Tree.o: /usr/local/include/cglm/project.h
src/Tree.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Tree.o: /usr/local/include/cglm/curve.h /usr/local/include/cglm/bezier.h
src/Tree.o: /usr/local/include/cglm/ray.h /usr/local/include/cglm/affine2d.h
src/Tree.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Tree.o: include/Global.h include/Logging.h include/MemoryTracker.h
src/Tree.o: include/HttpDebug.h include/Vertex.h
src/Tree.o: include/ElementBufferObject.h include/BoundingBox.h
src/Tree.o: include/Shader.h include/VertexBufferCombo.h include/Lockable.h
src/Tree.o: include/Species.h include/rapidjson/document.h
src/Tree.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/Tree.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/Tree.o: include/rapidjson/encodings.h include/rapidjson/encodedstream.h
src/Tree.o: include/rapidjson/memorystream.h
src/Tree.o: include/rapidjson/internal/clzll.h include/rapidjson/rapidjson.h
src/Tree.o: include/rapidjson/internal/meta.h
src/Tree.o: include/rapidjson/internal/stack.h include/rapidjson/allocators.h
src/Tree.o: include/rapidjson/internal/swap.h
src/Tree.o: include/rapidjson/internal/strtod.h
src/Tree.o: include/rapidjson/internal/ieee754.h
src/Tree.o: include/rapidjson/internal/biginteger.h
src/Tree.o: include/rapidjson/internal/diyfp.h
src/Tree.o: include/rapidjson/internal/clzll.h
src/Tree.o: include/rapidjson/internal/pow10.h
src/Tree.o: include/rapidjson/error/error.h
src/Tree.o: include/rapidjson/internal/strfunc.h include/rapidjson/stream.h
src/Tree.o: include/rapidjson/writer.h include/rapidjson/internal/dtoa.h
src/Tree.o: include/rapidjson/internal/itoa.h
src/Tree.o: include/rapidjson/internal/itoa.h
src/Tree.o: include/rapidjson/stringbuffer.h include/rapidjson/stringbuffer.h
src/Tree.o: include/rapidjson/error/en.h include/rapidjson/error/error.h
src/Tree.o: include/LeafModel.h include/JSONStructureChecker.h
src/Tree.o: include/Timeval.h include/Version.h include/TreePart.h
src/Tree.o: include/Quadtree.h include/DisplayList.h
src/Tree.o: include/LandSurfaceRegion.h include/LandSurfaceRegionPlanar.h
src/Tree.o: include/WoodySegment.h include/Cylinder.h include/AxialElement.h
src/Tree.o: include/TruncatedCone.h include/PmodDesign.h
src/Tree.o: include/LandSurface.h include/TexturedRect.h include/Texture.h
src/Tree.o: include/Camera.h include/HeightMarker.h include/BezierPatch.h
src/Tree.o: include/PmodConfig.h include/Boundary.h include/Scene.h
src/Tree.o: include/ColoredAxes.h include/LineStripList.h include/Grid.h
src/Tree.o: include/Material.h include/TextureAtlas.h include/LightingModel.h
src/Tree.o: include/CO2Scenario.h include/TaskQueue.h
src/Tree.o: include/SkySampleModel.h include/TaskQueueFarm.h
src/TreePart.o: include/TreePart.h include/HttpDebug.h include/Species.h
src/TreePart.o: include/rapidjson/document.h include/rapidjson/reader.h
src/TreePart.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/TreePart.o: include/rapidjson/stream.h include/rapidjson/encodings.h
src/TreePart.o: include/rapidjson/encodedstream.h
src/TreePart.o: include/rapidjson/memorystream.h
src/TreePart.o: include/rapidjson/internal/clzll.h
src/TreePart.o: include/rapidjson/rapidjson.h
src/TreePart.o: include/rapidjson/internal/meta.h
src/TreePart.o: include/rapidjson/internal/stack.h
src/TreePart.o: include/rapidjson/allocators.h
src/TreePart.o: include/rapidjson/internal/swap.h
src/TreePart.o: include/rapidjson/internal/strtod.h
src/TreePart.o: include/rapidjson/internal/ieee754.h
src/TreePart.o: include/rapidjson/internal/biginteger.h
src/TreePart.o: include/rapidjson/internal/diyfp.h
src/TreePart.o: include/rapidjson/internal/clzll.h
src/TreePart.o: include/rapidjson/internal/pow10.h
src/TreePart.o: include/rapidjson/error/error.h
src/TreePart.o: include/rapidjson/internal/strfunc.h
src/TreePart.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/TreePart.o: include/rapidjson/internal/dtoa.h
src/TreePart.o: include/rapidjson/internal/itoa.h
src/TreePart.o: include/rapidjson/internal/itoa.h
src/TreePart.o: include/rapidjson/stringbuffer.h
src/TreePart.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/TreePart.o: include/rapidjson/error/error.h include/LeafModel.h
src/TreePart.o: include/JSONStructureChecker.h include/Timeval.h
src/TreePart.o: include/Version.h /usr/local/include/cglm/cglm.h
src/TreePart.o: /usr/local/include/cglm/common.h
src/TreePart.o: /usr/local/include/cglm/types.h
src/TreePart.o: /usr/local/include/cglm/simd/intrin.h
src/TreePart.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/TreePart.o: /usr/local/include/cglm/vec2-ext.h
src/TreePart.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/TreePart.o: /usr/local/include/cglm/vec4-ext.h
src/TreePart.o: /usr/local/include/cglm/vec3-ext.h
src/TreePart.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/TreePart.o: /usr/local/include/cglm/mat2.h
src/TreePart.o: /usr/local/include/cglm/affine.h
src/TreePart.o: /usr/local/include/cglm/affine-mat.h
src/TreePart.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/TreePart.o: /usr/local/include/cglm/frustum.h
src/TreePart.o: /usr/local/include/cglm/quat.h
src/TreePart.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/TreePart.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/TreePart.o: /usr/local/include/cglm/project.h
src/TreePart.o: /usr/local/include/cglm/sphere.h
src/TreePart.o: /usr/local/include/cglm/ease.h
src/TreePart.o: /usr/local/include/cglm/curve.h
src/TreePart.o: /usr/local/include/cglm/bezier.h
src/TreePart.o: /usr/local/include/cglm/ray.h
src/TreePart.o: /usr/local/include/cglm/affine2d.h include/TriangleBuffer.h
src/TreePart.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/TreePart.o: include/VertexBufferObject.h /usr/local/include/GL/glew.h
src/TreePart.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/TreePart.o: include/Logging.h include/MemoryTracker.h include/Vertex.h
src/TreePart.o: include/ElementBufferObject.h include/BoundingBox.h
src/TreePart.o: include/Shader.h include/VertexBufferCombo.h
src/TriangleBuffer.o: include/Shader.h /usr/local/include/cglm/cglm.h
src/TriangleBuffer.o: /usr/local/include/cglm/common.h
src/TriangleBuffer.o: /usr/local/include/cglm/types.h
src/TriangleBuffer.o: /usr/local/include/cglm/simd/intrin.h
src/TriangleBuffer.o: /usr/local/include/cglm/vec2.h
src/TriangleBuffer.o: /usr/local/include/cglm/util.h
src/TriangleBuffer.o: /usr/local/include/cglm/vec2-ext.h
src/TriangleBuffer.o: /usr/local/include/cglm/vec3.h
src/TriangleBuffer.o: /usr/local/include/cglm/vec4.h
src/TriangleBuffer.o: /usr/local/include/cglm/vec4-ext.h
src/TriangleBuffer.o: /usr/local/include/cglm/vec3-ext.h
src/TriangleBuffer.o: /usr/local/include/cglm/mat4.h
src/TriangleBuffer.o: /usr/local/include/cglm/mat3.h
src/TriangleBuffer.o: /usr/local/include/cglm/mat2.h
src/TriangleBuffer.o: /usr/local/include/cglm/affine.h
src/TriangleBuffer.o: /usr/local/include/cglm/affine-mat.h
src/TriangleBuffer.o: /usr/local/include/cglm/cam.h
src/TriangleBuffer.o: /usr/local/include/cglm/plane.h
src/TriangleBuffer.o: /usr/local/include/cglm/frustum.h
src/TriangleBuffer.o: /usr/local/include/cglm/quat.h
src/TriangleBuffer.o: /usr/local/include/cglm/euler.h
src/TriangleBuffer.o: /usr/local/include/cglm/box.h
src/TriangleBuffer.o: /usr/local/include/cglm/color.h
src/TriangleBuffer.o: /usr/local/include/cglm/io.h
src/TriangleBuffer.o: /usr/local/include/cglm/project.h
src/TriangleBuffer.o: /usr/local/include/cglm/sphere.h
src/TriangleBuffer.o: /usr/local/include/cglm/ease.h
src/TriangleBuffer.o: /usr/local/include/cglm/curve.h
src/TriangleBuffer.o: /usr/local/include/cglm/bezier.h
src/TriangleBuffer.o: /usr/local/include/cglm/ray.h
src/TriangleBuffer.o: /usr/local/include/cglm/affine2d.h
src/TriangleBuffer.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/TriangleBuffer.o: include/VertexArrayObject.h
src/TriangleBuffer.o: include/VertexBufferObject.h
src/TriangleBuffer.o: /usr/local/include/GL/glew.h
src/TriangleBuffer.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/TriangleBuffer.o: include/Logging.h include/MemoryTracker.h
src/TriangleBuffer.o: include/HttpDebug.h include/Vertex.h
src/TriangleBuffer.o: include/ElementBufferObject.h
src/TruncatedCone.o: include/TruncatedCone.h include/AxialElement.h
src/TruncatedCone.o: include/VisualElement.h include/TriangleBuffer.h
src/TruncatedCone.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/TruncatedCone.o: include/VertexBufferObject.h
src/TruncatedCone.o: /usr/local/include/cglm/cglm.h
src/TruncatedCone.o: /usr/local/include/cglm/common.h
src/TruncatedCone.o: /usr/local/include/cglm/types.h
src/TruncatedCone.o: /usr/local/include/cglm/simd/intrin.h
src/TruncatedCone.o: /usr/local/include/cglm/vec2.h
src/TruncatedCone.o: /usr/local/include/cglm/util.h
src/TruncatedCone.o: /usr/local/include/cglm/vec2-ext.h
src/TruncatedCone.o: /usr/local/include/cglm/vec3.h
src/TruncatedCone.o: /usr/local/include/cglm/vec4.h
src/TruncatedCone.o: /usr/local/include/cglm/vec4-ext.h
src/TruncatedCone.o: /usr/local/include/cglm/vec3-ext.h
src/TruncatedCone.o: /usr/local/include/cglm/mat4.h
src/TruncatedCone.o: /usr/local/include/cglm/mat3.h
src/TruncatedCone.o: /usr/local/include/cglm/mat2.h
src/TruncatedCone.o: /usr/local/include/cglm/affine.h
src/TruncatedCone.o: /usr/local/include/cglm/affine-mat.h
src/TruncatedCone.o: /usr/local/include/cglm/cam.h
src/TruncatedCone.o: /usr/local/include/cglm/plane.h
src/TruncatedCone.o: /usr/local/include/cglm/frustum.h
src/TruncatedCone.o: /usr/local/include/cglm/quat.h
src/TruncatedCone.o: /usr/local/include/cglm/euler.h
src/TruncatedCone.o: /usr/local/include/cglm/box.h
src/TruncatedCone.o: /usr/local/include/cglm/color.h
src/TruncatedCone.o: /usr/local/include/cglm/io.h
src/TruncatedCone.o: /usr/local/include/cglm/project.h
src/TruncatedCone.o: /usr/local/include/cglm/sphere.h
src/TruncatedCone.o: /usr/local/include/cglm/ease.h
src/TruncatedCone.o: /usr/local/include/cglm/curve.h
src/TruncatedCone.o: /usr/local/include/cglm/bezier.h
src/TruncatedCone.o: /usr/local/include/cglm/ray.h
src/TruncatedCone.o: /usr/local/include/cglm/affine2d.h
src/TruncatedCone.o: /usr/local/include/GL/glew.h
src/TruncatedCone.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/TruncatedCone.o: include/Logging.h include/MemoryTracker.h
src/TruncatedCone.o: include/HttpDebug.h include/Vertex.h
src/TruncatedCone.o: include/ElementBufferObject.h include/BoundingBox.h
src/TruncatedCone.o: include/Shader.h include/VertexBufferCombo.h
src/Version.o: include/Version.h include/rapidjson/document.h
src/Version.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/Version.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/Version.o: include/rapidjson/encodings.h
src/Version.o: include/rapidjson/encodedstream.h
src/Version.o: include/rapidjson/memorystream.h
src/Version.o: include/rapidjson/internal/clzll.h
src/Version.o: include/rapidjson/rapidjson.h
src/Version.o: include/rapidjson/internal/meta.h
src/Version.o: include/rapidjson/internal/stack.h
src/Version.o: include/rapidjson/allocators.h
src/Version.o: include/rapidjson/internal/swap.h
src/Version.o: include/rapidjson/internal/strtod.h
src/Version.o: include/rapidjson/internal/ieee754.h
src/Version.o: include/rapidjson/internal/biginteger.h
src/Version.o: include/rapidjson/internal/diyfp.h
src/Version.o: include/rapidjson/internal/clzll.h
src/Version.o: include/rapidjson/internal/pow10.h
src/Version.o: include/rapidjson/error/error.h
src/Version.o: include/rapidjson/internal/strfunc.h
src/Version.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/Version.o: include/rapidjson/internal/dtoa.h
src/Version.o: include/rapidjson/internal/itoa.h
src/Version.o: include/rapidjson/internal/itoa.h
src/Version.o: include/rapidjson/stringbuffer.h
src/Version.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Version.o: include/rapidjson/error/error.h include/Global.h
src/Vertex.o: include/Vertex.h /usr/local/include/cglm/cglm.h
src/Vertex.o: /usr/local/include/cglm/common.h
src/Vertex.o: /usr/local/include/cglm/types.h
src/Vertex.o: /usr/local/include/cglm/simd/intrin.h
src/Vertex.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Vertex.o: /usr/local/include/cglm/vec2-ext.h
src/Vertex.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Vertex.o: /usr/local/include/cglm/vec4-ext.h
src/Vertex.o: /usr/local/include/cglm/vec3-ext.h
src/Vertex.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Vertex.o: /usr/local/include/cglm/mat2.h /usr/local/include/cglm/affine.h
src/Vertex.o: /usr/local/include/cglm/affine-mat.h
src/Vertex.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Vertex.o: /usr/local/include/cglm/frustum.h
src/Vertex.o: /usr/local/include/cglm/quat.h /usr/local/include/cglm/euler.h
src/Vertex.o: /usr/local/include/cglm/box.h /usr/local/include/cglm/color.h
src/Vertex.o: /usr/local/include/cglm/io.h /usr/local/include/cglm/project.h
src/Vertex.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/Vertex.o: /usr/local/include/cglm/curve.h
src/Vertex.o: /usr/local/include/cglm/bezier.h /usr/local/include/cglm/ray.h
src/Vertex.o: /usr/local/include/cglm/affine2d.h /usr/local/include/GL/glew.h
src/Vertex.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/Vertex.o: include/Logging.h include/MemoryTracker.h include/HttpDebug.h
src/Vertex.o: include/Shader.h
src/VertexArrayObject.o: /usr/local/include/GL/glew.h
src/VertexArrayObject.o: /usr/local/include/GLFW/glfw3.h
src/VertexArrayObject.o: include/VertexArrayObject.h include/Shader.h
src/VertexArrayObject.o: /usr/local/include/cglm/cglm.h
src/VertexArrayObject.o: /usr/local/include/cglm/common.h
src/VertexArrayObject.o: /usr/local/include/cglm/types.h
src/VertexArrayObject.o: /usr/local/include/cglm/simd/intrin.h
src/VertexArrayObject.o: /usr/local/include/cglm/vec2.h
src/VertexArrayObject.o: /usr/local/include/cglm/util.h
src/VertexArrayObject.o: /usr/local/include/cglm/vec2-ext.h
src/VertexArrayObject.o: /usr/local/include/cglm/vec3.h
src/VertexArrayObject.o: /usr/local/include/cglm/vec4.h
src/VertexArrayObject.o: /usr/local/include/cglm/vec4-ext.h
src/VertexArrayObject.o: /usr/local/include/cglm/vec3-ext.h
src/VertexArrayObject.o: /usr/local/include/cglm/mat4.h
src/VertexArrayObject.o: /usr/local/include/cglm/mat3.h
src/VertexArrayObject.o: /usr/local/include/cglm/mat2.h
src/VertexArrayObject.o: /usr/local/include/cglm/affine.h
src/VertexArrayObject.o: /usr/local/include/cglm/affine-mat.h
src/VertexArrayObject.o: /usr/local/include/cglm/cam.h
src/VertexArrayObject.o: /usr/local/include/cglm/plane.h
src/VertexArrayObject.o: /usr/local/include/cglm/frustum.h
src/VertexArrayObject.o: /usr/local/include/cglm/quat.h
src/VertexArrayObject.o: /usr/local/include/cglm/euler.h
src/VertexArrayObject.o: /usr/local/include/cglm/box.h
src/VertexArrayObject.o: /usr/local/include/cglm/color.h
src/VertexArrayObject.o: /usr/local/include/cglm/io.h
src/VertexArrayObject.o: /usr/local/include/cglm/project.h
src/VertexArrayObject.o: /usr/local/include/cglm/sphere.h
src/VertexArrayObject.o: /usr/local/include/cglm/ease.h
src/VertexArrayObject.o: /usr/local/include/cglm/curve.h
src/VertexArrayObject.o: /usr/local/include/cglm/bezier.h
src/VertexArrayObject.o: /usr/local/include/cglm/ray.h
src/VertexArrayObject.o: /usr/local/include/cglm/affine2d.h
src/VertexBufferCombo.o: /usr/local/include/GL/glew.h
src/VertexBufferCombo.o: /usr/local/include/GLFW/glfw3.h
src/VertexBufferCombo.o: include/VertexBufferCombo.h
src/VertexBufferCombo.o: include/VertexArrayObject.h
src/VertexBufferCombo.o: include/VertexBufferObject.h
src/VertexBufferCombo.o: /usr/local/include/cglm/cglm.h
src/VertexBufferCombo.o: /usr/local/include/cglm/common.h
src/VertexBufferCombo.o: /usr/local/include/cglm/types.h
src/VertexBufferCombo.o: /usr/local/include/cglm/simd/intrin.h
src/VertexBufferCombo.o: /usr/local/include/cglm/vec2.h
src/VertexBufferCombo.o: /usr/local/include/cglm/util.h
src/VertexBufferCombo.o: /usr/local/include/cglm/vec2-ext.h
src/VertexBufferCombo.o: /usr/local/include/cglm/vec3.h
src/VertexBufferCombo.o: /usr/local/include/cglm/vec4.h
src/VertexBufferCombo.o: /usr/local/include/cglm/vec4-ext.h
src/VertexBufferCombo.o: /usr/local/include/cglm/vec3-ext.h
src/VertexBufferCombo.o: /usr/local/include/cglm/mat4.h
src/VertexBufferCombo.o: /usr/local/include/cglm/mat3.h
src/VertexBufferCombo.o: /usr/local/include/cglm/mat2.h
src/VertexBufferCombo.o: /usr/local/include/cglm/affine.h
src/VertexBufferCombo.o: /usr/local/include/cglm/affine-mat.h
src/VertexBufferCombo.o: /usr/local/include/cglm/cam.h
src/VertexBufferCombo.o: /usr/local/include/cglm/plane.h
src/VertexBufferCombo.o: /usr/local/include/cglm/frustum.h
src/VertexBufferCombo.o: /usr/local/include/cglm/quat.h
src/VertexBufferCombo.o: /usr/local/include/cglm/euler.h
src/VertexBufferCombo.o: /usr/local/include/cglm/box.h
src/VertexBufferCombo.o: /usr/local/include/cglm/color.h
src/VertexBufferCombo.o: /usr/local/include/cglm/io.h
src/VertexBufferCombo.o: /usr/local/include/cglm/project.h
src/VertexBufferCombo.o: /usr/local/include/cglm/sphere.h
src/VertexBufferCombo.o: /usr/local/include/cglm/ease.h
src/VertexBufferCombo.o: /usr/local/include/cglm/curve.h
src/VertexBufferCombo.o: /usr/local/include/cglm/bezier.h
src/VertexBufferCombo.o: /usr/local/include/cglm/ray.h
src/VertexBufferCombo.o: /usr/local/include/cglm/affine2d.h include/Global.h
src/VertexBufferCombo.o: include/Logging.h include/MemoryTracker.h
src/VertexBufferCombo.o: include/HttpDebug.h include/Vertex.h
src/VertexBufferObject.o: include/VertexBufferObject.h
src/VertexBufferObject.o: /usr/local/include/cglm/cglm.h
src/VertexBufferObject.o: /usr/local/include/cglm/common.h
src/VertexBufferObject.o: /usr/local/include/cglm/types.h
src/VertexBufferObject.o: /usr/local/include/cglm/simd/intrin.h
src/VertexBufferObject.o: /usr/local/include/cglm/vec2.h
src/VertexBufferObject.o: /usr/local/include/cglm/util.h
src/VertexBufferObject.o: /usr/local/include/cglm/vec2-ext.h
src/VertexBufferObject.o: /usr/local/include/cglm/vec3.h
src/VertexBufferObject.o: /usr/local/include/cglm/vec4.h
src/VertexBufferObject.o: /usr/local/include/cglm/vec4-ext.h
src/VertexBufferObject.o: /usr/local/include/cglm/vec3-ext.h
src/VertexBufferObject.o: /usr/local/include/cglm/mat4.h
src/VertexBufferObject.o: /usr/local/include/cglm/mat3.h
src/VertexBufferObject.o: /usr/local/include/cglm/mat2.h
src/VertexBufferObject.o: /usr/local/include/cglm/affine.h
src/VertexBufferObject.o: /usr/local/include/cglm/affine-mat.h
src/VertexBufferObject.o: /usr/local/include/cglm/cam.h
src/VertexBufferObject.o: /usr/local/include/cglm/plane.h
src/VertexBufferObject.o: /usr/local/include/cglm/frustum.h
src/VertexBufferObject.o: /usr/local/include/cglm/quat.h
src/VertexBufferObject.o: /usr/local/include/cglm/euler.h
src/VertexBufferObject.o: /usr/local/include/cglm/box.h
src/VertexBufferObject.o: /usr/local/include/cglm/color.h
src/VertexBufferObject.o: /usr/local/include/cglm/io.h
src/VertexBufferObject.o: /usr/local/include/cglm/project.h
src/VertexBufferObject.o: /usr/local/include/cglm/sphere.h
src/VertexBufferObject.o: /usr/local/include/cglm/ease.h
src/VertexBufferObject.o: /usr/local/include/cglm/curve.h
src/VertexBufferObject.o: /usr/local/include/cglm/bezier.h
src/VertexBufferObject.o: /usr/local/include/cglm/ray.h
src/VertexBufferObject.o: /usr/local/include/cglm/affine2d.h
src/VertexBufferObject.o: /usr/local/include/GL/glew.h
src/VertexBufferObject.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/VertexBufferObject.o: include/Logging.h include/MemoryTracker.h
src/VertexBufferObject.o: include/HttpDebug.h include/Vertex.h
src/VertexBufferObject.o: include/Shader.h
src/VisualElement.o: include/VisualElement.h include/TriangleBuffer.h
src/VisualElement.o: include/ElementBufferCombo.h include/VertexArrayObject.h
src/VisualElement.o: include/VertexBufferObject.h
src/VisualElement.o: /usr/local/include/cglm/cglm.h
src/VisualElement.o: /usr/local/include/cglm/common.h
src/VisualElement.o: /usr/local/include/cglm/types.h
src/VisualElement.o: /usr/local/include/cglm/simd/intrin.h
src/VisualElement.o: /usr/local/include/cglm/vec2.h
src/VisualElement.o: /usr/local/include/cglm/util.h
src/VisualElement.o: /usr/local/include/cglm/vec2-ext.h
src/VisualElement.o: /usr/local/include/cglm/vec3.h
src/VisualElement.o: /usr/local/include/cglm/vec4.h
src/VisualElement.o: /usr/local/include/cglm/vec4-ext.h
src/VisualElement.o: /usr/local/include/cglm/vec3-ext.h
src/VisualElement.o: /usr/local/include/cglm/mat4.h
src/VisualElement.o: /usr/local/include/cglm/mat3.h
src/VisualElement.o: /usr/local/include/cglm/mat2.h
src/VisualElement.o: /usr/local/include/cglm/affine.h
src/VisualElement.o: /usr/local/include/cglm/affine-mat.h
src/VisualElement.o: /usr/local/include/cglm/cam.h
src/VisualElement.o: /usr/local/include/cglm/plane.h
src/VisualElement.o: /usr/local/include/cglm/frustum.h
src/VisualElement.o: /usr/local/include/cglm/quat.h
src/VisualElement.o: /usr/local/include/cglm/euler.h
src/VisualElement.o: /usr/local/include/cglm/box.h
src/VisualElement.o: /usr/local/include/cglm/color.h
src/VisualElement.o: /usr/local/include/cglm/io.h
src/VisualElement.o: /usr/local/include/cglm/project.h
src/VisualElement.o: /usr/local/include/cglm/sphere.h
src/VisualElement.o: /usr/local/include/cglm/ease.h
src/VisualElement.o: /usr/local/include/cglm/curve.h
src/VisualElement.o: /usr/local/include/cglm/bezier.h
src/VisualElement.o: /usr/local/include/cglm/ray.h
src/VisualElement.o: /usr/local/include/cglm/affine2d.h
src/VisualElement.o: /usr/local/include/GL/glew.h
src/VisualElement.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/VisualElement.o: include/Logging.h include/MemoryTracker.h
src/VisualElement.o: include/HttpDebug.h include/Vertex.h
src/VisualElement.o: include/ElementBufferObject.h include/BoundingBox.h
src/VisualElement.o: include/Shader.h include/VertexBufferCombo.h
src/VisualObject.o: include/VisualObject.h include/VisualElement.h
src/VisualObject.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/VisualObject.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/VisualObject.o: /usr/local/include/cglm/cglm.h
src/VisualObject.o: /usr/local/include/cglm/common.h
src/VisualObject.o: /usr/local/include/cglm/types.h
src/VisualObject.o: /usr/local/include/cglm/simd/intrin.h
src/VisualObject.o: /usr/local/include/cglm/vec2.h
src/VisualObject.o: /usr/local/include/cglm/util.h
src/VisualObject.o: /usr/local/include/cglm/vec2-ext.h
src/VisualObject.o: /usr/local/include/cglm/vec3.h
src/VisualObject.o: /usr/local/include/cglm/vec4.h
src/VisualObject.o: /usr/local/include/cglm/vec4-ext.h
src/VisualObject.o: /usr/local/include/cglm/vec3-ext.h
src/VisualObject.o: /usr/local/include/cglm/mat4.h
src/VisualObject.o: /usr/local/include/cglm/mat3.h
src/VisualObject.o: /usr/local/include/cglm/mat2.h
src/VisualObject.o: /usr/local/include/cglm/affine.h
src/VisualObject.o: /usr/local/include/cglm/affine-mat.h
src/VisualObject.o: /usr/local/include/cglm/cam.h
src/VisualObject.o: /usr/local/include/cglm/plane.h
src/VisualObject.o: /usr/local/include/cglm/frustum.h
src/VisualObject.o: /usr/local/include/cglm/quat.h
src/VisualObject.o: /usr/local/include/cglm/euler.h
src/VisualObject.o: /usr/local/include/cglm/box.h
src/VisualObject.o: /usr/local/include/cglm/color.h
src/VisualObject.o: /usr/local/include/cglm/io.h
src/VisualObject.o: /usr/local/include/cglm/project.h
src/VisualObject.o: /usr/local/include/cglm/sphere.h
src/VisualObject.o: /usr/local/include/cglm/ease.h
src/VisualObject.o: /usr/local/include/cglm/curve.h
src/VisualObject.o: /usr/local/include/cglm/bezier.h
src/VisualObject.o: /usr/local/include/cglm/ray.h
src/VisualObject.o: /usr/local/include/cglm/affine2d.h
src/VisualObject.o: /usr/local/include/GL/glew.h
src/VisualObject.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/VisualObject.o: include/Logging.h include/MemoryTracker.h
src/VisualObject.o: include/HttpDebug.h include/Vertex.h
src/VisualObject.o: include/ElementBufferObject.h include/BoundingBox.h
src/VisualObject.o: include/Shader.h include/VertexBufferCombo.h
src/VisualObject.o: include/Lockable.h include/LandSurfaceRegion.h
src/VisualObject.o: include/Quadtree.h include/DisplayList.h
src/VisualObject.o: include/LandSurfaceRegionPlanar.h include/ObjectGroup.h
src/Window3D.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Window3D.o: /usr/local/include/cglm/cglm.h
src/Window3D.o: /usr/local/include/cglm/common.h
src/Window3D.o: /usr/local/include/cglm/types.h
src/Window3D.o: /usr/local/include/cglm/simd/intrin.h
src/Window3D.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/Window3D.o: /usr/local/include/cglm/vec2-ext.h
src/Window3D.o: /usr/local/include/cglm/vec3.h /usr/local/include/cglm/vec4.h
src/Window3D.o: /usr/local/include/cglm/vec4-ext.h
src/Window3D.o: /usr/local/include/cglm/vec3-ext.h
src/Window3D.o: /usr/local/include/cglm/mat4.h /usr/local/include/cglm/mat3.h
src/Window3D.o: /usr/local/include/cglm/mat2.h
src/Window3D.o: /usr/local/include/cglm/affine.h
src/Window3D.o: /usr/local/include/cglm/affine-mat.h
src/Window3D.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/Window3D.o: /usr/local/include/cglm/frustum.h
src/Window3D.o: /usr/local/include/cglm/quat.h
src/Window3D.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/Window3D.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/Window3D.o: /usr/local/include/cglm/project.h
src/Window3D.o: /usr/local/include/cglm/sphere.h
src/Window3D.o: /usr/local/include/cglm/ease.h
src/Window3D.o: /usr/local/include/cglm/curve.h
src/Window3D.o: /usr/local/include/cglm/bezier.h
src/Window3D.o: /usr/local/include/cglm/ray.h
src/Window3D.o: /usr/local/include/cglm/affine2d.h
src/Window3D.o: imgui/backends/imgui_impl_opengl3.h imgui/imgui.h
src/Window3D.o: imgui/imconfig.h imgui/backends/imgui_impl_glfw.h
src/Window3D.o: include/Window3D.h include/Scene.h include/Shader.h
src/Window3D.o: include/ColoredAxes.h include/LineStripList.h
src/Window3D.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/Window3D.o: include/VertexBufferObject.h include/Global.h
src/Window3D.o: include/Logging.h include/MemoryTracker.h include/HttpDebug.h
src/Window3D.o: include/Vertex.h include/PmodConfig.h include/LandSurface.h
src/Window3D.o: include/TexturedRect.h include/Texture.h include/Quadtree.h
src/Window3D.o: include/DisplayList.h include/VisualObject.h
src/Window3D.o: include/VisualElement.h include/TriangleBuffer.h
src/Window3D.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/Window3D.o: include/BoundingBox.h include/Lockable.h
src/Window3D.o: include/LandSurfaceRegion.h include/LandSurfaceRegionPlanar.h
src/Window3D.o: include/Camera.h include/HeightMarker.h include/BezierPatch.h
src/Window3D.o: include/rapidjson/document.h include/rapidjson/reader.h
src/Window3D.o: include/rapidjson/allocators.h include/rapidjson/rapidjson.h
src/Window3D.o: include/rapidjson/stream.h include/rapidjson/encodings.h
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
src/Window3D.o: include/rapidjson/stream.h include/PmodDesign.h
src/Window3D.o: include/rapidjson/writer.h include/rapidjson/internal/dtoa.h
src/Window3D.o: include/rapidjson/internal/itoa.h
src/Window3D.o: include/rapidjson/internal/itoa.h
src/Window3D.o: include/rapidjson/stringbuffer.h
src/Window3D.o: include/rapidjson/stringbuffer.h include/rapidjson/error/en.h
src/Window3D.o: include/rapidjson/error/error.h include/Timeval.h
src/Window3D.o: include/JSONStructureChecker.h include/Version.h
src/Window3D.o: include/Boundary.h include/Grid.h include/Material.h
src/Window3D.o: include/TextureAtlas.h include/LightingModel.h
src/Window3D.o: include/Species.h include/LeafModel.h include/CO2Scenario.h
src/Window3D.o: include/TaskQueue.h include/MenuInterface.h
src/Window3D.o: include/ScriptController.h include/PmodException.h
src/WoodySegment.o: include/WoodySegment.h include/TreePart.h
src/WoodySegment.o: include/HttpDebug.h include/Species.h
src/WoodySegment.o: include/rapidjson/document.h include/rapidjson/reader.h
src/WoodySegment.o: include/rapidjson/allocators.h
src/WoodySegment.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/WoodySegment.o: include/rapidjson/encodings.h
src/WoodySegment.o: include/rapidjson/encodedstream.h
src/WoodySegment.o: include/rapidjson/memorystream.h
src/WoodySegment.o: include/rapidjson/internal/clzll.h
src/WoodySegment.o: include/rapidjson/rapidjson.h
src/WoodySegment.o: include/rapidjson/internal/meta.h
src/WoodySegment.o: include/rapidjson/internal/stack.h
src/WoodySegment.o: include/rapidjson/allocators.h
src/WoodySegment.o: include/rapidjson/internal/swap.h
src/WoodySegment.o: include/rapidjson/internal/strtod.h
src/WoodySegment.o: include/rapidjson/internal/ieee754.h
src/WoodySegment.o: include/rapidjson/internal/biginteger.h
src/WoodySegment.o: include/rapidjson/internal/diyfp.h
src/WoodySegment.o: include/rapidjson/internal/clzll.h
src/WoodySegment.o: include/rapidjson/internal/pow10.h
src/WoodySegment.o: include/rapidjson/error/error.h
src/WoodySegment.o: include/rapidjson/internal/strfunc.h
src/WoodySegment.o: include/rapidjson/stream.h include/rapidjson/writer.h
src/WoodySegment.o: include/rapidjson/internal/dtoa.h
src/WoodySegment.o: include/rapidjson/internal/itoa.h
src/WoodySegment.o: include/rapidjson/internal/itoa.h
src/WoodySegment.o: include/rapidjson/stringbuffer.h
src/WoodySegment.o: include/rapidjson/stringbuffer.h
src/WoodySegment.o: include/rapidjson/error/en.h
src/WoodySegment.o: include/rapidjson/error/error.h include/LeafModel.h
src/WoodySegment.o: include/JSONStructureChecker.h include/Timeval.h
src/WoodySegment.o: include/Version.h /usr/local/include/cglm/cglm.h
src/WoodySegment.o: /usr/local/include/cglm/common.h
src/WoodySegment.o: /usr/local/include/cglm/types.h
src/WoodySegment.o: /usr/local/include/cglm/simd/intrin.h
src/WoodySegment.o: /usr/local/include/cglm/vec2.h
src/WoodySegment.o: /usr/local/include/cglm/util.h
src/WoodySegment.o: /usr/local/include/cglm/vec2-ext.h
src/WoodySegment.o: /usr/local/include/cglm/vec3.h
src/WoodySegment.o: /usr/local/include/cglm/vec4.h
src/WoodySegment.o: /usr/local/include/cglm/vec4-ext.h
src/WoodySegment.o: /usr/local/include/cglm/vec3-ext.h
src/WoodySegment.o: /usr/local/include/cglm/mat4.h
src/WoodySegment.o: /usr/local/include/cglm/mat3.h
src/WoodySegment.o: /usr/local/include/cglm/mat2.h
src/WoodySegment.o: /usr/local/include/cglm/affine.h
src/WoodySegment.o: /usr/local/include/cglm/affine-mat.h
src/WoodySegment.o: /usr/local/include/cglm/cam.h
src/WoodySegment.o: /usr/local/include/cglm/plane.h
src/WoodySegment.o: /usr/local/include/cglm/frustum.h
src/WoodySegment.o: /usr/local/include/cglm/quat.h
src/WoodySegment.o: /usr/local/include/cglm/euler.h
src/WoodySegment.o: /usr/local/include/cglm/box.h
src/WoodySegment.o: /usr/local/include/cglm/color.h
src/WoodySegment.o: /usr/local/include/cglm/io.h
src/WoodySegment.o: /usr/local/include/cglm/project.h
src/WoodySegment.o: /usr/local/include/cglm/sphere.h
src/WoodySegment.o: /usr/local/include/cglm/ease.h
src/WoodySegment.o: /usr/local/include/cglm/curve.h
src/WoodySegment.o: /usr/local/include/cglm/bezier.h
src/WoodySegment.o: /usr/local/include/cglm/ray.h
src/WoodySegment.o: /usr/local/include/cglm/affine2d.h
src/WoodySegment.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/WoodySegment.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/WoodySegment.o: /usr/local/include/GL/glew.h
src/WoodySegment.o: /usr/local/include/GLFW/glfw3.h include/Global.h
src/WoodySegment.o: include/Logging.h include/MemoryTracker.h
src/WoodySegment.o: include/Vertex.h include/ElementBufferObject.h
src/WoodySegment.o: include/BoundingBox.h include/Shader.h
src/WoodySegment.o: include/VertexBufferCombo.h include/Cylinder.h
src/WoodySegment.o: include/AxialElement.h include/VisualElement.h
src/WoodySegment.o: include/TruncatedCone.h include/PmodDesign.h
src/WoodySegment.o: include/LandSurface.h include/TexturedRect.h
src/WoodySegment.o: include/Texture.h include/Quadtree.h
src/WoodySegment.o: include/DisplayList.h include/VisualObject.h
src/WoodySegment.o: include/Lockable.h include/LandSurfaceRegion.h
src/WoodySegment.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/WoodySegment.o: include/HeightMarker.h include/BezierPatch.h
src/WoodySegment.o: include/PmodConfig.h include/Boundary.h include/Tree.h
src/imgui.o: imgui/imgui.h imgui/imconfig.h imgui/imgui_internal.h
src/imgui.o: imgui/imstb_textedit.h
src/imgui_demo.o: imgui/imgui.h imgui/imconfig.h
src/imgui_draw.o: imgui/imgui.h imgui/imconfig.h imgui/imgui_internal.h
src/imgui_draw.o: imgui/imstb_textedit.h imgui/imstb_rectpack.h
src/imgui_draw.o: imgui/imstb_truetype.h
src/imgui_impl_glfw.o: imgui/imgui.h imgui/imconfig.h
src/imgui_impl_glfw.o: imgui/backends/imgui_impl_glfw.h
src/imgui_impl_glfw.o: /usr/local/include/GLFW/glfw3.h
src/imgui_impl_opengl3.o: imgui/imgui.h imgui/imconfig.h
src/imgui_impl_opengl3.o: imgui/backends/imgui_impl_opengl3.h
src/imgui_tables.o: imgui/imgui.h imgui/imconfig.h imgui/imgui_internal.h
src/imgui_tables.o: imgui/imstb_textedit.h
src/imgui_widgets.o: imgui/imgui.h imgui/imconfig.h imgui/imgui_internal.h
src/imgui_widgets.o: imgui/imstb_textedit.h
src/loadFileToBuf.o: include/loadFileToBuf.h
src/main.o: include/Window3D.h /usr/local/include/GL/glew.h
src/main.o: /usr/local/include/GLFW/glfw3.h include/Scene.h include/Shader.h
src/main.o: /usr/local/include/cglm/cglm.h /usr/local/include/cglm/common.h
src/main.o: /usr/local/include/cglm/types.h
src/main.o: /usr/local/include/cglm/simd/intrin.h
src/main.o: /usr/local/include/cglm/vec2.h /usr/local/include/cglm/util.h
src/main.o: /usr/local/include/cglm/vec2-ext.h /usr/local/include/cglm/vec3.h
src/main.o: /usr/local/include/cglm/vec4.h /usr/local/include/cglm/vec4-ext.h
src/main.o: /usr/local/include/cglm/vec3-ext.h /usr/local/include/cglm/mat4.h
src/main.o: /usr/local/include/cglm/mat3.h /usr/local/include/cglm/mat2.h
src/main.o: /usr/local/include/cglm/affine.h
src/main.o: /usr/local/include/cglm/affine-mat.h
src/main.o: /usr/local/include/cglm/cam.h /usr/local/include/cglm/plane.h
src/main.o: /usr/local/include/cglm/frustum.h /usr/local/include/cglm/quat.h
src/main.o: /usr/local/include/cglm/euler.h /usr/local/include/cglm/box.h
src/main.o: /usr/local/include/cglm/color.h /usr/local/include/cglm/io.h
src/main.o: /usr/local/include/cglm/project.h
src/main.o: /usr/local/include/cglm/sphere.h /usr/local/include/cglm/ease.h
src/main.o: /usr/local/include/cglm/curve.h /usr/local/include/cglm/bezier.h
src/main.o: /usr/local/include/cglm/ray.h /usr/local/include/cglm/affine2d.h
src/main.o: include/ColoredAxes.h include/LineStripList.h
src/main.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/main.o: include/VertexBufferObject.h include/Global.h include/Logging.h
src/main.o: include/MemoryTracker.h include/HttpDebug.h include/Vertex.h
src/main.o: include/PmodConfig.h include/LandSurface.h include/TexturedRect.h
src/main.o: include/Texture.h include/Quadtree.h include/DisplayList.h
src/main.o: include/VisualObject.h include/VisualElement.h
src/main.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/main.o: include/ElementBufferObject.h include/BoundingBox.h
src/main.o: include/Lockable.h include/LandSurfaceRegion.h
src/main.o: include/LandSurfaceRegionPlanar.h include/Camera.h
src/main.o: include/HeightMarker.h include/BezierPatch.h
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
src/main.o: include/PmodDesign.h include/rapidjson/writer.h
src/main.o: include/rapidjson/internal/dtoa.h
src/main.o: include/rapidjson/internal/itoa.h
src/main.o: include/rapidjson/internal/itoa.h
src/main.o: include/rapidjson/stringbuffer.h include/rapidjson/stringbuffer.h
src/main.o: include/rapidjson/error/en.h include/rapidjson/error/error.h
src/main.o: include/Timeval.h include/JSONStructureChecker.h
src/main.o: include/Version.h include/Boundary.h include/Grid.h
src/main.o: include/Material.h include/TextureAtlas.h include/LightingModel.h
src/main.o: include/Species.h include/LeafModel.h include/CO2Scenario.h
src/main.o: include/TaskQueue.h include/MenuInterface.h imgui/imgui.h
src/main.o: imgui/imconfig.h include/ScriptController.h include/Tree.h
src/main.o: include/TreePart.h include/SkySampleModel.h
