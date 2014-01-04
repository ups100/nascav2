MESSAGE( "Building Base target." )

find_package(PkgConfig)
pkg_check_modules(cryptopp REQUIRED libcrypto++)

#directories with headers for this target
INCLUDE_DIRECTORIES( 	${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/AAA/include
						${INZ_SOURCE_DIR}/Cryptographic/include
						${INZ_SOURCE_DIR}/FactoryRegistration/include
						${INZ_SOURCE_DIR}/Logger/include
						${INZ_SOURCE_DIR}/Base/include
						${INZ_SOURCE_DIR}/TcpStandardModule/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( BASE_SRC
		${INZ_SOURCE_DIR}/Base/src/ClientSession.cpp
		${INZ_SOURCE_DIR}/Base/src/ConfigurationParser.cpp
		${INZ_SOURCE_DIR}/Base/src/DataChannel.cpp
		${INZ_SOURCE_DIR}/Base/src/DataConsumer.cpp
		${INZ_SOURCE_DIR}/Base/src/DataConsumerFactory.cpp
		${INZ_SOURCE_DIR}/Base/src/DataPortion.cpp
		${INZ_SOURCE_DIR}/Base/src/DataProvider.cpp
		${INZ_SOURCE_DIR}/Base/src/DataProviderFactory.cpp
		${INZ_SOURCE_DIR}/Base/src/MyNscaMain.cpp
		${INZ_SOURCE_DIR}/Base/src/main.cpp
		${INZ_SOURCE_DIR}/Base/src/DataFile.cpp
		${INZ_SOURCE_DIR}/Base/src/ReadPortion.cpp
		${INZ_SOURCE_DIR}/Base/include/DataConsumer.h
		${INZ_SOURCE_DIR}/Base/include/DataFile.h
		${INZ_SOURCE_DIR}/Base/include/MyNscaMain.h
		${INZ_SOURCE_DIR}/Base/src/DefaultTcpProvider.cpp
		${INZ_SOURCE_DIR}/Base/src/ToScreenPrinter.cpp
		${INZ_SOURCE_DIR}/Base/src/ToIcingaWritter.cpp
		${INZ_SOURCE_DIR}/Base/include/ClientSession.h
		${INZ_SOURCE_DIR}/Base/include/DataChannel.h
)

add_executable( base ${BASE_SRC} )

TARGET_LINK_LIBRARIES(	base 
		${Boost_LIBRARIES}
		boost_program_options
		-Wl,-whole-archive
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/liblogger.a
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libaaamodule.a
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libtcpstandardmodule.a
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libcryptographic.a
		-Wl,-no-whole-archive
		${cryptopp_LDFLAGS}
		${cryptopp_LIBRARIES}
)

qt5_use_modules(base Core Network)
