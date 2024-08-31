include_directories(
    "${PROJECT_SOURCE_DIR}/library/include"
    )

set(SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/library/include/nominal.h"
    "${PROJECT_SOURCE_DIR}/library/include/nominal.hpp"
    "${PROJECT_SOURCE_DIR}/library/include/nominal/export.h"
    "${PROJECT_SOURCE_DIR}/library/include/nominal/function.h"
    "${PROJECT_SOURCE_DIR}/library/include/nominal/map.h"
    "${PROJECT_SOURCE_DIR}/library/include/nominal/number.h"
    "${PROJECT_SOURCE_DIR}/library/include/nominal/state.h"
    "${PROJECT_SOURCE_DIR}/library/include/nominal/string.h"
    "${PROJECT_SOURCE_DIR}/library/include/nominal/value.h"
    "${PROJECT_SOURCE_DIR}/library/source/codegen.c"
    "${PROJECT_SOURCE_DIR}/library/source/codegen.h"
    "${PROJECT_SOURCE_DIR}/library/source/function.c"
    "${PROJECT_SOURCE_DIR}/library/source/function.h"
    "${PROJECT_SOURCE_DIR}/library/source/hashtable.c"
    "${PROJECT_SOURCE_DIR}/library/source/hashtable.h"
    "${PROJECT_SOURCE_DIR}/library/source/heap.c"
    "${PROJECT_SOURCE_DIR}/library/source/heap.h"
    "${PROJECT_SOURCE_DIR}/library/source/lexer.c"
    "${PROJECT_SOURCE_DIR}/library/source/lexer.h"
    "${PROJECT_SOURCE_DIR}/library/source/map.c"
    "${PROJECT_SOURCE_DIR}/library/source/map.h"
    "${PROJECT_SOURCE_DIR}/library/source/node.c"
    "${PROJECT_SOURCE_DIR}/library/source/node.h"
    "${PROJECT_SOURCE_DIR}/library/source/number.c"
    "${PROJECT_SOURCE_DIR}/library/source/parser.c"
    "${PROJECT_SOURCE_DIR}/library/source/parser.h"
    "${PROJECT_SOURCE_DIR}/library/source/prelude.c"
    "${PROJECT_SOURCE_DIR}/library/source/prelude.h"
    "${PROJECT_SOURCE_DIR}/library/source/state.c"
    "${PROJECT_SOURCE_DIR}/library/source/state.h"
    "${PROJECT_SOURCE_DIR}/library/source/string.c"
    "${PROJECT_SOURCE_DIR}/library/source/string.h"
    "${PROJECT_SOURCE_DIR}/library/source/stringpool.c"
    "${PROJECT_SOURCE_DIR}/library/source/stringpool.h"
    "${PROJECT_SOURCE_DIR}/library/source/value.c"
    "${PROJECT_SOURCE_DIR}/library/source/value.h"
    )

if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS /DNOM_EXPORTS")
    include(CheckIncludeFiles)
    check_include_files(vld.h NOM_USE_VLD)
elseif(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -pedantic -Wno-missing-braces -Wno-missing-field-initializers")
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/library/config.h.in"
    "${PROJECT_SOURCE_DIR}/library/include/nominal/config.h"
    )

add_library(NominalLibrary SHARED ${SOURCE_FILES})

set(REG_INCLUDE ".*/include/")
set(REG_INCLUDE_NOMINAL ".*/include/nominal/")
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
