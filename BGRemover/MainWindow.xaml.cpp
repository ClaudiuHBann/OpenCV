#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "Utility/LoaderImage.h"
#include "Utility/UI.h"
#include "Utility/Window.h"

using namespace hbann;

namespace winrt::BGRemover::implementation
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

    DispatcherQueue().TryEnqueue([this]() {
        canvas().SetLeft(text(), canvas().ActualWidth() / 2 - text().ActualWidth() / 2);
        canvas().SetTop(text(), canvas().ActualHeight() / 2 - text().ActualHeight() / 2);
    });
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
    mPathLast = file.Path();
    auto bmpImg(co_await LoaderImage::ToBmpImg(file));
    canvasBgImgBrush().ImageSource(bmpImg);

    rectangleCanvas().Width(bmpImg.PixelWidth());
    rectangleCanvas().Height(bmpImg.PixelHeight());

    text().Visibility(Visibility::Collapsed);

    mROI = {0, 0, static_cast<float>(bmpImg.PixelWidth()), static_cast<float>(bmpImg.PixelHeight())};

    hbann::Window::Center(mHWND, {bmpImg.PixelWidth(), bmpImg.PixelHeight()});
}

IAsyncAction MainWindow::OnPointerPressedCanvas(IInspectable const &, PointerRoutedEventArgs const &aArgs)
{
    if (!aArgs.GetCurrentPoint(canvas()).Properties().IsLeftButtonPressed())
    {
        co_return;
    }

    // TODO: remove quick fix
    // this means that the ROI has been clicked and not the canvas
    if (mIsDraggingROI)
    {
        co_return;
    }

    mIsDraggingCanvas = true;
    mPointStartCanvas = aArgs.GetCurrentPoint(canvas()).Position();
}

IAsyncAction MainWindow::OnPointerReleasedCanvas(IInspectable const &, PointerRoutedEventArgs const &aArgs)
{
    if (!aArgs.GetCurrentPoint(canvas()).Properties().IsLeftButtonPressed())
    {
        co_return;
    }

    mIsDraggingCanvas = false;
    mPointEndCanvas = aArgs.GetCurrentPoint(canvas()).Position();
    mROI = {static_cast<float>(canvas().GetLeft(rectangleROI())), static_cast<float>(canvas().GetTop(rectangleROI())),
            static_cast<float>(rectangleROI().ActualWidth()), static_cast<float>(rectangleROI().ActualHeight())};

    co_return;
}

IAsyncAction MainWindow::OnPointerMovedCanvas(IInspectable const &, PointerRoutedEventArgs const &aArgs)
{
    if (!mIsDraggingCanvas)
    {
        co_return;
    }

    // we need to update the end point of the ROI so the select is updating in real time
    mPointEndCanvas = aArgs.GetCurrentPoint(canvas()).Position();

    auto x = min(mPointEndCanvas.X, mPointStartCanvas.X);
    auto y = min(mPointEndCanvas.Y, mPointStartCanvas.Y);

    canvas().SetLeft(rectangleROI(), x);
    canvas().SetTop(rectangleROI(), y);

    auto width = max(mPointEndCanvas.X, mPointStartCanvas.X) - x;
    auto height = max(mPointEndCanvas.Y, mPointStartCanvas.Y) - y;

    rectangleROI().Width(width);
    rectangleROI().Height(height);
}

IAsyncAction MainWindow::OnClickCMFIEdit(IInspectable const &aSender, RoutedEventArgs const &)
{
    // color
    TextBlock colorHeader;
    colorHeader.Margin({0., 10., 0., 0.});
    colorHeader.Text(L"Color :");

    ColorPicker colorPicker;
    colorPicker.Orientation(Orientation::Horizontal);
    colorPicker.Color(rectangleROI().Fill().as<SolidColorBrush>().Color());

    // threshold
    TextBlock thresholdHeader;
    thresholdHeader.Margin({0., 10., 0., 0.});
    thresholdHeader.Text(L"Threshold :");

    Slider sliderThreshold;
    sliderThreshold.Value(192.);
    sliderThreshold.Minimum(0.);
    sliderThreshold.Maximum(255.);

    // content
    StackPanel content;
    content.Children().Append(colorHeader);
    content.Children().Append(colorPicker);
    content.Children().Append(thresholdHeader);
    content.Children().Append(sliderThreshold);

    ScrollViewer contentScroll;
    contentScroll.VerticalScrollBarVisibility(ScrollBarVisibility::Auto);
    contentScroll.Content(content);

    ContentDialog dialog;
    dialog.Title(box_value(L"Edit BG:"));
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

    mColorBG = colorPicker.Color();
    mThreshold = sliderThreshold.Value();

    // TODO: blend with the image
    auto color = colorPicker.Color();
    color.A = 69;
    rectangleCanvas().Fill(SolidColorBrush(color));
}

IAsyncAction MainWindow::OnPointerPressedROI(IInspectable const &, PointerRoutedEventArgs const &aArgs)
{
    if (!aArgs.GetCurrentPoint(rectangleROI()).Properties().IsLeftButtonPressed())
    {
        co_return;
    }

    mIsDraggingROI = true;
    mPointStartROI = aArgs.GetCurrentPoint(rectangleROI()).Position();

    co_return;
}

IAsyncAction MainWindow::OnPointerReleasedROI(IInspectable const &, PointerRoutedEventArgs const &aArgs)
{
    if (!aArgs.GetCurrentPoint(rectangleROI()).Properties().IsLeftButtonPressed())
    {
        co_return;
    }

    mIsDraggingROI = false;
    mROI = {static_cast<float>(canvas().GetLeft(rectangleROI())), static_cast<float>(canvas().GetTop(rectangleROI())),
            static_cast<float>(rectangleROI().ActualWidth()), static_cast<float>(rectangleROI().ActualHeight())};

    co_return;
}

IAsyncAction MainWindow::OnPointerMovedROI(IInspectable const &, PointerRoutedEventArgs const &aArgs)
{
    if (!mIsDraggingROI)
    {
        co_return;
    }

    auto pointOffsetROI = aArgs.GetCurrentPoint(rectangleROI()).Position();
    pointOffsetROI.X -= mPointStartROI.X;
    pointOffsetROI.Y -= mPointStartROI.Y;

    auto x = canvas().GetLeft(rectangleROI()) + pointOffsetROI.X;
    auto y = canvas().GetTop(rectangleROI()) + pointOffsetROI.Y;

    // clamp x the ROI
    if (x < 0)
    {
        x = 0;
    }

    if (x + rectangleROI().Width() > canvas().ActualWidth())
    {
        x = canvas().ActualWidth() - rectangleROI().Width();
    }

    // clamp y the ROI
    if (y < 0)
    {
        y = 0;
    }

    if (y + rectangleROI().Height() > canvas().ActualHeight())
    {
        y = canvas().ActualHeight() - rectangleROI().Height();
    }

    canvas().SetLeft(rectangleROI(), x);
    canvas().SetTop(rectangleROI(), y);
}

IAsyncAction MainWindow::OnClickCMFISave(IInspectable const &, RoutedEventArgs const &)
{
    auto file(co_await hbann::UI::PickSaveFile(mHWND));
    if (!file)
    {
        co_return;
    }

    mPathSave = file.Path();

    Translate();
}

void MainWindow::Translate()
{
    try
    {
        // BGR and gray image
        auto image(cv::imread(to_string(mPathLast)));

        // roi
        cv::Rect roi(static_cast<int>(mROI.X), static_cast<int>(mROI.Y), static_cast<int>(mROI.Width),
                     static_cast<int>(mROI.Height));

        auto imageROI(image(roi));
        cv::Mat imageROIGray;
        cv::cvtColor(imageROI, imageROIGray, cv::COLOR_BGR2GRAY);

        // threshold
        cv::Mat threshold;
        cv::threshold(imageROIGray, threshold, mThreshold, 255, cv::THRESH_BINARY);

        // create the final image
        cv::Mat result(image.size(), image.type(), cv::Scalar(mColorBG.B, mColorBG.G, mColorBG.R));
        imageROI.copyTo(result(roi), ~threshold);

        cv::imwrite(to_string(mPathSave), result);
    }
    CATCH_EVERYTHING;
}
} // namespace winrt::BGRemover::implementation
