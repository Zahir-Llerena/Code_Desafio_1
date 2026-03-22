cmake_minimum_required(VERSION 3.16)

project(Code_Desafio_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Code_Desafio_1 main.cpp)

include(GNUInstallDirs)
install(TARGETS Code_Desafio_1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
