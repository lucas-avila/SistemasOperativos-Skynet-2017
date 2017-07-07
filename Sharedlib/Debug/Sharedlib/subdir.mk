################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sharedlib/PCB.c \
../Sharedlib/Socket.c 

OBJS += \
./Sharedlib/PCB.o \
./Sharedlib/Socket.o 

C_DEPS += \
./Sharedlib/PCB.d \
./Sharedlib/Socket.d 


# Each subdirectory must supply rules for building sources it contributes
Sharedlib/%.o: ../Sharedlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


