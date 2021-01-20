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
public:
    impl(int , char *[])
    { }

    void
    operator()(const string &text, const string &output_filename) {
        auto font = unique_ptr<remove_pointer_t<CTFontRef>, decltype(&CFRelease)>(
            CTFontCreateWithName(CFSTR("Sampradaya"), 48, nullptr),
            CFRelease
        );
        throw_if_failed(font.get());

        CFStringRef keys[] = { kCTFontAttributeName };
        CFTypeRef values[] = { font.get() };
        CFDictionaryRef attr = CFDictionaryCreate(nullptr, reinterpret_cast<const void **>(&keys), reinterpret_cast<const void **>(&values), sizeof(keys) / sizeof(keys[0]), &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        throw_if_failed(attr);

        CFStringRef textAsCFString = CFStringCreateWithBytes(nullptr, reinterpret_cast<unsigned char *>(const_cast<char *>(text.c_str())), static_cast<long>(text.length()), kCFStringEncodingUTF8, false);
        throw_if_failed(textAsCFString);
        CFAttributedStringRef attrString = CFAttributedStringCreate(nullptr, textAsCFString, attr);
        throw_if_failed(attrString);

        CFRelease(attr);

        // Draw the string
        CTLineRef line = CTLineCreateWithAttributedString(attrString);
        throw_if_failed(line);

        CGContextRef context = CGBitmapContextCreate(nullptr, 500, 150, 8, 0, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast);
        throw_if_failed(context);

        CGContextSetTextMatrix(context, CGAffineTransformIdentity);

        CGContextSetGrayFillColor(context, 1.0, 1.0);
        CGContextFillRect(context, CGRectMake(0, 0, 500, 150));
        CGContextSetTextPosition(context, 0, 100);
        CTLineDraw(line, context);

        auto image = CGBitmapContextCreateImage(context);
        throw_if_failed(image);

        CFStringRef path = CFStringCreateWithCString (nullptr, output_filename.c_str(), kCFStringEncodingUTF8);
        throw_if_failed(path);

        CFURLRef saveLocation = CFURLCreateWithFileSystemPath(nullptr, path, kCFURLPOSIXPathStyle, 0);
        throw_if_failed(saveLocation);

        CGImageDestinationRef imageDestination = CGImageDestinationCreateWithURL(saveLocation, kUTTypeBMP, 1, nullptr);
        throw_if_failed(imageDestination);

        CGImageDestinationAddImage(imageDestination, image, nullptr);
        CGImageDestinationFinalize(imageDestination);

        CFRelease(saveLocation);
        CFRelease(path);
        CFRelease(imageDestination);
        CFRelease(image);
        CFRelease(line);
        CFRelease(attrString);
        CFRelease(context);
    }

private:
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
