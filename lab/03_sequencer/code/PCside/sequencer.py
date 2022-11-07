"""CircuitPython Essentials HID Keyboard example"""
import time
import serial 
import os
import serial.tools.list_ports
    
    # def 

# w+address+" "+value
# r+address
# c+value      set color   // PIO function
# >+Pin        read gpio pin
# >            read last gpio pin 
# b+value      set brightness 
# #+seconds        start record num seconds
QTPY_BOOT_PIN=21

class sequencer():
    def __init__(self,dev):
        print("init")
        try:
            self.ser = serial.Serial(dev, 115200, timeout=0.05)
        except Exception as e:
            print("COM can not open,Exception=",e)

    def readIO(self,pin):
        self.ser.write('>'.encode('utf-8'))
        self.ser.write(QTPY_BOOT_PIN)
        self.ser.write(b'\n')
        tmp=self.ser.read(2)
        self.ser.flushInput()
        print(tmp)
    # record at a least a few seconds of button input to your RP2040 (in RAM)
    def record(seconds):   #
        start_t=time.time()

        while time.time()-start_t<seconds:
            print("record")

    def loopRecord():   # 
        print("loop")   # loop the recording when play 
    
    def saveRecord(path):
        print("save")

    # replay a recorded sequence on your NeoPixel
    def playRecord():
        print("replay")

    # play a recording from your laptop
    def playRecord(path):
        print("play from laptop")
    
    def readREG(self):
        self.ser.write('r'.encode('utf-8'))
        self.ser.write([0x00,0x00,0x00,0x50])
        data=self.ser.read(5)
        # self.ser.flushInput()
        # for a in data:
        #     print(a)
        print(data)
    def writeREG(self):
        self.ser.write('w'.encode('utf-8'))
        self.ser.write([0x50,0x00,0x00,0x00])
        self.ser.write(0x00)
        self.ser.write([0x66,0x55,0x44,0x33])
        data=self.ser.read(5)
        print(data)
def selectCOM():
    l_com=[]
    l_comde=[]
    comlist= list(serial.tools.list_ports.comports())
    for i in range(0,len(comlist)):
        l_com.append(comlist[i].device)
        l_comde.append( comlist[i].description)
        print("[{}],\t{},\t{}".format(i, comlist[i].device, comlist[i].description))
    return l_com

if __name__ =="__main__":
    COM_list=selectCOM()
    # iCOM=int(input())
    # seq=sequencer(COM_list[iCOM])
    seq=sequencer(COM_list[1])
    # seq.readIO(21)
    # seq.readIO(21)
    # seq.readREG()
    seq.writeREG()
    # seq.readREG()

    # while True:
    #     command=input()
    #     if command[0]=='#': # start recording
    #         seq.record(int(command[1:]))
