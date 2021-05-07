cmake_minimum_required(VERSION 3.1)
project(atem-shim VERSION 0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DVERSION="${PROJECT_VERSION}")

find_package(Qt5 COMPONENTS Core Gui Network REQUIRED)
set(CMAKE_AUTOMOC ON)

include_directories(libqatemcontrol)

set(LIBQATEMCONTROL
    libqatemcontrol/libqatemcontrol_global.h
    libqatemcontrol/qatemcameracontrol.cpp libqatemcontrol/qatemcameracontrol.h
    libqatemcontrol/qatemconnection.cpp    libqatemcontrol/qatemconnection.h
    libqatemcontrol/qatemdownstreamkey.cpp libqatemcontrol/qatemdownstreamkey.h
    libqatemcontrol/qatemmixeffect.cpp     libqatemcontrol/qatemmixeffect.h
    libqatemcontrol/qatemtypes.h
    libqatemcontrol/qupstreamkeysettings.h
)
set(SOURCES
    src/atem.cpp
    src/connection.cpp src/connection.hpp
    src/server.cpp     src/server.hpp
)

include(GNUInstallDirs)

########################
# executable
add_executable(${PROJECT_NAME} ${LIBQATEMCONTROL} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Network)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

########################
# package
set(CPACK_STRIP_FILES ON)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ATEM Shim for Crestron")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dimitry Ishenko <dimitry (dot) ishenko (at) (gee) mail (dot) com>")

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS )
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
