#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN

#define ICON_MAIN 100
#define RAKBOT_VERSION "0.8"

#define OWNIP_ID 25

#pragma comment(linker, "/manifestdependency:\"type='win32' \
                        name='Microsoft.Windows.Common-Controls' \
                        version='6.0.0.0' processorArchitecture='*'\
                        publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <locale.h>
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <commctrl.h>
#include <shellapi.h>
#include <process.h>
#pragma comment(lib, "comctl32.lib")
#include <direct.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <shlguid.h>
#include <objbase.h>
#pragma comment (lib, "ole32.lib")
#include <time.h>

#include "ini.h"
#include "gen.h"

#define INI_STRING_LEN 512

struct stRunData
{
	char szNick[INI_STRING_LEN];
	char szPass[INI_STRING_LEN];
	char szIP[INI_STRING_LEN];
	char szAutoRegEnabled[INI_STRING_LEN];
	char szMail[INI_STRING_LEN];
	char szReferer[INI_STRING_LEN];
	char szSex[INI_STRING_LEN];
	char szKickReconnect[INI_STRING_LEN];
	char szCoordDist[INI_STRING_LEN];
	char szCoordHeight[INI_STRING_LEN];
	char szCoordDelay[INI_STRING_LEN];
	char szLoaderDelay[INI_STRING_LEN];
	char szLoaderLimit[INI_STRING_LEN];
	char szBanDialog[INI_STRING_LEN];
	char szPassDialog[INI_STRING_LEN];
	char szAdminReconnect[INI_STRING_LEN];
	char szAdminOnline[INI_STRING_LEN];
	char szAdminNear[INI_STRING_LEN];
	char szBusRoute[INI_STRING_LEN];
	char szLoaderEnabled[INI_STRING_LEN];
	char szAntiAfkDelay[INI_STRING_LEN];
	char szAntiAfkOffset[INI_STRING_LEN];
};
extern stRunData *pRunData;

extern HWND hwnd, hwndServer, hwndIP, hwndInfo, hwndNick, hwndPass, hwndConnect, hwndMail, hwndReferer,
	hwndSex, hwndLoaderLimit, hwndBanDialog, hwndPassDialog, hwndCoordDist, hwndCoordHeight, hwndCoordTime,
	hwndLoaderTime, hwndReconTime, hwndAdminReconTime, hwndAdminOnline, hwndAdminNear, hwndAntiAfkDelay, hwndAntiAfkOffset;
extern HINSTANCE g_hInst;
extern HFONT g_hfText;