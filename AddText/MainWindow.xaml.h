#pragma once

#include "MainWindow.g.h"

namespace winrt::AddText::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow();

    IAsyncAction OnPointerPressedText(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);
    IAsyncAction OnPointerReleasedText(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);

    IAsyncAction OnPointerMovedCanvas(IInspectable const &aSender, PointerRoutedEventArgs const &aArgs);
    IAsyncAction OnDragOverCanvas(IInspectable const &aSender, DragEventArgs const &aArgs);
    IAsyncAction OnDropCanvas(IInspectable const &aSender, DragEventArgs const &aArgs);

    // CMFI -> Context Menu Flyout Item
    IAsyncAction OnClickCMFIEdit(IInspectable const &aSender, RoutedEventArgs const &aArgs);
    IAsyncAction OnClickCMFISave(IInspectable const &aSender, RoutedEventArgs const &aArgs);

  private:
    HWND mHWND{};

    bool mIsDraggingText{};
    Point mPointStartText{};
    string mFilePathImgBgLast{};

    fire_and_forget Initialize();
    void CenterText(std::optional<hstring> const &aText = {});

    static void PutText(cv::Mat &aImg, const string &aText, cv::Point aPoint, const int aMaxWidth,
                        const int aFontFace = cv::FONT_HERSHEY_SIMPLEX, const double aFontScale = 0.5,
                        const cv::Scalar &aColor = {}, const int aThickness = 2, const int aLineOffset = 5);
};
} // namespace winrt::AddText::implementation

namespace winrt::AddText::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::AddText::factory_implementation
