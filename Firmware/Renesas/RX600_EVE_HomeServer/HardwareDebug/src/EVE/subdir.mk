################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/EVE/EVE.c \
../src/EVE/EVE_SPI.c \
../src/EVE/Menu.c \
../src/EVE/copro_commands.c 

C_DEPS += \
./src/EVE/EVE.d \
./src/EVE/EVE_SPI.d \
./src/EVE/Menu.d \
./src/EVE/copro_commands.d 

OBJS += \
./src/EVE/EVE.o \
./src/EVE/EVE_SPI.o \
./src/EVE/Menu.o \
./src/EVE/copro_commands.o 


# Each subdirectory must supply rules for building sources it contributes
src/EVE/%.o: ../src/EVE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	$(file > $@.in,-O0 -ffunction-sections -fdata-sections -g2 -Wstack-usage=100 -mcpu=rx600 -misa=v1 -mlittle-endian-data -I"C:/projekte/repo/HomeServer/Firmware/Renesas/RX600_EVE_HomeServer/generate" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src\\EVE" -Wa,-adlnh="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" "$<" -c -o "$@")
	@echo 'rx-elf-gcc -O0 -ffunction-sections -fdata-sections -g2 -Wstack-usage=100 -mcpu=rx600 -misa=v1 -mlittle-endian-data -I"C:/projekte/repo/HomeServer/Firmware/Renesas/RX600_EVE_HomeServer/generate" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src\\EVE" -Wa,-adlnh="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" "$<" -c -o "$@"'

	rx-elf-gcc @"$@.in"
	@echo 'Finished building: $<'
	@echo ' '


