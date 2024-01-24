#include <string>
#include <codecvt>

std::wstring StringToWstring(const std::string& str)
{
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}