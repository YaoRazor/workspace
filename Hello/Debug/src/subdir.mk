################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/error_processing.cpp \
../src/linker.cpp \
../src/linker_pass.cpp \
../src/warning_and_error.cpp 

OBJS += \
./src/error_processing.o \
./src/linker.o \
./src/linker_pass.o \
./src/warning_and_error.o 

CPP_DEPS += \
./src/error_processing.d \
./src/linker.d \
./src/linker_pass.d \
./src/warning_and_error.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


