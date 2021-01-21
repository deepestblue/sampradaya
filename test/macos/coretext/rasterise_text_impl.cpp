#include <exception>
#include <iostream>
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
    unique_ptr<remove_pointer_t<CTFontRef>, decltype(&CFRelease)> font;

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
        CFStringRef keys[] = { kCTFontAttributeName };
        CFTypeRef values[] = { font.get() };
        CFDictionaryRef attr = CFDictionaryCreate(nullptr, reinterpret_cast<const void **>(&keys), reinterpret_cast<const void **>(&values), sizeof(keys) / sizeof(keys[0]), &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        throw_if_failed(attr);

        CFStringRef textAsCFString = CFStringCreateWithBytes(nullptr, reinterpret_cast<unsigned char *>(const_cast<char *>(text.c_str())), static_cast<long>(text.length()), kCFStringEncodingUTF8, false);
        throw_if_failed(textAsCFString);
        CFAttributedStringRef attrString = CFAttributedStringCreate(nullptr, textAsCFString, attr);
        throw_if_failed(attrString);

        CFRelease(attr);

        CTLineRef line = CTLineCreateWithAttributedString(attrString);
        throw_if_failed(line);

        CGContextRef context = CGBitmapContextCreate(nullptr, 500, 150, 8, 0, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast);
        throw_if_failed(context);

        CGContextSetGrayFillColor(context, 1.0, 1.0);
        CGContextFillRect(context, CGRectMake(0, 0, 500, 150));
        CGContextSetTextPosition(context, 0, 100);
        CTLineDraw(line, context);

        auto image = unique_ptr<remove_pointer_t<CGImageRef>, decltype(&CGImageRelease)>(
            CGBitmapContextCreateImage(context),
            CGImageRelease);
        throw_if_failed(image.get());

        auto path = unique_ptr<remove_pointer_t<CFStringRef>, decltype(&CFRelease)>(
            CFStringCreateWithCString(nullptr, output_filename.c_str(), kCFStringEncodingUTF8),
            [](const void *ref) { if (ref) CFRelease(ref); });
        throw_if_failed(path.get());

        auto destURL = unique_ptr<remove_pointer_t<CFURLRef>, decltype(&CFRelease)>(
            CFURLCreateWithFileSystemPath(nullptr, path.get(), kCFURLPOSIXPathStyle, 0),
            [](const void *ref) { if (ref) CFRelease(ref); });
        throw_if_failed(destURL.get());

        auto imageDestination = unique_ptr<remove_pointer_t<CGImageDestinationRef>, decltype(&CFRelease)>(
            CGImageDestinationCreateWithURL(destURL.get(), kUTTypePNG, 1, nullptr),
            [](const void *ref) { if (ref) CFRelease(ref); });
        throw_if_failed(imageDestination.get());

        CGImageDestinationAddImage(imageDestination.get(), image.get(), nullptr);
        throw_if_failed(CGImageDestinationFinalize(imageDestination.get()));

        CFRelease(line);
        CFRelease(attrString);
        CFRelease(context);
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
