#pragma once

namespace hbann
{
class LoaderImage
{
  public:
    [[nodiscard]] static IAsyncOperation<BitmapImage> ToBmpImg(const StorageFile &aFile);
};
} // namespace hbann
