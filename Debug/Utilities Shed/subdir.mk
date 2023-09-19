################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities\ Shed/binary_search_tree.c \
../Utilities\ Shed/constants.c \
../Utilities\ Shed/file_management.c 

OBJS += \
./Utilities\ Shed/binary_search_tree.o \
./Utilities\ Shed/constants.o \
./Utilities\ Shed/file_management.o 

C_DEPS += \
./Utilities\ Shed/binary_search_tree.d \
./Utilities\ Shed/constants.d \
./Utilities\ Shed/file_management.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities\ Shed/binary_search_tree.o: ../Utilities\ Shed/binary_search_tree.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utilities Shed/binary_search_tree.d" -MT"Utilities\ Shed/binary_search_tree.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Utilities\ Shed/constants.o: ../Utilities\ Shed/constants.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utilities Shed/constants.d" -MT"Utilities\ Shed/constants.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Utilities\ Shed/file_management.o: ../Utilities\ Shed/file_management.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Utilities Shed/file_management.d" -MT"Utilities\ Shed/file_management.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


