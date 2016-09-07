include_directories(
    "${PROJECT_SOURCE_DIR}/Library/Include"
    )

set(SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal.hpp"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/export.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/function.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/map.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/number.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/state.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/string.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/nominal/value.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/codegen.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/codegen.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/function.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/function.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/hashtable.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/hashtable.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/heap.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/heap.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/lexer.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/lexer.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/map.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/map.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/node.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/node.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/number.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/parser.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/parser.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/prelude.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/prelude.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/state.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/state.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/string.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/string.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/stringpool.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/stringpool.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/value.c"
    "${PROJECT_SOURCE_DIR}/Library/Source/value.h"
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

add_library(NominalLibrary SHARED ${SOURCE_FILES})

set(REG_INCLUDE ".*/Include/")
set(REG_INCLUDE_NOMINAL ".*/Include/nominal/")
set(REG_SOURCE ".*/Source/")
set(REG_EXT "[^/]*([.]c|[.]h|[.]hpp)$")

source_group("Include" REGULAR_EXPRESSION "${REG_INCLUDE}${REG_EXT}")
source_group("Include\\nominal" REGULAR_EXPRESSION "${REG_INCLUDE_NOMINAL}${REG_EXT}")
source_group("Source" REGULAR_EXPRESSION "${REG_SOURCE}${REG_EXT}")

set_target_properties(NominalLibrary PROPERTIES
    PROJECT_LABEL Library
    OUTPUT_NAME nominal-${NOM_VERSION}
    LINKER_LANGUAGE C
    )
