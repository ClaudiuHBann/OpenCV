#include "pch.h"
#include "UI.h"

namespace hbann
{
/* [[nodiscard]] static */ winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder>
PickFolder(const ::HWND aHWND, const winrt::Windows::Storage::Pickers::PickerLocationId aLocation /* =
                                   winrt::Windows::Storage::Pickers::PickerLocationId::Desktop */)
{
    winrt::Windows::Storage::Pickers::FolderPicker picker;
    picker.as<::IInitializeWithWindow>()->Initialize(aHWND);
    picker.SuggestedStartLocation(aLocation);
    picker.FileTypeFilter().Append(L"*");

    co_return co_await picker.PickSingleFolderAsync();
}

/* [[nodiscard]] static */
winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Xaml::Controls::ContentDialogResult> ShowDlgInfo(
    const winrt::Microsoft::UI::Xaml::UIElement &aUIElement, const winrt::hstring &aTitle,
    const winrt::hstring &aContent, const winrt::hstring &aSecondaryButtonText /* = L"" */)
{
    winrt::Microsoft::UI::Xaml::Controls::ContentDialog dialog;
    dialog.Content(winrt::box_value(aContent));
    dialog.Title(winrt::box_value(aTitle));

    dialog.PrimaryButtonText(L"OK");
    if (!aSecondaryButtonText.empty())
    {
        dialog.SecondaryButtonText(aSecondaryButtonText);
    }

    dialog.XamlRoot(aUIElement.XamlRoot());
    co_return co_await dialog.ShowAsync();
}
} // namespace hbann
