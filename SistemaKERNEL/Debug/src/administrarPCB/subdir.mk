################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/administrarPCB/EstadisticaProceso.c \
../src/administrarPCB/PCBData.c 

OBJS += \
./src/administrarPCB/EstadisticaProceso.o \
./src/administrarPCB/PCBData.o 

C_DEPS += \
./src/administrarPCB/EstadisticaProceso.d \
./src/administrarPCB/PCBData.d 


# Each subdirectory must supply rules for building sources it contributes
src/administrarPCB/%.o: ../src/administrarPCB/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Escritorio/tp-2017-1c-Skynet/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


