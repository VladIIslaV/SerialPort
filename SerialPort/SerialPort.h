#pragma once

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class SerialPort
{
private:
	HANDLE handler;
	bool isConnected;
	COMSTAT status;
	DWORD errors;
	DCB connectionSettings;
public:
	SerialPort(const char *portName);
	~SerialPort();

	int Read(char *buffer, unsigned int buf_size);
	bool Write(char *buffer, unsigned int buf_size);
	bool IsConnected();

	void SetSettings();
};
