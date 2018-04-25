#============================================================================
# Run: 
#    xtclsh free_rtos_ise.tcl  - creates XILINX ISE project file
#    ise free_rtos_project.ise - opens the project
#============================================================================
source "../../../../../base/xilinxise.tcl"

project_new "free_rtos_project"
project_set_props
puts "Adding source files"
xfile add "../../../../../fpga/units/clkgen/clkgen_config.vhd"
xfile add "free_rtos_config.vhd"
xfile add "../../../../../fpga/units/clkgen/clkgen.vhd"
xfile add "../../../../../fpga/units/math/math_pack.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc_entity.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc_autoincr.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_reg.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_ctrl.vhd"
xfile add "../../../../../fpga/chips/fpga_xc3s50.vhd"
xfile add "../../../../../fpga/chips/architecture_bare/arch_bare_ifc.vhd"
xfile add "../../../../../fpga/chips/architecture_bare/tlv_bare_ifc.vhd"
xfile add "../../../../../fpga/ctrls/lcd/lcd_raw.vhd"
xfile add "../../../../../fpga/ctrls/keyboard/keyboard_ctrl.vhd"
xfile add "../../../../../fpga/ctrls/keyboard/keyboard_ctrl_high.vhd"
xfile add "../../fpga/top_level.vhd"
puts "Libraries"
project_set_top "fpga"
project_close
