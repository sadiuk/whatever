#include <string>
#include <codecvt>
namespace wtv
{
    std::wstring StringToWstring(const std::string& str)
    {
        return std::wstring(str.begin(), str.end());
    }
}