MESSAGE( "Building TcpStandardModule target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 				${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/Cryptographic/include
						${INZ_SOURCE_DIR}/Logger/include
						${INZ_SOURCE_DIR}/TcpStandardModule/include
						${INZ_SOURCE_DIR}/Base/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( TCPSTANDARDMODULE_SRC
		${INZ_SOURCE_DIR}/TcpStandardModule/src/MessageCipher.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/MessageDecorator.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/MessageEndpointTcp.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/MessageFormer.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/MessageHasher.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/MessageSigner.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/MessageSink.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/TcpServer.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/AdditionalThread.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/include/MessageEndpointTcp.h
		${INZ_SOURCE_DIR}/TcpStandardModule/include/MessageCipher.h
		${INZ_SOURCE_DIR}/TcpStandardModule/include/MessageDecorator.h
		${INZ_SOURCE_DIR}/TcpStandardModule/include/MessageFormer.h
		${INZ_SOURCE_DIR}/TcpStandardModule/include/MessageHasher.h
		${INZ_SOURCE_DIR}/TcpStandardModule/include/MessageSink.h
		${INZ_SOURCE_DIR}/TcpStandardModule/include/MessageSigner.h
)


add_library( tcpstandardmodule STATIC ${TCPSTANDARDMODULE_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	tcpstandardmodule
		cryptographic
		logger
		${Boost_LIBRARIES}
)

qt5_use_modules(tcpstandardmodule Core Network)
