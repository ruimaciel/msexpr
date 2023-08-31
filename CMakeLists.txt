cmake_minimum_required(VERSION 3.7)

project(msexpr
    VERSION 1.0.0.1
    LANGUAGES C
    )

configure_file(src/version.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/include/msexpr/version.h" @ONLY
  )
option(USES_RE2C "Uses re2c to regenerate the lexer files" OFF)
if(USES_RE2C)
    #TODO check if re2c is installed
    find_program(RE2C re2c REQUIRED)

    add_custom_command( OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c
        MAIN_DEPENDENCY	${CMAKE_CURRENT_SOURCE_DIR}/src/deserializers.c
        DEPENDS	${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c.re2c ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.h
        COMMAND ${RE2C}
        ARGS -T -o ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c ${CMAKE_CURRENT_SOURCE_DIR}/src/deserializer/rivest_canonical_lexer.c.re2c
        COMMENT "Generating re2c files"
        )
endif()

set(msexpr_INCLUDES
    include/msexpr/sexpr.h
    include/msexpr/serializers.h
    include/msexpr/deserializers.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/msexpr/version.h
    )

set(msexpr_SOURCES
    src/sexpr.c
    src/sexpr_ops.c
    src/sexpr_stack.c
    src/serializers.c
    src/deserializers.c
    src/deserializer/token.c
    src/deserializer/input.c
    src/deserializer/rivest_canonical_lexer.c
    )

set(msexpr_HEADERS
    src/sexpr_ops.h
    src/sexpr_stack.h
    src/deserializer/token.h
    src/deserializer/input.h
    src/deserializer/rivest_canonical_lexer.h
    )

set(msexpr_RE2C
    src/deserializer/rivest_canonical_lexer.c.re2c
    )

add_library(${PROJECT_NAME} OBJECT
    ${msexpr_INCLUDES}
    ${msexpr_SOURCES}
    ${msexpr_HEADERS}
    ${msexpr_RE2C}
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${msexpr_INCLUDES}"
    C_STANDARD 90
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
    SOVERSION 1
    )

add_library(${PROJECT_NAME}_SHARED SHARED
    $<TARGET_OBJECTS:${PROJECT_NAME}>
)

set_target_properties(${PROJECT_NAME}_SHARED PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME ${PROJECT_NAME}
    DEBUG_POSTFIX d
    SOVERSION 1
    )

add_library(${PROJECT_NAME}_STATIC STATIC
    $<TARGET_OBJECTS:${PROJECT_NAME}>
)

set_target_properties(${PROJECT_NAME}_STATIC PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    DEBUG_POSTFIX d
    )

option(ENABLE_TESTS "Enable tests" ON)
if (${ENABLE_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS ${PROJECT_NAME}_SHARED ${PROJECT_NAME}_STATIC
    EXPORT msexpr-export
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    )

install( FILES ${msexpr_INCLUDES}
    DESTINATION include/msexpr
)

install(EXPORT msexpr-export
    FILE msexpr-targets.cmake
    DESTINATION lib/cmake/msexpr
)

include(CMakePackageConfigHelpers)

# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/msexpr-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/msexpr-config.cmake"
    INSTALL_DESTINATION "lib/cmake/example"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

# generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/msexpr-ConfigVersion.cmake"
    VERSION "${Tutorial_VERSION_MAJOR}.${Tutorial_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
    )

# install the configuration file
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/msexpr-config.cmake
    DESTINATION lib/cmake/msexpr
    )
    
#Setup CPACK to generate installer
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_GENERATOR "DEB;TGZ;ZIP")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_NAME "msexpr")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rui Maciel")
include(CPack)
