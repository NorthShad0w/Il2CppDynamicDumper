#include "Core/Core.hpp"

void main()
{
	system("cls");
	if (!I2Hrame->Setup())
		printf("[-] Failed to setup IL2CPP!\n");
	else
		printf("[+] IL2CPP setup successfully!\n");
	if (!Direct3D.Initialization())
		printf("[-] Failed to setup Direct3D!\n");
	else
	{
		printf("[+] Direct3D setup successfully!\n");
		printf("[*] Direct3D Present:%p\n[*] Direct3D ResizeBuffers:%p\n", Direct3D.Present, Direct3D.ResizeBuffers);
	}
	if (!Device::Setup())
		printf("[-] Failed to setup device hooks!\n");
	else
		printf("[+] Device hooks setup successfully!\n");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	}
	return TRUE;
}