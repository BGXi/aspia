//
// PROJECT:         Aspia
// FILE:            desktop_capture/win/screen_capture_utils.cc
// LICENSE:         GNU General Public License 3
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
// NOTE:            This file based on WebRTC source code
//

#include "desktop_capture/win/screen_capture_utils.h"

#include <QDebug>
#include <qt_windows.h>

namespace aspia {

// static
bool ScreenCaptureUtils::screenList(ScreenCapturer::ScreenList* screens)
{
    Q_ASSERT(screens->size() == 0U);

    for (int device_index = 0;; ++device_index)
    {
        DISPLAY_DEVICE device;
        device.cb = sizeof(device);

        // |enum_result| is 0 if we have enumerated all devices.
        if (!EnumDisplayDevicesW(nullptr, device_index, &device, 0))
            break;

        // We only care about active displays.
        if (!(device.StateFlags & DISPLAY_DEVICE_ACTIVE))
            continue;

        screens->push_back({device_index, QString::fromUtf16(
            reinterpret_cast<const ushort*>(device.DeviceName))});
    }

    return true;
}

// static
bool ScreenCaptureUtils::isScreenValid(ScreenCapturer::ScreenId screen, QString* device_key)
{
    if (screen == ScreenCapturer::kFullDesktopScreenId)
    {
        *device_key = QString();
        return true;
    }

    DISPLAY_DEVICE device;
    device.cb = sizeof(device);

    if (!EnumDisplayDevicesW(nullptr, screen, &device, 0))
        return false;

    *device_key = QString::fromUtf16(reinterpret_cast<const ushort*>(device.DeviceKey));
    return true;
}

// static
QRect ScreenCaptureUtils::fullScreenRect()
{
    return QRect(GetSystemMetrics(SM_XVIRTUALSCREEN),
                 GetSystemMetrics(SM_YVIRTUALSCREEN),
                 GetSystemMetrics(SM_CXVIRTUALSCREEN),
                 GetSystemMetrics(SM_CYVIRTUALSCREEN));
}

// static
QRect ScreenCaptureUtils::screenRect(ScreenCapturer::ScreenId screen, const QString& device_key)
{
    if (screen == ScreenCapturer::kFullDesktopScreenId)
        return fullScreenRect();

    DISPLAY_DEVICE device;
    device.cb = sizeof(device);
    if (!EnumDisplayDevicesW(nullptr, screen, &device, 0))
        return QRect();

    // Verifies the device index still maps to the same display device, to make sure we are
    // capturing the same device when devices are added or removed. DeviceKey is documented as
    // reserved, but it actually contains the registry key for the device and is unique for each
    // monitor, while DeviceID is not.
    if (wcscmp(device.DeviceKey, reinterpret_cast<const wchar_t*>(device_key.utf16())) != 0)
        return QRect();

    DEVMODE device_mode;
    device_mode.dmSize = sizeof(device_mode);
    device_mode.dmDriverExtra = 0;

    if (!EnumDisplaySettingsExW(device.DeviceName, ENUM_CURRENT_SETTINGS, &device_mode, 0))
        return QRect();

    return QRect(device_mode.dmPosition.x,
                 device_mode.dmPosition.y,
                 device_mode.dmPelsWidth,
                 device_mode.dmPelsHeight);
}

} // namespace aspia
