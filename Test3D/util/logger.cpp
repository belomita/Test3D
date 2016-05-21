#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

static const char* LogLevelText[LogLevel_Max] = {
	"", "DBG", "INF", "SYS", "WRN", "ERR", 
};

static int LogLevelColor[LogLevel_Max] = {
	FOREGROUND_INTENSITY, 
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED,
	FOREGROUND_INTENSITY | FOREGROUND_RED,
};

static int current_log_level = LogLevel_Debug;

const char* _util_current_time() {
	static char  buf[80];
	time_t       now = time(0);
	tm           ts;
	
	localtime_s(&ts, &now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%X", &ts);
	return buf;
}

static void util_log_message(int level, const char* fmt, va_list args)
{
	if (current_log_level > level)
		return;

	WORD wOldColorAttrs = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);
	if (hStderr == INVALID_HANDLE_VALUE) {
		return;
	}
	if (GetConsoleScreenBufferInfo(hStderr, &csbiInfo)) {
		wOldColorAttrs = csbiInfo.wAttributes;
	}
	SetConsoleTextAttribute(hStderr, LogLevelColor[level]);

	char buf[2048];
	vsnprintf_s(buf, _countof(buf), _TRUNCATE, fmt, args);
	const char* level_text = LogLevelText[level];
	fprintf(stderr, "[%s]  [%s]  %s\n", _util_current_time(), level_text, buf);

	if (wOldColorAttrs > 0) {
		SetConsoleTextAttribute(hStderr, wOldColorAttrs);
	}
}

static void util_log_message(int level, const char* filename, const char* funcname, int line_num, const char* fmt, va_list args)
{
	if (current_log_level > level)
		return;

	WORD wOldColorAttrs = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);
	if (hStderr == INVALID_HANDLE_VALUE) {
		return;
	}
	if (GetConsoleScreenBufferInfo(hStderr, &csbiInfo)) {
		wOldColorAttrs = csbiInfo.wAttributes;
	}
	SetConsoleTextAttribute(hStderr, LogLevelColor[level]);

	char buf[2048];
	vsnprintf_s(buf, _countof(buf), _TRUNCATE, fmt, args);

	const char* level_text = LogLevelText[level];
	fprintf(stderr, "[%s]  [%s]  %s\n", _util_current_time(), filename, funcname, line_num, buf);

	if (wOldColorAttrs > 0) {
		SetConsoleTextAttribute(hStderr, wOldColorAttrs);
	}
}

void util_log_message(int level, const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	util_log_message(level, fmt, args);
	va_end(args);
}

void util_log_message(int level, const char* filename, const char* funcname, int line_num, const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	util_log_message(level, filename, funcname, line_num, fmt, args);
	va_end(args);
}
