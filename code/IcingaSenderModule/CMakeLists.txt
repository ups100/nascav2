MESSAGE( "Building IcingaSenderModule target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 				${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/IcingaSenderModule/include
						${INZ_SOURCE_DIR}/Base/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( ICINGASENDERMODULE_SRC
		${INZ_SOURCE_DIR}/IcingaSenderModule/src/IcingaSender.cpp
)


add_library( icingasendermodule STATIC ${ICINGASENDERMODULE_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	icingasendermodule
		${Boost_LIBRARIES}
)

qt5_use_modules(icingasendermodule Core)
