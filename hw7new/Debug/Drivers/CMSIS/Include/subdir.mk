################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Include/arm_sin_f32.c 

OBJS += \
./Drivers/CMSIS/Include/arm_sin_f32.o 

C_DEPS += \
./Drivers/CMSIS/Include/arm_sin_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Include/%.o Drivers/CMSIS/Include/%.su Drivers/CMSIS/Include/%.cyclo: ../Drivers/CMSIS/Include/%.c Drivers/CMSIS/Include/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Drivers/CMSIS/DSP/Include -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Include

clean-Drivers-2f-CMSIS-2f-Include:
	-$(RM) ./Drivers/CMSIS/Include/arm_sin_f32.cyclo ./Drivers/CMSIS/Include/arm_sin_f32.d ./Drivers/CMSIS/Include/arm_sin_f32.o ./Drivers/CMSIS/Include/arm_sin_f32.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Include

