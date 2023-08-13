# WindowCaptureBug

The function winrt::Windows::Graphics::Capture::IGraphicsCaptureItemInterop::CreateForWindow appears to have a bug where it fails to create capture items immediately after a window was shown. Only after a sleep, the function succeeds.

This example creates windows in a loop. For each iteration, an attemt is made to create a `winrt::Windows::Graphics::Capture::GraphicsCaptureItem` instance using the `winrt::Windows::Graphics::Capture::IGraphicsCaptureItemInterop::CreateForWindow` function. If the attempt fails, a new attempt is made after a short sleep.

**Expected behavior**
winrt::Windows::Graphics::Capture::IGraphicsCaptureItemInterop::CreateForWindow succeeds on the first attempt

**Actual behavior**
winrt::Windows::Graphics::Capture::IGraphicsCaptureItemInterop::CreateForWindow often fails on the first attempt
