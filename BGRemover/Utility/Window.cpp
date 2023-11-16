#include "pch.h"
#include "Window.h"
#include "MainWindow.xaml.h"

namespace hbann
{
/* [[nodiscard]] static */ ::HWND Window::GetHWND(const winrt::BGRemover::implementation::MainWindow &aMainWindow)
{
    auto iWindowNative(aMainWindow.try_as<::IWindowNative>());
    winrt::check_bool(iWindowNative);

    ::HWND hwnd{};
    iWindowNative->get_WindowHandle(&hwnd);
    return hwnd;
}

/* [[nodiscard]] static */ ::RECT Window::GetDesktopResolution() noexcept
{
    ::RECT resolution{};
    if (!::GetWindowRect(::GetDesktopWindow(), &resolution))
    {
        TRACE("Could not get the resolution of the desktop: " << errstr);
    }

    return resolution;
}

/* static */ void Window::Center(const ::HWND aHWND, const ::POINT &aSize /* = {} */) noexcept
{
    ::RECT rectHWND{};
    if (!::GetWindowRect(aHWND, &rectHWND))
    {
        TRACE("Could not get the rect of the window: " << errstr);
        return;
    }

    if (aSize.x && aSize.y)
    {
        // update new size
        rectHWND.right = aSize.x;
        rectHWND.bottom = aSize.y;
    }
    else
    {
        // set rectHWND right and bottom to represent the width and height of the window
        rectHWND.right -= rectHWND.left;
        rectHWND.bottom -= rectHWND.top;
    }

    auto rectHWNDNew = GetDesktopResolution();
    if (!rectHWNDNew.right || !rectHWNDNew.bottom)
    {
        return;
    }

    // calculate centered x and y coords and update new hwnd size
    rectHWNDNew.left = (rectHWNDNew.right - rectHWND.right) / 2;
    rectHWNDNew.top = (rectHWNDNew.bottom - rectHWND.bottom) / 2;
    rectHWNDNew.right = rectHWND.right;
    rectHWNDNew.bottom = rectHWND.bottom;

    if (!::SetWindowPos(aHWND, nullptr, rectHWNDNew.left, rectHWNDNew.top, rectHWNDNew.right, rectHWNDNew.bottom,
                        SWP_NOZORDER | SWP_NOACTIVATE))
    {
        TRACE("Could not center main window: " << errstr);
    }
}
} // namespace hbann
