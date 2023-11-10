﻿// Neurozine.exe - Malware

#include <windows.h>
//#include <tchar.h>
//#include <ctime>
//#include <iostream>
//#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib,"Msimg32.lib")
#include <math.h>
//#include <time.h>
#define M_PI   3.14159265358979323846264338327950288
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
/*typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;*/
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
/*COLORREF RndRGB() {
	int clr = rand() % 5;
	if (clr == 0) return RGB(255, 0, 0); if (clr == 1) return RGB(0, 255, 0); if (clr == 2) return RGB(0, 0, 255); if (clr == 3) return RGB(255, 0, 255); if (clr == 4) return RGB(255, 255, 0);
}*/

DWORD WINAPI swirl(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), xSize = sh / 10, ySize = 9;
	while (1) {
		hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
		for (int i = 0; i < sh * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCCOPY);
		}
		for (int i = 0; i < sw * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCCOPY);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
		Sleep(1);
	}
}
DWORD WINAPI shader1(LPVOID lpvd) //credits to fr4ctalz, but I modified it
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;


	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
				int fx = (int)((i ^ 4) + (i * 4) * cbrt(30));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		Sleep(5);
	}

	return 0x00;
}
DWORD WINAPI colorswirl(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), xSize = sh / 10, ySize = 9;
	while (1) {
		hdc = GetDC(0); HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, screenshot);
		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCINVERT);
		for (int i = 0; i < sh * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, i, 0, 1, sh, hdcMem, i, wave, SRCPAINT);
		}
		for (int i = 0; i < sw * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(hdcMem, 0, i, sw, 1, hdcMem, wave, i, SRCPAINT);
		}
		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCERASE);
		ReleaseDC(0, hdc);
		DeleteDC(hdc); DeleteDC(hdcMem); DeleteObject(screenshot);
	}
}
DWORD WINAPI shader2(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 0xff;
		for (int i = 0; w * h > i; i++) {
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 50;
			((BYTE*)(data + i))[v % 3] = ((BYTE*)(data + i + v))[v]^byte;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}
DWORD WINAPI shader3(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 0xff;
		for (int i = 0; w * h > i; i++) {
			INT x = i % w, y = i / w;
			if (i % h == 0 && rand() % 100 == 0)
				v = rand() % 50;
			((BYTE*)(data + i))[v % 3] += x^y;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}
LPCWSTR  generateRandomUnicodeString(int len)
{
	wchar_t* ustr = new wchar_t[len + 1];      // +1 for '\0'

	for (int i = 0; i < len; i++) {
		ustr[i] = (rand() % 256) + 1024;
	}
	ustr[len] = L'\0';
	return ustr;
}
DWORD WINAPI textout1(LPVOID lpvd)
{
	int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
	LPCWSTR text1 = 0;
	//LPCSTR text2 = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		text1 = generateRandomUnicodeString(rand() % 10 + 10);
		//text2 = "you asked for it";
		SetTextColor(hdc, Hue(239));
		HFONT font = CreateFontA(43, 32, rand() % 3600, rand() % 3600, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Webdings");
		SelectObject(hdc, font);
		TextOutW(hdc, rand() % x, rand() % y, text1, strlen((const char *)text1));
		//TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(1);
	}
}
DWORD WINAPI textout2(LPVOID lpvd)
{
	int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
	LPCSTR text1 = 0;
	//LPCSTR text2 = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		text1 = "Neurozine.exe";
		//text2 = "you asked for it";
		SetTextColor(hdc, Hue(239));
		HFONT font = CreateFontA(rand()%100, rand()%100, rand() % 3600, rand() % 3600, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Webdings");
		SelectObject(hdc, font);
		TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
		//TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(1);
	}
}
DWORD WINAPI shader4(LPVOID lpvd) //credits to fr4ctalz, but I modified it
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;


	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
				int fx = (int)(i * cbrt(x^y));

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		Sleep(5);
	}

	return 0x00;
}
DWORD WINAPI shader5(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 0xff;
		for (int i = 0; w * h > i; i++) {
			INT x = i % w, y = i / w;
			if (i % h == 0 && rand () % 110)
				v = rand () % 32;
			((BYTE*)(data + i))[v] -= x&y;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}
DWORD WINAPI shader6(LPVOID lpvd) //credits to fr4ctalz, but I modified it
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;


	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
				FLOAT fx = (x + i*4) ^ (y);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		Sleep(5);
	}

	return 0x00;
}
DWORD WINAPI shader7(LPVOID lpParam) {
	int time = GetTickCount();
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {
		HDC desk = GetDC(NULL);
		HDC hdcdc = CreateCompatibleDC(desk);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcdc, hbm);
		BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;
		BYTE byte = 0;
		if ((GetTickCount() - time) > 60000)
			byte = rand() % 0xff;
		for (int i = 0; w * h > i; i++) {
			INT x = i % w, y = i / w;
			if (i % h == 0 && rand() % 110)
				v = rand() % 32;
			((BYTE*)(data + i))[v] = ((BYTE*)(data + i + v))[v]+(x^y);
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
		DeleteObject(hbm);
		DeleteObject(hdcdc);
		DeleteObject(desk);
	}
	return 0;
}
DWORD WINAPI shader8(LPVOID lpvd)
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = screenWidth;
	bmpi.bmiHeader.biHeight = screenHeight;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;


	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;

	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);

	INT i = 0;

	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

		RGBQUAD rgbquadCopy;

		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				int index = y * screenWidth + x;
				FLOAT fx = (y + i * 4);

				rgbquadCopy = rgbquad[index];

				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);

				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;

		StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
		Sleep(5);
	}

	return 0x00;
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 24000, 24000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[24000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t ^ t >> 8) * (t | t >> 5) + (t | t << 4) + (t | t >> 7)) / 674);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t + t >> 8) * (t | t >> 5) + (t | t << 4) + (t | t >> 7)) / 674);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t + t >> 8) * (t | t >> 5) ^ (t | t << 4) ^ (t | t >> 7)) / 67655);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(((t + t >> 8) & (t | t >> 5) ^ (t | t << 4) + (t | t >> 7)) & 6765);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if (t != 0) buffer[t] = static_cast<char>(((t + t >> 8) & (t + t >> 5) ^ (t & t << 4) + (t / t >> 7)) & 6765);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if (t != 0) buffer[t] = static_cast<char>(((t + t >> 8) & (t + t >> 5) ^ (t & t << 4) + (t & t >> 7)) & 675);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if (t != 0) buffer[t] = static_cast<char>((t % 127 & ((44583 ^ (t & 4095)) + 2 % 128)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if (t != 0) buffer[t] = static_cast<char>(((t + t >> 8) * (t + t >> 5) ^ (t & t << 4) ^ (t & t >> 7)) & 675);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound9() { //credits to N17Pro3426, but it's 11025 instead of 8000
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		if (t != 0) buffer[t] = static_cast<char>(2 * (t * (t >> 6 | t | t) + (7 & t >> 11)));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
DWORD WINAPI fakeerror(LPVOID lpParam) {
	MessageBox(NULL, L"The system has reached the maximum size allowed for the system part of the registry. Additional storage requests will be ignored.", L"Windows - Low On Registry Space", MB_ICONERROR);
	return 0;
}
int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"This is a GDI Only, Run?", L"Neurozine.exe by pankoza (GDI Only)", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"Are you sure? It still contains flashing lights - NOT for epilepsy", L"F̷i̷n̸a̴l̴ ̴W̸a̴r̸n̷i̸n̷g̷ - Neurozine.exe (GDI Only)", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			Sleep(2000);
			CreateThread(0, 0, fakeerror, 0, 0, 0);
			Sleep(5000);
			HANDLE thread1 = CreateThread(0, 0, swirl, 0, 0, 0);
			HANDLE thread1dot1 = CreateThread(0, 0, shader1, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			TerminateThread(thread1dot1, 0);
			CloseHandle(thread1dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread2 = CreateThread(0, 0, colorswirl , 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			//TerminateThread(thread2dot1, 0);
			//CloseHandle(thread2dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			//TerminateThread(thread3dot1, 0);
			//CloseHandle(thread3dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread4 = CreateThread(0, 0, shader3, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			InvalidateRect(0, 0, 0);
			HANDLE thread5 = CreateThread(0, 0, shader4, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, shader5, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, textout2, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			//TerminateThread(thread6dot1, 0);
			//CloseHandle(thread6dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread7 = CreateThread(0, 0, shader6, 0, 0, 0);
			//HANDLE thread7dot1 = CreateThread(0, 0, invcc, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			InvalidateRect(0, 0, 0);
			HANDLE thread8 = CreateThread(0, 0, shader7, 0, 0, 0);
			//HANDLE thread8dot1 = CreateThread(0, 0, textout2, 0, 0, 0);
			sound8();
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			InvalidateRect(0, 0, 0);
			HANDLE thread9 = CreateThread(0, 0, shader8, 0, 0, 0);
			//HANDLE thread8dot1 = CreateThread(0, 0, textout2, 0, 0, 0);
			sound9();
			Sleep(30000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			TerminateThread(thread6dot1, 0);
			CloseHandle(thread6dot1);
			InvalidateRect(0, 0, 0);
		}
	}
}