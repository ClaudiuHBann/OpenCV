#include "pch.h"
#include "LoaderImage.h"

namespace hbann
{
/* [[nodiscard]] static */ IAsyncOperation<BitmapImage> LoaderImage::ToBmpImg(const StorageFile &aFile)
{
    BitmapImage bitmap;
    bitmap.SetSource(co_await aFile.OpenAsync(FileAccessMode::Read));
    co_return bitmap;
}
} // namespace hbann
