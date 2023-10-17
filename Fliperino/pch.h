#pragma once

// win32
#include <ShObjIdl_core.h>
#include <Unknwn.h>

#undef GetCurrentTime

// winrt win32
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.UI.Core.h>

// winrt microsoft
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>

// winrt win32
using namespace winrt;
using namespace Windows;
using namespace Foundation;
using namespace Storage;
using namespace Pickers;

// winrt microsoft
using namespace Microsoft::UI::Xaml;
using namespace Controls;

// opencv
#pragma warning(push)
#pragma warning(disable : 5054)
#include <opencv2/imgcodecs.hpp>
#pragma warning(pop)

using namespace cv;

// std
#include <filesystem>
#include <iostream>

using namespace std;
using namespace filesystem;

constexpr const array OPENCV_IM_SUPPORTED_FILES = {
    ".bmp"sv, ".dib"sv, ".jpeg"sv, ".jpg"sv, ".jpe"sv, ".jp2"sv, ".png"sv,  ".webp"sv, ".avif"sv, ".pbm"sv, ".pgm"sv,
    ".ppm"sv, ".pxm"sv, ".pnm"sv,  ".pfm"sv, ".sr"sv,  ".ras"sv, ".tiff"sv, ".tif"sv,  ".exr"sv,  ".hdr"sv, ".pic"sv};

#define CATCH_EVERYTHING                                                                                               \
    catch (const exception &aException)                                                                                \
    {                                                                                                                  \
        cout << aException.what() << endl;                                                                             \
    }                                                                                                                  \
    catch (...)                                                                                                        \
    {                                                                                                                  \
        cout << "Unknown exception..." << endl;                                                                        \
    }

// hbann
#include "Utility/Logger.h"
