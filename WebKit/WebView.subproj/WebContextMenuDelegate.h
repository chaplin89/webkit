/*	
        WebContextMenuDelegate.h
	Copyright 2001, 2002, Apple Computer, Inc.

        Public header file.
*/

@class WebView;

/*!
    @category WebContextMenuDelegate
    @discussion WebContextMenuDelegate determine what context menu items are visible over
    a clicked element.
*/

/*!
    @enum WebMenuItemTag
    @discussion Each menu item in the default menu items array passed in
    contextMenuItemsForElement:defaultMenuItems: has its tag set to one of the WebMenuItemTags.
    When iterating through the default menu items array, use the tag to differentiate between them.
*/

enum {
    WebMenuItemTagOpenLinkInNewWindow=1,
    WebMenuItemTagDownloadLinkToDisk,
    WebMenuItemTagCopyLinkToClipboard,
    WebMenuItemTagOpenImageInNewWindow,
    WebMenuItemTagDownloadImageToDisk,
    WebMenuItemTagCopyImageToClipboard,
    WebMenuItemTagOpenFrameInNewWindow,
    WebMenuItemTagCopy
};

@interface NSObject (WebContextMenuDelegate)

/*!
    @method contextMenuItemsForElement:defaultMenuItems:
    @abstract Returns the menu items to display in an element's contextual menu.
    @param controller The WebController requesting the context menus.
    @param element A dictionary representation of the clicked element.
    @param defaultMenuItems An array of default NSMenuItems to include in all contextual menus.
    @result An array of NSMenuItems to include in the contextual menu.
*/
- (NSArray *)webView:(WebView *)webView contextMenuItemsForElement:(NSDictionary *)element defaultMenuItems:(NSArray *)defaultMenuItems;

@end



