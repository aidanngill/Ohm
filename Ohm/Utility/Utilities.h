#pragma once

// Attach a standard Windows console to the game process.
void AttachGameConsole();

// Detach the console and close the output file if it exists.
void DetachGameConsole();