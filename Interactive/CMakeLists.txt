include_directories(
    "${PROJECT_SOURCE_DIR}/Library/Include"
    )

file(
    GLOB
    SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Interactive/Source/*.h"
    "${PROJECT_SOURCE_DIR}/Interactive/Source/*.c"
)

if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
    include(CheckIncludeFiles)
    check_include_files(vld.h NOMINAL_USE_VLD)
elseif(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
endif()

add_executable(NominalInteractive ${SOURCE_FILES})    
target_link_libraries(NominalInteractive Nominal)

set(REG_EXT "[^/]*([.]c|[.]h)$")

source_group("Source" REGULAR_EXPRESSION "${REG_SOURCE}${REG_EXT}")

set_target_properties(NominalInteractive PROPERTIES
    PROJECT_LABEL Interactive
    OUTPUT_NAME nominal
    LINKER_LANGUAGE C
    )
