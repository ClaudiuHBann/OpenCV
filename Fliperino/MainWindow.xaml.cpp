#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "Utility/UI.h"
#include "Utility/Window.h"

namespace winrt::Fliperino::implementation
{
MainWindow::MainWindow() : mHWND(hbann::Window::GetHWND(*this))
{
    InitializeComponent();

    hbann::Window::Center(mHWND, {690, 420});
}

IAsyncAction MainWindow::OnButtonClickSrc(IInspectable const &, RoutedEventArgs const &)
{
    auto storageFolder(co_await hbann::UI::PickFolder(mHWND));
    if (!storageFolder)
    {
        co_return;
    }

    mPathSrc = storageFolder.Path();
    textBlockSrc().Text(L"Source: " + mPathSrc);
}

IAsyncAction MainWindow::OnButtonClickFliperino(IInspectable const &, RoutedEventArgs const &)
{
    auto pathSrc(to_string(mPathSrc));
    auto pathDest(to_string(mPathDest));

    if (!exists(pathSrc) || !exists(pathDest))
    {
        co_await hbann::UI::ShowDlgInfo(Content(), L"Invalid folder(s) !",
                                        L"Please choose a valid source/destination folder.");
        co_return;
    }

    auto selectedAxis(GetSelectedAxis());
    buttonFliperino().IsEnabled(false);

    apartment_context threadUI;
    co_await resume_background();

    Fliperino(pathSrc, pathDest, selectedAxis, [this](const size_t aTotal, const size_t aCurrent) {
        const auto percentage = static_cast<double>(aCurrent) / static_cast<double>(aTotal) * 100.;
        const auto percentageStr = to_hstring(static_cast<size_t>(percentage)) + L"%";

        DispatcherQueue().TryEnqueue([this, percentage, percentageStr]() {
            progressBar().Value(percentage);
            progressBarPercentage().Text(percentageStr);
        });
    });

    co_await threadUI;
    buttonFliperino().IsEnabled(true);
}

IAsyncAction MainWindow::OnButtonClickDest(IInspectable const &, RoutedEventArgs const &)
{
    auto storageFolder(co_await hbann::UI::PickFolder(mHWND));
    if (!storageFolder)
    {
        co_return;
    }

    mPathDest = storageFolder.Path();
    textBlockDest().Text(L"Destination: " + mPathDest);
}

/* [[nodiscard]] */ MainWindow::Axis MainWindow::GetSelectedAxis() noexcept
{
    switch (comboBoxAxis().SelectedIndex())
    {
    case 0:
        return Axis::X;

    case 1:
        return Axis::Y;

    default:
        return Axis::XY;
    }
}

/* static */ void MainWindow::Fliperino(const path &aSrc, const path &aDest, const Axis aAxis,
                                        const function<void(size_t, size_t)> aCallback) noexcept
{
    try
    {
        vector<path> files{};
        for (const auto &file : recursive_directory_iterator{aSrc, directory_options::skip_permission_denied})
        {
            if (file.is_directory() ||
                !ranges::contains(OPENCV_IM_SUPPORTED_FILES, file.path().extension().string().c_str()))
            {
                continue;
            }

            // clear the source dir from file and add it
            auto filePathStr(file.path().string());
            filePathStr.replace(0, aSrc.string().size() + 1, "");
            files.push_back(filePathStr);
        }

        for (size_t i = 0; i < files.size(); i++)
        {
            Mat imageOut;
            auto fileSrc(aSrc / files[i]);
            flip(imread(fileSrc.string()), imageOut, static_cast<int>(aAxis));

            auto fileDest(aDest / files[i]);
            create_directories(fileDest.parent_path());
            assert(imwrite(fileDest.string(), imageOut));
            aCallback(files.size(), i + 1);
        }
    }
    CATCH_EVERYTHING;
}
} // namespace winrt::Fliperino::implementation
