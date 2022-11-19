### TODO:

- modify your sequencer to use the PIO as its primary I/O engine, including the ability to R/W any register 

### Results:

- [x] Use the PIO as primary I/O engine. 
    <br>since  PIO is not suitable for button use(according to the datasheet), I decided to add PIO-I2C in the sequencer for the I2C device use such as APDS9960
<br>
The interface from PC side is:

        `i <address> <read length>`<br>
        `o <address> <write value>`


- [x]  Ability to R/W any register(details see lab03).
    #### commands
    | functions |command |
    | :--| :--  |
    | record  |`# <seconds>  <frequency(op)>`|
    | replay | `$ <filename> <frequency(op)>`|
    | read address|`r <address(8bits)>`|
    | write address|`w <address(8bits)> <value>`|
    |Input gpio|`< <Pin>`|
    |Output gpio|`> <Pin>` |
    |I2C read|`i <address uint8_t> <read length>`|
    |I2C write|`o <address> <write value>`|

