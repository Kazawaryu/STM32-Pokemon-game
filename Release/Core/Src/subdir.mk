################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/24cxx.c \
../Core/Src/24l01.c \
../Core/Src/ctiic.c \
../Core/Src/delay.c \
../Core/Src/ft5206.c \
../Core/Src/gt9147.c \
../Core/Src/key.c \
../Core/Src/lcd.c \
../Core/Src/led.c \
../Core/Src/main.c \
../Core/Src/myiic.c \
../Core/Src/ott2001a.c \
../Core/Src/spi.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/sys.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/touch.c 

OBJS += \
./Core/Src/24cxx.o \
./Core/Src/24l01.o \
./Core/Src/ctiic.o \
./Core/Src/delay.o \
./Core/Src/ft5206.o \
./Core/Src/gt9147.o \
./Core/Src/key.o \
./Core/Src/lcd.o \
./Core/Src/led.o \
./Core/Src/main.o \
./Core/Src/myiic.o \
./Core/Src/ott2001a.o \
./Core/Src/spi.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/sys.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/touch.o 

C_DEPS += \
./Core/Src/24cxx.d \
./Core/Src/24l01.d \
./Core/Src/ctiic.d \
./Core/Src/delay.d \
./Core/Src/ft5206.d \
./Core/Src/gt9147.d \
./Core/Src/key.d \
./Core/Src/lcd.d \
./Core/Src/led.d \
./Core/Src/main.d \
./Core/Src/myiic.d \
./Core/Src/ott2001a.d \
./Core/Src/spi.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/sys.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/touch.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/24cxx.d ./Core/Src/24cxx.o ./Core/Src/24cxx.su ./Core/Src/24l01.d ./Core/Src/24l01.o ./Core/Src/24l01.su ./Core/Src/ctiic.d ./Core/Src/ctiic.o ./Core/Src/ctiic.su ./Core/Src/delay.d ./Core/Src/delay.o ./Core/Src/delay.su ./Core/Src/ft5206.d ./Core/Src/ft5206.o ./Core/Src/ft5206.su ./Core/Src/gt9147.d ./Core/Src/gt9147.o ./Core/Src/gt9147.su ./Core/Src/key.d ./Core/Src/key.o ./Core/Src/key.su ./Core/Src/lcd.d ./Core/Src/lcd.o ./Core/Src/lcd.su ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/myiic.d ./Core/Src/myiic.o ./Core/Src/myiic.su ./Core/Src/ott2001a.d ./Core/Src/ott2001a.o ./Core/Src/ott2001a.su ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/sys.d ./Core/Src/sys.o ./Core/Src/sys.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/touch.d ./Core/Src/touch.o ./Core/Src/touch.su

.PHONY: clean-Core-2f-Src

