################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/interfaz/InterfazCPU.c \
../src/interfaz/InterfazConsola.c \
../src/interfaz/InterfazFS.c \
../src/interfaz/InterfazMemoria.c 

OBJS += \
./src/interfaz/InterfazCPU.o \
./src/interfaz/InterfazConsola.o \
./src/interfaz/InterfazFS.o \
./src/interfaz/InterfazMemoria.o 

C_DEPS += \
./src/interfaz/InterfazCPU.d \
./src/interfaz/InterfazConsola.d \
./src/interfaz/InterfazFS.d \
./src/interfaz/InterfazMemoria.d 


# Each subdirectory must supply rules for building sources it contributes
src/interfaz/%.o: ../src/interfaz/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-1c-Skynet/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


