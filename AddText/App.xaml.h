#pragma once

#include "App.xaml.g.h"

namespace winrt::AddText::implementation
{
struct App : AppT<App>
{
    App();

    void OnLaunched(LaunchActivatedEventArgs const &);

  private:
    Window window{nullptr};
};
} // namespace winrt::AddText::implementation
