################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LINKER_SCRIPT += \
../generate/linker_script.ld 

S_UPPER_SRCS += \
../generate/start.S 

C_SRCS += \
../generate/hwinit.c \
../generate/inthandler.c \
../generate/vects.c 

C_DEPS += \
./generate/hwinit.d \
./generate/inthandler.d \
./generate/vects.d 

OBJS += \
./generate/hwinit.o \
./generate/inthandler.o \
./generate/start.o \
./generate/vects.o 

S_UPPER_DEPS += \
./generate/start.d 


# Each subdirectory must supply rules for building sources it contributes
generate/%.o: ../generate/%.c
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	$(file > $@.in,-O0 -ffunction-sections -fdata-sections -g2 -Wstack-usage=100 -mcpu=rx600 -misa=v1 -mlittle-endian-data -I"C:/projekte/repo/HomeServer/Firmware/Renesas/RX600_EVE_HomeServer/generate" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src\\EVE" -Wa,-adlnh="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" "$<" -c -o "$@")
	@echo 'rx-elf-gcc -O0 -ffunction-sections -fdata-sections -g2 -Wstack-usage=100 -mcpu=rx600 -misa=v1 -mlittle-endian-data -I"C:/projekte/repo/HomeServer/Firmware/Renesas/RX600_EVE_HomeServer/generate" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src" -I"C:\\projekte\\repo\\HomeServer\\Firmware\\Renesas\\RX600_EVE_HomeServer\\src\\EVE" -Wa,-adlnh="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" "$<" -c -o "$@"'

	rx-elf-gcc @"$@.in"
	@echo 'Finished building: $<'
	@echo ' '

generate/%.o: ../generate/%.S
	@echo 'Building file: $<'
	@echo 'Invoking Assembler'
	$(file > $@.in,-O0 -ffunction-sections -fdata-sections -g2 -Wstack-usage=100 -mcpu=rx600 -misa=v1 -mlittle-endian-data -x assembler-with-cpp -Wa,--gdwarf2 -I"C:/projekte/repo/HomeServer/Firmware/Renesas/RX600_EVE_HomeServer/src" -Wa,-adlhn="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c "$<" -o "$@")
	@echo 'rx-elf-gcc -O0 -ffunction-sections -fdata-sections -g2 -Wstack-usage=100 -mcpu=rx600 -misa=v1 -mlittle-endian-data -x assembler-with-cpp -Wa,--gdwarf2 -I"C:/projekte/repo/HomeServer/Firmware/Renesas/RX600_EVE_HomeServer/src" -Wa,-adlhn="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c "$<" -o "$@"'

	rx-elf-gcc @"$@.in"
	@echo 'Finished building: $<'
	@echo ' '


