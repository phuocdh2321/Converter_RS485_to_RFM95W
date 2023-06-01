################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../WaterGaugeRs485/waterGaugeRS485.cpp 

OBJS += \
./WaterGaugeRs485/waterGaugeRS485.o 

CPP_DEPS += \
./WaterGaugeRs485/waterGaugeRS485.d 


# Each subdirectory must supply rules for building sources it contributes
WaterGaugeRs485/%.o WaterGaugeRs485/%.su: ../WaterGaugeRs485/%.cpp WaterGaugeRs485/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32_Project/WaterGauge/RFM95W" -I"D:/STM32_Project/WaterGauge/Termianl" -I"D:/STM32_Project/WaterGauge/WaterGaugeRs485" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-WaterGaugeRs485

clean-WaterGaugeRs485:
	-$(RM) ./WaterGaugeRs485/waterGaugeRS485.d ./WaterGaugeRs485/waterGaugeRS485.o ./WaterGaugeRs485/waterGaugeRS485.su

.PHONY: clean-WaterGaugeRs485

