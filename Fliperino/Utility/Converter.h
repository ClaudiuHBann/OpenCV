#pragma once

#define errstr std::to_wstring_error(GetLastError())

namespace std
{
[[nodiscard]] wstring to_wstring_error(const DWORD aErrorCode) noexcept;
}  // namespace std
