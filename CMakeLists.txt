cmake_minimum_required(VERSION 3.16)
project(Tetris LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# 定义可执行文件名
set(EXECUTABLE_NAME Tetris)

# 添加包含目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加 SFML 子目录
add_subdirectory(third_party/SFML)

file(GLOB SOURCES "src/*.cpp")

# 在这里添加 WIN32，并且使用变量 EXECUTABLE_NAME 作为可执行文件名
add_executable(${EXECUTABLE_NAME} WIN32 ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/third_party/SFML/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        VERBATIM)
endif()

# 复制images目录到bin目录
add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/images $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/images
    VERBATIM)
