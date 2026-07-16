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

# ---- 查找 Qt 模块（关键改动在这里！） ----
# 增加了 Multimedia 和 MultimediaWidgets
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets REQUIRED)

# ---- 可执行文件 ----
add_executable(mediaplayer
    mediaplayer/src/main.cpp
    mediaplayer/src/mainwindow.cpp
    mediaplayer/src/mainwindow.h
    # 如果你有 .qrc 资源文件，也列在这里
    # mediaplayer/resources.qrc
)

# ---- 链接 Qt 库（关键改动在这里！） ----
target_link_libraries(mediaplayer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets   # 如果只做音频，这行可以删掉
)