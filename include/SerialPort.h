/*
* Author: Manash Kumar Mandal
* Modified Library introduced in Arduino Playground which does not work
* This works perfectly
* LICENSE: MIT
*/


#ifndef SERIALPORT_H
#define SERIALPORT_H

#define WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class SerialPort
{

public:
    SerialPort(const std::string& portName, const DCB& parameter);
    ~SerialPort();

    int Open();

    //if you have: 0xff 0x01 0x55 0x88, just prepare a hexCmd string "ff015588" and the converted char will be returned.
    static std::string ConvertHexStrToChar(const std::string& hexCmd);

    int ReadSerialPort(char *buffer, unsigned int buf_size);
    bool WriteSerialPort(char *buffer, unsigned int buf_size);
    bool IsConnected();
    void Close();

    static DCB DefaultParameter();

private:
    HANDLE m_hander;
    bool m_connected;
    COMSTAT m_status;
    DWORD m_error;
    std::string m_portName;
    DCB m_parameter;


};

#endif // SERIALPORT_H
