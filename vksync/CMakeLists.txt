cmake_minimum_required(VERSION 3.24)

PROJECT(vksync)

SET(GLSLC glslc)

function(add_spirv_shader INPUT_FILE)
        add_custom_command(
                OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}.sprv
                COMMAND ${GLSLC} -O --target-env=vulkan1.2 -mfmt=num ${INPUT_FILE} -o ${INPUT_FILE}.sprv 
                # -g for debug 
                
#		COMMAND ${GLSLANGVALIDATOR} -g -x -V --target-env vulkan1.3 -o ${INPUT_FILE}.sprv ${INPUT_FILE} 
	# -x -V for spirv-v binary saved as text
	# -g for debug info
                DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
endfunction()

function(add_spirv_shader_gl INPUT_FILE)
        add_custom_command(
                OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}.sprv
                COMMAND ${GLSLC} -O0 --target-env=opengl -mfmt=num ${INPUT_FILE} -o ${INPUT_FILE}.sprv 
                DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
endfunction()

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(Vulkan REQUIRED)


add_spirv_shader(triangle.frag)
add_spirv_shader(triangle.vert)

add_spirv_shader_gl(triangle_gl.frag)
add_spirv_shader_gl(triangle_gl.vert)

ADD_EXECUTABLE(vksync
	triangle.frag.sprv
	triangle.vert.sprv

	triangle_gl.vert.sprv
	triangle_gl.frag.sprv

	triangle_renderer.h
	triangle_renderer.cpp
	
	renderer_vk.h
	renderer_vk.cpp
	
	renderer_gl.h
	renderer_gl.cpp

	interop/frame_sync_exported.h
	interop/frame_sync_exported.cpp

	renderer_interop.h
	renderer_interop.cpp

	vksync.cpp)

TARGET_INCLUDE_DIRECTORIES(vksync PRIVATE 
	..
)


TARGET_LINK_LIBRARIES(vksync
	Vulkan::Vulkan
	OpenGL::GL
	GLEW::GLEW
	igx
	igui
)




