################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/UrApHMP.cpp \
../src/main.cpp \
../src/model.cpp \
../src/solution.cpp \
../src/solver.cpp 

OBJS += \
./src/UrApHMP.o \
./src/main.o \
./src/model.o \
./src/solution.o \
./src/solver.o 

CPP_DEPS += \
./src/UrApHMP.d \
./src/main.d \
./src/model.d \
./src/solution.d \
./src/solver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/opt/ibm/ILOG/CPLEX_Studio1261/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio1261/concert/include -O0 -g3 -Wall -c -fmessage-length=0 -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" -c "$<"
	@echo 'Finished building: $<'
	@echo ' '


