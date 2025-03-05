################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../Core/Src/AD24C02.c \
../Core/Src/GPS.c \
../Core/Src/HMI.c \
../Core/Src/SD_APP.c \
../Core/Src/SPI_SD.c \
../Core/Src/adc.c \
../Core/Src/battery_show.c \
../Core/Src/buzzer.c \
../Core/Src/diskio.c \
../Core/Src/dma.c \
../Core/Src/dosage.c \
../Core/Src/ff.c \
../Core/Src/ffsystem.c \
../Core/Src/ffunicode.c \
../Core/Src/gas_press_clc.c \
../Core/Src/gpio.c \
../Core/Src/i2c_soft.c \
../Core/Src/key.c \
../Core/Src/main.c \
../Core/Src/motor.c \
../Core/Src/spi.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/AD24C02.o \
./Core/Src/GPS.o \
./Core/Src/HMI.o \
./Core/Src/SD_APP.o \
./Core/Src/SPI_SD.o \
./Core/Src/adc.o \
./Core/Src/battery_show.o \
./Core/Src/buzzer.o \
./Core/Src/diskio.o \
./Core/Src/dma.o \
./Core/Src/dosage.o \
./Core/Src/ff.o \
./Core/Src/ffsystem.o \
./Core/Src/ffunicode.o \
./Core/Src/gas_press_clc.o \
./Core/Src/gpio.o \
./Core/Src/i2c_soft.o \
./Core/Src/key.o \
./Core/Src/main.o \
./Core/Src/motor.o \
./Core/Src/spi.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/AD24C02.d \
./Core/Src/GPS.d \
./Core/Src/HMI.d \
./Core/Src/SD_APP.d \
./Core/Src/SPI_SD.d \
./Core/Src/adc.d \
./Core/Src/battery_show.d \
./Core/Src/buzzer.d \
./Core/Src/diskio.d \
./Core/Src/dma.d \
./Core/Src/dosage.d \
./Core/Src/ff.d \
./Core/Src/ffsystem.d \
./Core/Src/ffunicode.d \
./Core/Src/gas_press_clc.d \
./Core/Src/gpio.d \
./Core/Src/i2c_soft.d \
./Core/Src/key.d \
./Core/Src/main.d \
./Core/Src/motor.d \
./Core/Src/spi.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# 每个子目录必须为构建它所贡献的源提供规则
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AD24C02.cyclo ./Core/Src/AD24C02.d ./Core/Src/AD24C02.o ./Core/Src/AD24C02.su ./Core/Src/GPS.cyclo ./Core/Src/GPS.d ./Core/Src/GPS.o ./Core/Src/GPS.su ./Core/Src/HMI.cyclo ./Core/Src/HMI.d ./Core/Src/HMI.o ./Core/Src/HMI.su ./Core/Src/SD_APP.cyclo ./Core/Src/SD_APP.d ./Core/Src/SD_APP.o ./Core/Src/SD_APP.su ./Core/Src/SPI_SD.cyclo ./Core/Src/SPI_SD.d ./Core/Src/SPI_SD.o ./Core/Src/SPI_SD.su ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/battery_show.cyclo ./Core/Src/battery_show.d ./Core/Src/battery_show.o ./Core/Src/battery_show.su ./Core/Src/buzzer.cyclo ./Core/Src/buzzer.d ./Core/Src/buzzer.o ./Core/Src/buzzer.su ./Core/Src/diskio.cyclo ./Core/Src/diskio.d ./Core/Src/diskio.o ./Core/Src/diskio.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/dosage.cyclo ./Core/Src/dosage.d ./Core/Src/dosage.o ./Core/Src/dosage.su ./Core/Src/ff.cyclo ./Core/Src/ff.d ./Core/Src/ff.o ./Core/Src/ff.su ./Core/Src/ffsystem.cyclo ./Core/Src/ffsystem.d ./Core/Src/ffsystem.o ./Core/Src/ffsystem.su ./Core/Src/ffunicode.cyclo ./Core/Src/ffunicode.d ./Core/Src/ffunicode.o ./Core/Src/ffunicode.su ./Core/Src/gas_press_clc.cyclo ./Core/Src/gas_press_clc.d ./Core/Src/gas_press_clc.o ./Core/Src/gas_press_clc.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c_soft.cyclo ./Core/Src/i2c_soft.d ./Core/Src/i2c_soft.o ./Core/Src/i2c_soft.su ./Core/Src/key.cyclo ./Core/Src/key.d ./Core/Src/key.o ./Core/Src/key.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/motor.cyclo ./Core/Src/motor.d ./Core/Src/motor.o ./Core/Src/motor.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

