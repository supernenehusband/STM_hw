################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Include/SupportFunctions/SupportFunctions.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_copy_f32.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_copy_q15.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_copy_q31.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_copy_q7.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_fill_f32.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_fill_q15.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_fill_q31.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_fill_q7.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q15.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q31.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q7.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_float.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q31.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q7.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_float.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q15.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q7.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_float.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q15.c \
../Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q31.c 

OBJS += \
./Drivers/CMSIS/Include/SupportFunctions/SupportFunctions.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_copy_f32.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q15.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q31.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q7.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_fill_f32.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q15.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q31.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q7.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q15.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q31.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q7.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_float.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q31.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q7.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_float.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q15.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q7.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_float.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q15.o \
./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q31.o 

C_DEPS += \
./Drivers/CMSIS/Include/SupportFunctions/SupportFunctions.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_copy_f32.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q15.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q31.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q7.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_fill_f32.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q15.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q31.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q7.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q15.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q31.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q7.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_float.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q31.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q7.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_float.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q15.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q7.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_float.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q15.d \
./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q31.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Include/SupportFunctions/%.o Drivers/CMSIS/Include/SupportFunctions/%.su Drivers/CMSIS/Include/SupportFunctions/%.cyclo: ../Drivers/CMSIS/Include/SupportFunctions/%.c Drivers/CMSIS/Include/SupportFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/user/STM32Cube/Repository/STM32Cube_FW_L4_V1.18.1/Drivers/CMSIS/DSP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Include-2f-SupportFunctions

clean-Drivers-2f-CMSIS-2f-Include-2f-SupportFunctions:
	-$(RM) ./Drivers/CMSIS/Include/SupportFunctions/SupportFunctions.cyclo ./Drivers/CMSIS/Include/SupportFunctions/SupportFunctions.d ./Drivers/CMSIS/Include/SupportFunctions/SupportFunctions.o ./Drivers/CMSIS/Include/SupportFunctions/SupportFunctions.su ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_f32.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_f32.d ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_f32.o ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_f32.su ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q15.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q15.d ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q15.o ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q15.su ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q31.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q31.d ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q31.o ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q31.su ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q7.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q7.d ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q7.o ./Drivers/CMSIS/Include/SupportFunctions/arm_copy_q7.su ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_f32.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_f32.d ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_f32.o ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_f32.su ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q15.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q15.d ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q15.o ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q15.su ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q31.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q31.d ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q31.o ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q31.su ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q7.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q7.d ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q7.o ./Drivers/CMSIS/Include/SupportFunctions/arm_fill_q7.su ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q15.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q15.d ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q15.o ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q15.su ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q31.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q31.d ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q31.o ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q31.su ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q7.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q7.d ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q7.o ./Drivers/CMSIS/Include/SupportFunctions/arm_float_to_q7.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_float.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_float.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_float.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_float.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q31.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q31.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q31.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q31.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q7.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q7.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q7.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q15_to_q7.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_float.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_float.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_float.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_float.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q15.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q15.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q15.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q15.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q7.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q7.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q7.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q31_to_q7.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_float.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_float.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_float.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_float.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q15.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q15.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q15.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q15.su ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q31.cyclo ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q31.d ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q31.o ./Drivers/CMSIS/Include/SupportFunctions/arm_q7_to_q31.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Include-2f-SupportFunctions

