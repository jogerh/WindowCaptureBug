#include <iostream>
#include <unknwn.h>
#include <winrt/base.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <Windows.Graphics.Capture.Interop.h>
#include <windows.graphics.directx.direct3d11.interop.h>
#include <Windows.h>

using namespace winrt::Windows::Graphics::Capture;

LRESULT CALLBACK UniWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int main()
{
    init_apartment(winrt::apartment_type::multi_threaded);

    const auto className = L"MyWindowClass";

    WNDCLASSW UnicodeWndCls{};

    UnicodeWndCls.style = 0;
    UnicodeWndCls.lpfnWndProc = static_cast<WNDPROC>(UniWndProc);
    UnicodeWndCls.cbClsExtra = 0;
    UnicodeWndCls.cbWndExtra = 0;
    UnicodeWndCls.hInstance = GetModuleHandle(nullptr);
    UnicodeWndCls.hIcon = nullptr;
    UnicodeWndCls.hCursor = LoadCursor(nullptr, IDC_IBEAM);
    UnicodeWndCls.hbrBackground = nullptr;
    UnicodeWndCls.lpszMenuName = nullptr;
    UnicodeWndCls.lpszClassName = className;

    RegisterClassW(&UnicodeWndCls);

    const auto factory = winrt::get_activation_factory<GraphicsCaptureItem>();
    const auto interop = factory.as<IGraphicsCaptureItemInterop>();

    while (true) {
        std::wcerr << "Creating window" << std::endl;

        HWND hwnd = CreateWindowEx(0, className, L"MyTestWindow",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr, nullptr, GetModuleHandle(nullptr), nullptr
        );
        assert(IsWindow(hwnd));

        ShowWindow(hwnd, SW_MINIMIZE);
        SendMessage(hwnd, WM_PAINT, 0, 0);

        constexpr int maxRetries = 10;
        for (int retryNum = 0; retryNum < maxRetries; ++retryNum) {

            std::wcerr << "Creating capture item. Attempt number " << retryNum << ": ";

            GraphicsCaptureItem item = { nullptr };
            winrt::hresult status = interop->CreateForWindow(hwnd, winrt::guid_of<GraphicsCaptureItem>(),
                winrt::put_abi(item));

            if (status != S_OK)
                std::wcerr << "Failed to create capture item with error " << winrt::hresult_error(status).message().c_str() << std::endl;
            else {
                std::wcerr << "Success!" << std::endl;
                break;
            }
            Sleep(1);
        }

        DestroyWindow(hwnd);
        std::wcerr << "------- Done ------" << std::endl;
    }

    // Unreachable
    return 0;
}
