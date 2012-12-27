################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Net/Clock.cpp \
../Net/Command.cpp \
../Net/Ctx.cpp \
../Net/Epoll.cpp \
../Net/IOThread.cpp \
../Net/Mailbox.cpp \
../Net/Object.cpp \
../Net/PollerBase.cpp \
../Net/Signaler.cpp \
../Net/Thread.cpp 

OBJS += \
./Net/Clock.o \
./Net/Command.o \
./Net/Ctx.o \
./Net/Epoll.o \
./Net/IOThread.o \
./Net/Mailbox.o \
./Net/Object.o \
./Net/PollerBase.o \
./Net/Signaler.o \
./Net/Thread.o 

CPP_DEPS += \
./Net/Clock.d \
./Net/Command.d \
./Net/Ctx.d \
./Net/Epoll.d \
./Net/IOThread.d \
./Net/Mailbox.d \
./Net/Object.d \
./Net/PollerBase.d \
./Net/Signaler.d \
./Net/Thread.d 


# Each subdirectory must supply rules for building sources it contributes
Net/%.o: ../Net/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/cent/mysql-connector-c-6.0.2-linux-glibc2.3-x86-32bit/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


