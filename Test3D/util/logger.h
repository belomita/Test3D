#pragma once

enum ELogLevel
{
	LogLevel_Debug = 1,
	LogLevel_Info  = 2,
	LogLevel_Sys   = 3,
	LogLevel_Warn  = 4,
	LogLevel_Error = 5,
	LogLevel_Max
};

void util_log_message(int level, const char* fmt, ...);
void util_log_message(int level, const char* filename, const char* funcname, int line_num, const char* fmt, ...);

#define util_log_full(level, fmt, ...) util_log_message(level, __FILE__, __FUNCTION__, __LINE__, fmt,##__VA_ARGS__)

#define util_log_full_debug(fmt, ...) util_log_full(LogLevel_Debug, fmt,##__VA_ARGS__)
#define util_log_full_info(fmt, ...)  util_log_full(LogLevel_Info,  fmt,##__VA_ARGS__)
#define util_log_full_sys(fmt, ...)   util_log_full(LogLevel_Sys,   fmt,##__VA_ARGS__)
#define util_log_full_warn(fmt, ...)  util_log_full(LogLevel_Warn,  fmt,##__VA_ARGS__)
#define util_log_full_err(fmt, ...)   util_log_full(LogLevel_Error, fmt,##__VA_ARGS__)

#define util_log_debug(fmt, ...)      util_log_message(LogLevel_Debug, fmt,##__VA_ARGS__)
#define util_log_info(fmt, ...)       util_log_message(LogLevel_Info,  fmt,##__VA_ARGS__)
#define util_log_sys(fmt, ...)        util_log_message(LogLevel_Sys,   fmt,##__VA_ARGS__)
#define util_log_warn(fmt, ...)       util_log_message(LogLevel_Warn,  fmt,##__VA_ARGS__)
#define util_log_err(fmt, ...)        util_log_message(LogLevel_Error, fmt,##__VA_ARGS__)