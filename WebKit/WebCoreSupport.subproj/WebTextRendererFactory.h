/*	
    WebTextRendererFactory.m
    Copyright 2002, Apple, Inc. All rights reserved.
*/

#import <WebCore/WebCoreTextRendererFactory.h>
#import <WebKit/WebGlyphBuffer.h>
#import <WebFoundation/WebQueue.h>

@class WebTextRenderer;

@interface WebTextRendererFactory : WebCoreTextRendererFactory
{
    NSMutableDictionary *cache;
    NSMutableDictionary *viewBuffers;
    NSMutableArray *viewStack;
}

+ (void)createSharedFactory;
+ (WebTextRendererFactory *)sharedFactory;
- (NSFont *)cachedFontFromFamily:(NSString *)family traits:(NSFontTraitMask)traits size:(float)size;
- init;

- (WebTextRenderer *)rendererWithFont:(NSFont *)font;

- (BOOL)coalesceTextDrawing;
- (void)endCoalesceTextDrawing;
- (void)startCoalesceTextDrawing;

- (WebGlyphBuffer *)glyphBufferForFont: (NSFont *)font andColor: (NSColor *)color;

@end
