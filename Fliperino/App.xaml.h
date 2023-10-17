#pragma once

#include "App.xaml.g.h"

namespace winrt::Fliperino::implementation
{
struct App : AppT<App>
{
    App();

    void OnLaunched(LaunchActivatedEventArgs const &);

  private:
    Window window{nullptr};
};
} // namespace winrt::Fliperino::implementation
