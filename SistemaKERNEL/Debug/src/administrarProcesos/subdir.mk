################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/administrarProcesos/Proceso.c 

OBJS += \
./src/administrarProcesos/Proceso.o 

C_DEPS += \
./src/administrarProcesos/Proceso.d 


# Each subdirectory must supply rules for building sources it contributes
src/administrarProcesos/%.o: ../src/administrarProcesos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-1c-Skynet/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


