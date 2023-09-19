################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UtilityShed/binary_search_tree.c \
../UtilityShed/constants.c 

OBJS += \
./UtilityShed/binary_search_tree.o \
./UtilityShed/constants.o 

C_DEPS += \
./UtilityShed/binary_search_tree.d \
./UtilityShed/constants.d 


# Each subdirectory must supply rules for building sources it contributes
UtilityShed/%.o: ../UtilityShed/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/UShed" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


