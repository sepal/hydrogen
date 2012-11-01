
FILE(GLOB_RECURSE h2tacUI_SRCS *.cpp)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src/core/include        # core headers
    ${CMAKE_BINARY_DIR}/src/core/include        # generated config.h
    ${QT_INCLUDES}
    ${LASH_INCLUDE_DIR}
)

ADD_EXECUTABLE(h2tacUI WIN32 MACOSX_BUNDLE ${h2tacUI_SRCS} )
TARGET_LINK_LIBRARIES(h2tacUI
    hydrogen-core-${VERSION}
    ${QT_QTGUI_LIBRARY}
    ${LASH_LIBRARIES}
)

ADD_DEPENDENCIES(h2tacUI hydrogen-core-${VERSION})

INSTALL(TARGETS h2tacUI RUNTIME DESTINATION bin BUNDLE DESTINATION bin )
