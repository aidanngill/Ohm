#pragma once

class IClientEntity;

// Attach a standard Windows console to the game process.
void AttachGameConsole();

// Detach the console and close the output file if it exists.
void DetachGameConsole();

uint8_t* PatternScan(void* module, const char* signature);

IClientEntity* GetLocalPlayer();