################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/general/Semaforo.c \
../src/general/funcionesUtiles.c 

OBJS += \
./src/general/Semaforo.o \
./src/general/funcionesUtiles.o 

C_DEPS += \
./src/general/Semaforo.d \
./src/general/funcionesUtiles.d 


# Each subdirectory must supply rules for building sources it contributes
src/general/%.o: ../src/general/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Escritorio/tp-2017-1c-Skynet/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


