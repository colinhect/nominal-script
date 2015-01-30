include_directories(
    "${PROJECT_SOURCE_DIR}/Library/Include"
    )

file(
    GLOB
    SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Tests/Source/*.h"
    "${PROJECT_SOURCE_DIR}/Tests/Source/*.cpp"
)

if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
    include(CheckIncludeFiles)
    check_include_files(vld.h NOMINAL_USE_VLD)
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
endif()

add_executable(NominalTests ${SOURCE_FILES})    
target_link_libraries(NominalTests Nominal)

set(REG_EXT "[^/]*([.]c|[.]h)$")

source_group("Source" REGULAR_EXPRESSION "${REG_SOURCE}${REG_EXT}")

set_target_properties(NominalTests PROPERTIES
    PROJECT_LABEL Tests
    OUTPUT_NAME nominaltests
    LINKER_LANGUAGE CXX
    )
