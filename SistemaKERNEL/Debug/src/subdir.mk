################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/KERNEL.c \
../src/PCB.c \
../src/SolicitudesUsuario.c 

OBJS += \
./src/KERNEL.o \
./src/PCB.o \
./src/SolicitudesUsuario.o 

C_DEPS += \
./src/KERNEL.d \
./src/PCB.d \
./src/SolicitudesUsuario.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/2017/C/UNIVERSIDAD/PROYECTOS/ENTREGA_FINAL/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


