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
	
	const wchar_t* watermark_text = L"Ohm";

	Font font_base = 0x5; // One of the default CS:GO fonts.
	Font font_small;
};

inline std::unique_ptr<Render> render;