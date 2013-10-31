MESSAGE( "Building Cryptographic target." )

find_package(PkgConfig)
pkg_check_modules(cryptopp REQUIRED libcrypto++)
 
#directories with headers for this target
INCLUDE_DIRECTORIES( 				${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/Cryptographic/include
						${INZ_SOURCE_DIR}/FactoryRegistration/include
						${INZ_SOURCE_DIR}/Logger/include
						${cryptopp_INCLUDE_DIR}
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( CRYPTOGRAPHIC_SRC
		${INZ_SOURCE_DIR}/Cryptographic/src/AsymetricAlgorithm.cpp
		${INZ_SOURCE_DIR}/Cryptographic/src/CryptographicFactory.cpp
		${INZ_SOURCE_DIR}/Cryptographic/src/SymetricAlgorithm.cpp
		${INZ_SOURCE_DIR}/Cryptographic/src/AESAlgorithmCBCMode.cpp
		${INZ_SOURCE_DIR}/Cryptographic/src/RSAAlgorithmOAEPUsingSHA.cpp
)

SET(CMAKE_CXX_FLAGS -Wall)

add_library( cryptographic STATIC ${CRYPTOGRAPHIC_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	cryptographic
		${Boost_LIBRARIES}
		${cryptopp_LDFLAGS}
		${cryptopp_LIBRARIES}
)

qt5_use_modules(cryptographic Core)


SET( CRYPTOGRAPHIC_SRC
		${CRYPTOGRAPHIC_SRC}
		${INZ_SOURCE_DIR}/Cryptographic/src/mainTest.cpp
)

add_executable( cryptographicTest ${CRYPTOGRAPHIC_SRC} )
message("${cryptopp_LDDFLAGS}")
message("${cryptopp_LIBRARIES}")
#here add some additional qt modules
TARGET_LINK_LIBRARIES(	cryptographicTest
		${Boost_LIBRARIES}
		${cryptopp_LDFLAGS}
		${cryptopp_LIBRARIES}
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/liblogger.a
)

qt5_use_modules(cryptographicTest Core)
