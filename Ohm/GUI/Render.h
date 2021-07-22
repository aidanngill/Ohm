#pragma once

#include <iostream>

#include "./Font.h"

class Color;

class Render {
public:
	Render();

	void Text(const wchar_t* text, int x, int y, Font font, Color color);
	void Watermark();
	
	const wchar_t* watermark_text = L"Ohm";
	Font font_base;
};

inline std::unique_ptr<Render> render;