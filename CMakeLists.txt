cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)

project(
        Shirts_Engine   # Project Name
        VERSION 0.0.1   # Version
        LANGUAGES CXX   # Langauge
)

set(CMAKE_CXX_STANDARD 20)

# MSVC 환경에서 LNK 에러 발생해서 추가됨
if (WIN32)
    SET(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "/NODEFAULTLIBS:MSVCRT")
    if(CMAKE_BUILD_TYPE EQUAL Release)
        string(REPLACE "-O2" "-O0" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
        string(REPLACE "-O2" "-O0" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    endif()

    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.12.25835)
        set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
        set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
    endif()
endif ()

# 리소스 폴더 복사
file(COPY "${PROJECT_SOURCE_DIR}/resource" DESTINATION ${CMAKE_BINARY_DIR})

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# ---------------------------------------------------------------------------------------- #
# Engine
set(ENGINE_TARGET "${PROJECT_NAME}")
set(ENGINE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(ENGINE_INCLUDE_FILES
        "${ENGINE_INCLUDE_DIR}/component/Component.h"
        "${ENGINE_INCLUDE_DIR}/entity/Entity.h"
        "${ENGINE_INCLUDE_DIR}/entity/EntityDB.h"
#        "${ENGINE_INCLUDE_DIR}/shader/Shader.h"
        )

set(ENGINE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

set(ENGINE_SOURCE_FILES
        "${ENGINE_SOURCE_DIR}/component/Component.cpp"
        "${ENGINE_SOURCE_DIR}/entity/Entity.cpp"
        "${ENGINE_SOURCE_DIR}/entity/EntityDB.cpp"
        "${ENGINE_SOURCE_DIR}/math/Math.cpp"
	    "${ENGINE_SOURCE_DIR}/component/CollisionComponent.cpp" 
	    "${ENGINE_SOURCE_DIR}/component/PhysicsComponent.cpp" 
        "${ENGINE_SOURCE_DIR}/component/TransformComponent.cpp"
	    "${ENGINE_SOURCE_DIR}/render/Renderer.cpp"
	    "${ENGINE_SOURCE_DIR}/component/LightComponent.cpp"

#        "${ENGINE_SOURCE_DIR}/shader/Shader.cpp"
#        "${ENGINE_SOURCE_DIR}/core/Game.cpp"
#        "${ENGINE_SOURCE_DIR}/render/VertexArray.cpp"
#       "${ENGINE_SOURCE_DIR}/render/Texture.cpp"
#        "${ENGINE_SOURCE_DIR}/render/Camera.cpp"
#       "${ENGINE_SOURCE_DIR}/render/Mesh.cpp"
#        "${ENGINE_SOURCE_DIR}/render/Model.cpp"

        )

# debug
add_compile_options(-g -ggdb)


# Engine Target
add_library(${ENGINE_TARGET} STATIC ${ENGINE_SOURCE_FILES} ${ENGINE_INCLUDE_FILES})
target_include_directories(${ENGINE_TARGET} PUBLIC "${ENGINE_INCLUDE_DIR}")
target_link_libraries(${ENGINE_TARGET} ${CONAN_LIBS})

# ---------------------------------------------------------------------------------------- #
# Test
set(TEST_TARGET "${PROJECT_NAME}_test")
set(TEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/test/include")
set(TEST_SOURCE_DIR "${PROJECT_SOURCE_DIR}/test/src")
set(TEST_SOURCE_FILES "${TEST_SOURCE_DIR}/main.cpp")

# Test target
add_executable(${TEST_TARGET} ${TEST_SOURCE_FILES})
target_include_directories(${TEST_TARGET} PRIVATE "${TEST_INCLUDE_DIR}")
target_link_libraries(${TEST_TARGET} ${ENGINE_TARGET})