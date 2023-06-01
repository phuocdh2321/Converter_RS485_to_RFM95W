################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../RFM95W/rfm95w.cpp 

OBJS += \
./RFM95W/rfm95w.o 

CPP_DEPS += \
./RFM95W/rfm95w.d 


# Each subdirectory must supply rules for building sources it contributes
RFM95W/%.o RFM95W/%.su: ../RFM95W/%.cpp RFM95W/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32_Project/WaterGauge/RFM95W" -I"D:/STM32_Project/WaterGauge/Termianl" -I"D:/STM32_Project/WaterGauge/WaterGaugeRs485" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-RFM95W

clean-RFM95W:
	-$(RM) ./RFM95W/rfm95w.d ./RFM95W/rfm95w.o ./RFM95W/rfm95w.su

.PHONY: clean-RFM95W

