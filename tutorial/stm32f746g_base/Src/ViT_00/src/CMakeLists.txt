cmake_minimum_required(VERSION 3.5)

include_directories(
    ${CMAKE_SOURCE_DIR}/libs/flatbuffers}
    ${TENSORFLOW_LITE_INC}
    inc
)

aux_source_directory (./ DIR_USER_SRCS)

file(GLOB C_SOURCE
    ${OTHER_SRC_FILES}
#Add the local and project specific files
    ${DIR_USER_SRCS}
    ../config/system_stm32f7xx.c
)

set_source_files_properties(${C_SOURCE}
    PROPERTIES COMPILE_FLAGS ${STM32F7_DEFINES}
)

add_executable(${PROJECT_NAME}.elf
    ${C_SOURCE}
    ${EXTERNAL_EXECUTABLES}
)

if (EXTERNAL_DEPENDENCIES)
    add_dependencies(${PROJECT_NAME}.elf
        ${EXTERNAL_DEPENDENCIES}
    )
endif()

if (EXTERNAL_LIBS)
    target_link_libraries(${PROJECT_NAME}.elf
        ${EXTERNAL_LIBS}
    )
endif()

add_custom_target ( ${PROJECT_NAME}.hex ALL
    DEPENDS ${PROJECT_NAME}.elf 
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generating ${PROJECT_NAME}.hex"
)
add_custom_target ( ${PROJECT_NAME}.bin ALL
    DEPENDS ${PROJECT_NAME}.elf 
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Generating ${PROJECT_NAME}.bin"
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} --format=berkeley "${PROJECT_NAME}.elf")
