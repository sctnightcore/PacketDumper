// PacketDumper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

hSend oSend;
hRecv oRecv;

int WSAAPI mSend(SOCKET socket, const char *buf, int len, int flagst)
{
	logPacket(buf, len, "C2S");
	return oSend(socket, buf, len, flagst);
}

int WSAAPI mRecv(SOCKET socket, char *buf, int len, int flagst)
{
	int ret = oRecv(socket, buf, len, flagst);
	logPacket(buf, ret, "S2C");
	return ret;
}

void WINAPI DllThread()
{
	oSend = (hSend)DetourFunction((PBYTE)send, (PBYTE)mSend);
	oRecv = (hRecv)DetourFunction((PBYTE)recv, (PBYTE)mRecv);
}

void logPacket(const char *buf, int len, std::string context)
{
	char sdate[9];
	char stime[9];
	std::ofstream myfile;
	_strdate_s(sdate);
	_strtime_s(stime);
	std::string filename = "..\\PacketLog\\";
	filename.append(stime);
	filename.append("_");
	filename.append(sdate);
	filename.append("_");
	filename.append(std::to_string(len));
	filename.append("_");
	filename.append(context);
	filename.append(".bin");
	for (size_t i = 0; i < filename.length(); ++i) {
		if (filename[i] == '/' || filename[i] == ':')
			filename[i] = '-';
	}
	myfile.open(filename.c_str());
	if (myfile)
	{
		myfile.write(buf, len);
		myfile.close();
	}
}
