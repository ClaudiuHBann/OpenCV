#pragma once

namespace hbann
{
class UI
{
  public:
    [[nodiscard]] static IAsyncOperation<StorageFolder> PickFolder(
        const HWND aHWND, const PickerLocationId aLocation = PickerLocationId::Desktop);

    [[nodiscard]] static IAsyncOperation<ContentDialogResult> ShowDlgInfo(const UIElement &aUIElement,
                                                                          const hstring &aTitle,
                                                                          const hstring &aContent,
                                                                          const hstring &aSecondaryButtonText = L"");
};
} // namespace hbann
