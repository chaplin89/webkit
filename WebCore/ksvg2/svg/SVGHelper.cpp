/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "config.h"
#ifdef SVG_SUPPORT
#include "SVGHelper.h"

#include "Document.h"
#include "FrameView.h"
#include "RenderView.h"
#include "SVGLength.h"
#include "SVGSVGElement.h"
#include "SVGStringList.h"
#include "ksvg.h"
#include <math.h>

using namespace std;

namespace WebCore {

float SVGHelper::PercentageOfViewport(float value, const SVGElement* viewportElement, LengthMode mode)
{
    float width = 0, height = 0;
    if (!viewportElement)
        return 0.0;
 
    if (viewportElement->isSVG()) {
        const SVGSVGElement* svg = static_cast<const SVGSVGElement*>(viewportElement);
        if (svg->hasAttribute(SVGNames::viewBoxAttr)) {
            width = svg->viewBox().width();
            height = svg->viewBox().height();
        } else if (svg->width()->unitType() == SVGLength::SVG_LENGTHTYPE_PERCENTAGE ||
                svg->height()->unitType() == SVGLength::SVG_LENGTHTYPE_PERCENTAGE) {
            // TODO: Shouldn't w/h be multiplied with the percentage values?!
            // AFAIK, this assumes width & height == 100%, Rob??
            Document *doc = svg->document();
            if (doc->documentElement() == svg) {
                // We have to ask the canvas for the full "canvas size"...
                RenderView* view = static_cast<RenderView *>(doc->renderer());
                if (view) {
                    width = view->frameView()->visibleWidth(); // TODO: recheck!
                    height = view->frameView()->visibleHeight(); // TODO: recheck!
                }
            }
        } else {
            width = svg->width()->value();
            height = svg->height()->value();
        }
    }

    if (mode == LM_WIDTH)
        return value * width;
    else if (mode == LM_HEIGHT)
        return value * height;
    else if (mode == LM_OTHER)
        return value * sqrt(pow(double(width), 2) + pow(double(height), 2)) / sqrt(2.0);
    
    return 0.0;
}

void SVGHelper::parseSeparatedList(SVGStringList *list, const String& data, UChar delimiter)
{
    // TODO : more error checking/reporting
    ExceptionCode ec = 0;
    list->clear(ec);

    Vector<String> substrings = String(data).split(delimiter);
    
    Vector<String>::const_iterator end = substrings.end();
    for (Vector<String>::const_iterator it = substrings.begin(); it != end; ++it)
        list->appendItem(*it, ec);
}

}

// vim:ts=4:noet
#endif // SVG_SUPPORT

