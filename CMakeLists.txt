project(gearmand_worker_test)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

AUX_SOURCE_DIRECTORY(./src SRCS)

AUX_SOURCE_DIRECTORY(./framework FRAMEWORK)

AUX_SOURCE_DIRECTORY(./lib/iniparser INI_PARSER)

add_executable(./bin/gearmand_worker_test ${INI_PARSER} ${FRAMEWORK} ${SRCS})

target_link_libraries(./bin/gearmand_worker_test gearman)

