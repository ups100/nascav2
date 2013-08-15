MESSAGE( "Building TcpStandardModule target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 				${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/TcpStandardModule/include
						${INZ_SOURCE_DIR}/Base/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( TCPSTANDARDMODULE_SRC
		${INZ_SOURCE_DIR}/TcpStandardModule/src/ClientCommunication.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/ClientCommunicationFactory.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/ClientConnection.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/TcpReceiver.cpp
		${INZ_SOURCE_DIR}/TcpStandardModule/src/TcpServer.cpp

)


add_library( tcpstandardmodule STATIC ${TCPSTANDARDMODULE_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	tcpstandardmodule
		${Boost_LIBRARIES}
)

qt5_use_modules(tcpstandardmodule Core Network)
