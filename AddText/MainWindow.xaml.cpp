#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "Utility/LoaderImage.h"
#include "Utility/UI.h"
#include "Utility/Window.h"

using namespace hbann;

namespace
{
constexpr auto TEXT_RIGHT_CLICK = L"Now Right Click Me :)";
} // namespace

namespace winrt::AddText::implementation
{
MainWindow::MainWindow() : mHWND(hbann::Window::GetHWND(*this))
{
    InitializeComponent();
    Initialize();
}

fire_and_forget MainWindow::Initialize()
{
    setLogLevel(LogLevel::LOG_LEVEL_WARNING);

    hbann::Window::Center(mHWND, {640, 480});

    // workaround for not being able to move elements around on constructor
    co_await resume_after(100ms);

    DispatcherQueue().TryEnqueue([this]() { CenterText(); });
}

void MainWindow::CenterText(std::optional<hstring> const &aText /* = {} */)
{
    if (aText)
    {
        text().Text(*aText);
    }

    canvas().SetLeft(text(), canvas().ActualWidth() / 2 - text().ActualWidth() / 2);
    canvas().SetTop(text(), canvas().ActualHeight() / 2 - text().ActualHeight() / 2);
}

IAsyncAction MainWindow::OnClickCMFIEdit(IInspectable const &aSender, RoutedEventArgs const &aArgs)
{
    // font size
    TextBlock sliderFontSizeHeader;
    sliderFontSizeHeader.Text(L"Font Size:");

    Slider sliderFontSize;
    sliderFontSize.Value(text().FontSize());
    sliderFontSize.Minimum(10.);
    sliderFontSize.Maximum(100.);

    // thickness
    TextBlock sliderThicknessHeader;
    sliderThicknessHeader.Text(L"Thickness:");

    Slider sliderThickness;
    sliderThickness.Value(text().FontWeight().Weight);
    sliderThickness.Minimum(100.);
    sliderThickness.Maximum(1000.);

    // color
    TextBlock colorHeader;
    colorHeader.Margin({0., 10., 0., 0.});
    colorHeader.Text(L"Color :");

    ColorPicker colorPicker;
    colorPicker.Orientation(Orientation::Horizontal);
    colorPicker.IsAlphaEnabled(true);
    colorPicker.Color(text().Foreground().as<SolidColorBrush>().Color());

    // text
    TextBlock inputTextHeader;
    inputTextHeader.Text(L"Text :");

    TextBox inputText;
    inputText.AcceptsReturn(true);
    inputText.Text(text().Text());
    inputText.TextWrapping(TextWrapping::Wrap);

    // content
    StackPanel content;
    content.Children().Append(sliderFontSizeHeader);
    content.Children().Append(sliderFontSize);
    content.Children().Append(sliderThicknessHeader);
    content.Children().Append(sliderThickness);
    content.Children().Append(colorHeader);
    content.Children().Append(colorPicker);
    content.Children().Append(inputTextHeader);
    content.Children().Append(inputText);

    ScrollViewer contentScroll;
    contentScroll.VerticalScrollBarVisibility(ScrollBarVisibility::Auto);
    contentScroll.Content(content);

    ContentDialog dialog;
    dialog.Title(box_value(L"Edit Text :"));
    dialog.Content(contentScroll);

    dialog.IsSecondaryButtonEnabled(true);
    dialog.PrimaryButtonText(L"OK");
    dialog.SecondaryButtonText(L"Cancel");

    dialog.XamlRoot(Content().XamlRoot());

    auto result = co_await dialog.ShowAsync();
    if (result != ContentDialogResult::Primary)
    {
        co_return;
    }

    text().FontSize(sliderFontSize.Value());
    text().Foreground(SolidColorBrush{colorPicker.Color()});
    text().Text(inputText.Text());
    text().FontWeight(Windows::UI::Text::FontWeight{static_cast<uint16_t>(sliderThickness.Value())});
}

/* static */ void MainWindow::PutText(cv::Mat &aImg, const string &aText, cv::Point aPoint, const int aMaxWidth,
                                      const int aFontFace /* = cv::FONT_HERSHEY_SIMPLEX */,
                                      const double aFontScale /* = 0.5 */, const cv::Scalar &aColor /* = {} */,
                                      const int aThickness /* = 2 */, const int aLineOffset /* = 5 */)
{
    if (aText.empty())
    {
        return;
    }

    // find the maximum chars that can fit in the maximum width
    size_t charsCount{};
    cv::Size textSize{};
    while (charsCount < aText.size() && textSize.width < aMaxWidth && aText[charsCount] != '\r')
    {
        textSize = cv::getTextSize(aText.substr(0, ++charsCount), aFontFace, aFontScale, aThickness, nullptr);
    }

    if (aText[charsCount] == '\r')
    {
        putText(aImg, aText.substr(0, charsCount), aPoint, aFontFace, aFontScale, aColor, aThickness);

        aPoint.y += textSize.height + aLineOffset;
        PutText(aImg, aText.substr(charsCount + 1), aPoint, aMaxWidth, aFontFace, aFontScale, aColor, aThickness);
    }
    else
    {
        // split by words if possible
        const auto spacePos = aText.rfind(' ', charsCount);
        if (spacePos != string::npos && textSize.width >= aMaxWidth)
        {
            charsCount = spacePos + 1;
        }

        putText(aImg, aText.substr(0, charsCount), aPoint, aFontFace, aFontScale, aColor, aThickness);

        aPoint.y += textSize.height + aLineOffset;
        PutText(aImg, aText.substr(charsCount), aPoint, aMaxWidth, aFontFace, aFontScale, aColor, aThickness);
    }
}

IAsyncAction MainWindow::OnClickCMFISave(IInspectable const &aSender, RoutedEventArgs const &aArgs)
{
    auto file(co_await hbann::UI::PickSaveFile(mHWND));
    if (!file)
    {
        co_return;
    }

    // translate to OpenCV
    auto imgBg(cv::imread(mFilePathImgBgLast));
    cv::resize(imgBg, imgBg, {static_cast<int>(canvas().ActualWidth()), static_cast<int>(canvas().ActualHeight())});

    auto textText(to_string(text().Text()));
    cv::Point point{static_cast<int>(canvas().GetLeft(text())), static_cast<int>(canvas().GetTop(text()))};
    auto textWidthMax(static_cast<int>(text().ActualWidth() * 1.14)); // hardcoded 14% more
    auto fontSize(text().FontSize() / 33.);                           // hardcoded WinUI3 font size to OpenCV
    auto thickness(text().FontWeight().Weight / 300);                 // hardcoded WinUI3 font weight to OpenCV

    auto colorWinUI3(text().Foreground().as<SolidColorBrush>().Color());
    cv::Scalar colorOpenCV{static_cast<double>(colorWinUI3.B), static_cast<double>(colorWinUI3.G),
                           static_cast<double>(colorWinUI3.R), static_cast<double>(colorWinUI3.A)};

    PutText(imgBg, textText, point, textWidthMax, 0, fontSize, colorOpenCV, thickness);

    cv::imwrite(to_string(file.Path()), imgBg, {cv::IMWRITE_JPEG_QUALITY, 100});
}

IAsyncAction MainWindow::OnPointerPressedText(IInspectable const &, PointerRoutedEventArgs const &aArgs)
{
    mIsDraggingText = true;
    mPointStartText = aArgs.GetCurrentPoint(text()).Position();
    co_return;
}

IAsyncAction MainWindow::OnPointerReleasedText(IInspectable const &, PointerRoutedEventArgs const &)
{
    mIsDraggingText = false;
    co_return;
}

IAsyncAction MainWindow::OnPointerMovedCanvas(IInspectable const &, PointerRoutedEventArgs const &aArgs)
{
    if (!mIsDraggingText)
    {
        co_return;
    }

    auto pointOffsetText = aArgs.GetCurrentPoint(text()).Position();
    pointOffsetText.X -= mPointStartText.X;
    pointOffsetText.Y -= mPointStartText.Y;

    canvas().SetLeft(text(), canvas().GetLeft(text()) + pointOffsetText.X);
    canvas().SetTop(text(), canvas().GetTop(text()) + pointOffsetText.Y);
}

IAsyncAction MainWindow::OnDragOverCanvas(IInspectable const &, DragEventArgs const &aArgs)
{
    aArgs.AcceptedOperation(DataPackageOperation::Copy);
    co_return;
}

IAsyncAction MainWindow::OnDropCanvas(IInspectable const &, DragEventArgs const &aArgs)
{
    if (!aArgs.DataView().Contains(StandardDataFormats::StorageItems()))
    {
        co_return;
    }

    vector<IStorageItem> files{};

    auto storageItems(co_await aArgs.DataView().GetStorageItemsAsync());
    for (const auto &storageItem : storageItems)
    {
        if (!storageItem.IsOfType(StorageItemTypes::File))
        {
            continue;
        }

        files.push_back(storageItem);
    }

    if (files.empty())
    {
        co_return;
    }

    auto file(files.front().as<StorageFile>());
    auto bmpImg(co_await LoaderImage::ToBmpImg(file));
    canvasBgImgBrush().ImageSource(bmpImg);

    mFilePathImgBgLast = to_string(file.Path());

    CenterText(TEXT_RIGHT_CLICK);
}
} // namespace winrt::AddText::implementation
