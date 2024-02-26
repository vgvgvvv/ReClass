cmake_minimum_required(VERSION 3.17)

project(ReClass)
if(NOT REMAKE_ROOT_PATH)
    include(InitProject.cmake)
    InitProject()
    ReMake_AddSubDirsRec(test)
endif()
ReMake_AddSubDirsRec(src)
