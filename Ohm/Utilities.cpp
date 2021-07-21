#include <iostream>
#include <Windows.h>

#include "Utilities.h"

FILE* f;

void AttachGameConsole() {
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
}

void DetachGameConsole() {
	if (f) fclose(f);
	FreeConsole();
}