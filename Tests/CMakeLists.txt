include_directories(
    "${PROJECT_SOURCE_DIR}/Library/Include"
    )

set(SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Tests/Source/functiontests.cpp"
    "${PROJECT_SOURCE_DIR}/Tests/Source/main.cpp"
    "${PROJECT_SOURCE_DIR}/Tests/Source/maptests.cpp"
    "${PROJECT_SOURCE_DIR}/Tests/Source/negativetests.cpp"
    "${PROJECT_SOURCE_DIR}/Tests/Source/numbertests.cpp"
    "${PROJECT_SOURCE_DIR}/Tests/Source/positivetests.cpp"
    "${PROJECT_SOURCE_DIR}/Tests/Source/statetests.cpp"
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
    ${PROJECT_SOURCE_DIR}/Tests/Positive ${OUTPUT_DIR}/Tests/Positive
    )

add_custom_command(TARGET NominalTests PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/Tests/Negative ${OUTPUT_DIR}/Tests/Negative
    )
