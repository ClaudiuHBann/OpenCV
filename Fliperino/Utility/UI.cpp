#include "pch.h"
#include "UI.h"

namespace hbann
{
/* [[nodiscard]] static */ IAsyncOperation<StorageFolder> UI::PickFolder(
    const HWND aHWND, const PickerLocationId aLocation /* = PickerLocationId::Desktop */)
{
    FolderPicker picker;
    picker.as<::IInitializeWithWindow>()->Initialize(aHWND);
    picker.SuggestedStartLocation(aLocation);
    picker.FileTypeFilter().Append(L"*");

    co_return co_await picker.PickSingleFolderAsync();
}

/* [[nodiscard]] static */ IAsyncOperation<ContentDialogResult> UI::ShowDlgInfo(
    const UIElement &aUIElement, const hstring &aTitle, const hstring &aContent,
    const hstring &aSecondaryButtonText /* = L"" */)
{
    ContentDialog dialog;
    dialog.Content(box_value(aContent));
    dialog.Title(box_value(aTitle));

    dialog.PrimaryButtonText(L"OK");
    if (!aSecondaryButtonText.empty())
    {
        dialog.SecondaryButtonText(aSecondaryButtonText);
    }

    dialog.XamlRoot(aUIElement.XamlRoot());
    co_return co_await dialog.ShowAsync();
}
} // namespace hbann
