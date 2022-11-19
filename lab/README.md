Outline:
- 01_registers
- 02_repl
- 03_sequencer
- 04_slow_motion
- 05_i2c_traffic
- 06_pioscope
- 07_pio_sequencer
- 08_adps_protocol
- 09_lab_on_a_chip
- 10_protoboard
- EC_analog_knob
- EC_ansi_escapes
- EC_asm_registers
- EC_dma_i2c
- EC_hardware_i2c
- EC_interrupt_i2c
- EC_lovr_viz
- EC_pwm_scope
- EC_socket_server
- EC_speaker

# Feedback:
 * I already have experince in serial use in python and even used ctypes to call slef-made dll in python. Lab02 is quite confusing since we learned what is REPL from lab01, which is an embedded python interpretor inside the board. So I thought we need to build such a thing on board to directly interact via serial, which means we may need to based on micropytohn to achive register access, though it is unachievable in python without using Ctypes(call external dll or sth else). The code is not difficult actually, but it took a long time to figure out what we need to do.
 * The following sequencer quesitons is very frustrated, the requirement is vague, I extracted functions includes single register r/w, record/replay, i2c r/w, single gpio r/w. But I am still not clear about what shall we add on this sequencer. I don't wanna spend time on unimportant functions, but the description make me afraid of adding each new functions. Actually I don't know how to make a suitable rubric for a homework completely based on student's own guess. 


