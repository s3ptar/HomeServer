################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/RX600_EVE_HomeServer.c \
../src/SPI.c \
../src/board.c 

C_DEPS += \
./src/RX600_EVE_HomeServer.d \
./src/SPI.d \
./src/board.d 

OBJS += \
./src/RX600_EVE_HomeServer.o \
./src/SPI.o \
./src/board.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	$(file > $@.in,-O0 -ffunction-sections -fdata-sections -g2 -Wstack-usage=100 -mcpu=rx600 -misa=v1 -mlittle-endian-data -I"C:/projekte/repo/HomeServer/Firmware/Renesas/RX600_EVE_HomeServer/generate" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src\\EVE" -Wa,-adlnh="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" "$<" -c -o "$@")
	@echo 'rx-elf-gcc -O0 -ffunction-sections -fdata-sections -g2 -Wstack-usage=100 -mcpu=rx600 -misa=v1 -mlittle-endian-data -I"C:/projekte/repo/HomeServer/Firmware/Renesas/RX600_EVE_HomeServer/generate" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src\\EVE" -Wa,-adlnh="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" "$<" -c -o "$@"'

	rx-elf-gcc @"$@.in"
	@echo 'Finished building: $<'
	@echo ' '


