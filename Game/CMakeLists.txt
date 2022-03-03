include_directories( inlcude headers )

file(GLOB SOURCES "src/*.cpp") 

add_executable(game ${SOURCES})

target_link_libraries(game PRIVATE engine)