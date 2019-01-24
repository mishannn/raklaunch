#include "main.h"

#define IDC_SERVER 100
#define IDC_IP 101
#define IDC_CONNECT 102
#define IDC_BUSWORK 103
#define IDC_SEX 104
#define IDC_RESETSETS 107
#define IDC_SAVEACC 108
#define IDC_GENNICK 111
#define IDC_GENPASS 112
#define IDC_LOADER 113

stRunData *pRunData = new stRunData();

HWND hwnd, hwndServer, hwndIP, hwndInfo, hwndNick, hwndPass, hwndMail, hwndReferer, hwndSex, hwndLoaderLimit,
hwndBanDialog, hwndPassDialog, hwndCoordHeight, hwndCoordDist, hwndCoordTime, hwndLoaderTime, hwndReconTime,
hwndAdminReconTime, hwndAdminOnline, hwndAdminNear, hwndAntiAfkDelay, hwndAntiAfkOffset;
HINSTANCE g_hInst;
HFONT g_hfText;

char szPass[128], szNickName[25];

int RandomValue(int iMin, int iMax) {
	static int lastSRand = 0;
	if (GetTickCount() - lastSRand > 1000) {
		srand(time(NULL) + GetTickCount());
		lastSRand = GetTickCount();
	}
	int iTemp = (rand() * rand()) / (rand() ^ rand());
	srand(iTemp * GetTickCount());
	iTemp = rand() % GetTickCount();
	srand(iTemp);
	return iMin + (rand() % (iMax - iMin));
}

char *GenNick() {
	_snprintf(szNickName, sizeof(szNickName) - 1, "%s_%s",
		szFirstName[RandomValue(0, 57)], szLastName[RandomValue(0, 1368)]);
	return szNickName;
}

char *GenPass() {
	static const char alphanum[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	int iLen = RandomValue(6, 12);

	for (int i = 0; i < iLen; ++i)
		szPass[i] = alphanum[RandomValue(0, sizeof(alphanum) - 1)];

	szPass[iLen] = 0;

	return szPass;
}

void GetInputData() {
	pRunData->szAutoRegEnabled[0] = '1';

	SendMessage(hwndIP, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szIP);
	SendMessage(hwndNick, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szNick);
	SendMessage(hwndPass, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szPass);
	SendMessage(hwndMail, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szMail);

	if (pRunData->szMail[0] == NULL) {
		pRunData->szMail[0] = '0';
		pRunData->szAutoRegEnabled[0] = '0';
	}

	SendMessage(hwndReferer, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szReferer);

	if (pRunData->szReferer[0] == NULL) {
		pRunData->szReferer[0] = '0';
		pRunData->szAutoRegEnabled[0] = '0';
	}

	if (SendDlgItemMessage(hwnd, IDC_SEX, CB_GETCURSEL, 0, 0))
		pRunData->szSex[0] = '1';
	else
		pRunData->szSex[0] = '0';

	SendMessage(hwndReconTime, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szKickReconnect);
	SendMessage(hwndCoordDist, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szCoordDist);
	SendMessage(hwndCoordHeight, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szCoordHeight);
	SendMessage(hwndCoordTime, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szCoordDelay);
	SendMessage(hwndLoaderTime, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szLoaderDelay);
	SendMessage(hwndLoaderLimit, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szLoaderLimit);
	SendMessage(hwndBanDialog, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szBanDialog);
	SendMessage(hwndPassDialog, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szPassDialog);
	SendMessage(hwndAdminReconTime, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szAdminReconnect);
	SendMessage(hwndAdminOnline, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szAdminOnline);
	SendMessage(hwndAdminNear, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szAdminNear);
	SendMessage(hwndAntiAfkDelay, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szAntiAfkDelay);
	SendMessage(hwndAntiAfkOffset, WM_GETTEXT, (WPARAM)INI_STRING_LEN, (LPARAM)pRunData->szAntiAfkOffset);

	pRunData->szBusRoute[0] = (char)(48 + SendDlgItemMessage(hwnd, IDC_BUSWORK, CB_GETCURSEL, 0, 0));
	pRunData->szLoaderEnabled[0] = (char)(48 + SendDlgItemMessage(hwnd, IDC_LOADER, CB_GETCURSEL, 0, 0));
}

void RunExecutable() {
	char szWorkDir[MAX_PATH];
	GetModuleFileName(NULL, szWorkDir, sizeof(szWorkDir));
	*strrchr(szWorkDir, '\\') = 0;

	char szPath[MAX_PATH];
	strcpy(szPath, szWorkDir);
	strcat(szPath, "\\RakBot.exe");

	long hInst = (long)ShellExecute(hwnd, "open", szPath, NULL, szWorkDir, SW_SHOW);

	switch (hInst) {
		case NULL:
			MessageBox(hwnd, "Нехватка системных ресурсов", "Ошибка!", MB_ICONERROR);
			break;
		case ERROR_FILE_NOT_FOUND:
			MessageBox(hwnd, "Файл не найден. Проверьте наличие файла 'RakBot.exe' в папке лаунчера", "Ошибка!", MB_ICONERROR);
			break;
		case ERROR_PATH_NOT_FOUND:
			MessageBox(hwnd, "Неверный путь", "Ошибка!", MB_ICONERROR);
			break;
		case ERROR_BAD_FORMAT:
			MessageBox(hwnd, "Файл поврежден", "Ошибка!", MB_ICONERROR);
			break;
		case SE_ERR_ACCESSDENIED:
			MessageBox(hwnd, "Нет доступа", "Ошибка!", MB_ICONERROR);
			break;
		case SE_ERR_OOM:
			MessageBox(hwnd, "Нехватка памяти", "Ошибка!", MB_ICONERROR);
			break;
	}
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CREATE:
		{
			HWND hwndTemp;

			HICON hIcon = LoadIcon(g_hInst, IDI_APPLICATION);
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

			hwndTemp = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTONA, "Данные аккаунта",
				WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				10, 10, 270, 135, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndServer = CreateWindowEx(NULL, WC_COMBOBOXA, "Сервер",
				WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CB_SHOWDROPDOWN | WS_VSCROLL,
				20, 30, 250, 20, hWnd, (HMENU)IDC_SERVER, g_hInst, NULL);
			SendMessage(hwndServer, WM_SETFONT, (WPARAM)g_hfText, FALSE);
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 1 | Red Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 2 | Green Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 3 | Yellow Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 4 | Orange Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 5 | Blue Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 6 | White Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 7 | Silver Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 8 | Purple Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Advance RolePlay 9 | Chocolate Server");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Diamond RolePlay 1 | Emerald");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Diamond RolePlay 2 | Radiant");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Diamond RolePlay 3 | Trilliant");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Diamond RolePlay 4 | Crystal");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Diamond RolePlay 5 | Sapphire");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Diamond RolePlay 6 | Onyx");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Diamond RolePlay 7 | Amber");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Diamond RolePlay 8 | Quartz");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Samp-Rp.Ru | Server: 01 | Client: 0.3.7");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Samp-Rp.Ru | Server: 02 | Client: 0.3.7");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Samp-Rp.Ru | Server: 03 | Client: 0.3.7");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Samp-Rp.Ru | Server: 04 | Client: 0.3.7");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Samp-Rp.Ru | Server: 09 | Client: 0.3.7");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Samp-Rp.Ru | Server: Legacy | Client: 0.3.7");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Samp-Rp.Ru | Server: Reborn | Client: 0.3.7");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Samp-Rp.Ru | Server: Revolution | Client: 0.3.7");
			SendMessage(hwndServer, CB_ADDSTRING, 0, (LPARAM)"Свой IP");
			SendMessage(hwndServer, CB_SETCURSEL, 17, 0);

			hwndIP = CreateWindowEx(NULL, WC_EDITA, "5.254.105.122:7777",
				WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_READONLY | ES_AUTOHSCROLL,
				20, 60, 250, 20, hWnd, (HMENU)IDC_IP, g_hInst, NULL);
			SendMessage(hwndIP, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndNick = CreateWindowEx(NULL, WC_EDITA, GenNick(),
				WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				20, 85, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndNick, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_BUTTONA, "G",
				WS_VISIBLE | WS_TABSTOP | WS_CHILD,
				124, 84, 22, 22, hWnd, (HMENU)IDC_GENNICK, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndPass = CreateWindowEx(NULL, WC_EDITA, GenPass(),
				WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				150, 85, 95, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndPass, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_BUTTONA, "G",
				WS_VISIBLE | WS_TABSTOP | WS_CHILD,
				249, 84, 22, 22, hWnd, (HMENU)IDC_GENPASS, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_BUTTONA, "Подключиться",
				WS_VISIBLE | WS_TABSTOP | WS_CHILD,
				20, 110, 115, 25, hWnd, (HMENU)IDC_CONNECT, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_BUTTONA, "Сохранить аккаунт",
				WS_VISIBLE | WS_TABSTOP | WS_CHILD,
				140, 110, 130, 25, hWnd, (HMENU)IDC_SAVEACC, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTONA, "Информация сервера",
				WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				10, 150, 270, 125, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndInfo = CreateWindowEx(NULL, WC_STATIC, "Загрузка...",
				WS_VISIBLE | WS_CHILD | SS_CENTER,
				20, 170, 250, 100, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndInfo, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTONA, "Авторегистрация (Samp-Rp.Ru)",
				WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				290, 10, 315, 55, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndMail = CreateWindowEx(NULL, WC_EDITA, "mail@mail.ru",
				WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				300, 32, 130, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndMail, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndReferer = CreateWindowEx(NULL, WC_EDITA, "CMishaN",
				WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				435, 32, 115, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndReferer, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndSex = CreateWindowEx(NULL, WC_COMBOBOXA, "Пол",
				WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CB_SHOWDROPDOWN,
				555, 30, 40, 20, hWnd, (HMENU)IDC_SEX, g_hInst, NULL);
			SendMessage(hwndSex, WM_SETFONT, (WPARAM)g_hfText, FALSE);
			SendMessage(hwndSex, CB_ADDSTRING, 0, (LPARAM)"Ж");
			SendMessage(hwndSex, CB_ADDSTRING, 0, (LPARAM)"М");
			SendMessage(hwndSex, CB_SETCURSEL, 1, 0);

			hwndTemp = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTONA, "Настройки CoordMaster'а",
				WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				290, 70, 170, 100, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Дистанция:",
				WS_CHILD | WS_VISIBLE,
				300, 92, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Высота:",
				WS_CHILD | WS_VISIBLE,
				300, 117, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Задержка:",
				WS_CHILD | WS_VISIBLE,
				300, 142, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndCoordDist = CreateWindowEx(NULL, WC_EDITA, "80.0",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				380, 90, 60, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndCoordDist, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndCoordHeight = CreateWindowEx(NULL, WC_EDITA, "-40.0",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				380, 115, 60, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndCoordHeight, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndCoordTime = CreateWindowEx(NULL, WC_EDITA, "1500",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				380, 140, 60, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndCoordTime, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTONA, "Настройки грузчика",
				WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				470, 70, 135, 100, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Ожидание:",
				WS_CHILD | WS_VISIBLE,
				480, 102, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Лимит:",
				WS_CHILD | WS_VISIBLE,
				480, 127, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndLoaderTime = CreateWindowEx(NULL, WC_EDITA, "10000",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				555, 100, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndLoaderTime, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndLoaderLimit = CreateWindowEx(NULL, WC_EDITA, "20",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				555, 125, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndLoaderLimit, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTONA, "Настройки",
				WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				290, 175, 315, 131, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Перезаход(кик):",
				WS_CHILD | WS_VISIBLE,
				300, 197, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndReconTime = CreateWindowEx(NULL, WC_EDITA, "15",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				400, 195, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndReconTime, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Перезаход(адм.):",
				WS_CHILD | WS_VISIBLE,
				447, 197, 120, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndAdminReconTime = CreateWindowEx(NULL, WC_EDITA, "300",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				555, 195, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndAdminReconTime, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Диалог пароля:",
				WS_CHILD | WS_VISIBLE,
				300, 222, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndPassDialog = CreateWindowEx(NULL, WC_EDITA, "1",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				400, 220, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndPassDialog, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Диалог бана:",
				WS_CHILD | WS_VISIBLE,
				447, 222, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndBanDialog = CreateWindowEx(NULL, WC_EDITA, "58",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				555, 220, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndBanDialog, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Админ в сети:",
				WS_CHILD | WS_VISIBLE,
				300, 247, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndAdminOnline = CreateWindowEx(NULL, WC_EDITA, "0",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				400, 245, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndAdminOnline, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "Админ рядом:",
				WS_CHILD | WS_VISIBLE,
				447, 247, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndAdminNear = CreateWindowEx(NULL, WC_EDITA, "0",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				555, 245, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndAdminNear, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "АнтиAFK время:",
				WS_CHILD | WS_VISIBLE,
				300, 272, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndAntiAfkDelay = CreateWindowEx(NULL, WC_EDITA, "1",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				400, 270, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndAntiAfkDelay, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_STATIC, "АнтиAFK оффс.:",
				WS_CHILD | WS_VISIBLE,
				447, 272, 100, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndAntiAfkOffset = CreateWindowEx(NULL, WC_EDITA, "0.001",
				SS_CENTER | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_CHILD | ES_AUTOHSCROLL,
				555, 270, 40, 20, hWnd, NULL, g_hInst, NULL);
			SendMessage(hwndAntiAfkOffset, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_BUTTONA, "Сброс",
				WS_VISIBLE | WS_TABSTOP | WS_CHILD,
				10, 280, 60, 25, hWnd, (HMENU)IDC_RESETSETS, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);

			hwndTemp = CreateWindowEx(NULL, WC_COMBOBOXA, "Маршрут водителя автобуса",
				WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CB_SHOWDROPDOWN,
				75, 281, 100, 200, hWnd, (HMENU)IDC_BUSWORK, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"0. Нет маршрута");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"1. Городской ЛС");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"2. Городской СФ");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"3. Городской ЛВ");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"4. Междугородний ЛС-СФ");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"5. Междугородний ЛС-ЛВ");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"6. Междугородний СФ-ЛВ");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"7. Пригородный ЛС-ФК");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"8. Пригородный ЛС-ЗАВОДЫ");
			SendMessage(hwndTemp, CB_SETCURSEL, 0, 0);

			hwndTemp = CreateWindowEx(NULL, WC_COMBOBOXA, "Грузчик",
				WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CB_SHOWDROPDOWN,
				180, 281, 100, 200, hWnd, (HMENU)IDC_LOADER, g_hInst, NULL);
			SendMessage(hwndTemp, WM_SETFONT, (WPARAM)g_hfText, FALSE);
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"0. Грузчик отключен");
			SendMessage(hwndTemp, CB_ADDSTRING, 0, (LPARAM)"1. Грузчик включен");
			SendMessage(hwndTemp, CB_SETCURSEL, 0, 0);
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam)) {
				case IDC_RESETSETS:
				{
					DeleteFile(GetRakBotPath("settings\\settings.ini"));
					MessageBox(NULL, "Настройки сброшены. Перезайдите в программу.", "Готово!", MB_ICONASTERISK);
					PostQuitMessage(0);
				}
				break;

				case IDC_SAVEACC:
				{
					GetInputData();

					HRESULT hResult;
					IShellLink *pShellLink = NULL;
					IPersistFile *pPersistFile = NULL;

					char szAddress[26];
					strcpy(szAddress, pRunData->szIP);
					*strchr(szAddress, ':') = ';';

					CoInitialize(NULL);
					hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&pShellLink);
					CoUninitialize();

					if (FAILED(hResult)) {
						if (pPersistFile)
							pPersistFile->Release();
						if (pShellLink)
							pShellLink->Release();
					}

					hResult = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPersistFile);

					if (FAILED(hResult)) {
						if (pPersistFile)
							pPersistFile->Release();
						if (pShellLink)
							pShellLink->Release();
					}

					char szLauncherPath[MAX_PATH];
					GetModuleFileName(NULL, szLauncherPath, sizeof(szLauncherPath));
					hResult = pShellLink->SetPath(szLauncherPath);

					if (FAILED(hResult)) {
						if (pPersistFile)
							pPersistFile->Release();
						if (pShellLink)
							pShellLink->Release();
					}

					char szAccountsPath[MAX_PATH];
					GetModuleFileName(NULL, (char *)&szAccountsPath, sizeof(szAccountsPath));
					strcpy(strrchr(szAccountsPath, '\\') + 1, "accounts");
					CreateDirectory(szAccountsPath, NULL);

					char szAccountsDataPath[MAX_PATH];
					strcpy(szAccountsDataPath, szAccountsPath);
					strcat(szAccountsDataPath, "\\data");
					CreateDirectory(szAccountsDataPath, NULL);
					SetFileAttributes(szAccountsDataPath, FILE_ATTRIBUTE_HIDDEN);

					char szAccountData[MAX_PATH];
					_snprintf(szAccountData, sizeof(szAccountData), "%s\\%s@%s.ini", szAccountsDataPath, pRunData->szNick, szAddress);
					WriteConfig(szAccountData);

					hResult = pShellLink->SetArguments(szAccountData);

					if (FAILED(hResult)) {
						if (pPersistFile)
							pPersistFile->Release();
						if (pShellLink)
							pShellLink->Release();
					}

					char szAccountLink[MAX_PATH];
					_snprintf(szAccountLink, sizeof(szAccountLink), "%s\\%s@%s.lnk", szAccountsPath, pRunData->szNick, szAddress);

					wchar_t wszAccountLink[MAX_PATH];
					MultiByteToWideChar(CP_ACP, 0, szAccountLink, -1, wszAccountLink, MAX_PATH);

					hResult = pPersistFile->Save(wszAccountLink, TRUE);

					if (FAILED(hResult)) {
						if (pPersistFile)
							pPersistFile->Release();
						if (pShellLink)
							pShellLink->Release();
					}

					MessageBox(hwnd, "Аккаунт сохранен", "Готово", MB_ICONASTERISK);
				}
				break;

				case IDC_GENNICK:
					SetWindowText(hwndNick, GenNick());
					break;

				case IDC_GENPASS:
					SetWindowText(hwndPass, GenPass());
					break;

				case IDC_SERVER:
				{
					if (HIWORD(wParam) == CBN_SELENDOK) {
						switch (SendDlgItemMessage(hwnd, IDC_SERVER, CB_GETCURSEL, 0, 0)) {
							case 0:
								SetWindowText(hwndIP, "5.254.104.131:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 1:
								SetWindowText(hwndIP, "5.254.104.132:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 2:
								SetWindowText(hwndIP, "5.254.104.133:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 3:
								SetWindowText(hwndIP, "5.254.104.134:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 4:
								SetWindowText(hwndIP, "5.254.104.135:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 5:
								SetWindowText(hwndIP, "5.254.104.136:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 6:
								SetWindowText(hwndIP, "5.254.104.137:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 7:
								SetWindowText(hwndIP, "5.254.104.138:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 8:
								SetWindowText(hwndIP, "5.254.104.139:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 9:
								SetWindowText(hwndIP, "194.61.44.61:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 10:
								SetWindowText(hwndIP, "5.254.123.3:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 11:
								SetWindowText(hwndIP, "5.254.123.4:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 12:
								SetWindowText(hwndIP, "5.254.123.5:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 13:
								SetWindowText(hwndIP, "5.254.123.6:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 14:
								SetWindowText(hwndIP, "5.254.105.202:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 15:
								SetWindowText(hwndIP, "5.254.105.203:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 16:
								SetWindowText(hwndIP, "5.254.105.204:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 17:
								SetWindowText(hwndIP, "185.169.134.19:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 18:
								SetWindowText(hwndIP, "185.169.134.20:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 19:
								SetWindowText(hwndIP, "185.169.134.21:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 20:
								SetWindowText(hwndIP, "185.169.134.22:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 21:
								SetWindowText(hwndIP, "185.169.134.22:7771");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 22:
								SetWindowText(hwndIP, "185.169.134.22:7773");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 23:
								SetWindowText(hwndIP, "185.169.134.34:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case 24:
								SetWindowText(hwndIP, "185.169.134.11:7777");
								SendMessage(hwndIP, EM_SETREADONLY, 1, 0);
								break;

							case OWNIP_ID:
								SendMessage(hwndIP, EM_SETREADONLY, 0, 0);
								break;
						}
					}
				}
				break;

				case IDC_CONNECT:
				{
					if (GetWindowTextLength(hwndNick) < 3 || GetWindowTextLength(hwndNick) > 20) {
						MessageBox(hwnd, "Неверный формат ника!", "Ошибка!", MB_ICONERROR);
						break;
					}
					if (GetWindowTextLength(hwndPass) == NULL) {
						MessageBox(hwnd, "Неверный формат пароля!", "Ошибка!", MB_ICONERROR);
						break;
					}

					GetInputData();

					WriteConfig();
					RunExecutable();
				}
				break;
			}
		}
		break;

		case WM_GETMINMAXINFO:
		{
			int windowWidth = 630;
			int windowHeight = 350;

			((LPMINMAXINFO)lParam)->ptMinTrackSize.x = windowWidth;
			((LPMINMAXINFO)lParam)->ptMinTrackSize.y = windowHeight;
			((LPMINMAXINFO)lParam)->ptMaxTrackSize.x = windowWidth;
			((LPMINMAXINFO)lParam)->ptMaxTrackSize.y = windowHeight;
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		default:
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void WindowThread(void *) {
	WNDCLASSEX wcex;
	HDC hdcScreen;
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = g_hInst;
	wcex.lpszClassName = "RakLaunch for RakBot " RAKBOT_VERSION;
	wcex.lpfnWndProc = MainWindowProc;
	wcex.hCursor = LoadCursor(g_hInst, IDC_ARROW);
	wcex.hIcon = LoadIcon(g_hInst, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	if (!RegisterClassEx(&wcex))
		return;

	hdcScreen = GetDC(HWND_DESKTOP);
	g_hfText = CreateFont(15,
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Tahoma");
	ReleaseDC(HWND_DESKTOP, hdcScreen);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, wcex.lpszClassName, wcex.lpszClassName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
		HWND_DESKTOP, NULL, g_hInst, NULL);

	if (hwnd) {
		ShowWindow(hwnd, 1);
		UpdateWindow(hwnd);
		while (GetMessage(&msg, NULL, 0, 0)) {
			if (!IsDialogMessage(hwnd, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	DeleteObject(g_hfText);
	delete pRunData;
	ExitProcess(0);
}

void SetUpWindow(HINSTANCE hInstance) {
	g_hInst = hInstance;
	_beginthread(WindowThread, 0, NULL);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	char *locale = "RUSSIAN";
	setlocale(LC_COLLATE, locale);
	setlocale(LC_MONETARY, locale);
	setlocale(LC_TIME, locale);
	setlocale(LC_CTYPE, locale);

	if (lpCmdLine && strlen(lpCmdLine) != 0) {
		LoadConfig(lpCmdLine);
		WriteConfig();
		RunExecutable();
		return 1;
	}

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES | ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_USEREX_CLASSES;
	InitCommonControlsEx(&icex);

	SetUpWindow(hInstance);
	Sleep(300);

	LoadConfig();

	WSAData wsa;
	WSAStartup(1, &wsa);
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	while (true) {
		Sleep(200);

		WORD sPort;
		char szIP[22], szRequest[15], szResponse[1024], szText[1024];
		SendMessage(hwndIP, WM_GETTEXT, (WPARAM)22, (LPARAM)szIP);

		char *pch = szIP;
		while (*pch) {
			if (*pch == ':') {
				*pch = '\0';
				sPort = atoi(pch + 1);
			}
			pch++;
		}

		DWORD arg = 1;
		ioctlsocket(s, FIONBIO, &arg);

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(szIP);
		*(DWORD *)&szRequest[4] = addr.sin_addr.s_addr;
		addr.sin_port = htons(sPort);
		*(WORD *)&szRequest[8] = sPort;

		memcpy(szRequest, "SAMP\x00\x00\x00\x00\x00\x00i", sizeof(szRequest));
		sendto(s, szRequest, 11, 0, (SOCKADDR *)&addr, sizeof(addr));
		ZeroMemory(szResponse, 1024);

		if (recvfrom(s, szResponse, 1024, 0, NULL, 0) < 11)
			continue;

		BYTE byteCount = 11;
		BYTE bytePassword = *(BYTE *)&szResponse[byteCount];
		byteCount++;
		WORD wOnlinePlayers = *(WORD *)&szResponse[byteCount];
		byteCount += 2;
		WORD wMaxPlayers = *(WORD *)&szResponse[byteCount];
		byteCount += 2;
		int iHostNameLen = *(int *)&szResponse[byteCount];
		char *szHostName = (char *)calloc(iHostNameLen + 1, 1);
		byteCount += 4;
		strncpy(szHostName, &szResponse[byteCount], iHostNameLen);
		byteCount += iHostNameLen;
		int iGameModeLen = *(int *)&szResponse[byteCount];
		char *szGameMode = (char *)calloc(iGameModeLen + 1, 1);
		byteCount += 4;
		strncpy(szGameMode, &szResponse[byteCount], iGameModeLen);
		byteCount += iGameModeLen;
		int iMapNameLen = *(int *)&szResponse[byteCount];
		char *szMapName = (char *)calloc(iMapNameLen + 1, 1);
		byteCount += 4;
		strncpy(szMapName, &szResponse[byteCount], iMapNameLen);

		_snprintf(szText, sizeof(szText), "Название: %s\nНазвание мода: %s\nКарта: %s\nИгроки: %d/%d\nПароль: %s",
			szHostName, szGameMode, szMapName, wOnlinePlayers, wMaxPlayers, bytePassword == 1 ? "есть" : "нет");

		SetWindowText(hwndInfo, szText);

		free(szHostName);
		free(szGameMode);
		free(szMapName);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}