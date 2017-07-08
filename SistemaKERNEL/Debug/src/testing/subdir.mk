################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/testing/TestingCapaFileSystem.c \
../src/testing/TestingGeneral.c \
../src/testing/TestingInterfazMemoria.c \
../src/testing/TestingMenu.c \
../src/testing/TestingPlanificacion.c 

OBJS += \
./src/testing/TestingCapaFileSystem.o \
./src/testing/TestingGeneral.o \
./src/testing/TestingInterfazMemoria.o \
./src/testing/TestingMenu.o \
./src/testing/TestingPlanificacion.o 

C_DEPS += \
./src/testing/TestingCapaFileSystem.d \
./src/testing/TestingGeneral.d \
./src/testing/TestingInterfazMemoria.d \
./src/testing/TestingMenu.d \
./src/testing/TestingPlanificacion.d 


# Each subdirectory must supply rules for building sources it contributes
src/testing/%.o: ../src/testing/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/2017/C/UNIVERSIDAD/PROYECTOS/ENTREGA_FINAL/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


