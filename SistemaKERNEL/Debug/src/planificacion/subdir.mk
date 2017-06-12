################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/planificacion/Planificacion.c \
../src/planificacion/PlanificacionFIFO.c \
../src/planificacion/PlanificacionRR.c 

OBJS += \
./src/planificacion/Planificacion.o \
./src/planificacion/PlanificacionFIFO.o \
./src/planificacion/PlanificacionRR.o 

C_DEPS += \
./src/planificacion/Planificacion.d \
./src/planificacion/PlanificacionFIFO.d \
./src/planificacion/PlanificacionRR.d 


# Each subdirectory must supply rules for building sources it contributes
src/planificacion/%.o: ../src/planificacion/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/UNIVERSIDAD/2017/OPERATIVOS/PROYECTOS/Sharedlib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


