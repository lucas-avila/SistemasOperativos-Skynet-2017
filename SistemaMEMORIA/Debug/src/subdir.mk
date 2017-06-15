################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Interfaz.c \
../src/Main.c \
../src/MemoriaCache.c \
../src/MemoriaPrincipal.c \
../src/SolicitudesUsuario.c 

OBJS += \
./src/Interfaz.o \
./src/Main.o \
./src/MemoriaCache.o \
./src/MemoriaPrincipal.o \
./src/SolicitudesUsuario.o 

C_DEPS += \
./src/Interfaz.d \
./src/Main.d \
./src/MemoriaCache.d \
./src/MemoriaPrincipal.d \
./src/SolicitudesUsuario.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Escritorio/tp-2017-1c-Skynet/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


