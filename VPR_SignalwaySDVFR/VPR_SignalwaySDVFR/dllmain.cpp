// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "coredump/MiniDumper.h"
#include "utilityTool/ToolFunction.h"
#include "utilityTool/log4z.h"
CMiniDumper g_coredump(true);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    TCHAR szFileName[MAX_PATH] = { 0 };
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        GetModuleFileName(hModule, szFileName, MAX_PATH);	//取得包括程序名的全路径
        PathRemoveFileSpec(szFileName);				//去掉程序名
        Tool_SetDllDirPath(szFileName);

        zsummer::log4z::ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, "./XLWLog/");
        zsummer::log4z::ILog4zManager::getRef().setLoggerMonthdir(LOG4Z_MAIN_LOGGER_ID, true);
        zsummer::log4z::ILog4zManager::getRef().start();
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

