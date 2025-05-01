################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Include/CommonTables/CommonTables.c \
../Drivers/CMSIS/Include/CommonTables/arm_common_tables.c \
../Drivers/CMSIS/Include/CommonTables/arm_const_structs.c 

OBJS += \
./Drivers/CMSIS/Include/CommonTables/CommonTables.o \
./Drivers/CMSIS/Include/CommonTables/arm_common_tables.o \
./Drivers/CMSIS/Include/CommonTables/arm_const_structs.o 

C_DEPS += \
./Drivers/CMSIS/Include/CommonTables/CommonTables.d \
./Drivers/CMSIS/Include/CommonTables/arm_common_tables.d \
./Drivers/CMSIS/Include/CommonTables/arm_const_structs.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Include/CommonTables/%.o Drivers/CMSIS/Include/CommonTables/%.su Drivers/CMSIS/Include/CommonTables/%.cyclo: ../Drivers/CMSIS/Include/CommonTables/%.c Drivers/CMSIS/Include/CommonTables/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/user/STM32Cube/Repository/STM32Cube_FW_L4_V1.18.1/Drivers/CMSIS/DSP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Include-2f-CommonTables

clean-Drivers-2f-CMSIS-2f-Include-2f-CommonTables:
	-$(RM) ./Drivers/CMSIS/Include/CommonTables/CommonTables.cyclo ./Drivers/CMSIS/Include/CommonTables/CommonTables.d ./Drivers/CMSIS/Include/CommonTables/CommonTables.o ./Drivers/CMSIS/Include/CommonTables/CommonTables.su ./Drivers/CMSIS/Include/CommonTables/arm_common_tables.cyclo ./Drivers/CMSIS/Include/CommonTables/arm_common_tables.d ./Drivers/CMSIS/Include/CommonTables/arm_common_tables.o ./Drivers/CMSIS/Include/CommonTables/arm_common_tables.su ./Drivers/CMSIS/Include/CommonTables/arm_const_structs.cyclo ./Drivers/CMSIS/Include/CommonTables/arm_const_structs.d ./Drivers/CMSIS/Include/CommonTables/arm_const_structs.o ./Drivers/CMSIS/Include/CommonTables/arm_const_structs.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Include-2f-CommonTables

