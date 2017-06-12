################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/testing/testearPrimitivasFunciones.c \
../src/testing/testingPCB.c 

OBJS += \
./src/testing/testearPrimitivasFunciones.o \
./src/testing/testingPCB.o 

C_DEPS += \
./src/testing/testearPrimitivasFunciones.d \
./src/testing/testingPCB.d 


# Each subdirectory must supply rules for building sources it contributes
src/testing/%.o: ../src/testing/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/UNIVERSIDAD/2017/OPERATIVOS/PROYECTOS/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


