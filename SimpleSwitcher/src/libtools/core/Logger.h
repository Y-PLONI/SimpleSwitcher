﻿#pragma once
#include <fstream>
#include <clocale>
#include "Errors.h"
#include <mutex>
#include <source_location>

enum TLogLevel
{
	LOG_LEVEL_0 = 0,  // No log
	LOG_LEVEL_1 = 1,
	LOG_LEVEL_2 = 2,
	LOG_LEVEL_3 = 3,
	LOG_LEVEL_4 = 4,
};

namespace _log_int {
	inline std::atomic<TLogLevel> log_level;
}

inline TLogLevel GetLogLevel() { return _log_int::log_level;}
inline void SetLogLevel(TLogLevel val) { _log_int::log_level = val; }

namespace _log_int {

	class SwLogger {
	public:
		static SwLogger& Get() {
			static SwLogger logger;
			return logger;
		}
		void VFormat(const TChar* format, va_list args) {
			if (LazyOpen()) {
				vfwprintf_s(m_fp, format, args);
			}
		}
		void Append(const TChar* data) {
			if (!data) return;
			if (LazyOpen()) {
				fputws(data, m_fp);
			}
		}
		void Append(const char* data) {
			if (!data) return;
			if (LazyOpen()) {
				Format(L"%S", data);
				//fputs(data, m_fp);
			}
		}
		void Format(const TChar* Format, ...) {
			va_list alist;
			va_start(alist, Format);
			VFormat(Format, alist);
			va_end(alist);
		}
		void Flash() {
			if (m_fp)
				fflush(m_fp);
		}
		void EndLineFlash() {
			if (LazyOpen()) {
				fwprintf_s(m_fp, L"\n");
				fflush(m_fp);
			}
		}
		~SwLogger() {
			if (m_fp) {
				fclose(m_fp);
			}
		}
		std::mutex& Mtx() {
			return m_mtxLog;
		}
	private:
		FILE* LazyOpen() {
			if (!m_fp) {
				if (!m_tryOpen) {
					m_tryOpen = true;

					static const size_t nSize = 0x1000;
					std::unique_ptr<TChar[]> buf(new TChar[nSize]);
					TChar* sFolder = buf.get();
					if (!sFolder) {
						return m_fp;
					}

					if (!GetModuleFileNameEx(GetCurrentProcess(), GetCurrentModule(), sFolder, nSize))
						return NULL;
					TChar* sLast = wcsrchr(sFolder, L'\\');
					if (sLast)
						*sLast = 0;
					wcscat_s(sFolder, nSize, L"\\log");
					CreateDirectory(sFolder, NULL);
					TChar base[512];
					base[0] = 0;
					GetModuleBaseName(GetCurrentProcess(), NULL, base, std::ssize(base));

					auto path = std::format(L"{}\\{}.log", sFolder, base);
					//TChar sLogPath[0x1000];
					//sLogPath[0] = 0;
					//if (swprintf_s(sLogPath, L"%s\\%s(%d)_%u.log", sFolder, base, GetCurrentProcessId(), GetTick()) == -1)
					//{
					//	return m_fp;
					//}
					m_fp = _wfsopen(path.c_str(), L"wt, ccs=UTF-8", _SH_DENYNO);
					//out_file.open(path, std::ios_base::out | std::ios::trunc);
				}
			}
			return m_fp;
		}
		std::mutex m_mtxLog;
		TLogLevel m_logLevel = LOG_LEVEL_0;
		FILE* m_fp = NULL;
		bool m_tryOpen = false;
		std::wofstream out_file;
		SwLogger() {}

	};


	inline SwLogger& SwLoggerGlobal() { return SwLogger::Get(); }

	inline void __SW_LOG_FORMAT_V__(const TChar* format, va_list alist) {
		SwLoggerGlobal().VFormat(format, alist);
	}
	inline void __SW_LOG_TIME() {
		SYSTEMTIME ST;
		::GetLocalTime(&ST);
		SwLoggerGlobal().Format(
			L"%02u.%02u|%02u:%02u:%02u.%03u|%05u ",
			(TUInt32)ST.wDay,
			(TUInt32)ST.wMonth,
			(TUInt32)ST.wHour,
			(TUInt32)ST.wMinute,
			(TUInt32)ST.wSecond,
			(TUInt32)ST.wMilliseconds,
			GetCurrentThreadId());
	}

	inline void __SW_LOG_FORMAT__(const TChar* Format, ...) {
		va_list alist;
		va_start(alist, Format);
		__SW_LOG_FORMAT_V__(Format, alist);
		va_end(alist);
	}

	inline void SW_LOG_INFO(const TChar* Format, ...) {
		std::unique_lock<std::mutex> _lock(SwLoggerGlobal().Mtx());

		__SW_LOG_TIME();
		va_list alist;
		va_start(alist, Format);
		SwLoggerGlobal().VFormat(Format, alist);
		va_end(alist);
		SwLoggerGlobal().EndLineFlash();
	}


	template<typename... Args>
	void __FORMAT_APPEND(const std::wformat_string<Args...>& s, Args&&... v) {
		auto res = std::vformat(s.get(), std::make_wformat_args(v...));
		SwLoggerGlobal().Append(res.c_str());
	}

	void __LOG_LINE_FORMAT(auto&&... v) {
		std::unique_lock<std::mutex> _lock(SwLoggerGlobal().Mtx());
		__SW_LOG_TIME();
		__FORMAT_APPEND(FORWARD(v)...);
		SwLoggerGlobal().EndLineFlash();
	}

	class WinErrBOOL {
		bool m_res = false;
		DWORD m_dwErr = 0;
	public:
		WinErrBOOL(BOOL r) : m_res(r) {}
		WinErrBOOL(bool r) : m_res(r) {}
		WinErrBOOL() {}
		void SetError(DWORD err) { m_dwErr = err; }
		void Log() const {
			__FORMAT_APPEND(L"WinErr={} ", m_dwErr);

			CAutoWinMem lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				m_dwErr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);
			SwLoggerGlobal().Append((TStr)lpMsgBuf.get());
		}
		operator bool() {
			if (m_res)
				return false;
			m_dwErr = GetLastError();
			return true;
		}
		TStatus ToTStatus() { return SW_ERR_WINAPI; }
	};

	struct WinErrDwordWait {
		DWORD res;
		DWORD dwErr;
		WinErrDwordWait(DWORD r) : res(r) {}
		void Log() const {
			CAutoWinMem lpMsgBuf;
			DWORD dwErr = GetLastError();
			__FORMAT_APPEND(L"Wait result={} LastErr={} ", res, dwErr);
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dwErr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);
			SwLoggerGlobal().Append((TStr)lpMsgBuf.get());
		}
		operator bool() {
			if (res == WAIT_OBJECT_0)
				return false;
			dwErr = GetLastError();
			return true;
		}
		TStatus ToTStatus() { return SW_ERR_WAIT_PROCESS; }
	};

	struct SwErrTStatus {
		TStatus res;
		SwErrTStatus(TStatus r) : res(r) {}
		void Log() const {
			__FORMAT_APPEND(L"TStatus={}({})", res >= std::ssize(c_StatusNames) ? L"NO_INFO" : c_StatusNames[res], (int)res);
		}
		operator bool() const { return res != SW_ERR_SUCCESS; }
		TStatus ToTStatus() { return res; }
	};



	struct WinErrLSTATUS {
		LSTATUS res;
		WinErrLSTATUS(LSTATUS r) : res(r) {}
		void Log() const { __FORMAT_APPEND(L"LSTATUS={}", (int)res); }
		bool IsError() const { return res != ERROR_SUCCESS; }
		operator bool() const { return IsError(); }
		TStatus ToTStatus() { return SW_ERR_WINAPI; }
	};

	struct ErrnoDescr {
		errno_t res;
		ErrnoDescr(errno_t r) : res(r) {}

		void Log() const {
			TChar sBuf[0x1000];
			*sBuf = 0;
			_wcserror_s(sBuf, res);
			__FORMAT_APPEND(L"errno_t={} msg={}", res, sBuf);
		}
		bool IsError() const { return res != 0; }
		operator bool() const { return IsError(); }
		TStatus ToTStatus() { return SW_ERR_ERRNO; }
	};

	struct WinErrHRESULT {
		HRESULT res;
		WinErrHRESULT(HRESULT r) : res(r) {}
		void Log() const { __SW_LOG_FORMAT__(L"HResult=%d(0x%x)", res, res); }
		operator bool() const { return FAILED(res); }
		TStatus ToTStatus() { return SW_ERR_HRESULT; }
	};


	template<typename... Args>
	inline void __Log_Err_Common(const auto& err, std::source_location loc, const std::wformat_string<Args...> s, Args&&... v) {
		if (GetLogLevel() < LOG_LEVEL_1)
			return;

		std::unique_lock<std::mutex> _lock(SwLoggerGlobal().Mtx());

		__SW_LOG_TIME();
		err.Log();
		auto file = loc.file_name();
		auto cur = strrchr(file, '\\');
		SwLoggerGlobal().Append(std::format("file={}({})", cur ? cur + 1 : file, loc.line()).c_str());
		__FORMAT_APPEND(s, FORWARD(v)...);
		SwLoggerGlobal().EndLineFlash();
	}

	inline void __Log_Err_Common(const auto& err, std::source_location loc) { __Log_Err_Common(err, loc, L""); }

}


#define __RET_ERR(CLASS, X, ...) {CLASS __res = (X); __Log_Err_Common(__res, std::source_location::current(), __VA_ARGS__); return __res.ToTStatus(); }
#define  _SW_ERR_RET(ClassName, X, ...) {if (ClassName __res = (X)) { __Log_Err_Common(__res, std::source_location::current(), __VA_ARGS__); return __res.ToTStatus(); } }
#define  _SW_ERR_LOG(ClassName, X, ...) {if (ClassName __res = (X))   __Log_Err_Common(__res, std::source_location::current(), __VA_ARGS__); }

#define IFW_RET(X, ...) _SW_ERR_RET(_log_int::WinErrBOOL, X, __VA_ARGS__)
#define IFW_LOG(X, ...) _SW_ERR_LOG(_log_int::WinErrBOOL, X, __VA_ARGS__)

#define IFS_RET(X, ...) _SW_ERR_RET(_log_int::SwErrTStatus, X, __VA_ARGS__)
#define IFS_LOG(X, ...) _SW_ERR_LOG(_log_int::SwErrTStatus, X, __VA_ARGS__)

#define IF_LSTATUS_RET(X, ...) _SW_ERR_RET(_log_int::WinErrLSTATUS, X, __VA_ARGS__)
#define IF_LSTATUS_LOG(X, ...) _SW_ERR_LOG(_log_int::WinErrLSTATUS, X, __VA_ARGS__)

#define IF_ERRNO_RET(X, ...) _SW_ERR_RET(_log_int::ErrnoDescr, X, __VA_ARGS__)
#define IF_ERRNO_LOG(X, ...) _SW_ERR_LOG(_log_int::ErrnoDescr, X, __VA_ARGS__)
#define RET_ERRNO(...) __RET_ERR(_log_int::ErrnoDescr, errno, __VA_ARGS__)

#define IFH_RET(X, ...) _SW_ERR_RET(_log_int::WinErrHRESULT, X, __VA_ARGS__)
#define IFH_LOG(X, ...) _SW_ERR_LOG(_log_int::WinErrHRESULT, X, __VA_ARGS__)

#define IF_WAITDWORD_RET(X, ...) _SW_ERR_RET(_log_int::WinErrDwordWait, X, __VA_ARGS__)
#define IF_WAITDWORD_LOG(X, ...) _SW_ERR_LOG(_log_int::WinErrDwordWait, X, __VA_ARGS__)

template<typename... Args>
inline void LOG_ANY(const std::wformat_string<Args...> s, Args&&... v) 	{
 if (GetLogLevel() >= LOG_LEVEL_2) { _log_int::__LOG_LINE_FORMAT(s, FORWARD(v)...); } }

template<typename... Args>
inline void LOG_ANY_4(const std::wformat_string<Args...> s, Args&&... v) {
	if (GetLogLevel() >= LOG_LEVEL_4) { _log_int::__LOG_LINE_FORMAT(s, FORWARD(v)...); }
}

template<typename... Args>
inline void LOG_WARN(const std::wformat_string<Args...> s, Args&&... v) {
	using namespace _log_int;
	if (GetLogLevel() >= LOG_LEVEL_1) {
		std::unique_lock<std::mutex> _lock(SwLoggerGlobal().Mtx());
		__SW_LOG_TIME();
		SwLoggerGlobal().Append(L"[WARN] ");
		__FORMAT_APPEND(s, FORWARD(v)...);
		SwLoggerGlobal().EndLineFlash();
	}
}

inline void SetLogLevel_info(TLogLevel logLevel) {
	SetLogLevel(logLevel);
	LOG_ANY(L"Log level now {}", (int)logLevel);
}

#define RETURN_SUCCESS {return SW_ERR_SUCCESS; }

#define LOG_INFO_4(...) {if(GetLogLevel() >= LOG_LEVEL_4){_log_int::SW_LOG_INFO(__VA_ARGS__);}}
#define LOG_INFO_3(...) {if(GetLogLevel() >= LOG_LEVEL_3){_log_int::SW_LOG_INFO(__VA_ARGS__);}}
#define LOG_INFO_2(...) {if(GetLogLevel() >= LOG_LEVEL_2){_log_int::SW_LOG_INFO(__VA_ARGS__);}}
#define LOG_INFO_1(...) {if(GetLogLevel() >= LOG_LEVEL_1){_log_int::SW_LOG_INFO(__VA_ARGS__);}}
