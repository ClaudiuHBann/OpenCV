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
