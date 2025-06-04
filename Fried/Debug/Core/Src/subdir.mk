################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App_KeyDisplay.c \
../Core/Src/Display.c \
../Core/Src/HAL.c \
../Core/Src/Key.c \
../Core/Src/USART2_Interface.c \
../Core/Src/main.c \
../Core/Src/shiftreg.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/timer.c 

OBJS += \
./Core/Src/App_KeyDisplay.o \
./Core/Src/Display.o \
./Core/Src/HAL.o \
./Core/Src/Key.o \
./Core/Src/USART2_Interface.o \
./Core/Src/main.o \
./Core/Src/shiftreg.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/timer.o 

C_DEPS += \
./Core/Src/App_KeyDisplay.d \
./Core/Src/Display.d \
./Core/Src/HAL.d \
./Core/Src/Key.d \
./Core/Src/USART2_Interface.d \
./Core/Src/main.d \
./Core/Src/shiftreg.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/App_KeyDisplay.cyclo ./Core/Src/App_KeyDisplay.d ./Core/Src/App_KeyDisplay.o ./Core/Src/App_KeyDisplay.su ./Core/Src/Display.cyclo ./Core/Src/Display.d ./Core/Src/Display.o ./Core/Src/Display.su ./Core/Src/HAL.cyclo ./Core/Src/HAL.d ./Core/Src/HAL.o ./Core/Src/HAL.su ./Core/Src/Key.cyclo ./Core/Src/Key.d ./Core/Src/Key.o ./Core/Src/Key.su ./Core/Src/USART2_Interface.cyclo ./Core/Src/USART2_Interface.d ./Core/Src/USART2_Interface.o ./Core/Src/USART2_Interface.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/shiftreg.cyclo ./Core/Src/shiftreg.d ./Core/Src/shiftreg.o ./Core/Src/shiftreg.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/timer.cyclo ./Core/Src/timer.d ./Core/Src/timer.o ./Core/Src/timer.su

.PHONY: clean-Core-2f-Src

