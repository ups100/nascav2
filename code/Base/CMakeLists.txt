MESSAGE( "Building Base target." )

#directories with headers for this target
INCLUDE_DIRECTORIES( 	${Boost_INCLUDE_DIRS}
						${INZ_SOURCE_DIR}/AAA/include
						${INZ_SOURCE_DIR}/FactoryRegistration/include
						${INZ_SOURCE_DIR}/Base/include
)

#if some class inherits QObject (has Q_OBJECT macro) add it's header file here
SET( BASE_SRC
		${INZ_SOURCE_DIR}/Base/src/Client.cpp
		${INZ_SOURCE_DIR}/Base/src/ConfigurationParser.cpp
		${INZ_SOURCE_DIR}/Base/src/DataChannel.cpp
		${INZ_SOURCE_DIR}/Base/src/DataConsumer.cpp
		${INZ_SOURCE_DIR}/Base/src/DataConsumerFactory.cpp
		${INZ_SOURCE_DIR}/Base/src/DataPortion.cpp
		${INZ_SOURCE_DIR}/Base/src/DataProvider.cpp
		${INZ_SOURCE_DIR}/Base/src/DataProviderFactory.cpp
		${INZ_SOURCE_DIR}/Base/src/MyNascaMain.cpp
		${INZ_SOURCE_DIR}/Base/src/main.cpp	
)

add_executable( base ${BASE_SRC} )

#here add some additional qt modules
TARGET_LINK_LIBRARIES(	base
		${Boost_LIBRARIES}
)

qt5_use_modules(base Core)
