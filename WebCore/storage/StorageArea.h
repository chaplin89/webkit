/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef StorageArea_h
#define StorageArea_h

#if ENABLE(DOM_STORAGE)

#include "PlatformString.h"
#include "SecurityOrigin.h"
#include "StorageAreaSync.h"
#include "StorageMap.h"
#include "StorageSyncManager.h"

#include <wtf/Forward.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>

namespace WebCore {

    class Frame;
    class Page;
    class SecurityOrigin;
    class StorageAreaSync;
    class StorageMap;
    class StorageSyncManager;
    typedef int ExceptionCode;
    enum StorageType { LocalStorage, SessionStorage };

    // This interface is required for Chromium since these actions need to be proxied between processes.
    class StorageArea : public ThreadSafeShared<StorageArea> {
    public:
        static PassRefPtr<StorageArea> create(StorageType, SecurityOrigin*, PassRefPtr<StorageSyncManager>);
        virtual ~StorageArea() { }
        virtual PassRefPtr<StorageArea> copy(SecurityOrigin*) = 0;

        // The HTML5 DOM Storage API
        virtual unsigned length() const = 0;
        virtual String key(unsigned index, ExceptionCode& ec) const = 0;
        virtual String getItem(const String& key) const = 0;
        virtual void setItem(const String& key, const String& value, ExceptionCode& ec, Frame* sourceFrame) = 0;
        virtual void removeItem(const String& key, Frame* sourceFrame) = 0;
        virtual void clear(Frame* sourceFrame) = 0;

        virtual bool contains(const String& key) const = 0;
        virtual void close() = 0;

        // Could be called from a background thread.
        virtual void importItem(const String& key, const String& value) = 0;
        virtual SecurityOrigin* securityOrigin() = 0;
    };

} // namespace WebCore

#endif // ENABLE(DOM_STORAGE)

#endif // StorageArea_h
