cmake_minimum_required(VERSION 3.10)

project(BrowserEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(browser_engine main.cpp parser/parser.cpp html/parser.cpp html/html.cpp css/parser.cpp)