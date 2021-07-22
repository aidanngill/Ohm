#include "./Render.h"
#include "../Interfaces.h"

#include "../SDK/Color.h"
#include "../SDK/ISurface.h"

Render::Render() {
	font_base = interfaces->Surface->CreateFont();
	interfaces->Surface->SetFontGlyphSet(font_base, "Tahoma", 15, 400, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS);
}

void Render::Text(const wchar_t* text, int x, int y, Font font, Color color = Color(255, 255, 255, 255)) {
	interfaces->Surface->DrawSetTextPos(x, y);
	interfaces->Surface->DrawSetTextColor(color);
	interfaces->Surface->DrawSetTextFont(font);
	interfaces->Surface->DrawPrintText(text, wcslen(text));
}

void Render::Watermark() {
	this->Text(this->watermark_text, 10, 10, this->font_base);
}