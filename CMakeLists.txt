cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(forgesigner)

SET(CMAKE_AUTOMOC ON)

find_package(onnxruntime REQUIRED)
find_package(Qt6 COMPONENTS Widgets Pdf REQUIRED)

set(SOURCES
    src/main.cpp
    src/ImageViewer.cpp
    src/Sidebar.cpp
    src/MainWindow.cpp
    src/SignatureTargetWidget.cpp
    src/InitialHintProvider.cpp
)

# # Doesn't work due to following error:
# # error: error: unknown argument: '-mno-direct-extern-access'
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

add_executable(${PROJECT_NAME} ${SOURCES})

# TODO: how do I link onnxruntime? for some reason, aux vars are empty
# # message(STATUS ${onnxruntime_INCLUDE_DIRS})
# # message(STATUS ${onnxruntime_LIBRARIES})
set(onnxruntime_LIBRARIES /usr/lib/libonnxruntime.so)

target_link_libraries(${PROJECT_NAME} PRIVATE ${onnxruntime_LIBRARIES})
target_include_directories(forgesigner PRIVATE ${onnxruntime_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt::Pdf
)

# copy_ort_dlls(forgesigner)  # TODO: Needed ?
