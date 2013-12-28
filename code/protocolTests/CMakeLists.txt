MESSAGE( "Building protocolTests target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 				${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/AAA/include
						${INZ_SOURCE_DIR}/Cryptographic/include
						${INZ_SOURCE_DIR}/Logger/include
						${INZ_SOURCE_DIR}/TcpStandardModule/include
						${INZ_SOURCE_DIR}/Base/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( PROTOCOLTESTS_SRC
		${INZ_SOURCE_DIR}/protocolTests/src/main.cpp
		
)


add_executable( protocolTests ${PROTOCOLTESTS_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	protocolTests
		-Wl,-whole-archive
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/liblogger.a
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libaaamodule.a
		
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libcryptographic.a
		-Wl,-no-whole-archive
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libtcpstandardmodule.a
		${Boost_LIBRARIES}
		${cryptopp_LDFLAGS}
		${cryptopp_LIBRARIES}
)

qt5_use_modules(protocolTests Core Network)
