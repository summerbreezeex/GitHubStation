################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Serialization/ByteBuffer.cpp 

OBJS += \
./Serialization/ByteBuffer.o 

CPP_DEPS += \
./Serialization/ByteBuffer.d 


# Each subdirectory must supply rules for building sources it contributes
Serialization/%.o: ../Serialization/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/cent/mysql-connector-c-6.0.2-linux-glibc2.3-x86-32bit/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


