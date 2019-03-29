#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <iomanip>
#include <vector>

#include "comdef.h"
#include "d2d1_1.h"
#include "dwrite.h"
#include "wincodec.h"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

// cl rasterise_text.cxx /EHsc /std:c++latest /nologo /W4 /Zi d2d1.lib dwrite.lib

using namespace std;

template<typename T, typename U>
void
assert_and_throw(
    T exp,
    const U &what
) {
    if (exp)
        return;

    throw runtime_error(what);
}

// Helper class for COM exceptions
class Com_exception
    : public exception {
public:
    Com_exception(HRESULT hr)
    : hresult(hr), com_error(hresult) { }

    virtual const char *
    what() const override {
        auto s = wstringstream{};
        s << L"Failure with HRESULT of 0x";
        s << setfill(L'0') << setw(8) << hex << static_cast<unsigned int>(hresult);
        s << setw(0) << " (" << com_error.ErrorMessage() << " )\n";
        const auto &utf16_message = s.str();

        auto required_size = WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            utf16_message.c_str(),
            utf16_message.length(),
            nullptr,
            0,
            nullptr,
            nullptr
        );
        auto message = vector<char>(required_size);
        WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            utf16_message.c_str(),
            utf16_message.length(),
            message.data(),
            required_size,
            nullptr,
            nullptr
        );

        return message.data();
    }

private:
    HRESULT hresult;
    _com_error com_error;
};

// Helper class for Win32 exceptions
class GLE_exception
    : public exception {
public:
    GLE_exception()
    : last_error(GetLastError()) { }

    virtual const char *
    what() const override {
        auto buffer = static_cast<char *>(nullptr);
        auto len = FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            last_error,
            MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
            buffer,
            0,
            nullptr
        );
        auto error_string = string{buffer, len};
        LocalFree(buffer);

        return error_string.c_str();
    }

private:
    int last_error;
};

void
throw_if_failed(HRESULT hr) {
    if (SUCCEEDED(hr))
        return;

    throw Com_exception(hr);
}

void
throw_if_failed(int return_code) {
    if (return_code > 0)
        return;

    throw GLE_exception();
}

class Text_to_image_renderer {
public:
    Text_to_image_renderer()
    {
        throw_if_failed(
            CoInitializeEx(
                nullptr,
                COINIT_MULTITHREADED
            )
        );
        throw_if_failed(
            CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&wic_factory)
            )
        );

        throw_if_failed(
            wic_factory->CreateBitmap(
                1000,
                72,
                GUID_WICPixelFormat32bppBGR,
                WICBitmapCacheOnDemand,
                &wic_bitmap
            )
        );

        auto options = D2D1_FACTORY_OPTIONS{};
        throw_if_failed(
            D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED,
                __uuidof(ID2D1Factory1),
                &options,
                (void **)&d2d_factory
            )
        );

        auto render_props = D2D1_RENDER_TARGET_PROPERTIES{
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,D2D1_ALPHA_MODE_IGNORE),
            0,
            0,
            D2D1_RENDER_TARGET_USAGE_NONE,
            D2D1_FEATURE_LEVEL_DEFAULT,
        };
        throw_if_failed(
            d2d_factory->CreateWicBitmapRenderTarget(
                wic_bitmap,
                &render_props,
                &render_target
            )
        );

        throw_if_failed(
            render_target->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &black_brush
            )
        );

        throw_if_failed(
            DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_SHARED,
                __uuidof(dwrite_factory),
                reinterpret_cast<IUnknown **>(&dwrite_factory)
            )
        );

        throw_if_failed(
            dwrite_factory->CreateTextFormat(
                L"Sampradaya",
                nullptr,
                DWRITE_FONT_WEIGHT_NORMAL,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                5000.f/96,
                L"", //locale
                &text_format
            )
        );
    }

    ~Text_to_image_renderer() {
        CoUninitialize();
    }

    void
    operator ()(
        const string &text,
        const wstring &output_filename
    ) {
        render_target->BeginDraw();

        render_target->SetTransform(
            D2D1::Matrix3x2F::Identity()
        );

        render_target->Clear(
            D2D1::ColorF(D2D1::ColorF::White)
        );

        auto buf_size = MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            text.c_str(),
            text.length(),
            nullptr,
            0
        );
        auto utf16_text = wstring(buf_size, 0);
        throw_if_failed(
            MultiByteToWideChar(
                CP_UTF8,
                MB_ERR_INVALID_CHARS,
                text.c_str(),
                text.length(),
                utf16_text.data(),
                buf_size
            )
        );

        auto render_target_size = render_target->GetSize();
        render_target->DrawText(
            utf16_text.c_str(),
            utf16_text.length(),
            text_format,
            D2D1::RectF(0, 0, render_target_size.width, render_target_size.height),
            black_brush
            );

        throw_if_failed(
            render_target->EndDraw()
        );

        auto stream = static_cast<IWICStream *>(nullptr);
        throw_if_failed(
            wic_factory->CreateStream(&stream)
        );
        throw_if_failed(
            stream->InitializeFromFilename(
                output_filename.c_str(),
                GENERIC_WRITE
            )
        );

        {
            auto wic_bitmap_encoder = static_cast<IWICBitmapEncoder *>(nullptr);
            throw_if_failed(
                wic_factory->CreateEncoder(
                    GUID_ContainerFormatPng,
                    nullptr,
                    &wic_bitmap_encoder
                )
            );

            throw_if_failed(
                wic_bitmap_encoder->Initialize(
                    stream,
                    WICBitmapEncoderNoCache
                )
            );

            {
                auto wic_frame_encode = static_cast<IWICBitmapFrameEncode *>(nullptr);
                throw_if_failed(
                    wic_bitmap_encoder->CreateNewFrame(
                        &wic_frame_encode,
                        nullptr
                    )
                );
                throw_if_failed(
                    wic_frame_encode->Initialize(
                        nullptr
                    )
                );
                throw_if_failed(
                    wic_frame_encode->WriteSource(
                        wic_bitmap,
                        nullptr
                    )
                );
                throw_if_failed(
                    wic_frame_encode->Commit()
                );
            }

            throw_if_failed(
                wic_bitmap_encoder->Commit()
            );
        }
    }

private:
    ID2D1Factory1 *d2d_factory;
    ID2D1RenderTarget *render_target;
    ID2D1SolidColorBrush *black_brush;
    IDWriteFactory *dwrite_factory;
    IWICImagingFactory2 *wic_factory;
    IWICBitmap *wic_bitmap;
    IDWriteTextFormat *text_format;
};

int
wmain(
    int argc,
    wchar_t *argv[]
) try {
    assert_and_throw(
        argc == 3,
        "Need 3 arguments"
    );
    const auto input_file = wstring{argv[1]};
    const auto output_dir = wstring{argv[2]};

    auto renderer = Text_to_image_renderer{};

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
        renderer(line, output_dir + L"/" + to_wstring(i) + L".png");
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
