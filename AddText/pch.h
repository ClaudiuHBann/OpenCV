#pragma once

// OpenCV
#pragma warning(push)
#pragma warning(disable : 5054)
#include <opencv2/opencv.hpp>
#pragma warning(pop)
#include <opencv2/core/utils/logger.hpp>

using namespace cv::utils::logging;

// win32
#include <ShObjIdl_core.h>
#include <Unknwn.h>

#undef GetCurrentTime

// winrt win32
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.UI.Core.h>

// winrt microsoft
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>

// winrt win32
using namespace winrt;
using namespace Windows;
using namespace Foundation;
using namespace Storage;
using namespace Pickers;
using namespace ApplicationModel::DataTransfer;

// winrt microsoft
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Media;
using namespace Imaging;
using namespace Controls;
using namespace Input;

// std
#include <functional>
#include <iostream>

using namespace std;

#define CATCH_EVERYTHING                                                                                               \
    catch (const std::exception &aException)                                                                           \
    {                                                                                                                  \
        std::cout << aException.what() << std::endl;                                                                   \
    }                                                                                                                  \
    catch (...)                                                                                                        \
    {                                                                                                                  \
        std::cout << "Unknown exception..." << std::endl;                                                              \
    }

// hbann
#include "Utility/Logger.h"
