################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Include/ComplexMathFunctions/ComplexMathFunctions.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_f32.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q15.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q31.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_f32.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q15.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q31.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_f32.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q15.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q31.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_f32.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q15.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q31.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_f32.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q15.c \
../Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q31.c 

OBJS += \
./Drivers/CMSIS/Include/ComplexMathFunctions/ComplexMathFunctions.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_f32.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q15.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q31.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_f32.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q15.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q31.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_f32.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q15.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q31.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_f32.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q15.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q31.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_f32.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q15.o \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q31.o 

C_DEPS += \
./Drivers/CMSIS/Include/ComplexMathFunctions/ComplexMathFunctions.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_f32.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q15.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q31.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_f32.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q15.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q31.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_f32.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q15.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q31.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_f32.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q15.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q31.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_f32.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q15.d \
./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q31.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Include/ComplexMathFunctions/%.o Drivers/CMSIS/Include/ComplexMathFunctions/%.su Drivers/CMSIS/Include/ComplexMathFunctions/%.cyclo: ../Drivers/CMSIS/Include/ComplexMathFunctions/%.c Drivers/CMSIS/Include/ComplexMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/user/STM32Cube/Repository/STM32Cube_FW_L4_V1.18.1/Drivers/CMSIS/DSP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Include-2f-ComplexMathFunctions

clean-Drivers-2f-CMSIS-2f-Include-2f-ComplexMathFunctions:
	-$(RM) ./Drivers/CMSIS/Include/ComplexMathFunctions/ComplexMathFunctions.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/ComplexMathFunctions.d ./Drivers/CMSIS/Include/ComplexMathFunctions/ComplexMathFunctions.o ./Drivers/CMSIS/Include/ComplexMathFunctions/ComplexMathFunctions.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_f32.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_f32.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_f32.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_f32.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q15.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q15.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q15.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q15.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q31.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q31.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q31.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_conj_q31.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_f32.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_f32.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_f32.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_f32.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q15.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q15.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q15.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q15.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q31.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q31.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q31.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_dot_prod_q31.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_f32.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_f32.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_f32.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_f32.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q15.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q15.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q15.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q15.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q31.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q31.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q31.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_q31.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_f32.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_f32.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_f32.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_f32.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q15.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q15.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q15.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q15.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q31.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q31.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q31.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mag_squared_q31.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_f32.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_f32.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_f32.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_f32.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q15.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q15.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q15.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q15.su ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q31.cyclo ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q31.d ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q31.o ./Drivers/CMSIS/Include/ComplexMathFunctions/arm_cmplx_mult_real_q31.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Include-2f-ComplexMathFunctions

