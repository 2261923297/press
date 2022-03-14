#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <list>

static size_t find_char(const char* chs, size_t size
	, char ch, std::vector<size_t>& vecs) {

	size_t rt = 0;
	for (size_t i = 0; i < size; i++) {
		if (chs[i] == ch) {
			vecs.push_back(i);
			rt++;
		}
	}
	return rt;
}

static std::string sp_file(const char* file_name) {
	std::vector<size_t> sps;
	size_t nsp = find_char(file_name, strlen(file_name), '\\', sps);
	nsp = nsp > 2 ? 2 : nsp;
	std::string rt(file_name);
	return rt.substr(sps[sps.size() - nsp] + 1);
}

#define LOG_TMP_BUFFER_SIZE 1024
static char __buffer__[LOG_TMP_BUFFER_SIZE] = { 0 };

#define LOG_APPANDAR std::cout
#define Z std::string()					\
				+ sp_file(__FILE__)	+ " > "			\
				+ std::string(__FUNCTION__)	+ ":"	\
				+ std::to_string(__LINE__)			\
				+ " > "								\
//
#define LOG_PROFIX std::string(sp_file(__FILE__) + ":"			\
	+ std::to_string(__LINE__) + ":" + __FUNCTION__ + " > ")	\
//
#define LOG_FORMAT(level)		\
	(std::string("["#level"]") + " "			\
				+ LOG_PROFIX).c_str()		\
//

#define _LOGGER(apandar, level, log_format)				\
	memset(__buffer__, 0, LOG_TMP_BUFFER_SIZE);		\
	sprintf(__buffer__, "%s", log_format(level));	\
	apandar << std::string(__buffer__)					\
//


#define LOGFMT(apandar, level, log_format, ...) do {	\
	::fprintf(apandar, "%s", log_format(level));			\
	::fprintf(apandar, ##__VA_ARGS__);						\
	::fprintf(apandar, "\n");								\
} while(0)												\
//


#ifndef def_appandar
#define def_appandar std::cout
#endif // def_appandar

#define __logger(level) \
	_LOGGER(def_appandar, level, LOG_FORMAT)
#define __logfmt(level, ...) \
	LOGFMT(stdout, level, LOG_FORMAT,__VA_ARGS__);


#define __logger_debug __logger(debug)
#define __logfmt_debug(...) __logfmt(debug, __VA_ARGS__)

#define __logger_error __logger(error)
#define __logfmt_error(...) __logfmt(error, __VA_ARGS__)

#define str_val(val) std::string(#val) + " = " << val

#define func_excute_time(func, time)do {	\
	uint64_t beg = clock();					\
	func();									\
	time = clock() - beg;					\
} while(0)		\
//
