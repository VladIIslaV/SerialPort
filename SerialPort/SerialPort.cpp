#include "stdafx.h"
#include "SerialPort.h"

SerialPort::SerialPort(const char *portName)
{
	isConnected = false;

	handler = CreateFileA(
		static_cast<LPCSTR>(portName),
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);
	
	if (handler == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) 
		{
			cout << "ERROR: Handle was not attached. Reason: " << portName << " not available" << endl;
		}
		else
		{
			cout << "ERROR: Invalid handler" << endl;
		}
	}
	else {

		if (!GetCommState(handler, &dcbSerialParameters)) 
		{
			cout << "ERROR: Failed to get current serial parameters" << endl;
		}
		else 
		{
			

			if (!SetCommState(handler, &dcbSerialParameters))
			{
				cout << "WARNING: could not set Serial port parameters" << endl;
			}
			else 
			{
				isConnected = true;
				PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
}

SerialPort::~SerialPort()
{
	if (isConnected) 
	{
		isConnected = false;
		CloseHandle(handler);
	}
}

int SerialPort::Read(char *buffer, unsigned int buf_size)
{
	DWORD bytesRead;
	unsigned int toRead = buf_size;

	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue > 0) 
	{
		if (this->status.cbInQue > buf_size) 
			toRead = buf_size;
		else 
			toRead = this->status.cbInQue;
	}

	if (ReadFile(this->handler, buffer, 10, &bytesRead, NULL)) 
		return bytesRead;

	return 0;
}

bool SerialPort::Write(char *buffer, unsigned int buf_size)
{
	DWORD bytesSend;

	if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0)) {
		ClearCommError(this->handler, &this->errors, &this->status);
		return false;
	}
	else return true;
}

bool SerialPort::IsConnected()
{
	return isConnected;
}

void SerialPort::SetSettings()
{
	connectionSettings = { 0 };

	connectionSettings.BaudRate = CBR_9600;
	connectionSettings.ByteSize = 8;
	connectionSettings.StopBits = ONESTOPBIT;
	connectionSettings.Parity = NOPARITY;
	connectionSettings.fDtrControl = DTR_CONTROL_ENABLE;
}
