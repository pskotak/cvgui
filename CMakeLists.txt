cmake_minimum_required(VERSION 3.11)

project(cvgui)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
find_package( OpenCV REQUIRED )

set(SOURCE_FILES
    fonty.cpp
    cvgui.cpp
    main.cpp    
)

add_executable(cvgui ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(cvgui ${OpenCV_LIBS})

install(TARGETS cvgui RUNTIME DESTINATION bin)
