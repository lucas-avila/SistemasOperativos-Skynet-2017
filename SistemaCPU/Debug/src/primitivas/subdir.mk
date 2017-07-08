################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/primitivas/FuncionesAuxiliares.c \
../src/primitivas/IndiceCodigo.c \
../src/primitivas/PrimitivasFunciones.c \
../src/primitivas/Stack.c 

OBJS += \
./src/primitivas/FuncionesAuxiliares.o \
./src/primitivas/IndiceCodigo.o \
./src/primitivas/PrimitivasFunciones.o \
./src/primitivas/Stack.o 

C_DEPS += \
./src/primitivas/FuncionesAuxiliares.d \
./src/primitivas/IndiceCodigo.d \
./src/primitivas/PrimitivasFunciones.d \
./src/primitivas/Stack.d 


# Each subdirectory must supply rules for building sources it contributes
src/primitivas/%.o: ../src/primitivas/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/2017/C/UNIVERSIDAD/PROYECTOS/ENTREGA_FINAL/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


