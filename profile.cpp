#include "profile.h"

ProFileA::ProFileA(std::string file)
{
	char buffer[MAX_PATH]{ NULL };
	GetModuleFileNameA(NULL, buffer, sizeof(buffer));
	this->m_file.append(buffer);

	std::size_t pos = this->m_file.find_last_of('\\');
	if (pos != std::string::npos) this->m_file.erase(pos);

    this->m_file.append("\\");
    this->m_file.append(file);
}

std::list<std::string> ProFileA::ReadPrivateProfileSectionNames()
{
    int32_t size = 4096;
    char *buffer = new char[size];
    ZeroMemory(buffer, size);

    int32_t resul = GetPrivateProfileSectionNamesA(buffer, size, this->m_file.c_str());

    while(true)
    {
        if(resul == (size - 2))
        {
            size *= 2;
            delete[] buffer;
            buffer = new char[size];
            ZeroMemory(buffer, size);
        }
        else
        {
            break;
        }
    }

    std::list<std::string> sections;
    char* segment = buffer;
    while (*segment)
    {
        sections.push_back(std::string(segment));
        segment += strlen(segment) + 1;
    }

    delete[] buffer;
    return sections;
}

int32_t ProFileA::ReadProfileIntegerA(std::string section, std::string key)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx); // ����
	return GetPrivateProfileIntA(section.c_str(), key.c_str(), 0, this->m_file.c_str());
}

uint32_t ProFileA::ReadProfileStringsA(std::string section, std::string key, char* buffer, uint32_t size)
{
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
	return GetPrivateProfileStringA(section.c_str(), key.c_str(), NULL, buffer, size, this->m_file.c_str());
}

std::map<std::string, std::string> ProFileA::ReadProfileSectionsA(std::string section)
{
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);

    std::map<std::string, std::string> result;

	uint32_t size = 0x256;
	char* buffer = new char[size];
	ZeroMemory(buffer, size);
	int res = 0;

	while (true)
	{
		res = GetPrivateProfileSectionA(section.c_str(), buffer, size, this->m_file.c_str());

        if (res == (size - 2))
		{
			size *= 2;
			delete[] buffer;
			buffer = new char[size];
			ZeroMemory(buffer, size);
		}
		else
		{
			break;
		}
	}

	std::string_view str(buffer, res);
	size_t pos = 0;

	while (pos < str.size())
	{
		size_t end_pos = str.find('\0', pos);
		if (end_pos == std::wstring_view::npos)
		{
			break;
		}

		std::string_view pair(&str[pos], end_pos - pos);
		size_t eq_pos = pair.find("=");

		if (eq_pos != std::string_view::npos)
		{
			std::string key(pair.data(), eq_pos);
			std::string value(pair.data() + eq_pos + 1, pair.size() - eq_pos - 1);
			result.emplace(key, value);
		}
		pos = end_pos + 1;
	}

	delete[] buffer;
	return result;
}

int32_t ProFileA::WriteProfileStringsA(std::string section, std::string key, std::string buffer)
{
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
	return WritePrivateProfileStringA(section.c_str(), key.c_str(), buffer.c_str(), this->m_file.c_str());
}

int32_t ProFileA::WriteProfileIntegerA(std::string section, std::string key, int32_t contect)
{
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
	char buffer[0x32]{ NULL };
	sprintf_s(buffer, "%d", contect);
	return WritePrivateProfileStringA(section.c_str(), key.c_str(), buffer, this->m_file.c_str());
}

int32_t ProFileA::WriteProfileSectonsA(std::string section, std::string buffer)
{
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    return WritePrivateProfileSectionA(section.c_str(), buffer.c_str(), this->m_file.c_str());
}

int32_t ProFileA::DeleteProfileString(std::string section, std::string key)
{
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    return WritePrivateProfileStringA(section.c_str(), key.c_str(),NULL, this->m_file.c_str());
}

int32_t ProFileA::DeleteProfileSection(std::string section)
{
    return WritePrivateProfileStringA(section.c_str(),NULL,NULL,this->m_file.c_str());
}

ProFileW::ProFileW(std::wstring file)
{
	wchar_t buffer[MAX_PATH]{ NULL };
	GetModuleFileNameW(NULL, buffer, sizeof(buffer));
	this->m_file = buffer;

	std::size_t pos = this->m_file.find_last_of('\\');
	if (pos != std::wstring::npos) this->m_file.erase(pos);
	this->m_file.append(L"\\");
	this->m_file.append(file);
}

uint32_t ProFileW::ReadProfileStringsW(std::wstring section, std::wstring key, wchar_t* buffer, uint32_t size)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx); // ����
	return GetPrivateProfileStringW(section.c_str(), key.c_str(), NULL, buffer, size, this->m_file.c_str());
}

int32_t ProFileW::ReadProfileIntegerW(std::wstring section, std::wstring key)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx); // ����
	return GetPrivateProfileIntW(section.c_str(), key.c_str(), 0, this->m_file.c_str());
}

std::map<std::wstring, std::wstring> ProFileW::ReadProfileSectionsW(std::wstring section)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx); // ����

	std::map<std::wstring, std::wstring> result;

	uint32_t size = 0x256;
	wchar_t* buffer = new wchar_t[size];
	ZeroMemory(buffer, size);
	int res = 0;

	while (true)
	{
		res = GetPrivateProfileSectionW(section.c_str(), buffer, size, this->m_file.c_str());

		if (res == 0 || (res == (size - 2)))
		{
			size *= 2;
			delete[] buffer;
			buffer = new wchar_t[size];
			ZeroMemory(buffer, size);
		}
		else
		{
			break;
		}
	}

	std::wstring_view str(buffer, res);
	size_t pos = 0;

	while (pos < str.size())
	{
		size_t end_pos = str.find(L'\0', pos);
		if (end_pos == std::wstring_view::npos)
		{
			break;
		}

		std::wstring_view pair(&str[pos], end_pos - pos);
		size_t eq_pos = pair.find(L"=");

		if (eq_pos != std::wstring_view::npos)
		{
			std::wstring key(pair.data(), eq_pos);
			std::wstring value(pair.data() + eq_pos + 1, pair.size() - eq_pos - 1);
			result.emplace(key, value);
		}
		pos = end_pos + 1;
	}

	delete[] buffer;
	return result;
}

int32_t ProFileW::WriteProfileStringsW(std::wstring section, std::wstring key, std::wstring buffer)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx); // ����
	return WritePrivateProfileStringW(section.c_str(), key.c_str(), buffer.c_str(), this->m_file.c_str());
}

int32_t ProFileW::WriteProfileIntegerW(std::wstring section, std::wstring key, int64_t contect)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx); // ����
	wchar_t buffer[0x32]{ NULL };
	swprintf_s(buffer, L"%lld", contect);
	return WritePrivateProfileStringW(section.c_str(), key.c_str(), buffer, this->m_file.c_str());
}

int32_t ProFileW::WriteProfileSectonsW(std::wstring section, std::wstring buffer)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx); // ����
	return WritePrivateProfileSectionW(section.c_str(), buffer.c_str(), this->m_file.c_str());
}
