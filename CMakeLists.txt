cmake_minimum_required(VERSION 3.13)




option(BUILD_DOC "Build documentation" OFF)


if (PICO_SDK_PATH)
	project(es100 C ASM)
	
	add_library(es100 INTERFACE)

	target_sources(es100 INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/src/es100.c
		${CMAKE_CURRENT_LIST_DIR}/src/es100.h
		${CMAKE_CURRENT_LIST_DIR}/src/es100_registers.c
		${CMAKE_CURRENT_LIST_DIR}/src/es100_registers.h
		${CMAKE_CURRENT_LIST_DIR}/src/es100_i2c.c
		${CMAKE_CURRENT_LIST_DIR}/src/es100_i2c.h
		${CMAKE_CURRENT_LIST_DIR}/src/es100_i2c_rp2040.c
		${CMAKE_CURRENT_LIST_DIR}/src/es100_i2c_rp2040.h
		${CMAKE_CURRENT_LIST_DIR}/src/es100_debug.h
		)
	target_include_directories( es100 INTERFACE
		${CMAKE_CURRENT_LIST_DIR}/src
		)
	target_link_libraries(
                es100 INTERFACE
                pico_stdlib
                pico_time
		        hardware_i2c
		)

	return()
elseif(BUILD_DOC)
	project(es100 C ASM)
	# check if Doxygen is installed
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
	    # set input and output files
	    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
	    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	
	    # request to configure the file
	    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
	    message("Doxygen build started")
	
	    # note the option ALL which allows to build the docs together with the application
	    add_custom_target( doc_doxygen ALL
	        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
	        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	        COMMENT "Generating API documentation with Doxygen"
	        VERBATIM )
	else (DOXYGEN_FOUND)
		message("Doxygen can't be found by cmake")
	endif (DOXYGEN_FOUND)
	message("######################################################")
	message("Warning: this is a library, it can't be directly built")
        message("you need to include it into your own project (see examples/)")
	message("")
	message("You are in DOC BUILD MODE! (-DBUILD_DOC=ON) so I'll build the docs now.")
	message("######################################################")
else()
	message("######################################################")
        message("Warning: this is a library, it can't be directly built")
	message("you need to include it into your own project (see examples/)")
	message("")
	message("You can build docs by adding -DBUILD_DOC=ON to your cmake")
        message("######################################################")
        message(FATAL_ERROR "Quitting configuration")

endif()
