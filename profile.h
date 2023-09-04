#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <codecvt>
#include <list>

#define PROFILE_NAME "config.ini"

class ProFileA
{
private:
	std::string m_file;
	std::recursive_mutex  m_mtx;
public:
	ProFileA(std::string file);

    std::list<std::string> ReadPrivateProfileSectionNames();
	int32_t ReadProfileIntegerA(std::string section, std::string key);
	uint32_t ReadProfileStringsA(std::string section, std::string key, char* buffer, uint32_t size);
	std::map<std::string, std::string> ReadProfileSectionsA(std::string section);

	int32_t WriteProfileStringsA(std::string section, std::string key, std::string buffer);
	int32_t WriteProfileIntegerA(std::string seciton, std::string key, int32_t contect);
	int32_t WriteProfileSectonsA(std::string section, std::string buffer);

    int32_t DeleteProfileString(std::string section, std::string key);
    int32_t DeleteProfileSection(std::string section);
};

class ProFileW
{
public:
	std::wstring m_file;
	std::recursive_mutex  m_mtx;

public:
	ProFileW(std::wstring file);
	uint32_t ReadProfileStringsW(std::wstring section, std::wstring key, wchar_t* buffer, uint32_t size);
	int32_t ReadProfileIntegerW(std::wstring section, std::wstring key);
	std::map<std::wstring, std::wstring> ReadProfileSectionsW(std::wstring section);

	int32_t WriteProfileStringsW(std::wstring section, std::wstring key, std::wstring buffer);
	int32_t WriteProfileIntegerW(std::wstring seciton, std::wstring key, int64_t contect);
	int32_t WriteProfileSectonsW(std::wstring section, std::wstring buffer);
};
