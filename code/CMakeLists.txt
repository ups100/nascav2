cmake_minimum_required (VERSION 2.8.9) 

#project name
project (INZ) 

#look for boost library (required for this project)
FIND_PACKAGE( Boost COMPONENTS thread random REQUIRED )

#look for qt library (required for this project)
find_package(Qt5Core REQUIRED )

#look for doxygen to make documentation
find_package(Doxygen REQUIRED)

SET(QT_DONT_USE_QTGUI TRUE)

#turn on automoc
SET(CMAKE_AUTOMOC ON)

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

#subdirectories
add_subdirectory(AAA)
add_subdirectory(Base)
add_subdirectory(Cryptographic)
add_subdirectory(IcingaSenderModule)
add_subdirectory(TcpStandardModule)
add_subdirectory(Logger)
add_subdirectory(doxygen)
