#pragma once

#include <iostream>
#include <Windows.h>

#include "./Color.h"

#include "../Utility/VirtualFunc.h"

class ISurface {
public:
	void DrawSetColor(Color color) {
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc<OriginalFn>(this, 15)(this, color.r(), color.g(), color.b(), color.a());
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1) {
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc<OriginalFn>(this, 16)(this, x0, y0, x1, y1);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1) {
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc<OriginalFn>(this, 18)(this, x0, y0, x1, y1);
	}
	void DrawLine(int x0, int y0, int x1, int y1) {
		typedef void(__thiscall* DrawLineFn)(void*, int, int, int, int);
		GetVFunc<DrawLineFn>(this, 19)(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(unsigned long font) {
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		GetVFunc<OriginalFn>(this, 23)(this, font);
	}
	void DrawSetTextColor(Color color) {
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		GetVFunc<OriginalFn>(this, 25)(this, color.r(), color.g(), color.b(), color.a());
	}
	void DrawSetTextPos(int x, int y) {
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		GetVFunc<OriginalFn>(this, 26)(this, x, y);
	}
	void DrawPrintText(const wchar_t* text, int textLen) {
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t*, int, int);
		return GetVFunc<OriginalFn>(this, 28)(this, text, textLen, 0);
	}
	void GetScreenSize(int& wide, int& tall) {
		typedef void(__thiscall* oGetScreenSize)(void*, int&, int&);
		GetVFunc<oGetScreenSize>(this, 44)(this, wide, tall);
	}
	void UnlockCursor() {
		typedef void(__thiscall* OriginalFn)(ISurface*);
		return GetVFunc<OriginalFn>(this, 66)(this);
	}
	void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) {
		typedef void(__thiscall* OriginalFn)(void*, unsigned long, const char*, int, int, int, int, int, int, int);
		GetVFunc<OriginalFn>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	unsigned long CreateFont() {
		typedef unsigned int(__thiscall* OriginalFn)(void*);
		return GetVFunc<OriginalFn>(this, 71)(this);
	}
	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall) {
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t*, int&, int&);
		GetVFunc< OriginalFn >(this, 79)(this, font, text, wide, tall);
	}
	void GetCursorPosition(int& x, int& y) {
		return GetVFunc<void(__thiscall*)(void*, int&, int&)>(this, 96)(this, x, y);
	}
};