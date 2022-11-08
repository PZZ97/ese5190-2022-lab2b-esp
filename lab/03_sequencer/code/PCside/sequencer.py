"""CircuitPython Essentials HID Keyboard example"""
import time
import serial 
import os
import serial.tools.list_ports
from functions import selectCOM,mkdir
    # def 

# w+address+" "+value
# r+address
# c+value      set color   // PIO function
# <+Pin        read gpio pin
# <            read last gpio pin 
# b+value      set brightness 
# #+seconds        start record num seconds
# p+Pin         Set read pins
QTPY_BOOT_PIN=21

class sequencer():
    def __init__(self,dev):
        print("init")
        try:
            self.ser = serial.Serial(dev, 115200, timeout=0)
        except Exception as e:
            print("COM can not open,Exception=",e)
        self.file=None

    def readIO(self):
        self.__readIO(21)

    def __readIO(self,pin):
        def sendPin(__pin):
            self.ser.write('<'.encode('utf-8')) # io read identifier
            self.ser.write(str(__pin).encode('utf-8'))
            self.ser.write(b'\n')   # scanf identifier
        sendPin(pin)
        tmp=self.ser.read(2)
        if tmp==b'':
            return ""
        print(tmp[1])
        # print(type(tmp[1]))
#
        if tmp[1]==49:
            print("\t49")
            return '-'
        else:
            print("\t48")
            return '_'
    # record at a least a few seconds of button input to your RP2040 (in RAM)
    # time period and frequency
    def record(self,seconds,frequency=200):   #
        start_t=time.time()
        period = 1/frequency
        s='p'+str(QTPY_BOOT_PIN)
        self.ser.write(s.encode('utf-8'))
        filename= time.strftime("%m%d%H%M%S", time.localtime())  
        print(filename)
        with open('./records/'+filename+'.txt', 'w') as self.file:
            self.file.write('frequency='+str(frequency)+'\n')
            while time.time()-start_t<seconds:
                self.file.write(self.__readIO(QTPY_BOOT_PIN))
                # time.sleep(period)
        self.file.close()
        print("Done")

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
    
    def readREG(self,address):
        self.ser.write('r'.encode('utf-8'))
        self.ser.write([0x50,0x00,0x00,0x00])
        data=self.ser.readline()
        # self.ser.flushInput()
        # for a in data:
        #     print(a)
        print("read:")
        print(data)
    def writeREG(self,address,value):
        self.ser.write('w'.encode('utf-8'))
        # self.ser.write([0x10,0x00,0x00,0x60])
        self.ser.write(b'500000A0\n')
        # self.ser.write(b'\n')
        # self.ser.write([0x00,0x00,0x03,0x00])
        self.ser.write(b'12340000\n')

        # self.ser.write(b'\n')
        # data=self.ser.read(5)
        data=self.ser.readline()
        print("write")
        print(data)
        print(len(data))
        data=self.ser.readline()
        # print("write")
        print(data)
        print(len(data))




if __name__ =="__main__":
    mkdir() #create folders for recordings 
    COM_list=selectCOM()
    # iCOM=int(input())
    # seq=sequencer(COM_list[iCOM])
    seq=sequencer(COM_list[0])
    # seq.readREG()
    # seq.writeREG()
    # x=input()
    # while True:
    #     seq.readIO()

    while True:
        command=input()
        args= command.split(" ")
        if args[0]=='#': # start recording
            seq.record(int(args[1]))

