################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/KERNEL.c \
../src/PCB.c \
../src/Planificacion.c \
../src/SolicitudesUsuario.c 

OBJS += \
./src/KERNEL.o \
./src/PCB.o \
./src/Planificacion.o \
./src/SolicitudesUsuario.o 

C_DEPS += \
./src/KERNEL.d \
./src/PCB.d \
./src/Planificacion.d \
./src/SolicitudesUsuario.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


