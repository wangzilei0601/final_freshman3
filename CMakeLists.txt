# 指定 CMake 最低版本
cmake_minimum_required(VERSION 3.19)

# 项目名（不要用横杠）
project(mediaplayer VERSION 1.0 LANGUAGES CXX)

# 使用 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 自动处理 moc / rcc / uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ---- 查找 Qt 模块 ----
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets REQUIRED)

# ---- 可执行文件（所有 .cpp 源文件都列在这里） ----
add_executable(mediaplayer
    mediaplayer/src/main.cpp
    mediaplayer/src/MainWindow.cpp
    mediaplayer/src/Track.cpp
    mediaplayer/src/Playlist.cpp
    mediaplayer/src/LibraryManager.cpp
    mediaplayer/src/LrcParser.cpp
    mediaplayer/src/LyricsWidget.cpp
    mediaplayer/src/PlayerController.cpp
    mediaplayer/src/Transcoder.cpp
    # 如果你还有别的 .cpp 文件，继续加在这里
)

# ---- 【关键】告诉编译器：头文件在 mediaplayer/include/ 里 ----
target_include_directories(mediaplayer PRIVATE
    mediaplayer/include
)

# ---- 链接 Qt 库 ----
target_link_libraries(mediaplayer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
)