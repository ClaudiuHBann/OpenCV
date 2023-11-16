#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

namespace winrt::BGRemover::implementation
{
App::App()
{
    InitializeComponent();

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const &, UnhandledExceptionEventArgs const &aArgs) {
        if (IsDebuggerPresent())
        {
            auto errorMessage = aArgs.Message();
            __debugbreak();
        }
    });
#endif
}

void App::OnLaunched(LaunchActivatedEventArgs const &)
{
    window = make<MainWindow>();
    window.Activate();
}
} // namespace winrt::BGRemover::implementation
