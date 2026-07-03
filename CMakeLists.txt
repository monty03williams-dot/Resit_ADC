

cmake_minimum_required(VERSION 3.20)
project(adc_processor C)

set(CMAKE_C_STANDARD 99)

add_executable(adc_processor
        main.c
        adc.c
        io.c
        stats.c
)

target_link_libraries(adc_processor m)



