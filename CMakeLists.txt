cmake_minimum_required(VERSION 3.5)

project(forgesigner)

SET(CMAKE_AUTOMOC ON)


find_package(Qt6 COMPONENTS Widgets Pdf REQUIRED)

set(SOURCES
    src/main.cpp
    src/ImageViewer.cpp
    src/Sidebar.cpp
    src/MainWindow.cpp
    src/SignatureTargetWidget.cpp
)

# # Doesn't work due to following error:
# # error: error: unknown argument: '-mno-direct-extern-access'
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt::Pdf
)
