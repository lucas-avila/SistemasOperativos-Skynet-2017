################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/InterfazCPU.c \
../src/InterfazConsola.c \
../src/InterfazFS.c \
../src/InterfazMemoria.c \
../src/KERNEL.c \
../src/SolicitudesUsuario.c 

OBJS += \
./src/InterfazCPU.o \
./src/InterfazConsola.o \
./src/InterfazFS.o \
./src/InterfazMemoria.o \
./src/KERNEL.o \
./src/SolicitudesUsuario.o 

C_DEPS += \
./src/InterfazCPU.d \
./src/InterfazConsola.d \
./src/InterfazFS.d \
./src/InterfazMemoria.d \
./src/KERNEL.d \
./src/SolicitudesUsuario.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


