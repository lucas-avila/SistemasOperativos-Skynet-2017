################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/administrarPCB/EstadisticaProceso.c 

OBJS += \
./src/administrarPCB/EstadisticaProceso.o 

C_DEPS += \
./src/administrarPCB/EstadisticaProceso.d 


# Each subdirectory must supply rules for building sources it contributes
src/administrarPCB/%.o: ../src/administrarPCB/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


