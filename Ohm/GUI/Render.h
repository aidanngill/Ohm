#pragma once

#include <iostream>

#include "./Font.h"

class Color;

bool IsInRegion(int x, int y, int tx, int ty, int tw, int th);

class Render {
public:
	Render();
	void SetupFonts();

	void Text(const wchar_t* text, int x, int y, Font font, Color color);

	void Watermark();
	void Visuals();
	
	const wchar_t* watermarkText = L"Ohm";

	Font fontBase = 0x5;
};

inline std::unique_ptr<Render> render;