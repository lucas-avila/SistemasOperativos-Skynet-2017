################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/header/AppConfig.c \
../src/header/Estructuras.c \
../src/header/ExitCodes.c \
../src/header/InterfazKernel.c \
../src/header/Semaforo.c \
../src/header/Socket.c \
../src/header/funcionesUtiles.c 

OBJS += \
./src/header/AppConfig.o \
./src/header/Estructuras.o \
./src/header/ExitCodes.o \
./src/header/InterfazKernel.o \
./src/header/Semaforo.o \
./src/header/Socket.o \
./src/header/funcionesUtiles.o 

C_DEPS += \
./src/header/AppConfig.d \
./src/header/Estructuras.d \
./src/header/ExitCodes.d \
./src/header/InterfazKernel.d \
./src/header/Semaforo.d \
./src/header/Socket.d \
./src/header/funcionesUtiles.d 


# Each subdirectory must supply rules for building sources it contributes
src/header/%.o: ../src/header/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Escritorio/tp-2017-1c-Skynet/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


