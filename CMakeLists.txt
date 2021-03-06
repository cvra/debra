cmake_minimum_required(VERSION 2.6)

project(cvra-simu)

# Check for CppUTest and bail out if they don't have it
if(DEFINED ENV{CPPUTEST_HOME})
    message("Using CppUTest found in $ENV{CPPUTEST_HOME}")
else()
    message(FATAL_ERROR "CPPUTEST_HOME is not set; You must tell CMake where to find CppUTest")
endif()

set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE
    modules_source
    modules/modules/uptime/uptime.c
    modules/modules/pid/pid.c
    modules/modules/control_system_manager/control_system_manager.c
    modules/modules/control_system_manager/control_system_manager.c
    modules/modules/math/vect2/vect2.c
    modules/modules/math/fast_math/fast_math.c
    modules/modules/math/geometry/circles.c
    modules/modules/math/geometry/vect_base.c
    modules/modules/math/geometry/lines.c
    modules/modules/math/geometry/polygon.c
    modules/modules/quadramp/quadramp.c
    modules/modules/position_manager/2wheels/position_manager.c
    modules/modules/robot_system/2wheels/robot_system.c
    modules/modules/robot_system/2wheels/angle_distance.c
    modules/modules/obstacle_avoidance.c
    modules/modules/json.c
    modules/modules/cvra_dc/cvra_dc.c
    modules/modules/platform/platform_host.c

)

file(GLOB_RECURSE
    lwip_source
    lwip/src/api/*.c
    lwip/src/core/*.c
    lwip/src/netif/eth*.c
    lwip/src/contrib/ports/unix/*.c
    )

file(GLOB_RECURSE
    tests_source
    tests/*
)

file(GLOB_RECURSE
    debra_source
    arm_trajectories.c
    arm_cinematics.c
    arm_utils.c
    arm.c
    hardware.c
    arm_cs.c
    obstacle_avoidance_protocol.c
)


include_directories(modules/include/)
include_directories(modules/modules/scheduler)
include_directories(modules/modules/blocking_detection_manager)
include_directories(modules/modules/control_system_manager)
include_directories(modules/modules/cvra_servo)
include_directories(modules/modules/cvra_adc)
include_directories(modules/modules/cvra_bldc)
include_directories(modules/modules/cvra_dc)
include_directories(modules/modules/cvra_logger)
include_directories(modules/modules/dual_quadramp)
include_directories(modules/modules/error)
include_directories(modules/modules/obstacle_avoidance)
include_directories(modules/modules/pid)
include_directories(modules/modules/position_manager)
include_directories(modules/modules/quadramp)
include_directories(modules/modules/robot_system)
include_directories(modules/modules/scheduler)
include_directories(modules/modules/trajectory_manager)
include_directories(modules/modules/math/fast_math)
include_directories(modules/modules/math/fixed_point)
include_directories(modules/modules/math/geometry)
include_directories(modules/modules/math/vect2)
include_directories(modules/modules/uptime)
include_directories(modules/modules/rdline)
include_directories(modules/modules/parse)
include_directories(modules/modules/platform)
include_directories(modules/modules/ramp)
include_directories(modules/modules/cvra_pio)
include_directories(modules/modules/cvra_servo)
include_directories(modules/modules/commandline)
include_directories(modules/modules/json)

include_directories(lwip/src/include/ipv4)
include_directories(lwip/src/include/)
include_directories(.)
include_directories(lwip/src/include/ipv6)
include_directories(lwip/src/include/netif)
include_directories(lwip/src/contrib/ports/unix/include)




include_directories($ENV{CPPUTEST_HOME}/include)
link_directories($ENV{CPPUTEST_HOME}/lib)


add_executable(
	tests
    ${debra_source}
    ${modules_source}
    ${tests_source}
    ${lwip_source}
)

target_link_libraries(
	tests
	m
    CppUTest
    CppUTestExt
)

find_package (Threads)
target_link_libraries (tests ${CMAKE_THREAD_LIBS_INIT})

#add_subdirectory(cinematics_test)
#add_subdirectory(cinematics_test_table)
add_subdirectory(integration_testing)
