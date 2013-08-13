MESSAGE( "Building Cryptographic target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 				${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/Cryptographic/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( CRYPTOGRAPHIC_SRC
		${INZ_SOURCE_DIR}/Cryptographic/src/AsymetricAlgorithm.cpp
		${INZ_SOURCE_DIR}/Cryptographic/src/CryptographicFactory.cpp
		${INZ_SOURCE_DIR}/Cryptographic/src/SymetricAlgorithm.cpp
)


add_library( cryptographic STATIC ${CRYPTOGRAPHIC_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	cryptographic
		${Boost_LIBRARIES}
)

qt5_use_modules(cryptographic Core)
