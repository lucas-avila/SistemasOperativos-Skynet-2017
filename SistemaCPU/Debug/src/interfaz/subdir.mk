################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/interfaz/InterfazKernel.c \
../src/interfaz/InterfazMemoria.c 

OBJS += \
./src/interfaz/InterfazKernel.o \
./src/interfaz/InterfazMemoria.o 

C_DEPS += \
./src/interfaz/InterfazKernel.d \
./src/interfaz/InterfazMemoria.d 


# Each subdirectory must supply rules for building sources it contributes
src/interfaz/%.o: ../src/interfaz/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

