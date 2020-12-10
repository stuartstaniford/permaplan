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
				$(RM) src/*.o *~ $(MAIN)

depend: $(SRCS)
				makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

src/Arrow.o: include/Arrow.h include/VisualObject.h include/BoundingBox.h
src/Arrow.o: /usr/local/include/cglm/cglm.h /usr/local/include/cglm/common.h
src/Arrow.o: /usr/local/include/cglm/types.h
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
src/Arrow.o: include/Shader.h include/VertexBufferCombo.h
src/Arrow.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Arrow.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Arrow.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/Arrow.o: include/HttpDebug.h include/TriangleBuffer.h
src/Arrow.o: include/ElementBufferCombo.h include/ElementBufferObject.h
src/BezierPatch.o: include/BezierPatch.h include/LandSurfaceRegion.h
src/BezierPatch.o: include/VisualObject.h include/BoundingBox.h
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
src/BezierPatch.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/BezierPatch.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/BezierPatch.o: include/VertexBufferObject.h /usr/local/include/GL/glew.h
src/BezierPatch.o: /usr/local/include/GLFW/glfw3.h include/GlobalMacros.h
src/BezierPatch.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/BezierPatch.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/BezierPatch.o: include/ElementBufferObject.h include/Quadtree.h
src/BezierPatch.o: include/DisplayList.h include/LandSurfaceRegionPlanar.h
src/BezierPatch.o: include/HeightMarker.h include/Arrow.h
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
src/BoundingBox.o: include/VertexBufferObject.h include/GlobalMacros.h
src/BoundingBox.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/Box.o: include/Box.h include/VisualObject.h include/BoundingBox.h
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
src/Box.o: include/Shader.h include/VertexBufferCombo.h
src/Box.o: include/VertexArrayObject.h include/VertexBufferObject.h
src/Box.o: /usr/local/include/GL/glew.h /usr/local/include/GLFW/glfw3.h
src/Box.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/Box.o: include/HttpDebug.h include/TriangleBuffer.h
src/Box.o: include/ElementBufferCombo.h include/ElementBufferObject.h
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
src/Camera.o: include/Shader.h include/HttpDebug.h
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
src/ColoredAxes.o: include/VertexBufferObject.h include/GlobalMacros.h
src/ColoredAxes.o: include/Logging.h include/Vertex.h include/Shader.h
src/ColoredAxes.o: include/HttpDebug.h
src/DisplayList.o: include/DisplayList.h include/VisualObject.h
src/DisplayList.o: include/BoundingBox.h /usr/local/include/cglm/cglm.h
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
src/DisplayList.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/DisplayList.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/DisplayList.o: include/VertexBufferObject.h /usr/local/include/GL/glew.h
src/DisplayList.o: /usr/local/include/GLFW/glfw3.h include/GlobalMacros.h
src/DisplayList.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/DisplayList.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/DisplayList.o: include/ElementBufferObject.h include/HeightMarker.h
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
src/ElementBufferCombo.o: /usr/local/include/cglm/affine2d.h
src/ElementBufferCombo.o: include/GlobalMacros.h include/Logging.h
src/ElementBufferCombo.o: include/Vertex.h include/ElementBufferObject.h
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
src/HeightMarker.o: include/BoundingBox.h /usr/local/include/cglm/cglm.h
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
src/HeightMarker.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/HeightMarker.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/HeightMarker.o: include/VertexBufferObject.h /usr/local/include/GL/glew.h
src/HeightMarker.o: /usr/local/include/GLFW/glfw3.h include/GlobalMacros.h
src/HeightMarker.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/HeightMarker.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/HeightMarker.o: include/ElementBufferObject.h
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
src/HttpDebug.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/HttpDebug.o: include/PmodConfig.h include/LandSurface.h
src/HttpDebug.o: include/PmodDesign.h include/rapidjson/document.h
src/HttpDebug.o: include/rapidjson/reader.h include/rapidjson/allocators.h
src/HttpDebug.o: include/rapidjson/rapidjson.h include/rapidjson/stream.h
src/HttpDebug.o: include/rapidjson/encodings.h
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
src/LandSurface.o: include/HttpDebug.h include/LandSurface.h
src/LandSurface.o: include/PmodDesign.h include/rapidjson/document.h
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
src/LandSurfaceRegion.o: include/BoundingBox.h /usr/local/include/cglm/cglm.h
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
src/LandSurfaceRegion.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/LandSurfaceRegion.o: include/VertexBufferCombo.h
src/LandSurfaceRegion.o: include/VertexArrayObject.h
src/LandSurfaceRegion.o: include/VertexBufferObject.h
src/LandSurfaceRegion.o: /usr/local/include/GL/glew.h
src/LandSurfaceRegion.o: /usr/local/include/GLFW/glfw3.h
src/LandSurfaceRegion.o: include/GlobalMacros.h include/Logging.h
src/LandSurfaceRegion.o: include/Vertex.h include/HttpDebug.h
src/LandSurfaceRegion.o: include/TriangleBuffer.h
src/LandSurfaceRegion.o: include/ElementBufferCombo.h
src/LandSurfaceRegion.o: include/ElementBufferObject.h
src/LandSurfaceRegionPlanar.o: include/LandSurfaceRegionPlanar.h
src/LandSurfaceRegionPlanar.o: include/LandSurfaceRegion.h
src/LandSurfaceRegionPlanar.o: include/VisualObject.h include/BoundingBox.h
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
src/LandSurfaceRegionPlanar.o: include/Shader.h include/VertexBufferCombo.h
src/LandSurfaceRegionPlanar.o: include/VertexArrayObject.h
src/LandSurfaceRegionPlanar.o: include/VertexBufferObject.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/GL/glew.h
src/LandSurfaceRegionPlanar.o: /usr/local/include/GLFW/glfw3.h
src/LandSurfaceRegionPlanar.o: include/GlobalMacros.h include/Logging.h
src/LandSurfaceRegionPlanar.o: include/Vertex.h include/HttpDebug.h
src/LandSurfaceRegionPlanar.o: include/TriangleBuffer.h
src/LandSurfaceRegionPlanar.o: include/ElementBufferCombo.h
src/LandSurfaceRegionPlanar.o: include/ElementBufferObject.h
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
src/LineStripList.o: /usr/local/include/GLFW/glfw3.h include/GlobalMacros.h
src/LineStripList.o: include/Logging.h include/Vertex.h include/Shader.h
src/LineStripList.o: include/HttpDebug.h
src/Logging.o: include/Logging.h include/Vertex.h
src/Logging.o: /usr/local/include/cglm/cglm.h
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
src/Logging.o: include/GlobalMacros.h
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
src/Material.o: /usr/local/include/cglm/affine2d.h include/Texture.h
src/Material.o: /usr/local/include/GL/glew.h include/Shader.h
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
src/Scene.o: /usr/local/include/GL/glew.h include/Scene.h include/Shader.h
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
src/Scene.o: include/ColoredAxes.h include/LineStripList.h
src/Scene.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/Scene.o: include/VertexBufferObject.h /usr/local/include/GLFW/glfw3.h
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
src/ScriptController.o: /usr/local/include/GLFW/glfw3.h
src/ScriptController.o: include/GlobalMacros.h include/Logging.h
src/ScriptController.o: include/Vertex.h include/ScriptController.h
src/ScriptController.o: include/HttpDebug.h include/PmodDesign.h
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
src/ScriptController.o: include/rapidjson/error/error.h include/PmodConfig.h
src/ScriptController.o: include/Camera.h include/Shader.h
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
src/TextureAtlas.o: include/GlobalMacros.h
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
src/TexturedRect.o: include/GlobalMacros.h include/Logging.h include/Vertex.h
src/TexturedRect.o: include/Texture.h
src/Timeval.o: include/Timeval.h
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
src/TriangleBuffer.o: /usr/local/include/GLFW/glfw3.h include/GlobalMacros.h
src/TriangleBuffer.o: include/Logging.h include/Vertex.h
src/TriangleBuffer.o: include/ElementBufferObject.h include/HttpDebug.h
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
src/Vertex.o: /usr/local/include/GLFW/glfw3.h include/GlobalMacros.h
src/Vertex.o: include/Logging.h include/Shader.h
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
src/VertexBufferCombo.o: /usr/local/include/cglm/affine2d.h
src/VertexBufferCombo.o: include/GlobalMacros.h include/Logging.h
src/VertexBufferCombo.o: include/Vertex.h
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
src/VertexBufferObject.o: /usr/local/include/GLFW/glfw3.h
src/VertexBufferObject.o: include/GlobalMacros.h include/Logging.h
src/VertexBufferObject.o: include/Vertex.h include/Shader.h
src/VisualObject.o: include/VisualObject.h include/BoundingBox.h
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
src/VisualObject.o: /usr/local/include/cglm/affine2d.h include/Shader.h
src/VisualObject.o: include/VertexBufferCombo.h include/VertexArrayObject.h
src/VisualObject.o: include/VertexBufferObject.h /usr/local/include/GL/glew.h
src/VisualObject.o: /usr/local/include/GLFW/glfw3.h include/GlobalMacros.h
src/VisualObject.o: include/Logging.h include/Vertex.h include/HttpDebug.h
src/VisualObject.o: include/TriangleBuffer.h include/ElementBufferCombo.h
src/VisualObject.o: include/ElementBufferObject.h
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
