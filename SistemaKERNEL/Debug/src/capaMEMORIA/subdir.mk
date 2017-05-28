################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/capaMEMORIA/AdministrarSemaforos.c \
../src/capaMEMORIA/AdministrarVarCompartidas.c \
../src/capaMEMORIA/GestMemoriaFuncionesAux.c \
../src/capaMEMORIA/GestionMemoriaDinamica.c 

OBJS += \
./src/capaMEMORIA/AdministrarSemaforos.o \
./src/capaMEMORIA/AdministrarVarCompartidas.o \
./src/capaMEMORIA/GestMemoriaFuncionesAux.o \
./src/capaMEMORIA/GestionMemoriaDinamica.o 

C_DEPS += \
./src/capaMEMORIA/AdministrarSemaforos.d \
./src/capaMEMORIA/AdministrarVarCompartidas.d \
./src/capaMEMORIA/GestMemoriaFuncionesAux.d \
./src/capaMEMORIA/GestionMemoriaDinamica.d 


# Each subdirectory must supply rules for building sources it contributes
src/capaMEMORIA/%.o: ../src/capaMEMORIA/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


