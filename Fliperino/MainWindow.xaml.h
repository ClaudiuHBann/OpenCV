#pragma once

#include "MainWindow.g.h"

namespace winrt::Fliperino::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow();

    IAsyncAction OnButtonClickSrc(IInspectable const &aSender, RoutedEventArgs const &aArgs);
    IAsyncAction OnButtonClickFliperino(IInspectable const &aSender, RoutedEventArgs const &aArgs);
    IAsyncAction OnButtonClickDest(IInspectable const &aSender, RoutedEventArgs const &aArgs);

  private:
    enum class Axis : int8_t
    {
        X = 1,
        Y = 0,
        XY = -1
    };

    HWND mHWND{};

    hstring mPathSrc{};
    hstring mPathDest{};

    [[nodiscard]] Axis GetSelectedAxis() noexcept;

    static void Fliperino(const path &aSrc, const path &aDest, const Axis aAxis,
                          const function<void(size_t, size_t)> aCallback) noexcept;
    //                                         total, current
};
} // namespace winrt::Fliperino::implementation

namespace winrt::Fliperino::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::Fliperino::factory_implementation
