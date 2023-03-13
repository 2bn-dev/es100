
include(../pico_sdk_import.cmake)

project(es100)

pico_sdk_init()

add_library(es100 STATIC es100.c )
add_library(es100_registers STATIC es100_registers.c )
add_library(es100_i2c STATIC es100_i2c.c)
add_library(es100_i2c_rp2040 STATIC es100_i2c_rp2040.c)

target_include_directories(es100 PUBLIC ../include/es100)
target_include_directories(es100_registers PUBLIC ../include/es100)
target_include_directories(es100_i2c PUBLIC ../include/es100)
target_include_directories(es100_i2c_rp2040 PUBLIC ../include/es100)



target_link_libraries(es100_registers)
target_link_libraries(es100_i2c)
target_link_libraries(es100_i2c_rp2040 es100_registers es100_i2c pico_stdlib pico_stdio hardware_i2c)
target_link_libraries(es100 LINK_PUBLIC es100_i2c_rp2040)



