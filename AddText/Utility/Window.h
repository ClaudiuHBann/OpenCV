#pragma once

namespace winrt::AddText::implementation
{
struct MainWindow;
}

namespace hbann
{
class Window
{
  public:
    [[nodiscard]] static ::HWND GetHWND(const winrt::AddText::implementation::MainWindow &aMainWindow);

    [[nodiscard]] static ::RECT GetDesktopResolution() noexcept;

    static void Center(const ::HWND aHWND, const ::POINT &aSize = {}) noexcept;
};
} // namespace hbann
