################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ErrorManager/error_handling.c 

OBJS += \
./ErrorManager/error_handling.o 

C_DEPS += \
./ErrorManager/error_handling.d 


# Each subdirectory must supply rules for building sources it contributes
ErrorManager/%.o: ../ErrorManager/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/UShed" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


