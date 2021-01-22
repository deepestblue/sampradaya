#include <exception>
#include <iostream>
#include <array>
#include <filesystem>

#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

#include "../../rasterise_text.hpp"

const auto typeface_file_path = filesystem::path{"../../../src/Sampradaya.ttf"s};

void
throw_if_failed(bool exp) {
    static auto l = []() {
        return "Assertion failed."s;
    };
    throw_if_failed(
        exp,
        l
    );
}

class Renderer::impl {
private:
    const unique_ptr<const remove_pointer_t<CTFontRef>, decltype(&CFRelease)> font;

    unique_ptr<const remove_pointer_t<CTLineRef>, decltype(&CFRelease)>
    create_line(const string &text) {
        auto keys = array<const CFStringRef, 1>{ kCTFontAttributeName };
        auto values = array<const CFTypeRef, 1>{ font.get() };
        const auto attr = unique_ptr<const remove_pointer_t<CFDictionaryRef>, decltype(&CFRelease)>(
            CFDictionaryCreate(nullptr, reinterpret_cast<const void **>(&keys), reinterpret_cast<const void **>(&values), sizeof(keys) / sizeof(keys[0]), &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks),
            [](const void *ref) { if (ref) CFRelease(ref); }
        );
        throw_if_failed(attr.get());

        const auto textAsCFString = unique_ptr<const remove_pointer_t<CFStringRef>, decltype(&CFRelease)>(
            CFStringCreateWithBytes(nullptr, reinterpret_cast<unsigned char *>(const_cast<char *>(text.c_str())), static_cast<long>(text.length()), kCFStringEncodingUTF8, false),
            [](const void *ref) { if (ref) CFRelease(ref); }
        );
        throw_if_failed(textAsCFString.get());

        const auto attrString = unique_ptr<const remove_pointer_t<CFAttributedStringRef>, decltype(&CFRelease)>(
            CFAttributedStringCreate(nullptr, textAsCFString.get(), attr.get()),
            [](const void *ref) { if (ref) CFRelease(ref); }
        );
        throw_if_failed(attrString.get());

        auto line = unique_ptr<const remove_pointer_t<CTLineRef>, decltype(&CFRelease)>(
            CTLineCreateWithAttributedString(attrString.get()),
            [](const void *ref) { if (ref) CFRelease(ref); }
        );
        throw_if_failed(line.get());

        return line;
    }

public:
    impl(int , char *[])
    : font(
        CTFontCreateWithName(CFSTR("Sampradaya"), 48, nullptr),
        [](CFTypeRef ref) { if (ref) CFRelease(ref); }
    ) {
        throw_if_failed(font.get());
    }

    void
    operator()(const string &text, const string &output_filename) {
        const auto line = create_line(text);

        const auto context_guard = unique_ptr<remove_pointer_t<CGContextRef>, decltype(&CGContextRelease)>(
            CGBitmapContextCreate(nullptr, 500, 150, 8, 0, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast),
            CGContextRelease);
        auto context = context_guard.get();
        throw_if_failed(context);

        CGContextSetGrayFillColor(context, 1.0, 1.0);
        CGContextFillRect(context, CGRectMake(0, 0, 500, 150));
        CGContextSetTextPosition(context, 0, 100);
        CTLineDraw(line.get(), context);

        const auto image = unique_ptr<remove_pointer_t<CGImageRef>, decltype(&CGImageRelease)>(
            CGBitmapContextCreateImage(context),
            CGImageRelease);
        throw_if_failed(image.get());

        const auto path = unique_ptr<const remove_pointer_t<CFStringRef>, decltype(&CFRelease)>(
            CFStringCreateWithCString(nullptr, output_filename.c_str(), kCFStringEncodingUTF8),
            [](const void *ref) { if (ref) CFRelease(ref); });
        throw_if_failed(path.get());

        const auto destURL = unique_ptr<const remove_pointer_t<CFURLRef>, decltype(&CFRelease)>(
            CFURLCreateWithFileSystemPath(nullptr, path.get(), kCFURLPOSIXPathStyle, 0),
            [](const void *ref) { if (ref) CFRelease(ref); });
        throw_if_failed(destURL.get());

        const auto imageDestination = unique_ptr<remove_pointer_t<CGImageDestinationRef>, decltype(&CFRelease)>(
            CGImageDestinationCreateWithURL(destURL.get(), kUTTypePNG, 1, nullptr),
            [](const void *ref) { if (ref) CFRelease(ref); });
        throw_if_failed(imageDestination.get());

        CGImageDestinationAddImage(imageDestination.get(), image.get(), nullptr);
        throw_if_failed(CGImageDestinationFinalize(imageDestination.get()));
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
    const string &output_filename) {
    (*p_impl)(
        text,
        output_filename
    );
}

Renderer::~Renderer() = default;
