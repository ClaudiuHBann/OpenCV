#pragma once

#include "App.xaml.g.h"

namespace winrt::BGRemover::implementation
{
struct App : AppT<App>
{
    App();

    void OnLaunched(LaunchActivatedEventArgs const &);

  private:
    Window window{nullptr};
};
} // namespace winrt::BGRemover::implementation
