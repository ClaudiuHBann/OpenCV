#pragma once

namespace hbann
{
class UI
{
  public:
    [[nodiscard]] static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder> PickFolder(
        const ::HWND aHWND, const winrt::Windows::Storage::Pickers::PickerLocationId aLocation =
                                winrt::Windows::Storage::Pickers::PickerLocationId::Desktop);

    [[nodiscard]] static winrt::Windows::Foundation::IAsyncOperation<
        winrt::Microsoft::UI::Xaml::Controls::ContentDialogResult>
    ShowDlgInfo(const winrt::Microsoft::UI::Xaml::UIElement &aUIElement, const winrt::hstring &aTitle,
                const winrt::hstring &aContent, const winrt::hstring &aSecondaryButtonText = L"");
};
} // namespace hbann
