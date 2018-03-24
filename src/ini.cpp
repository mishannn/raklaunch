#include "main.h"

char *GetRakBotPath(char *append) {
	static char path[MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path));

	*(strrchr(path, '\\') + 1) = 0;
	strcat(path, append);

	return path;
}

void LoadConfig(char *szSettingsPath) {
	char *szPath, *szAppName = NULL;

	if (!szSettingsPath || strlen(szSettingsPath) == 0) {
		CreateDirectory(GetRakBotPath("settings"), NULL);
		szPath = GetRakBotPath("settings\\settings.ini");
	} else {
		szPath = szSettingsPath;
	}

	FILE *fSettings = fopen(szPath, "r");
	if (fSettings == NULL)
		return;
	else
		fclose(fSettings);

	szAppName = "Account";
	GetPrivateProfileString(szAppName, "Server", "5.254.105.122:7777", pRunData->szIP, INI_STRING_LEN, szPath);
	SetWindowText(hwndIP, pRunData->szIP);
	SendMessage(hwndIP, EM_SETREADONLY, 0, 0);
	SendDlgItemMessage(hwnd, 100, CB_SETCURSEL, OWNIP_ID, 0);

	GetPrivateProfileString(szAppName, "NickName", "MishaN", pRunData->szNick, INI_STRING_LEN, szPath);
	SetWindowText(hwndNick, pRunData->szNick);

	GetPrivateProfileString(szAppName, "Password", "qwerty", pRunData->szPass, INI_STRING_LEN, szPath);
	SetWindowText(hwndPass, pRunData->szPass);

	szAppName = "AutoReg";
	GetPrivateProfileString(szAppName, "Enabled", "1", pRunData->szAutoRegEnabled, INI_STRING_LEN, szPath);

	if (pRunData->szAutoRegEnabled[0] == '1') {
		GetPrivateProfileString(szAppName, "Mail", "mail@mail.ru", pRunData->szMail, INI_STRING_LEN, szPath);
		SetWindowText(hwndMail, pRunData->szMail);

		GetPrivateProfileString(szAppName, "NickName", "CMishaN", pRunData->szReferer, INI_STRING_LEN, szPath);
		SetWindowText(hwndReferer, pRunData->szReferer);

		GetPrivateProfileString(szAppName, "Sex", "1", pRunData->szSex, INI_STRING_LEN, szPath);
		SendDlgItemMessage(hwnd, 104, CB_SETCURSEL, atoi(pRunData->szSex), 0);
	} else {
		SetWindowText(hwndMail, "");
		SetWindowText(hwndReferer, "");
	}

	szAppName = "CoordMaster";
	GetPrivateProfileString(szAppName, "Distance", "80.0", pRunData->szCoordDist, INI_STRING_LEN, szPath);
	SetWindowText(hwndCoordDist, pRunData->szCoordDist);

	GetPrivateProfileString(szAppName, "Height", "-40.0", pRunData->szCoordHeight, INI_STRING_LEN, szPath);
	SetWindowText(hwndCoordHeight, pRunData->szCoordHeight);

	GetPrivateProfileString(szAppName, "Delay", "1500", pRunData->szCoordDelay, INI_STRING_LEN, szPath);
	SetWindowText(hwndCoordTime, pRunData->szCoordDelay);

	szAppName = "Loader";
	GetPrivateProfileString(szAppName, "Delay", "10000", pRunData->szLoaderDelay, INI_STRING_LEN, szPath);
	SetWindowText(hwndLoaderTime, pRunData->szLoaderDelay);

	GetPrivateProfileString(szAppName, "Limit", "20", pRunData->szLoaderLimit, INI_STRING_LEN, szPath);
	SetWindowText(hwndLoaderLimit, pRunData->szLoaderLimit);

	GetPrivateProfileString(szAppName, "Enabled", "0", pRunData->szLoaderEnabled, INI_STRING_LEN, szPath);
	SendDlgItemMessage(hwnd, 113, CB_SETCURSEL, atoi(pRunData->szLoaderEnabled), 0);

	szAppName = "Settings";
	GetPrivateProfileString(szAppName, "KickReconnect", "15", pRunData->szKickReconnect, INI_STRING_LEN, szPath);
	SetWindowText(hwndReconTime, pRunData->szKickReconnect);

	GetPrivateProfileString(szAppName, "AdminReconnect", "300", pRunData->szAdminReconnect, INI_STRING_LEN, szPath);
	SetWindowText(hwndAdminReconTime, pRunData->szAdminReconnect);

	GetPrivateProfileString(szAppName, "PasswordDialog", "1", pRunData->szPassDialog, INI_STRING_LEN, szPath);
	SetWindowText(hwndPassDialog, pRunData->szPassDialog);

	GetPrivateProfileString(szAppName, "BanDialog", "58", pRunData->szBanDialog, INI_STRING_LEN, szPath);
	SetWindowText(hwndBanDialog, pRunData->szBanDialog);

	GetPrivateProfileString(szAppName, "AdminOnline", "0", pRunData->szAdminOnline, INI_STRING_LEN, szPath);
	SetWindowText(hwndAdminOnline, pRunData->szAdminOnline);

	GetPrivateProfileString(szAppName, "AdminNear", "0", pRunData->szAdminNear, INI_STRING_LEN, szPath);
	SetWindowText(hwndAdminNear, pRunData->szAdminNear);

	GetPrivateProfileString(szAppName, "BusRoute", "0", pRunData->szBusRoute, INI_STRING_LEN, szPath);
	SendDlgItemMessage(hwnd, 103, CB_SETCURSEL, atoi(pRunData->szBusRoute), 0);

	GetPrivateProfileString(szAppName, "AntiAfkDelay", "1", pRunData->szAntiAfkDelay, INI_STRING_LEN, szPath);
	SetWindowText(hwndAntiAfkDelay, pRunData->szAntiAfkDelay);

	GetPrivateProfileString(szAppName, "AntiAfkOffset", "0.001", pRunData->szAntiAfkOffset, INI_STRING_LEN, szPath);
	SetWindowText(hwndAntiAfkOffset, pRunData->szAntiAfkOffset);
}

void WriteConfig(char *szSettingsPath) {
	char *szPath, *szAppName = NULL;

	if (!szSettingsPath || strlen(szSettingsPath) == 0) {
		CreateDirectory(GetRakBotPath("settings"), NULL);
		szPath = GetRakBotPath("settings\\settings.ini");
	} else {
		szPath = szSettingsPath;
	}

	szAppName = "Account";
	WritePrivateProfileString(szAppName, "Server", pRunData->szIP, szPath);
	WritePrivateProfileString(szAppName, "NickName", pRunData->szNick, szPath);
	WritePrivateProfileString(szAppName, "Password", pRunData->szPass, szPath);

	szAppName = "AutoReg";
	WritePrivateProfileString(szAppName, "Enabled", pRunData->szAutoRegEnabled, szPath);
	WritePrivateProfileString(szAppName, "Mail", pRunData->szMail, szPath);
	WritePrivateProfileString(szAppName, "NickName", pRunData->szReferer, szPath);
	WritePrivateProfileString(szAppName, "Sex", pRunData->szSex, szPath);

	szAppName = "CoordMaster";
	WritePrivateProfileString(szAppName, "Distance", pRunData->szCoordDist, szPath);
	WritePrivateProfileString(szAppName, "Height", pRunData->szCoordHeight, szPath);
	WritePrivateProfileString(szAppName, "Delay", pRunData->szCoordDelay, szPath);

	szAppName = "Loader";
	WritePrivateProfileString(szAppName, "Delay", pRunData->szLoaderDelay, szPath);
	WritePrivateProfileString(szAppName, "Limit", pRunData->szLoaderLimit, szPath);
	WritePrivateProfileString(szAppName, "Enabled", pRunData->szLoaderEnabled, szPath);

	szAppName = "Settings";
	WritePrivateProfileString(szAppName, "KickReconnect", pRunData->szKickReconnect, szPath);
	WritePrivateProfileString(szAppName, "AdminReconnect", pRunData->szAdminReconnect, szPath);
	WritePrivateProfileString(szAppName, "PasswordDialog", pRunData->szPassDialog, szPath);
	WritePrivateProfileString(szAppName, "BanDialog", pRunData->szBanDialog, szPath);
	WritePrivateProfileString(szAppName, "AdminOnline", pRunData->szAdminOnline, szPath);
	WritePrivateProfileString(szAppName, "AdminNear", pRunData->szAdminNear, szPath);
	WritePrivateProfileString(szAppName, "BusRoute", pRunData->szBusRoute, szPath);
	WritePrivateProfileString(szAppName, "AntiAfkDelay", pRunData->szAntiAfkDelay, szPath);
	WritePrivateProfileString(szAppName, "AntiAfkOffset", pRunData->szAntiAfkOffset, szPath);
}