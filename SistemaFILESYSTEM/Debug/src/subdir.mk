################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Archivo.c \
../src/FILE_SYSTEM.c \
../src/FileManager.c \
../src/InterfazKernel.c 

OBJS += \
./src/Archivo.o \
./src/FILE_SYSTEM.o \
./src/FileManager.o \
./src/InterfazKernel.o 

C_DEPS += \
./src/Archivo.d \
./src/FILE_SYSTEM.d \
./src/FileManager.d \
./src/InterfazKernel.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Escritorio/tp-2017-1c-Skynet/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


