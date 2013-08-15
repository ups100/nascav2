MESSAGE( "Building MyLogger target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 				${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/Logger/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( MYLOGGER_SRC
		${INZ_SOURCE_DIR}/Logger/src/Logger.cpp
		${INZ_SOURCE_DIR}/Logger/src/main.cpp
)


add_library( logger STATIC ${MYLOGGER_SRC} )

#add_executable( logger ${MYLOGGER_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	logger
		${Boost_LIBRARIES}
)

qt5_use_modules(logger Core)
