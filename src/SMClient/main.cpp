#include "pch.h"
#include "main.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	srand(time(NULL));
	CPU_RUN(1920/ WINDIVIDER, 1080/ WINDIVIDER, true);
    return 0;
}