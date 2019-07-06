cmake_minimum_required(VERSION 3.7)

project(msexpr
    VERSION 0.0.1.0
    LANGUAGES C
    )

configure_file(version.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY
  )

set(msexpr_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/msexpr/sexpr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/msexpr/serializers.h
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    )

set(msexpr_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr_ops.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr_stack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/serializers.c
    )

set(msexpr_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr_ops.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sexpr_stack.h
    )

add_library(${PROJECT_NAME}
    ${msexpr_INCLUDES}
    ${msexpr_SOURCES}
    ${msexpr_HEADERS}
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    C_STANDARD 90
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    )

option(ENABLE_TESTS "Enable tests" ON)
if (${ENABLE_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()

# configure library installation
install(TARGETS ${PROJECT_NAME}
     ARCHIVE DESTINATION lib
     LIBRARY DESTINATION lib
     RUNTIME DESTINATION bin
     )

install(FILES ${msexpr_INCLUDES}
     DESTINATION include/msexpr
     )

# support for packaging with CPack
set(CPACK_PACKAGE_CONTACT "Rui Maciel rui.maciel@gmail.com")

include(CPack)
