cmake_minimum_required(VERSION 3.15)

project(Terminux LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

option(DOWNLOAD_OPENCV "Download OpenCV if not available" ON)

#[[
if (DOWNLOAD_OPENCV)
  include(FetchContent)
  FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv.git
  )
  FetchContent_MakeAvailable(opencv)
  include_directories(${opencv_SOURCE_DIR}/include)
else()
  find_package(OpenCV REQUIRED)
endif()
]]

find_package(OpenCV REQUIRED)
find_package(PNG REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES
  src/Frame.cpp
  src/util.cpp
  src/StaticCanvas.cpp
  src/BrightnessVector.cpp
)

set(TEST_FILES
  tests/module-1-tests.cpp
)

add_executable(terminux ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(terminux ${OpenCV_LIBS})
target_link_libraries(terminux PNG::PNG)
if (DOWNLOAD_OPENCV)
  target_link_libraries(terminux opencv_core opencv_imgcodecs opencv_highgui)
endif()

set_target_properties(terminux PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_BINARY_DIR}/bin)
