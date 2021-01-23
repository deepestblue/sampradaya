#include <exception>
#include <iostream>
#include <array>
#include <filesystem>

#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>

using namespace std;

#include "../../rasterise_text.hpp"

const auto typeface_file_path = filesystem::path{"../../../src/Sampradaya.ttf"s};

auto
throw_if_failed(
    bool exp
) {
    const static auto l = []() {
        return "Assertion failed."s;
    };
    throw_if_failed(
        exp,
        l
    );
}

template<typename T, typename U>
auto
ConstCFReleaser(
    const U &u
) {
    auto ret = unique_ptr<const remove_pointer_t<T>, decltype(&CFRelease)>(
        u,
        [](const void *ref) {
            if (! ref) {
                return;
            }
            CFRelease(ref);
        }
    );
    throw_if_failed(
        ret.get()
    );
    return ret;
}

template<typename T, typename U, typename V>
auto
CFReleaser(
    const U &u,
    const V &v
) {
    auto ret = unique_ptr<remove_pointer_t<T>, decltype(v)>(
        u,
        v
    );
    throw_if_failed(
        ret.get()
    );
    return ret;
}

class Renderer::impl {
private:
    const unique_ptr<const remove_pointer_t<CTFontRef>, decltype(&CFRelease)> font;

    auto
    create_line(
        const string &text
    ) const {
        auto keys = array<const CFStringRef, 1>{
            kCTFontAttributeName
        };
        auto values = array<const CFTypeRef, keys.size()>{
            font.get()
        };
        const auto attr = ConstCFReleaser<CFDictionaryRef>(
            CFDictionaryCreate(
                nullptr,
                reinterpret_cast<const void **>(&keys),
                reinterpret_cast<const void **>(&values),
                keys.size(),
                &kCFTypeDictionaryKeyCallBacks,
                &kCFTypeDictionaryValueCallBacks
            )
        );

        const auto textAsCFString = ConstCFReleaser<CFStringRef>(
            CFStringCreateWithBytes(
                nullptr,
                reinterpret_cast<unsigned char *>(const_cast<char *>(text.c_str())),
                static_cast<long>(text.length()),
                kCFStringEncodingUTF8,
                false
            )
        );

        const auto attrString = ConstCFReleaser<CFAttributedStringRef>(
            CFAttributedStringCreate(
                nullptr,
                textAsCFString.get(),
                attr.get()
            )
        );

        auto line = ConstCFReleaser<CTLineRef>(
            CTLineCreateWithAttributedString(
                attrString.get()
            )
        );

        return line;
    }

public:
    impl(int , char *[])
    : font(
        ConstCFReleaser<CTFontRef>(
            CTFontCreateWithName(
                CFSTR("Sampradaya"),
                48u,
                nullptr
            )
        )
    ) {}

    auto
    operator()(
        const string &text,
        const string &output_filename
    ) const {
        const auto line = create_line(
            text
        );

        const auto context_guard = CFReleaser<CGContextRef>(
            CGBitmapContextCreate(
                nullptr,
                500u,
                150u,
                8u,
                0u,
                CGColorSpaceCreateDeviceRGB(),
                kCGImageAlphaPremultipliedLast
            ),
            CGContextRelease
        );
        const auto &context = context_guard.get();

        CGContextSetGrayFillColor(
            context,
            1.,
            1.
        );
        CGContextFillRect(
            context, CGRectMake(
                0u,
                0u,
                500u,
                150u
            )
        );
        CGContextSetTextPosition(
            context,
            0u,
            100u
        );
        CTLineDraw(
            line.get(),
            context
        );

        const auto image = CFReleaser<CGImageRef>(
            CGBitmapContextCreateImage(
                context
            ),
            CGImageRelease
        );

        const auto path = ConstCFReleaser<CFStringRef>(
            CFStringCreateWithCString(
                nullptr,
                output_filename.c_str(),
                kCFStringEncodingUTF8
            )
        );
        const auto destURL = ConstCFReleaser<CFURLRef>(
            CFURLCreateWithFileSystemPath(
                nullptr,
                path.get(),
                kCFURLPOSIXPathStyle,
                0
            )
        );

        const auto imageDestination = CFReleaser<CGImageDestinationRef>(
            CGImageDestinationCreateWithURL(
                destURL.get(),
                kUTTypePNG,
                1,
                nullptr
            ),
            [](const void *ref) { if (ref) CFRelease(ref); }
        );

        CGImageDestinationAddImage(
            imageDestination.get(),
            image.get(),
            nullptr
        );

        throw_if_failed(
            CGImageDestinationFinalize(
                imageDestination.get()
            )
        );
    }
};

Renderer::Renderer(
    int argc,
    char *argv[]
) : p_impl{
    std::make_unique<impl>(
        argc,
        argv
    )
} {}

void
Renderer::operator()(
    const string &text,
    const string &output_filename) const {
    (*p_impl)(
        text,
        output_filename
    );
}

Renderer::~Renderer() = default;
