################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/header/AppConfig.c \
../src/header/Socket.c 

OBJS += \
./src/header/AppConfig.o \
./src/header/Socket.o 

C_DEPS += \
./src/header/AppConfig.d \
./src/header/Socket.d 


# Each subdirectory must supply rules for building sources it contributes
src/header/%.o: ../src/header/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/UNIVERSIDAD/2017/OPERATIVOS/PROYECTOS/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


