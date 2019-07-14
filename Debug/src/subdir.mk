################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/De_Encode.c \
../src/ThinkGearStreamParser.c \
../src/bld_decode.c \
../src/crc16.c \
../src/eeg_tools.c \
../src/parserEEG.c 

OBJS += \
./src/De_Encode.o \
./src/ThinkGearStreamParser.o \
./src/bld_decode.o \
./src/crc16.o \
./src/eeg_tools.o \
./src/parserEEG.o 

C_DEPS += \
./src/De_Encode.d \
./src/ThinkGearStreamParser.d \
./src/bld_decode.d \
./src/crc16.d \
./src/eeg_tools.d \
./src/parserEEG.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


