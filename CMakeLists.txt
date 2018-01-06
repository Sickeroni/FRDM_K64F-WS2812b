
SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake")
PROJECT(FRDMK64Fws2812b C CXX ASM)
# CROSS COMPILER SETTING

SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.7)

IF(POLICY CMP0048)
	cmake_policy(SET CMP0048 OLD)
ENDIF()
# THE VERSION NUMBER
SET (PROJECT_VERSION_MAJOR 1)
SET (PROJECT_VERSION_MINOR 0)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

message(STATUS "Generator: ${CMAKE_GENERATOR}")
if (WIN32)
	if(CMAKE_GENERATOR STREQUAL "NMake Makefiles")
		message("=============================================================")
		message("== Sorry NMake is not supported.")
		message("== Please try other Generator.")
		message("===========================================================")
		message( FATAL_ERROR "You can not do this at all, CMake will exit." )
	endif(CMAKE_GENERATOR STREQUAL "NMake Makefiles")
endif (WIN32)


# CURRENT DIRECTORY
SET(ProjDirPath ${PROJECT_BINARY_DIR})
SET(PROJECT_DEPENDENCY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dep")
SET(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

IF(NOT CMAKE_TOOLCHAIN_FILE)
    MESSAGE(FATAL_ERROR "***Missing Toolchain File!!!!***")
ENDIF()

#ENABLE_LANGUAGE(ASM)
ENABLE_LANGUAGE(CXX)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)


# define Optimization based on build type
IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    SET(CUSTOM_COMPILE_FLAGS "${CUSTOM_COMPILE_FLAGS} -Os -MMD -MP")
ELSE(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    SET(CUSTOM_COMPILE_FLAGS "${CUSTOM_COMPILE_FLAGS} -Og -g3 -DDEBUG -MMD -MP -mapcs")
ENDIF(NOT CMAKE_BUILD_TYPE MATCHES Debug)

# flags for asm, c and cxx and linker that is compiling too
# needed defines for kinetis sdk
SET(CUSTOM_COMPILE_FLAGS "${CUSTOM_COMPILE_FLAGS} -DCPU_MK64FN1M0VLL12 -DFRDM_K64F -DFREEDOM")


# define hardware for compiler
# we are using a cortex-m4, thumb-instructions allowed, user hardware for floats, set used FPU 
SET(CUSTOM_COMPILE_FLAGS "${CUSTOM_COMPILE_FLAGS} -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
# define configs for library (newlib-nano & no sys[OS])
SET(CUSTOM_COMPILE_FLAGS "${CUSTOM_COMPILE_FLAGS} --specs=nano.specs --specs=nosys.specs")
# activate all warnings
SET(CUSTOM_COMPILE_FLAGS "${CUSTOM_COMPILE_FLAGS} -Wall")

# uninitialized global variables in the data section (makes startupcode easier) and not in own section "common"
SET(CUSTOM_COMPILE_FLAGS  "${CUSTOM_COMPILE_FLAGS} -fno-common")
# put data and functions in own sections. This way Linker can remove unused functions and variables
SET(CUSTOM_COMPILE_FLAGS  "${CUSTOM_COMPILE_FLAGS} -ffunction-sections -fdata-sections")
# we use a freestanding environment (no OS and other stuff) 
SET(CUSTOM_COMPILE_FLAGS  "${CUSTOM_COMPILE_FLAGS} -ffreestanding")
# gcc got builtin functions that we dont use. 
SET(CUSTOM_COMPILE_FLAGS  "${CUSTOM_COMPILE_FLAGS} -fno-builtin")
# other stuff TODO
SET(CUSTOM_COMPILE_FLAGS  "${CUSTOM_COMPILE_FLAGS} -fstack-usage")

#unknown TODO
#SET(CUSTOM_COMPILE_FLAGS "${CUSTOM_COMPILE_FLAGS} -MMD -MP")" 
# set ARM calling Convention (ABI) 
# TL;TR: stack is 8byte-aligned, without it LDRD & STRD might cause alignment faults for doublewords(64bit).  
# https://gcc.gnu.org/onlinedocs/gcc-4.8.3/gcc/ARM-Options.html
# http://infocenter.arm.com/help/topic/com.arm.doc.ihi0042f/IHI0042F_aapcs.pdf
# quote: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka4127.html
# "If the stack is not eight-byte aligned the use of LDRD and STRD might cause an alignment fault, depending on the target and configuration used."
SET(CUSTOM_COMPILE_FLAGS "${CUSTOM_COMPILE_FLAGS} -mapcs-frame")


SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${CUSTOM_COMPILE_FLAGS} -D__STARTUP_CLEAR_BSS -std=gnu99")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CUSTOM_COMPILE_FLAGS} -std=c11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CUSTOM_COMPILE_FLAGS} -flto -std=c++17")

# link our linkerscript
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_CURRENT_SOURCE_DIR}/MK64FN1M0xxx12_flash.ld -static")
# remove unused functions/variables (gc = garbage collector)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker --gc-sections")
# static linking only
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -static")
# Normally when a symbol is defined multiple times, the linker will report a fatal error. These options allow multiple definitions and the first definition will be used.
# two commands because XLINKER accepts only one argument. 
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -z -Xlinker muldefs")
# define for startup.s from ksdk
# creating a global symbol (__ram_vector_table__)
# TODO make that more elegant...
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -Xlinker --defsym=__ram_vector_table__=1")


include_directories(${PROJECT_SOURCE_DIR})






file(GLOB ksdk_RESULT "${PROJECT_DEPENDENCY_DIR}/ksdk/*")
list(LENGTH ksdk_RESULT ksdk_RES_LEN)

if(ksdk_RES_LEN EQUAL 0)
	message( SEND_ERROR "Kinetis SDK is missing.
Please download Version 2.3.0 from
https://mcuxpresso.nxp.com/")
endif(ksdk_RES_LEN EQUAL 0)


file(GLOB kvasir_RESULT "${PROJECT_DEPENDENCY_DIR}/Kvasir*")
list(LENGTH kvasir_RESULT kvasir_RES_LEN)
if(kvasir_RES_LEN EQUAL 0)
	message( SEND_ERROR "The git submodules are not available. Please run
			git submodule update --init --recursive"
)
endif(kvasir_RES_LEN EQUAL 0)

include_directories("${PROJECT_SOURCE_DIR}/ksdk_init")
include_directories(${PROJECT_DEPENDENCY_DIR}/Kvasir/Lib)


include_directories("${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12"
		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers"
		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities"
		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/log"
		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/io"
		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/str"
		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/devices/MK64F12"
		"${PROJECT_DEPENDENCY_DIR}/ksdk/CMSIS/Include"
		"${PROJECT_SOURCE_DIR}/ksdk_init/"
		"${PROJECT_SOURCE_DIR}/inc/"
		"${PROJECT_SOURCE_DIR}"

		)



add_executable(${PROJECT_NAME}.elf
		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/gcc/startup_MK64F12.S"

		"${PROJECT_SOURCE_DIR}/main.cpp"

#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_gpio.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_smc.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_uart.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_sim.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_clock.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_enet.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_pit.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_flash.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_common.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/drivers/fsl_wdog.c"

		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/system_MK64F12.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/fsl_debug_console.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/fsl_assert.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/fsl_debug_console.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/fsl_notifier.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/fsl_shell.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/io/fsl_io.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/log/fsl_log.c"
#		"${PROJECT_DEPENDENCY_DIR}/ksdk/devices/MK64F12/utilities/str/fsl_str.c"
		)

