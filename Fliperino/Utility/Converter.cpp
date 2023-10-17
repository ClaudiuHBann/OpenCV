#include "pch.h"
#include "Converter.h"

namespace std
{
/* [[nodiscard]] */ wstring to_wstring_error(const DWORD aErrorCode) noexcept
{
  wchar_t * errorStrRaw{};
  auto      errorStrRawSize = FormatMessageW(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr, aErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (wchar_t *)&errorStrRaw, 0,
    nullptr);
  if (!errorStrRawSize)
  {
    TRACE("FormatMessageW failed with error code: " << GetLastError());
    return {};
  }

  wstring message(errorStrRaw, errorStrRawSize);
  LocalFree(errorStrRaw);
  return message;
}
}  // namespace std
