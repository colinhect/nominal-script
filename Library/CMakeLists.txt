include_directories(
    "${PROJECT_SOURCE_DIR}/Library/Include"
    )

file(
    GLOB
    SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Library/Include/*.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/*.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/*.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/*.c"
    )

if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS /DNOM_EXPORTS")
    include(CheckIncludeFiles)
    check_include_files(vld.h NOM_USE_VLD)
elseif(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -pedantic -Wno-missing-braces -Wno-missing-field-initializers")
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/Library/config.h.in"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/config.h"
    )

add_library(Nominal SHARED ${SOURCE_FILES})

set(REG_INCLUDE ".*/Include/")
set(REG_INCLUDE_NOMINAL ".*/Include/nominal/")
set(REG_SOURCE ".*/Source/")
set(REG_EXT "[^/]*([.]c|[.]h)$")

source_group("Include" REGULAR_EXPRESSION "${REG_INCLUDE}${REG_EXT}")
source_group("Include\\nominal" REGULAR_EXPRESSION "${REG_INCLUDE_NOMINAL}${REG_EXT}")
source_group("Source" REGULAR_EXPRESSION "${REG_SOURCE}${REG_EXT}")

set_target_properties(Nominal PROPERTIES
    PROJECT_LABEL Library
    OUTPUT_NAME nominal-${NOM_VERSION}
    LINKER_LANGUAGE C
    )
