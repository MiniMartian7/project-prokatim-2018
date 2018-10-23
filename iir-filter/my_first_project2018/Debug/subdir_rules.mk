################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"c:/ti/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="c:/ti/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/ti/C6xCSL/include" --include_path="C:/ti/DSK6713/c6000/dsk6713/include" --include_path="C:/Users/Mackintosh/Documents/GitHub/project-prokatim-2018/iir-filter/my_first_project2018/Debug" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

my_first_project2018cfg.cmd: ../my_first_project2018.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/ti/bios_5_42_01_09/xdctools/tconf" -b -Dconfig.importPath="C:/ti/bios_5_42_01_09/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

my_first_project2018cfg.s??: | my_first_project2018cfg.cmd
my_first_project2018cfg_c.c: | my_first_project2018cfg.cmd
my_first_project2018cfg.h: | my_first_project2018cfg.cmd
my_first_project2018cfg.h??: | my_first_project2018cfg.cmd
my_first_project2018.cdb: | my_first_project2018cfg.cmd

my_first_project2018cfg.obj: ./my_first_project2018cfg.s?? $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"c:/ti/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="c:/ti/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/ti/C6xCSL/include" --include_path="C:/ti/DSK6713/c6000/dsk6713/include" --include_path="C:/Users/Mackintosh/Documents/GitHub/project-prokatim-2018/iir-filter/my_first_project2018/Debug" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="my_first_project2018cfg.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

my_first_project2018cfg_c.obj: ./my_first_project2018cfg_c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"c:/ti/ccsv6/tools/compiler/c6000_7.4.7/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="c:/ti/ccsv6/tools/compiler/c6000_7.4.7/include" --include_path="C:/ti/C6xCSL/include" --include_path="C:/ti/DSK6713/c6000/dsk6713/include" --include_path="C:/Users/Mackintosh/Documents/GitHub/project-prokatim-2018/iir-filter/my_first_project2018/Debug" --include_path="C:/ti/bios_5_42_01_09/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_01_09/packages/ti/rtdx/include/c6000" --define=c6713 --define=CHIP_6713 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="my_first_project2018cfg_c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


