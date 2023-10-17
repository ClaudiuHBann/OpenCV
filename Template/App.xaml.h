#pragma once

#include "App.xaml.g.h"

namespace winrt::XXX::implementation
{
struct App : AppT<App>
{
    App();

    void OnLaunched(LaunchActivatedEventArgs const &);

  private:
    Window window{nullptr};
};
} // namespace winrt::XXX::implementation
