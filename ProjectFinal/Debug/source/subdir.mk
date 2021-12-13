################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Project.c \
../source/Terminal.c \
../source/cbfifo.c \
../source/cbfifo_testcase.c \
../source/commandprocessor.c \
../source/i2c.c \
../source/led_tmp.c \
../source/led_tmp_testcase.c \
../source/mma8451.c \
../source/msec_timers.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/statemachine.c \
../source/switch.c \
../source/sysclock.c \
../source/testmodules.c \
../source/touch_slider.c \
../source/uart.c 

OBJS += \
./source/Project.o \
./source/Terminal.o \
./source/cbfifo.o \
./source/cbfifo_testcase.o \
./source/commandprocessor.o \
./source/i2c.o \
./source/led_tmp.o \
./source/led_tmp_testcase.o \
./source/mma8451.o \
./source/msec_timers.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/statemachine.o \
./source/switch.o \
./source/sysclock.o \
./source/testmodules.o \
./source/touch_slider.o \
./source/uart.o 

C_DEPS += \
./source/Project.d \
./source/Terminal.d \
./source/cbfifo.d \
./source/cbfifo_testcase.d \
./source/commandprocessor.d \
./source/i2c.d \
./source/led_tmp.d \
./source/led_tmp_testcase.d \
./source/mma8451.d \
./source/msec_timers.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/statemachine.d \
./source/switch.d \
./source/sysclock.d \
./source/testmodules.d \
./source/touch_slider.d \
./source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Pradyumna\Documents\MCUXpressoIDE_11.2.0_4120\workspace\ProjectFinal\board" -I"C:\Users\Pradyumna\Documents\MCUXpressoIDE_11.2.0_4120\workspace\ProjectFinal\source" -I"C:\Users\Pradyumna\Documents\MCUXpressoIDE_11.2.0_4120\workspace\ProjectFinal" -I"C:\Users\Pradyumna\Documents\MCUXpressoIDE_11.2.0_4120\workspace\ProjectFinal\drivers" -I"C:\Users\Pradyumna\Documents\MCUXpressoIDE_11.2.0_4120\workspace\ProjectFinal\CMSIS" -I"C:\Users\Pradyumna\Documents\MCUXpressoIDE_11.2.0_4120\workspace\ProjectFinal\utilities" -I"C:\Users\Pradyumna\Documents\MCUXpressoIDE_11.2.0_4120\workspace\ProjectFinal\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


