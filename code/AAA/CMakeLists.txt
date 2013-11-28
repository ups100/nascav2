MESSAGE( "Building AAA target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 				${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/AAA/include
						${INZ_SOURCE_DIR}/FactoryRegistration/include
						${INZ_SOURCE_DIR}/Logger/include
						${INZ_SOURCE_DIR}/Base/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( AAA_SRC
		${INZ_SOURCE_DIR}/AAA/src/AAAFactory.cpp
		${INZ_SOURCE_DIR}/AAA/src/AAAModule.cpp
		${INZ_SOURCE_DIR}/AAA/src/ConversationInterface.cpp
		${INZ_SOURCE_DIR}/AAA/src/AlwaysAllow.cpp
)

add_library( aaamodule STATIC ${AAA_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	aaamodule
		${Boost_LIBRARIES}
)

qt5_use_modules(aaamodule Core)
