include_directories(
    "${PROJECT_SOURCE_DIR}/library/include"
    )

set(SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/tests/source/functiontests.cpp"
    "${PROJECT_SOURCE_DIR}/tests/source/main.cpp"
    "${PROJECT_SOURCE_DIR}/tests/source/maptests.cpp"
    "${PROJECT_SOURCE_DIR}/tests/source/negativetests.cpp"
    "${PROJECT_SOURCE_DIR}/tests/source/numbertests.cpp"
    "${PROJECT_SOURCE_DIR}/tests/source/positivetests.cpp"
    "${PROJECT_SOURCE_DIR}/tests/source/statetests.cpp"
)

if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")
endif()

add_executable(NominalTests ${SOURCE_FILES})
target_link_libraries(NominalTests NominalLibrary)

set(REG_EXT "[^/]*([.]c|[.]h)$")

source_group("Source" REGULAR_EXPRESSION "${REG_SOURCE}${REG_EXT}")

set_target_properties(NominalTests PROPERTIES
    PROJECT_LABEL Tests
    OUTPUT_NAME nominaltests
    LINKER_LANGUAGE CXX
    )

add_custom_command(TARGET NominalTests PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/tests/positive ${OUTPUT_DIR}/tests/positive)

add_custom_command(TARGET NominalTests PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/tests/negative ${OUTPUT_DIR}/tests/negative)
