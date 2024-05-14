set(STM32F7_HAL_DRIVER_DIR ${CMAKE_SOURCE_DIR}/libs/STM32F7xx_HAL_Driver)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/libs/CMSIS)

# Make sure that git submodule is initialized and updated
if (NOT EXISTS "${STM32F7_HAL_DRIVER_DIR}")
  message(FATAL_ERROR "STM32F7 HAL Driver submodule not found. Initialize with 'git submodule update --init' in the source directory")
endif()

# Make sure that git submodule is initialized and updated
if (NOT EXISTS "${CMSIS_DIR}")
  message(FATAL_ERROR "cmsis submodule not found. Initialize with 'git submodule update --init' in the source directory")
endif()

include_directories(
    ${CMSIS_DIR}/DSP/Include
    ${CMSIS_DIR}/NN/Include
    ${CMSIS_DIR}/Include
    ${CMSIS_DIR}/Device/ST/STM32F7xx/Include
    ${STM32F7_HAL_DRIVER_DIR}/Inc
    ${SRC}/inc
)


# Get all source files from the Src directory
aux_source_directory(${STM32F7_HAL_DRIVER_DIR}/Src STM32F7_HAL_DRIVER_SRC)

set_source_files_properties(${STM32F7_HAL_DRIVER_SRC}
    PROPERTIES COMPILE_FLAGS ${STM32F7_DEFINES}
)

add_library(STM32F7xx_HAL_Driver STATIC ${STM32F7_HAL_DRIVER_SRC})
set_target_properties(STM32F7xx_HAL_Driver PROPERTIES LINKER_LANGUAGE C)

set(EXTERNAL_EXECUTABLES ${EXTERNAL_EXECUTABLES} ${STARTUP_ASM_FILE})

set(EXTERNAL_LIBS ${EXTERNAL_LIBS} STM32F7xx_HAL_Driver)