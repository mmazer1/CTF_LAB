################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2031/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/maxma/workspace_ccstheia/ctf_LAB/lcd_write_memory_LP_MSPM0L2228_nortos_ticlang" -I"C:/Users/maxma/workspace_ccstheia/ctf_LAB/lcd_write_memory_LP_MSPM0L2228_nortos_ticlang/Debug" -I"C:/ti/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_07_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-877713311: ../lcd_write_memory.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs2031/ccs/utils/sysconfig_1.25.0/sysconfig_cli.bat" --script "C:/Users/maxma/workspace_ccstheia/ctf_LAB/lcd_write_memory_LP_MSPM0L2228_nortos_ticlang/lcd_write_memory.syscfg" -o "." -s "C:/ti/mspm0_sdk_2_07_00_05/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-877713311 ../lcd_write_memory.syscfg
device.opt: build-877713311
device.cmd.genlibs: build-877713311
ti_msp_dl_config.c: build-877713311
ti_msp_dl_config.h: build-877713311

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2031/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/maxma/workspace_ccstheia/ctf_LAB/lcd_write_memory_LP_MSPM0L2228_nortos_ticlang" -I"C:/Users/maxma/workspace_ccstheia/ctf_LAB/lcd_write_memory_LP_MSPM0L2228_nortos_ticlang/Debug" -I"C:/ti/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_07_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0l222x_ticlang.o: C:/ti/mspm0_sdk_2_07_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0l222x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2031/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/maxma/workspace_ccstheia/ctf_LAB/lcd_write_memory_LP_MSPM0L2228_nortos_ticlang" -I"C:/Users/maxma/workspace_ccstheia/ctf_LAB/lcd_write_memory_LP_MSPM0L2228_nortos_ticlang/Debug" -I"C:/ti/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_07_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


