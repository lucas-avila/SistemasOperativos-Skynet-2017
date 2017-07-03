################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CONSOLA.c \
../src/Estructuras.c \
../src/ExitCodes.c \
../src/InterfazKernel.c \
../src/Semaforo.c \
../src/funcionesUtiles.c 

OBJS += \
./src/CONSOLA.o \
./src/Estructuras.o \
./src/ExitCodes.o \
./src/InterfazKernel.o \
./src/Semaforo.o \
./src/funcionesUtiles.o 

C_DEPS += \
./src/CONSOLA.d \
./src/Estructuras.d \
./src/ExitCodes.d \
./src/InterfazKernel.d \
./src/Semaforo.d \
./src/funcionesUtiles.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/2017/C/UNIVERSIDAD/PROYECTOS/ENTREGA_FINAL/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


