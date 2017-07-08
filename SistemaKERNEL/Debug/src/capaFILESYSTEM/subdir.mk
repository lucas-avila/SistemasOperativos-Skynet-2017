################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/capaFILESYSTEM/GestionArchivoFuncAux.c \
../src/capaFILESYSTEM/GestionArchivos.c \
../src/capaFILESYSTEM/TablaGlobalArchivo.c \
../src/capaFILESYSTEM/TablaProcesoArchivo.c 

OBJS += \
./src/capaFILESYSTEM/GestionArchivoFuncAux.o \
./src/capaFILESYSTEM/GestionArchivos.o \
./src/capaFILESYSTEM/TablaGlobalArchivo.o \
./src/capaFILESYSTEM/TablaProcesoArchivo.o 

C_DEPS += \
./src/capaFILESYSTEM/GestionArchivoFuncAux.d \
./src/capaFILESYSTEM/GestionArchivos.d \
./src/capaFILESYSTEM/TablaGlobalArchivo.d \
./src/capaFILESYSTEM/TablaProcesoArchivo.d 


# Each subdirectory must supply rules for building sources it contributes
src/capaFILESYSTEM/%.o: ../src/capaFILESYSTEM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2017-1c-Skynet/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


