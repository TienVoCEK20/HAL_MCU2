################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/it.c \
../Core/Src/main.c \
../Core/Src/msp.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/it.o \
./Core/Src/main.o \
./Core/Src/msp.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/it.d \
./Core/Src/main.d \
./Core/Src/msp.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F407xx -I../USB_HOST/App -I../USB_HOST/Target -I"D:/stm32-projects/UART2_example/Core/Inc" -I"D:/stm32-projects/UART2_example/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/stm32-projects/UART2_example/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I"D:/stm32-projects/UART2_example/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/stm32-projects/UART2_example/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


