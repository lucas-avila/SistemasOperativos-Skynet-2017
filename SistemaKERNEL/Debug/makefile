################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/testing/subdir.mk
-include src/planificacion/subdir.mk
-include src/interfaz/subdir.mk
-include src/header/subdir.mk
-include src/general/subdir.mk
-include src/capaMEMORIA/subdir.mk
-include src/capaFILESYSTEM/subdir.mk
-include src/administrarProcesos/subdir.mk
-include src/administrarPCB/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SistemaKERNEL

dependents:
<<<<<<< HEAD
	-cd /home/utnso/workspace/tp-2017-1c-Skynet/Sharedlib/Debug && $(MAKE) all
=======
	-cd /home/utnso/2017/C/UNIVERSIDAD/PROYECTOS/ENTREGA_FINAL/Sharedlib/Debug && $(MAKE) clean all
>>>>>>> cda82e0a43aaf1e3d0d373258805f6d80413d3e4

# Tool invocations
SistemaKERNEL: $(OBJS) $(USER_OBJS) /home/utnso/2017/C/UNIVERSIDAD/PROYECTOS/ENTREGA_FINAL/Sharedlib/Debug/libSharedlib.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/2017/C/UNIVERSIDAD/PROYECTOS/ENTREGA_FINAL/Sharedlib/Debug" -o "SistemaKERNEL" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) SistemaKERNEL
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/2017/C/UNIVERSIDAD/PROYECTOS/ENTREGA_FINAL/Sharedlib/Debug/libSharedlib.so:

-include ../makefile.targets
