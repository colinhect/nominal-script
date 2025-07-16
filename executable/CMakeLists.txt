include_directories(
    "${PROJECT_SOURCE_DIR}/library/include"
    )

set(SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/executable/source/main.c"
)

if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
elseif(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -Werror -pedantic -Wno-missing-braces -Wno-missing-field-initializers")
endif()

add_executable(NominalExecutable ${SOURCE_FILES})
target_link_libraries(NominalExecutable NominalLibrary readline)

set(REG_EXT "[^/]*([.]c|[.]h)$")

source_group("Source" REGULAR_EXPRESSION "${REG_SOURCE}${REG_EXT}")

set_target_properties(NominalExecutable PROPERTIES
    PROJECT_LABEL Executable
    OUTPUT_NAME nominal
    LINKER_LANGUAGE C
    )
