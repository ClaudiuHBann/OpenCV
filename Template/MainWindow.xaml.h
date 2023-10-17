#pragma once

#include "MainWindow.g.h"

namespace winrt::XXX::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow();
};
} // namespace winrt::XXX::implementation

namespace winrt::XXX::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::XXX::factory_implementation
