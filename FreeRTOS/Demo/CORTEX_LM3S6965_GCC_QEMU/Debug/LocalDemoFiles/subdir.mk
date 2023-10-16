################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LocalDemoFiles/display.c \
../LocalDemoFiles/globals.c \
../LocalDemoFiles/osram128x64x4.c \
../LocalDemoFiles/serial.c \
../LocalDemoFiles/snake.c \
../LocalDemoFiles/worker.c 

OBJS += \
./LocalDemoFiles/display.o \
./LocalDemoFiles/globals.o \
./LocalDemoFiles/osram128x64x4.o \
./LocalDemoFiles/serial.o \
./LocalDemoFiles/snake.o \
./LocalDemoFiles/worker.o 

C_DEPS += \
./LocalDemoFiles/display.d \
./LocalDemoFiles/globals.d \
./LocalDemoFiles/osram128x64x4.d \
./LocalDemoFiles/serial.d \
./LocalDemoFiles/snake.d \
./LocalDemoFiles/worker.d 


# Each subdirectory must supply rules for building sources it contributes
LocalDemoFiles/%.o: ../LocalDemoFiles/%.c LocalDemoFiles/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra -g3 -I"C:\FreeRTOSv202212.01\FreeRTOS-Plus\Source\FreeRTOS-Plus-Trace\Include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\Common\drivers\LuminaryMicro" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU\LocalDemoFiles" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\portable\GCC\ARM_CM3" -std=gnu90 -specs=nano.specs -specs=nosys.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


