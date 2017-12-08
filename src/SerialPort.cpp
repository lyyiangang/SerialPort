/*
* Author: Manash Kumar Mandal
* Modified Library introduced in Arduino Playground which does not work
* This works perfectly
* LICENSE: MIT
*/
#include "stdafx.h"
#include "SerialPort.h"
#include <assert.h>

SerialPort::SerialPort(const std::string& portName ,const DCB& parameter):
    m_portName(portName),
    m_parameter(parameter),
    m_error(0)
{
}

SerialPort::~SerialPort()
{
    if (this->m_connected){
        this->m_connected = false;
        CloseHandle(this->m_hander);
    }
}

int SerialPort::Open()
{
    this->m_connected = false;
    this->m_hander = CreateFileA(static_cast<LPCSTR>(m_portName.c_str()),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (this->m_hander == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("ERROR: Handle was not attached. Reason: %s not available\n", m_portName.c_str());
            return 1;
        }
        else
        {
            return 2;
            printf("ERROR!!!");
        }
    }
    else {
        DCB dcbSerialParameters = { 0 };

        if (!GetCommState(this->m_hander, &dcbSerialParameters)) {
            printf("failed to get current serial parameters");
            return 3;
        }
        else {
            dcbSerialParameters = m_parameter;
            if (!SetCommState(m_hander, &dcbSerialParameters))
            {
                printf("ALERT: could not set Serial port parameters\n");
                return 4;
            }
            else {
                this->m_connected = true;
                PurgeComm(this->m_hander, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(WAIT_TIME);
            }
        }
    }
    return 0;
}

std::string SerialPort::ConvertHexStrToChar(const std::string & hexCmd)
{
    assert(hexCmd.size() > 0 && hexCmd.size() % 2 == 0);
    int nHex = hexCmd.size() / 2;
    std::string result(nHex, ' ');
    for (int ii = 0; ii < nHex; ++ii)
    {
        std::string hex_str = "0x" + hexCmd.substr(ii * 2, 2);
        int tmpval = std::stoul(hex_str, nullptr, 16);
        result[ii] = static_cast<char>(tmpval);
    }
    return result;
}

int SerialPort::ReadSerialPort(char *buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(this->m_hander, &this->m_error, &this->m_status);

    if (this->m_status.cbInQue > 0){
        if (this->m_status.cbInQue > buf_size){
            toRead = buf_size;
        }
        else toRead = this->m_status.cbInQue;
    }

    if (ReadFile(this->m_hander, buffer, toRead, &bytesRead, NULL)) return bytesRead;

    return 0;
}

bool SerialPort::WriteSerialPort(char *buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->m_hander, (void*) buffer, buf_size, &bytesSend, 0)){
        ClearCommError(this->m_hander, &this->m_error, &this->m_status);
        return false;
    }
    else return true;
}

bool SerialPort::IsConnected()
{
    return this->m_connected;
}

void SerialPort::Close()
{
    if (this->m_connected) {
        this->m_connected = false;
        CloseHandle(this->m_hander);
    }
}

 DCB SerialPort::DefaultParameter() 
{
    DCB defaultParameter = { 0 };
    defaultParameter.BaudRate = CBR_9600;
    defaultParameter.ByteSize = 8;
    defaultParameter.StopBits = ONESTOPBIT;
    defaultParameter.Parity = NOPARITY;
    defaultParameter.fDtrControl = DTR_CONTROL_ENABLE;
    return defaultParameter;
}
