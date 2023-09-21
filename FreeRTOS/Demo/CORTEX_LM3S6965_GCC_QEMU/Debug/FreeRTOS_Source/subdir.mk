################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/FreeRTOSv202212.01/FreeRTOS/Source/croutine.c \
C:/FreeRTOSv202212.01/FreeRTOS/Source/event_groups.c \
C:/FreeRTOSv202212.01/FreeRTOS/Source/list.c \
C:/FreeRTOSv202212.01/FreeRTOS/Source/queue.c \
C:/FreeRTOSv202212.01/FreeRTOS/Source/stream_buffer.c \
C:/FreeRTOSv202212.01/FreeRTOS/Source/tasks.c \
C:/FreeRTOSv202212.01/FreeRTOS/Source/timers.c 

OBJS += \
./FreeRTOS_Source/croutine.o \
./FreeRTOS_Source/event_groups.o \
./FreeRTOS_Source/list.o \
./FreeRTOS_Source/queue.o \
./FreeRTOS_Source/stream_buffer.o \
./FreeRTOS_Source/tasks.o \
./FreeRTOS_Source/timers.o 

C_DEPS += \
./FreeRTOS_Source/croutine.d \
./FreeRTOS_Source/event_groups.d \
./FreeRTOS_Source/list.d \
./FreeRTOS_Source/queue.d \
./FreeRTOS_Source/stream_buffer.d \
./FreeRTOS_Source/tasks.d \
./FreeRTOS_Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS_Source/croutine.o: C:/FreeRTOSv202212.01/FreeRTOS/Source/croutine.c FreeRTOS_Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra -g3 -I"C:\FreeRTOSv202212.01\FreeRTOS-Plus\Source\FreeRTOS-Plus-Trace\Include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\Common\drivers\LuminaryMicro" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU\LocalDemoFiles" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\portable\GCC\ARM_CM3" -std=gnu90 -specs=nano.specs -specs=nosys.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS_Source/event_groups.o: C:/FreeRTOSv202212.01/FreeRTOS/Source/event_groups.c FreeRTOS_Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra -g3 -I"C:\FreeRTOSv202212.01\FreeRTOS-Plus\Source\FreeRTOS-Plus-Trace\Include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\Common\drivers\LuminaryMicro" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU\LocalDemoFiles" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\portable\GCC\ARM_CM3" -std=gnu90 -specs=nano.specs -specs=nosys.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS_Source/list.o: C:/FreeRTOSv202212.01/FreeRTOS/Source/list.c FreeRTOS_Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra -g3 -I"C:\FreeRTOSv202212.01\FreeRTOS-Plus\Source\FreeRTOS-Plus-Trace\Include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\Common\drivers\LuminaryMicro" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU\LocalDemoFiles" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\portable\GCC\ARM_CM3" -std=gnu90 -specs=nano.specs -specs=nosys.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS_Source/queue.o: C:/FreeRTOSv202212.01/FreeRTOS/Source/queue.c FreeRTOS_Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra -g3 -I"C:\FreeRTOSv202212.01\FreeRTOS-Plus\Source\FreeRTOS-Plus-Trace\Include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\Common\drivers\LuminaryMicro" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU\LocalDemoFiles" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\portable\GCC\ARM_CM3" -std=gnu90 -specs=nano.specs -specs=nosys.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS_Source/stream_buffer.o: C:/FreeRTOSv202212.01/FreeRTOS/Source/stream_buffer.c FreeRTOS_Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra -g3 -I"C:\FreeRTOSv202212.01\FreeRTOS-Plus\Source\FreeRTOS-Plus-Trace\Include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\Common\drivers\LuminaryMicro" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU\LocalDemoFiles" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\portable\GCC\ARM_CM3" -std=gnu90 -specs=nano.specs -specs=nosys.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS_Source/tasks.o: C:/FreeRTOSv202212.01/FreeRTOS/Source/tasks.c FreeRTOS_Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra -g3 -I"C:\FreeRTOSv202212.01\FreeRTOS-Plus\Source\FreeRTOS-Plus-Trace\Include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\Common\drivers\LuminaryMicro" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU\LocalDemoFiles" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\portable\GCC\ARM_CM3" -std=gnu90 -specs=nano.specs -specs=nosys.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS_Source/timers.o: C:/FreeRTOSv202212.01/FreeRTOS/Source/timers.c FreeRTOS_Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra -g3 -I"C:\FreeRTOSv202212.01\FreeRTOS-Plus\Source\FreeRTOS-Plus-Trace\Include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\Common\drivers\LuminaryMicro" -I"C:\FreeRTOSv202212.01\FreeRTOS\Demo\CORTEX_LM3S6965_GCC_QEMU\LocalDemoFiles" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\include" -I"C:\FreeRTOSv202212.01\FreeRTOS\Source\portable\GCC\ARM_CM3" -std=gnu90 -specs=nano.specs -specs=nosys.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


