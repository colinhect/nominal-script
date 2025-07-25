project(Nominal CXX)

enable_language(C)

set(NOM_MAJOR_VERSION 0)
set(NOM_MINOR_VERSION 0)
set(NOM_PATCH_VERSION 2)
set(NOM_VERSION ${NOM_MAJOR_VERSION}.${NOM_MINOR_VERSION}.${NOM_PATCH_VERSION})

option(COVERAGE "Whether Nominal should be built with code coverage" OFF)
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/output" CACHE PATH "Output directory for built files")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
endforeach()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(
    SYSTEM "${PROJECT_SOURCE_DIR}/dependencies/catch"
    )

add_subdirectory("${PROJECT_SOURCE_DIR}/executable")
add_subdirectory("${PROJECT_SOURCE_DIR}/library")
add_subdirectory("${PROJECT_SOURCE_DIR}/tests")

