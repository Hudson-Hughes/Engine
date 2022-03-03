set (CMAKE_CXX_STANDARD 20)

include_directories( inlcude headers )

file(GLOB SOURCES "src/*.cpp") 

file(GLOB INCLUDE "include/*.hpp") 

add_library( engine STATIC ${SOURCES} ${INCLUDE} )

target_include_directories( engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/headers" )

target_compile_definitions( engine PUBLIC ENGINE_VERSION=8 )