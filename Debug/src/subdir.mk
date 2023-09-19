################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Assembler.c \
../src/machine_code_factory.c \
../src/memory_buffer.c \
../src/pre_assembler.c 

OBJS += \
./src/Assembler.o \
./src/machine_code_factory.o \
./src/memory_buffer.o \
./src/pre_assembler.o 

C_DEPS += \
./src/Assembler.d \
./src/machine_code_factory.d \
./src/memory_buffer.d \
./src/pre_assembler.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/Utility Shed" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


