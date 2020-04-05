set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(Threads REQUIRED)
find_package(check REQUIRED)
find_package(subunit REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

set(msexpr_test_SOURCES_PREFIX "check_msexpr_")

file(GLOB msexpr_test_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/${msexpr_test_SOURCES_PREFIX}*.c"
    )

foreach(msexpr_test_SOURCE_FILE ${msexpr_test_SOURCES})

    get_filename_component(msexpr_test_FILENAME ${msexpr_test_SOURCE_FILE} NAME_WE)
    string(REGEX REPLACE "^${msexpr_test_SOURCES_PREFIX}" "test_" msexpr_test_NAME ${msexpr_test_FILENAME} )

    add_executable(${msexpr_test_NAME} ${msexpr_test_SOURCE_FILE})

    target_link_libraries(${msexpr_test_NAME}
        check
        subunit
        m rt
        ${CMAKE_THREAD_LIBS_INIT}
        msexpr
        )

    add_test(NAME ${msexpr_test_NAME}
        COMMAND ${msexpr_test_NAME}
        )

endforeach(msexpr_test_SOURCE_FILE)
