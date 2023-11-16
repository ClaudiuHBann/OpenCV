#pragma once

#include "MainWindow.g.h"

namespace winrt::BGRemover::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow();

    IAsyncAction OnPointerPressedCanvas(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);
    IAsyncAction OnPointerReleasedCanvas(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);
    IAsyncAction OnPointerMovedCanvas(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);

    IAsyncAction OnPointerPressedROI(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);
    IAsyncAction OnPointerReleasedROI(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);
    IAsyncAction OnPointerMovedROI(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);

    IAsyncAction OnDragOverCanvas(IInspectable const &aSender, DragEventArgs const &aArgs);
    IAsyncAction OnDropCanvas(IInspectable const &aSender, DragEventArgs const &aArgs);

    // CMFI -> Context Menu Flyout Item
    IAsyncAction OnClickCMFIEdit(IInspectable const &aSender, RoutedEventArgs const &aArgs);
    IAsyncAction OnClickCMFISave(IInspectable const &aSender, RoutedEventArgs const &aArgs);

  private:
    HWND mHWND{};

    bool mIsDraggingCanvas{};
    Point mPointStartCanvas{};
    Point mPointEndCanvas{};

    bool mIsDraggingROI{};
    Point mPointStartROI{};

    fire_and_forget Initialize();

    hstring mPathLast{};
    hstring mPathSave{};
    double mThreshold{};
    Rect mROI{};
    Windows::UI::Color mColorBG{};

    void Translate();
};
} // namespace winrt::BGRemover::implementation

namespace winrt::BGRemover::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::BGRemover::factory_implementation
