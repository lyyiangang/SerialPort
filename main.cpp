#include "stdafx.h"
#include <iostream>
#include "SerialPort.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <assert.h>
#include <vector>

using namespace std;

char* portName = "\\\\.\\COM3";

#define MAX_DATA_LENGTH 255

char incomingData[MAX_DATA_LENGTH];

//Control signals for turning on and turning off the led
//Check relay12v code
char ledON[] = "ON\n";
char ledOFF[] = "OFF\n";

SerialPort *relay12v;

//Blinking Delay
const unsigned int BLINKING_DELAY = 1000;

//If you want to send data then define "SEND" else comment it out
#define SEND

void exampleReceiveData(void)
{
    int readResult = relay12v->ReadSerialPort(incomingData, MAX_DATA_LENGTH);
    printf("%s", incomingData);
    Sleep(10);
}

void exampleWriteData(unsigned int delayTime)
{
    relay12v->WriteSerialPort(ledON, MAX_DATA_LENGTH);
    Sleep(delayTime);
    relay12v->WriteSerialPort(ledOFF, MAX_DATA_LENGTH);
    Sleep(delayTime);
}


void Test12vUsb()
{
    const std::string myPort = "\\\\.\\COM4";
    SerialPort usb12v(myPort, SerialPort::DefaultParameter());
    int ret = usb12v.Open();
    if (ret != 0)
    {
        std::cout << "open failed. Error code:" << ret << std::endl;
        return;
    }
    std::string hex_open_all_cmd = "00f1ff";
    std::string hex_close_3_cmd = "0001ff";//0x24,0x01,0x06...
    std::string byte_open_all_cmd = SerialPort::ConvertHexStrToChar(hex_open_all_cmd);
    std::string byte_close_3_cmd = SerialPort::ConvertHexStrToChar(hex_close_3_cmd);
    usb12v.WriteSerialPort(const_cast<char*>(byte_open_all_cmd.c_str()), byte_open_all_cmd.size());
    Sleep(1000);
    usb12v.WriteSerialPort(const_cast<char*>(byte_close_3_cmd.c_str()),byte_close_3_cmd.size());
    Sleep(1000);
    usb12v.Close();
}
int main()
{
    Test12vUsb();
    //old codes
//    if(0)
//    {
//        const std::string myPort = "\\\\.\\COM3";
//        relay12v = new SerialPort(myPort.c_str());
//
//        //Checking if relay12v is connected or not
//        if (relay12v->IsConnected()) {
//            std::cout << "Connection established at port " << portName << endl;
//        }
//
//#ifdef SEND
//        while (relay12v->IsConnected()) exampleWriteData(BLINKING_DELAY);
//#else // SEND
//        while (relay12v->IsConnected()) exampleReceiveData();
//#endif // SEND
//    }
}
