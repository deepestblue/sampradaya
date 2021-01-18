#include <exception>
#include <iostream>
#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>

using namespace std;

template <typename E>
void
throw_if_failed(
    bool exp,
    const E &e
) {
    if (exp)
        return;

    throw runtime_error(e());
}

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

int main(int argc, const char * argv[])
try {

    CTFontRef font = CTFontCreateWithName(CFSTR("Sampradaya"), 48, NULL);
    throw_if_failed(font);

    CFStringRef keys[] = { kCTFontAttributeName };
    CFTypeRef values[] = { font };
    CFDictionaryRef attr = CFDictionaryCreate(NULL, (const void **)&keys, (const void **)&values,
                          sizeof(keys) / sizeof(keys[0]), &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    throw_if_failed(attr);

    CFAttributedStringRef attrString = CFAttributedStringCreate(NULL, CFSTR("𑌶𑍍𑌰𑍀𑌃"), attr);
    throw_if_failed(attrString);
    
    CFRelease(attr);

    // Draw the string
    CTLineRef line = CTLineCreateWithAttributedString(attrString);
    throw_if_failed(line);

    CGContextRef context = CGBitmapContextCreate(NULL, 500, 150, 8, 0, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast);
    throw_if_failed(context);

    CGContextSetTextMatrix(context, CGAffineTransformIdentity);

    CGContextSetGrayFillColor(context, 1.0, 1.0);
    CGContextFillRect(context, CGRectMake(0, 0, 500, 150));
    CGContextSetTextPosition(context, 0, 100);
    CTLineDraw(line, context);

    auto image = CGBitmapContextCreateImage(context);
    throw_if_failed(image);

    CFStringRef path = CFStringCreateWithCString (NULL, "/tmp/foo.bmp", kCFStringEncodingUTF8);
    throw_if_failed(path);

    CFURLRef saveLocation = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, 0);
    throw_if_failed(saveLocation);

    CGImageDestinationRef imageDestination = CGImageDestinationCreateWithURL(saveLocation, kUTTypeBMP, 1, NULL);
    throw_if_failed(imageDestination);

    CGImageDestinationAddImage(imageDestination, image, NULL);
    CGImageDestinationFinalize(imageDestination);
    
    CFRelease(saveLocation);
    CFRelease(path);
    CFRelease(imageDestination);
    CFRelease(image);
    CFRelease(line);
    CFRelease(attrString);
    CFRelease(font);
    CFRelease(context);
} catch (const exception &e) {
    cerr << "Exception thrown: "s << e.what() << '\n';
}
