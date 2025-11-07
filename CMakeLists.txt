cmake_minimum_required(VERSION 3.10)

project(rtosScheduler)

set(CMAKE_CXX_STANDARD 14) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(rtosSuite ${SOURCES})

# Embed absolute asset path
target_compile_definitions(rtosSuite PRIVATE
    ASSET_PATH="${CMAKE_SOURCE_DIR}/assets"
)

set_target_properties(rtosSuite PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY bin
)

#SFML -- Must download 2.6.2 for C++14 compatability
if(APPLE)
    # $brew install sfml@2
    set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
    include_directories(/opt/homebrew/opt/sfml@2/include)
elseif(WIN32)
    # https://www.sfml-dev.org/download/sfml/2.6.2/
    # Adjust if wrong path
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")
    include_directories("C:/SFML/include")
endif()

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
target_link_libraries(rtosSuite PRIVATE sfml-graphics sfml-window sfml-system)
