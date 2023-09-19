################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utility\ Shed/binary_searchtree_labels.c \
../Utility\ Shed/binary_searchtree_string.c \
../Utility\ Shed/constants.c \
../Utility\ Shed/error_handling.c \
../Utility\ Shed/file_manager.c \
../Utility\ Shed/utility.c 

OBJS += \
./Utility\ Shed/binary_searchtree_labels.o \
./Utility\ Shed/binary_searchtree_string.o \
./Utility\ Shed/constants.o \
./Utility\ Shed/error_handling.o \
./Utility\ Shed/file_manager.o \
./Utility\ Shed/utility.o 

C_DEPS += \
./Utility\ Shed/binary_searchtree_labels.d \
./Utility\ Shed/binary_searchtree_string.d \
./Utility\ Shed/constants.d \
./Utility\ Shed/error_handling.d \
./Utility\ Shed/file_manager.d \
./Utility\ Shed/utility.d 


# Each subdirectory must supply rules for building sources it contributes
Utility\ Shed/binary_searchtree_labels.o: ../Utility\ Shed/binary_searchtree_labels.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/Utility Shed" -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utility Shed/binary_searchtree_labels.d" -MT"Utility\ Shed/binary_searchtree_labels.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Utility\ Shed/binary_searchtree_string.o: ../Utility\ Shed/binary_searchtree_string.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/Utility Shed" -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utility Shed/binary_searchtree_string.d" -MT"Utility\ Shed/binary_searchtree_string.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Utility\ Shed/constants.o: ../Utility\ Shed/constants.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/Utility Shed" -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utility Shed/constants.d" -MT"Utility\ Shed/constants.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Utility\ Shed/error_handling.o: ../Utility\ Shed/error_handling.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/Utility Shed" -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utility Shed/error_handling.d" -MT"Utility\ Shed/error_handling.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Utility\ Shed/file_manager.o: ../Utility\ Shed/file_manager.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/Utility Shed" -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utility Shed/file_manager.d" -MT"Utility\ Shed/file_manager.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Utility\ Shed/utility.o: ../Utility\ Shed/utility.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/Utility Shed" -I"/home/user/Desktop/Custom Assembler/CusAssembler/Assembler/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utility Shed/utility.d" -MT"Utility\ Shed/utility.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


