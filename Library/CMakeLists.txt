configure_file(
    "${PROJECT_SOURCE_DIR}/Library/Config.h.in"
    "${PROJECT_SOURCE_DIR}/Library/Include/Nominal/Config.h"
    )

include_directories(
    "${PROJECT_SOURCE_DIR}/Library/Include"
    )

file(
    GLOB
    SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Library/Include/*.h"
    "${PROJECT_SOURCE_DIR}/Library/Include/Nominal/*.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/*.h"
    "${PROJECT_SOURCE_DIR}/Library/Source/*.c"
)

if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS /DNOMINAL_EXPORTS")
elseif(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -pedantic -Wno-missing-braces -Wno-missing-field-initializers")
endif()

add_library(Nominal SHARED ${SOURCE_FILES})

set(REG_INCLUDE ".*/Include/")
set(REG_INCLUDE_NOMINAL ".*/Include/Nominal/")
set(REG_SOURCE ".*/Source/")
set(REG_EXT "[^/]*([.]c|[.]h)$")

source_group("Include" REGULAR_EXPRESSION "${REG_INCLUDE}${REG_EXT}")
source_group("Include\\Nominal" REGULAR_EXPRESSION "${REG_INCLUDE_NOMINAL}${REG_EXT}")
source_group("Source" REGULAR_EXPRESSION "${REG_SOURCE}${REG_EXT}")

set_target_properties(Nominal PROPERTIES
    PROJECT_LABEL Library
    OUTPUT_NAME nominal
    LINKER_LANGUAGE C
    )
