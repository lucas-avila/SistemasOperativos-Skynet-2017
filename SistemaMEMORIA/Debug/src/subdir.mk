################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FuncionesMemoria.c \
../src/InterfazCPU.c \
../src/InterfazKernel.c \
../src/MEMORIA.c \
../src/SolicitudesUsuario.c 

OBJS += \
./src/FuncionesMemoria.o \
./src/InterfazCPU.o \
./src/InterfazKernel.o \
./src/MEMORIA.o \
./src/SolicitudesUsuario.o 

C_DEPS += \
./src/FuncionesMemoria.d \
./src/InterfazCPU.d \
./src/InterfazKernel.d \
./src/MEMORIA.d \
./src/SolicitudesUsuario.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


