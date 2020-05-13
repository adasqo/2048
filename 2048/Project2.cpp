// Project2.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include "framework.h"
#include "Project2.h"
#include <dwmapi.h>
#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                MyRegisterClass2(HINSTANCE hInstance);
ATOM                MyRegisterClassChild(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProc2(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK    WndProcChild(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Draw();
void calculatePosition(int);
void addField();
void resetGame();
void DisplayBitmap(HDC hdc, COLORREF color, LPCWSTR mess, RECT rc, int trans);
void drawText(HDC hdc, HFONT font, LPCWSTR message, RECT rc);
void readFromFile();
void saveToFile();

HWND hWnd1;
HWND hWnd2;
HWND hWndChild[4][4];
HWND hWndChild2[4][4];
HWND hWndPoints;
HWND hWndPoints2;
HWND hWndGameWon;
HBITMAP hbitmap;

HBRUSH br1 = CreateSolidBrush(RGB(204, 192, 174));
HBRUSH br2 = CreateSolidBrush(RGB(238, 228, 198));
HBRUSH br4 = CreateSolidBrush(RGB(239, 225, 218));
HBRUSH br8 = CreateSolidBrush(RGB(243, 179, 124));
HBRUSH br16 = CreateSolidBrush(RGB(246, 153, 100));
HBRUSH br32 = CreateSolidBrush(RGB(246, 125, 98));
HBRUSH br64 = CreateSolidBrush(RGB(247, 93, 60));
HBRUSH br128 = CreateSolidBrush(RGB(237, 206, 116));
HBRUSH br256 = CreateSolidBrush(RGB(239, 204, 98));
HBRUSH br512 = CreateSolidBrush(RGB(243, 201, 85));
HBRUSH br1024 = CreateSolidBrush(RGB(238, 200, 72));
HBRUSH br2048 = CreateSolidBrush(RGB(239, 192, 47));
HBRUSH Colors[12] = { br1, br2, br4, br8, br16, br32, br64, br128, br256, br512, br1024, br2048};

HPEN p1 = CreatePen(PS_SOLID, 0, RGB(204, 192, 174));
HPEN p2 = CreatePen(PS_SOLID, 0, RGB(238, 228, 198));
HPEN p4 = CreatePen(PS_SOLID, 0, RGB(239, 225, 218));
HPEN p8 = CreatePen(PS_SOLID, 0, RGB(243, 179, 124));
HPEN p16 = CreatePen(PS_SOLID, 0, RGB(246, 153, 100));
HPEN p32 = CreatePen(PS_SOLID, 0, RGB(246, 125, 98));
HPEN p64 = CreatePen(PS_SOLID, 0, RGB(247, 93, 60));
HPEN p128 = CreatePen(PS_SOLID, 0, RGB(237, 206, 116));
HPEN p256 = CreatePen(PS_SOLID, 0, RGB(239, 204, 98));
HPEN p512 = CreatePen(PS_SOLID, 0, RGB(243, 201, 85));
HPEN p1024 = CreatePen(PS_SOLID, 0, RGB(238, 200, 72));
HPEN p2048 = CreatePen(PS_SOLID, 0, RGB(239, 192, 47));
HPEN ColorsPen[12] = { p1, p2, p4, p8, p16, p32, p64, p128, p256, p512, p1024, p2048 };

HANDLE hFile;

int xSize = 305;
int ySize = 420;
int offset = 10;
int length = 60;

int direction;
bool move;

int score = 0;
int limit = 2048;
bool endGameWon = false;
bool endGameOver = false;
bool endGamePerm = false;
int menuIdent[4] = {32772, 32773, 32774, 32775};
bool ifRead = false;

typedef struct
{
	int x;
	int y;
	int val;
	int color;
}Field;

Field fields[4][4], fields2[4][4];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.


	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJECT2, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);
	MyRegisterClass2(hInstance);
	MyRegisterClassChild(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT2));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(250, 247, 238)));
	wcex.lpszMenuName = MAKEINTRESOURCE(130);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}
ATOM MyRegisterClass2(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc2;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(250, 247, 238)));
	wcex.lpszMenuName = MAKEINTRESOURCE(130);
	wcex.lpszClassName = L"szWindowClass2";
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}
ATOM MyRegisterClassChild(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(250, 247, 238)));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Child";
	wcex.hIconSm = NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	RECT rect = { 0, 0, xSize, ySize };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd1 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX | WS_CLIPCHILDREN,
		100, 100, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

	hWnd2 = CreateWindowW(L"szWindowClass2", szTitle, NULL,
		GetSystemMetrics(SM_CXSCREEN) - xSize - 100, GetSystemMetrics(SM_CYSCREEN) - ySize - 100, rect.right - rect.left, rect.bottom - rect.top, hWnd1, nullptr, hInstance, nullptr);

	int sumx = 0, sumy = 70;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (!ifRead)
			{
				fields[i][j].val = 1;
				fields[i][j].color = 0;
			}
			fields[i][j].x = offset + sumx;
			fields[i][j].y = offset + sumy;

			fields2[i][j].x = offset + sumx;
			fields2[i][j].y = offset + sumy;

			hWndChild[i][j] = CreateWindowW(L"Child", NULL, WS_VISIBLE | WS_CHILD, fields[i][j].x, fields[i][j].y, length, length, hWnd1, nullptr, hInstance, nullptr);
			hWndChild2[i][j] = CreateWindowW(L"Child", NULL, WS_VISIBLE | WS_CHILD, fields2[i][j].x, fields2[i][j].y, length, length, hWnd2, nullptr, hInstance, nullptr);

			sumx += offset + length;
		}
		sumx = 0;
		sumy += offset + length;
	}

	/*
	for(int i=0; i<4; i++)
		for (int j = 0; j < 4; j++)
		{
			ShowWindow(hWndChild[i][j], nCmdShow);
			UpdateWindow(hWndChild[i][j]);
			ShowWindow(hWndChild2[i][j], nCmdShow);
			UpdateWindow(hWndChild2[i][j]);
		}

		*/
	hWndPoints = CreateWindowW(L"Child", NULL, WS_VISIBLE | WS_CHILD | WS_EX_LAYERED, offset, offset, 4 * length + 3 * offset, length, hWnd1, nullptr, hInstance, nullptr);
	hWndPoints2 = CreateWindowW(L"Child", NULL, WS_VISIBLE | WS_CHILD | WS_EX_LAYERED, offset, offset, 4 * length + 3 * offset, length, hWnd2, nullptr, hInstance, nullptr);

	TCHAR s[256];
	_stprintf_s(s, 256, _T("2048"));
	SetWindowText(hWnd1, s);
	SetWindowText(hWnd2, s);

	if (!hWnd1)
	{
		return FALSE;
	}
	std::srand(0);

	//First field
	if (!ifRead)
	{
		int row = rand() % 4, col = rand() % 4;
		fields[row][col].color = 1;
		fields[row][col].val = 2;
	}
;
	ShowWindow(hWnd1, nCmdShow);
	UpdateWindow(hWnd1);

	ShowWindow(hWnd2, nCmdShow);
	UpdateWindow(hWnd2);


	readFromFile();

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

void moveWindows(HWND hWnd1, HWND hWnd2)
{
	RECT rc1, rc2;
	GetWindowRect(hWnd1, &rc1);
	GetWindowRect(hWnd2, &rc2);

	int x0 = GetSystemMetrics(SM_CXSCREEN) / 2 - xSize / 2;
	int y0 = GetSystemMetrics(SM_CYSCREEN) / 2 - ySize / 2;

	int vx = rc1.left - x0;
	int vy = rc1.top - y0;
	MoveWindow(hWnd2, x0 - vx, y0 - vy, length, length, TRUE);
}
void makeTransparent(HWND hWnd1, HWND hWnd2)
{
	RECT rc1, rc2;
	GetWindowRect(hWnd1, &rc1);
	GetWindowRect(hWnd2, &rc2);

	int sizex = rc1.right - rc1.left - 15;
	int sizey = rc1.bottom - rc1.top - 5;

	if ((abs(rc2.left - rc1.left) <= sizex) && (abs(rc2.top - rc1.top) <= sizey))
	{
		SetWindowLong(hWnd1, GWL_EXSTYLE, GetWindowLong(hWnd1, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd1, 0, (255 * 100) / 100, LWA_ALPHA);

		SetWindowLong(hWnd2, GWL_EXSTYLE, GetWindowLong(hWnd2, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd2, 0, (255 * 50) / 100, LWA_ALPHA);
	}
	else
	{
		SetWindowLong(hWnd1, GWL_EXSTYLE, GetWindowLong(hWnd1, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd1, 0, (255 * 100) / 100, LWA_ALPHA);

		SetWindowLong(hWnd2, GWL_EXSTYLE, GetWindowLong(hWnd2, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd2, 0, (255 * 100) / 100, LWA_ALPHA);
	}
}

//home part
void Draw()
{
	RECT rc;
	HDC hdc1, hdc2;
	COLORREF colorFont, colorBck;
	LOGBRUSH br;

	HFONT font = CreateFont(25, 10, 0, 0, 700, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Calabri");

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			hdc1 = GetDC(hWndChild[i][j]);
			hdc2 = GetDC(hWndChild2[i][j]);
			rc = { 0, 0, 60, 60 };
			
			SelectObject(hdc1, Colors[fields[i][j].color]);
			SelectObject(hdc1, ColorsPen[fields[i][j].color]);
			SelectObject(hdc2, Colors[fields[i][j].color]);
			SelectObject(hdc2, ColorsPen[fields[i][j].color]);
			FillRect(hdc1, &rc, CreateSolidBrush(RGB(250, 247, 238)));
			FillRect(hdc2, &rc, CreateSolidBrush(RGB(250, 247, 238)));
			RoundRect(hdc1, rc.left, rc.top, rc.right, rc.bottom, 20, 20);
			RoundRect(hdc2, rc.left, rc.top, rc.right, rc.bottom, 20, 20);

			if (fields[i][j].val != 1)
			{
				std::string str = std::to_string(fields[i][j].val);
				std::wstring stemp = std::wstring(str.begin(), str.end());
				LPCWSTR mess = stemp.c_str();
				drawText(hdc1, font, mess, rc);
				DeleteDC(hdc1);

				drawText(hdc2, font, mess, rc);
				DeleteDC(hdc2);
			}
		}
	}
	std::string str = std::to_string(score);
	std::wstring stemp = std::wstring(str.begin(), str.end());
	LPCWSTR mess_score = stemp.c_str();
	rc = { 0, 0, 270, 60};
	
	HDC hdcPoints = GetDC(hWndPoints);
	SelectObject(hdcPoints, Colors[0]);
	SelectObject(hdcPoints, ColorsPen[0]);
	(HFONT)SelectObject(hdcPoints, font);
	FillRect(hdcPoints, &rc, CreateSolidBrush(RGB(250, 247, 238)));
	RoundRect(hdcPoints, rc.left, rc.top, rc.right, rc.bottom, 20, 20);
	drawText(hdcPoints, font, mess_score, rc);
	DeleteDC(hdcPoints);

	HDC hdcPoints2 = GetDC(hWndPoints2);
	SelectObject(hdcPoints2, Colors[0]);
	SelectObject(hdcPoints2, ColorsPen[0]);
	(HFONT)SelectObject(hdcPoints2, font);
	FillRect(hdcPoints2, &rc, CreateSolidBrush(RGB(250, 247, 238)));
	RoundRect(hdcPoints2, rc.left, rc.top, rc.right, rc.bottom, 20, 20);
	drawText(hdcPoints2, font, mess_score, rc);
	DeleteDC(hdcPoints2);

	rc = { 0, 0, xSize, ySize };

	if (endGameWon)
	{
		LONG lStyle = GetWindowLong(hWnd1, GWL_STYLE);
		lStyle &= ~(WS_CLIPCHILDREN);
		SetWindowLong(hWnd1, GWL_STYLE, lStyle);

		HDC hdcGameWon = GetDC(hWnd1);
		HDC hdcGameWon2 = GetDC(hWnd2);

		std::string str = "WIN!";
		std::wstring stemp = std::wstring(str.begin(), str.end());
		LPCWSTR mess_win = stemp.c_str();
		
		rc = {0, 0, xSize, ySize - 50};
		DisplayBitmap(hdcGameWon, RGB(0, 255, 0), mess_win, rc, 50);
		DisplayBitmap(hdcGameWon2, RGB(0, 255, 0), mess_win, rc, 50);

		endGamePerm = true;
		
		DeleteDC(hdcGameWon);
		DeleteDC(hdcGameWon2);
		return;
	}
	
	if (endGameOver)
	{
		LONG lStyle = GetWindowLong(hWnd1, GWL_STYLE);
		lStyle &= ~(WS_CLIPCHILDREN);
		SetWindowLong(hWnd1, GWL_STYLE, lStyle);

		HDC hdcGameLost = GetDC(hWnd1);
		HDC hdcGameLost2 = GetDC(hWnd2);

		std::string str = "GAME OVER!";
		std::wstring stemp = std::wstring(str.begin(), str.end());
		LPCWSTR mess_lose = stemp.c_str();

		rc = { 0, 0, xSize, ySize - 50 };
		DisplayBitmap(hdcGameLost, RGB(0, 0, 255), mess_lose, rc, 50);
		DisplayBitmap(hdcGameLost2, RGB(0, 0, 255), mess_lose, rc, 50);

		endGamePerm = true;
		DeleteDC(hdcGameLost);
		DeleteDC(hdcGameLost2);

		return;
	}
}
void calculatePosition(int direction)
{
	int j, it, iter, it1, prev_pos, sum = 0;
	int scores[4] = {0, 0, 0, 0};
	int prev = 0;
	bool flag = false, flag2 = false, flag3 = true;
	//GAME OVER check
	for (int i = 0; i < 4 && !flag; i++)
		for (int j = 0; j < 4; j++)
		{
			if (fields[i][j].val == 1)
			{
				flag = true;
				break;
			}
			if (fields[i][j].val == prev)
				flag2 = true;

			prev = fields[i][j].val;
		}
	if (!flag && !flag2)
		flag3 = true;
	prev = 0;
	flag = flag2 = false;
	for (int i = 0; i < 4 && !flag; i++)
		for (int j = 0; j < 4; j++)
		{
			if (fields[j][i].val == 1)
			{
				flag = true;
				break;
			}
			if (fields[j][i].val == prev)
				flag2 = true;

			prev = fields[j][i].val;
		}
	if (!flag && !flag2 && flag3)
	{
		endGameOver = true;
		return;
	}

	switch (direction)
	{
		case 0:
		{
			for (int i = 0; i < 4; i++)
			{
				j = 0;
				while (j < 3)
				{
					it1 = j + 1;
					while (it1 < 4)
					{
						
							if (fields[i][j].val != fields[i][it1].val && (fields[i][j].val != 1 && fields[i][it1].val != 1))
							{
								it1++;
								break;
							}
						if (fields[i][j].val == fields[i][it1].val && fields[i][j].val != 1)
						{
							fields[i][it1].val *= 2;
							fields[i][it1].color++;
							fields[i][j].val = 1;
							fields[i][j].color = 0;	
							
							scores[0] = fields[i][it1].val;

							j += 2;
							it1 += 2;
						}	
						else
							it1++;
					}
					j++;
				}
				j = 0;
				iter = 0;
				while (j + iter < 3)
				{
					if (fields[i][j].val == 1)
					{
						it = j;
						while (it < 3 - iter)
						{
							prev_pos = fields[i][it].color;

							fields[i][it].val = fields[i][it + 1].val;
							fields[i][it].color = fields[i][it + 1].color;

							if (fields[i][it].color != prev_pos)
								move = true;

							it++;
						}
						fields[i][3 - iter].val = 1;
						fields[i][3 - iter].color = 0;
						iter++;
					}
					else
						j++;
				}
				for (int k = 0; k < 4; k++)
					if (fields[i][k].val >= limit)
						endGameWon = true;
			}
		}break;
		case 2:
		{
			for (int i = 0; i < 4; i++)
			{
				j = 3;
				while (j > 0)
				{
					it1 = j - 1;
					while (it1 >= 0)
					{
							if (fields[i][j].val != fields[i][it1].val && (fields[i][j].val != 1 && fields[i][it1].val != 1))
							{
								it1--;
								break;
							}
						if (fields[i][j].val == fields[i][it1].val && fields[i][j].val != 1)
						{
							fields[i][it1].val *= 2;
							fields[i][it1].color++;
							fields[i][j].val = 1;
							fields[i][j].color = 0;

							scores[1] = fields[i][it1].val;

							it1 -= 2;
							j -= 2;
						}
						else
							it1--;
					}
					j--;
				}
				j = 0;
				iter = 0;
				while (j + iter < 3)
				{
					if (fields[i][3 - j].val == 1)
					{
						it = 3 - j;
						while (it > iter)
						{
							prev_pos = fields[i][it].color;

							fields[i][it].val = fields[i][it - 1].val;
							fields[i][it].color = fields[i][it - 1].color;

							if (fields[i][it].color != prev_pos)
								move = true;

							it--;
						}
						fields[i][iter].val = 1;
						fields[i][iter].color = 0;
						iter++;
					}
					else
						j++;
				}
				for (int k = 0; k < 4; k++)
					if (fields[i][k].val >= limit)
						endGameWon = true;
			}
		}break;
		case 1:
		{
			for (int i = 0; i < 4; i++)
			{
				j = 3;
				while (j > 0)
				{
					it1 = j - 1;
					while (it1 >= 0)
					{
						
							if (fields[j][i].val != fields[it1][i].val && (fields[j][i].val != 1 && fields[it1][i].val != 1))
							{
								it1--;
								break;
							}
						if (fields[j][i].val == fields[it1][i].val && fields[j][i].val != 1)
						{
							fields[it1][i].val *= 2;
							fields[it1][i].color++;
							fields[j][i].val = 1;
							fields[j][i].color = 0;

							scores[2] = fields[it1][i].val;

							if (fields[it1][i].val >= limit)
								endGameWon = true;

							j -= 2;
							it1 -= 2;
							move = 2;
						}
						else
							it1--;
					}
					j--;
				}
				j = 0;
				iter = 0;
				while (j + iter < 3)
				{
					if (fields[3 - j][i].val == 1)
					{
						it = 3 - j;
						while (it > iter)
						{
							prev_pos = fields[it][i].color;

							fields[it][i].val = fields[it - 1][i].val;
							fields[it][i].color = fields[it - 1][i].color;

							if (fields[it][i].color != prev_pos)
								move = true;


							it--;
						}
						fields[iter][i].val = 1;
						fields[iter][i].color = 0;
						iter++;
					}
					else
						j++;
				}
				for (int k = 0; k < 4; k++)
					if (fields[i][k].val >= limit)
						endGameWon = true;
			}
		}break;
		case 3:
		{
			for (int i = 0; i < 4; i++)
			{
				j = 0;
				while (j < 3)
				{
					it1 = j + 1;
					while (it1 < 4)
					{
							if (fields[j][i].val != fields[it1][i].val && (fields[j][i].val != 1 && fields[it1][i].val != 1))
							{
								it1++;
								break;
							}
						if (fields[j][i].val == fields[it1][i].val && fields[j][i].val != 1)
						{
							fields[it1][i].val *= 2;
							fields[it1][i].color++;
							fields[j][i].val = 1;
							fields[j][i].color = 0;

							scores[3] = fields[it1][i].val;

							j += 2;
							it1 += 2;
							move = true;
						}
						else
							it1++;
					}
					j++;
				}
				j = 0;
				iter = 0;
				while (j + iter < 3)
				{
					if (fields[j][i].val == 1)
					{
						it = j;
						while (it < 3 - iter)
						{
							prev_pos = fields[it][i].color;

							fields[it][i].val = fields[it + 1][i].val;
							fields[it][i].color = fields[it + 1][i].color;
							
							if (fields[it][i].color != prev_pos)
								move = true;

							it++;
						}
						fields[3 - iter][i].val = 1;
						fields[3 - iter][i].color = 0;
						iter++;
					}
					else
						j++;
				}
				for (int k = 0; k < 4; k++)
					if (fields[i][k].val >= limit)
						endGameWon = true;
			}
		}break;
	}
	for (int k = 0; k < 4; k++)
		score += scores[k];
}
void addField()
{
	int r1 = rand() % 4, r2 = rand() % 4;
	while (fields[r1][r2].val != 1)
	{
		int t = rand() % 2;
		if (t == 0)
			r1 = (r1 + 1) % 4;
		else
			r2 = (r2 + 1) % 4;
	}
	fields[r1][r2].val = 2;
	fields[r1][r2].color = 1;

	move = false;
}
void resetGame()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			fields[i][j].val = 1;
			fields[i][j].color = 0;
			score = 0;
		}
	int row = rand() % 4, col = rand() % 4;
	fields[row][col].color = 1;
	fields[row][col].val = 2;

	endGameWon = false;
	endGameOver = false;
	endGamePerm = false;
	ifRead = false;

	LONG lStyle = GetWindowLong(hWnd1, GWL_STYLE);
	lStyle |= WS_CLIPCHILDREN;
	SetWindowLong(hWnd1, GWL_STYLE, lStyle);

	RECT rc = { 0, 0, xSize, ySize };

	std::string str = "";
	std::wstring stemp = std::wstring(str.begin(), str.end());
	LPCWSTR mess = stemp.c_str();
	DisplayBitmap(GetDC(hWnd1), RGB(238, 247, 250), NULL, rc, 100);
	DisplayBitmap(GetDC(hWnd2), RGB(238, 247, 250), NULL, rc, 100);
	Draw();
}
void setMenu(int state, HWND hwnd)
{
	HMENU hMenu = GetMenu(hwnd);
	
	for (int i = 0; i < 4; i++)
		if (i == state)
			CheckMenuItem(hMenu, menuIdent[i], MF_BYCOMMAND ^ MF_CHECKED);
		else
			CheckMenuItem(hMenu, menuIdent[i], MF_BYCOMMAND ^ MF_UNCHECKED);
	DeleteObject(hMenu);
}
void drawText(HDC hdc, HFONT font, LPCWSTR message, RECT rc)
{
	COLORREF colorFont;
	colorFont = SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	(HFONT)SelectObject(hdc, font);
	DrawText(hdc, message, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void DisplayBitmap(HDC hdc, COLORREF color, LPCWSTR mess, RECT rc, int trans)
{
	// Create compatible memory DC
	HDC hdcMem = CreateCompatibleDC(hdc);

	VOID* pvBits;
	BITMAPINFO bmi;

	ZeroMemory(&bmi, sizeof(BITMAPINFO));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = xSize;
	bmi.bmiHeader.biHeight = ySize;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;         // four 8-bit components 
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = xSize * ySize * 4;

	HBITMAP hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);

	for (int i = 0; i < ySize; i++)
		for (int j= 0; j < xSize; j++)
			((UINT32*)pvBits)[j + i * xSize] = color;

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, (double)trans/100*255, 0};

	// Select the bitmap into DC
	(HBITMAP)SelectObject(hdcMem, hbitmap);

	AlphaBlend(hdc, 0, 0, xSize, ySize, hdcMem, 0, 0, xSize, ySize, bf);

	HFONT font = CreateFont(50, 20, 0, 0, 700, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Calabri");

	drawText(hdc, font, mess, rc);

	DeleteDC(hdcMem);
	DeleteObject(hbitmap);
}
void saveToFile()
{
	std::string status = "0", str_score, goal, board, buff;
	if (endGameWon)
		status = "1";
	if (endGameOver)
		status = "2";
	str_score = std::to_string(score);
	goal = std::to_string(limit);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			board += std::to_string(fields[i][j].val);
			board += ",";
		}
	
	buff = "[GAME]\nSTATUS=" + status + "\nSCORE=" + str_score + "\nGOAL=" + goal + "\nBOARD=" + board;
	
	char databuff[100];
	strcpy_s(databuff, buff.c_str());

	std::ofstream file("2048.txt", std::ios::trunc);

	file << buff;
	file.close();
}
void readFromFile()
{
	std::ifstream file("2048.txt", std::ios::in);
	if (!file)
		return;

	std::string str, s, s2, s3;
	int j = 0;

	for (int i = 0; i < 5; i++)
	{
		getline(file, str);
		j = 0;
		if (i == 1)
		{
			while (str[j] != '=')
				j++;
			j++;
			if (str[j] == '1')
				endGameWon = true;
			else if (str[j] == '2')
				endGameOver = true;
		}
		if (i == 2)
		{
			while (str[j] != '=')
				j++;
			j++;
			while (str[j] != '\0')
			{
				s += str[j];
				j++;
			}
			score = atoi(s.c_str());
		}
		if (i == 3)
		{
			while (str[j] != '=')
				j++;
			j++;
			while (str[j] != '\0')
			{
				s2 += str[j];
				j++;
			}
			limit = atoi(s2.c_str());
		}
		if (i == 4)
		{
			while (str[j] != '=')
				j++;
			j++;
			while (str[j] != '\0')
			{
				for (int i = 0; i < 4; i++)
				{
					for (int k = 0; k < 4; k++)
					{
						while (str[j] != ',')
						{
							s3 += str[j];
							j++;
							
						}
						j++;

						fields[i][k].val = atoi(s3.c_str());
						fields[i][k].color = (int)(log(fields[i][k].val) / log(2));
						s3.clear();
					}
				}

			}
		}
	}

	file.close();

	switch (limit)
	{
	case 8:
		setMenu(0, hWnd1);
		setMenu(0, hWnd2);
		break;
	case 16:
		setMenu(1, hWnd1);
		setMenu(1, hWnd2);
		break;
	case 64:
		setMenu(2, hWnd1);
		setMenu(2, hWnd2);
		break;
	case 2048:
		setMenu(3, hWnd1);
		setMenu(3, hWnd2);
		break;
	}
	ifRead = true;
	Draw();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		
		switch (wmId)
		{
		case ID_GAME_NEWGAME:
			resetGame();
			break;
		case ID_GOAL_8:
			limit = 8;
			setMenu(0, hWnd1);
			break;
		case ID_GOAL_16:
			setMenu(1, hWnd1);
			limit = 16;
			break;
		case ID_GOAL_64:
			setMenu(2, hWnd1);
			limit = 64;
			break;
		case ID_GOAL_2048:
			setMenu(3, hWnd1);
			limit = 2048;
			break;
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		if(!endGamePerm)
			Draw();

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
		minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = xSize;
		minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = ySize;
		minMaxInfo->ptMinTrackSize.x = xSize;
		minMaxInfo->ptMinTrackSize.y = ySize;
	}
	break;
	case WM_MOVE:
		moveWindows(hWnd1, hWnd2);
		makeTransparent(hWnd1, hWnd2);
		break;
	case WM_KEYDOWN:
	{
		if(!endGamePerm)
			switch (wParam)
			{
				//A key
				case 0x41:
				{
					direction = 0;
					calculatePosition(direction);
					if(move)
						addField();
					InvalidateRect(hWnd1, NULL, FALSE);
					UpdateWindow(hWnd1);
				}break;
				//S key
				case 0x53:
				{
					direction = 1;
					calculatePosition(direction);
					if(move)
						addField();
					InvalidateRect(hWnd1, NULL, FALSE);
					UpdateWindow(hWnd1);
				}break;
				//D key
				case 0x44:
				{
					direction = 2;
					calculatePosition(direction);
					if(move)
						addField();
					InvalidateRect(hWnd1, NULL, FALSE);
					UpdateWindow(hWnd1);
				}break;
				//W key
				case 0x57:
				{
					direction = 3;
					calculatePosition(direction);
					if(move)
						addField();
					InvalidateRect(hWnd1, NULL, FALSE);
					UpdateWindow(hWnd1);
				}break;
			}
	}
	break;
	case WM_CTLCOLORLISTBOX:
		SetBkColor((HDC)wParam, RGB(10, 10, 10));
		SetTextColor((HDC)wParam, RGB(100, 200, 10));
		return (LRESULT)CreateSolidBrush(RGB(10, 50, 10));
	case WM_DESTROY:
		PostQuitMessage(0);
		saveToFile();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		case ID_GAME_NEWGAME:
			resetGame();
			break;
		case ID_GOAL_8:
			limit = 8;
			setMenu(0, hWnd2);
			break;
		case ID_GOAL_16:
			setMenu(1, hWnd2);
			limit = 16;
			break;
		case ID_GOAL_64:
			setMenu(2, hWnd2);
			limit = 64;
			break;
		case ID_GOAL_2048:
			setMenu(3, hWnd2);
			limit = 2048;
			break;
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		if (!endGamePerm)
			Draw();

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
		minMaxInfo->ptMaxSize.x = minMaxInfo->ptMaxTrackSize.x = xSize;
		minMaxInfo->ptMaxSize.y = minMaxInfo->ptMaxTrackSize.y = ySize;
		minMaxInfo->ptMinTrackSize.x = xSize;
		minMaxInfo->ptMinTrackSize.y = ySize;
	}
	break;
	case WM_KEYDOWN:
	{
		if (!endGamePerm)
			switch (wParam)
			{
				//A key
			case 0x41:
			{
				direction = 0;
				calculatePosition(direction);
				if (move)
					addField();
				InvalidateRect(hWnd1, NULL, FALSE);
				UpdateWindow(hWnd1);
			}break;
			//S key
			case 0x53:
			{
				direction = 1;
				calculatePosition(direction);
				if (move)
					addField();
				InvalidateRect(hWnd1, NULL, FALSE);
				UpdateWindow(hWnd1);
			}break;
			//D key
			case 0x44:
			{
				direction = 2;
				calculatePosition(direction);
				if (move)
					addField();
				InvalidateRect(hWnd1, NULL, FALSE);
				UpdateWindow(hWnd1);
			}break;
			//W key
			case 0x57:
			{
				direction = 3;
				calculatePosition(direction);
				if (move)
					addField();
				InvalidateRect(hWnd1, NULL, FALSE);
				UpdateWindow(hWnd1);
			}break;
			}
	}break;
	case WM_MOVE:
		moveWindows(hWnd2, hWnd1);
		makeTransparent(hWnd1, hWnd2);
		break;
	
	case WM_CTLCOLORLISTBOX:
		SetBkColor((HDC)wParam, RGB(10, 10, 10));
		SetTextColor((HDC)wParam, RGB(100, 200, 10));
		return (LRESULT)CreateSolidBrush(RGB(10, 50, 10));
	case WM_DESTROY:
		PostQuitMessage(0);
		saveToFile();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
