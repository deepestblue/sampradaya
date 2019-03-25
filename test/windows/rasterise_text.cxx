#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <iomanip>
#include <vector>

#include "windows.h"
#include "objbase.h"
#include "d2d1.h"
#include "d2d1_1.h"
#include "d2d1helper.h"
#include "dwrite.h"
#include "wincodec.h"
#include "comdef.h"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

// cl rasterise_text.cxx /EHsc /std:c++latest /nologo /W4 /Zi d2d1.lib

using namespace std;

template<typename T, typename U>
void assert_and_throw(T exp, const U &what) {
    if (exp)
        return;

    throw runtime_error(what);
}

// Helper class for COM exceptions
class com_exception
    : public exception
    {
public:
    com_exception(HRESULT hr) : hresult(hr), com_error(hresult) {}

    virtual const char* what() const override
    {
        wstringstream s;
        s << setfill(L'0') << setw(8) << hex;
        s << "Failure with HRESULT of 0x" << static_cast<unsigned int>(hresult)
            << " (" << com_error.ErrorMessage() << " )\n";
        const wstring &message = s.str();

        int size = WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            message.c_str(),
            message.length(),
            nullptr,
            0,
            nullptr,
            nullptr
        );
        vector<char> result(size);
        WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            message.c_str(),
            message.length(),
            result.data(),
            size,
            nullptr,
            nullptr
        );
        return result.data();
    }

private:
    HRESULT hresult;
    _com_error com_error;
};

void ThrowIfFailed(HRESULT hr)
{
    if (SUCCEEDED(hr))
        return;

    throw com_exception(hr);
}

class Text_to_image_renderer {
public:
    Text_to_image_renderer()
//    , font("Sampradaya")
    {
        ThrowIfFailed(
            CoInitializeEx(
                nullptr,
                COINIT_MULTITHREADED
            )
        );
        ThrowIfFailed(
            CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&m_wicFactory)
            )
        );

        ThrowIfFailed(
            m_wicFactory->CreateBitmap(
                256,
                256,
                GUID_WICPixelFormat32bppBGR,
                WICBitmapCacheOnDemand,
                &wicBitMap
            )
        );

        const D2D1_FACTORY_OPTIONS options = {};
        ThrowIfFailed(
            D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED,
                __uuidof(ID2D1Factory1),
                &options,
                (void **)&d2dFactory
            )
        );

        const D2D1_RENDER_TARGET_PROPERTIES d2d1_render_target_properties = {
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,D2D1_ALPHA_MODE_IGNORE),
            0,
            0,
            D2D1_RENDER_TARGET_USAGE_NONE,
            D2D1_FEATURE_LEVEL_DEFAULT,
        };
        ThrowIfFailed(
            d2dFactory->CreateWicBitmapRenderTarget(
                wicBitMap,
                &d2d1_render_target_properties,
                &m_pRenderTarget
            )
        );
    }

    ~Text_to_image_renderer() {
        CoUninitialize();
    }

    void
    operator()(const string &text, const wstring &output_filename) {
        D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();

        m_pRenderTarget->BeginDraw();

        m_pRenderTarget->SetTransform(
            D2D1::Matrix3x2F::Identity()
        );

        m_pRenderTarget->Clear(
            D2D1::ColorF(D2D1::ColorF::Blue)
        );
/*
        MultiByteToWideChar
        m_pRenderTarget->DrawText(
            text,
            text.length(),
            m_pTextFormat,
            D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
            m_pBlackBrush
            );*/

        ThrowIfFailed(
            m_pRenderTarget->EndDraw()
        );

        IWICStream *stream;
        ThrowIfFailed(
            m_wicFactory->CreateStream(&stream)
        );
        ThrowIfFailed(
            stream->InitializeFromFilename(output_filename.c_str(), GENERIC_WRITE)
        );

        IWICBitmapEncoder *wicBitmapEncoder;
        ThrowIfFailed(
            m_wicFactory->CreateEncoder(
                GUID_ContainerFormatPng,
                nullptr,
                &wicBitmapEncoder
            )
        );

        ThrowIfFailed(
            wicBitmapEncoder->Initialize(
                stream,
                WICBitmapEncoderNoCache
            )
        );

        IWICBitmapFrameEncode *wicFrameEncode;
        ThrowIfFailed(
            wicBitmapEncoder->CreateNewFrame(
                &wicFrameEncode,
                nullptr
            )
        );
        ThrowIfFailed(
            wicFrameEncode->Initialize(nullptr)
        );
        ThrowIfFailed(
            wicFrameEncode->WriteSource(wicBitMap, nullptr)
        );
        ThrowIfFailed(
            wicFrameEncode->Commit()
        );
        ThrowIfFailed(
            wicBitmapEncoder->Commit()
        );
    }

private:
    IWICBitmap *wicBitMap;
    ID2D1Factory1 *d2dFactory;
    ID2D1RenderTarget *m_pRenderTarget;
    IWICImagingFactory2 *m_wicFactory;
};

int
wmain(int argc, wchar_t *argv[]) try {
    assert_and_throw(
        argc == 3,
        "Need 3 arguments"
    );
    const auto input_file = wstring{argv[1]};
    const auto output_dir = wstring{argv[2]};

    Text_to_image_renderer text_to_image_renderer{};

    auto input_stream = ifstream{input_file};
    auto line = string{};

    //
    // Starting at one, because technically 0 is a zero-digit number, so ostreaming a '0'
    // should be the empty-string, but it's not, so the output filename for the zeroth
    // file looks wrong.
    //
    auto i = 1u;
    while (getline(input_stream, line)) {
        if (line.empty())
            continue;
        text_to_image_renderer(line, output_dir + L"/" + to_wstring(i) + L".png");
        ++i;
    }

    return 0;
}
catch (const exception &e) {
    wcout << "Exception thrown: " << e.what() << endl;
}
catch (...) {
    wcout << "Something else thrown" << endl;
}
