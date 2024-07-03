cmake_minimum_required(VERSION 3.10)

# Указываем компилятор GCC 13
set(CMAKE_C_COMPILER "gcc-13")
set(CMAKE_CXX_COMPILER "g++-13")

# Название проекта
project(CacheSimulatorProject C)

# Указываем путь к исходникам
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Указываем путь к папке с инклюдами (если их несколько, добавьте здесь другие пути)
include_directories(${SOURCE_DIR})

# Собирать все файлы .c в папке src
file(GLOB SOURCES ${SOURCE_DIR}/*.c)

# Создаем исполняемый файл, указывая все исходные файлы
add_executable(CacheSimulator ${SOURCES})

# Если переменная DEBUG_MODE установлена в ON, добавляем макрос компиляции
if(DEBUG_MODE)
    message("Debug mode enabled")
    target_compile_definitions(CacheSimulator PRIVATE DEBUG_MODE)
else()
    message("Debug mode disabled")
endif()

# Указываем путь для исполняемого файла
set_target_properties(CacheSimulator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
