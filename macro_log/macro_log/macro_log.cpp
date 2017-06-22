// macro_log.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>

#pragma warning(disable:4996)

typedef enum _ModuleTypeEnums
{
	LOG_MODULE_UI = 0,
	LOG_MODULE_NETWORK,
	LOG_MODULE_PARSER,
	LOG_MODULE_MAX
} ModuleTypeEnums;

const char* ModuleTypeEnumsString[LOG_MODULE_MAX] = 
{
	"UI", "NETWORK", "PARSER"
};

#define LOG_USING_FLAG 1
#define LOG_USING_DEBUG 1
#define LOG_USING_WARNING 1
#define LOG_USING_ERROR 1
#define LOG_USING_FATA 1

typedef void (*LOG_OUTPUT_FUNC)(char*);
LOG_OUTPUT_FUNC logFunc;
int moduleLogFlag[LOG_MODULE_MAX] = {0, };

int IsPrintableModule(ModuleTypeEnums _type)
{
	return moduleLogFlag[_type];
}

void SetPrintableModule(ModuleTypeEnums _type, int _flag)
{
	moduleLogFlag[_type] = _flag;
}

void dprintf(const char* _format, ...)
{
	if (_format && logFunc)
	{
		va_list args;
		va_start(args, _format);
		char log[1024] = {0, };
		_vsnprintf(log, sizeof(log) - 1, _format, args);
		logFunc(log);
		va_end(args);
	}
}


#if LOG_USING_FLAG == 1
#define LOG(level, module, args) do {\
	DWORD tick = GetTickCount();\
	SYSTEMTIME time = {0,};\
	::GetSystemTime(&time);\
	if (IsPrintableModule(module)) {\
		dprintf("%02d-%02d-%02d %02d:%02d:%02d,%d [%s(%s, %d), %s, %s] - ", time.wYear, time.wMonth, \
			time.wDay, time.wHour, time.wMinute, time.wSecond, tick, __FUNCTION__, __FILE__, __LINE__, \
			ModuleTypeEnumsString[module], level);\
			dprintf args;\
	}} while (0)
#else
#define LOG(level, type, args) ((void) 0)
#endif

#if LOG_USING_DEBUG == 1
#define LOG_DEBUG(module, args) LOG("DEBUG", module, args)
#else
#define LOG_DEBUG(module, args) ((void) 0)
#endif

#if LOG_USING_WARNING == 1
#define LOG_WARNING(module, args) LOG("WARNING", module, args)
#else
#define LOG_WARNING(module, args) ((void) 0)
#endif

#if LOG_USING_ERROR == 1
#define LOG_ERROR(module, args) LOG("ERROR", module, args)
#else
#define LOG_ERROR(module, args) ((void) 0)
#endif

#if LOG_USING_FATAL == 1
#define LOG_FATAL(module, args) LOG("FATAL", module, args)
#else
#define LOG_FATAL(module, args) ((void) 0)
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	{
		logFunc = (LOG_OUTPUT_FUNC) printf;
		int size = 10;
		SetPrintableModule(LOG_MODULE_UI, 1);
		LOG_DEBUG(LOG_MODULE_UI, ("size = %d", size));
	}

	return 0;
}

