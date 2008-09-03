// -*- mode: c++; c-basic-offset: 4 -*-
/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef BatchedTransitionOptimizer_h
#define BatchedTransitionOptimizer_h

#include <wtf/Noncopyable.h>
#include "JSObject.h"

namespace KJS {

    class BatchedTransitionOptimizer : Noncopyable {
    public:
        BatchedTransitionOptimizer(JSObject* object)
            : m_object(object)
        {
            if (!m_object->structureID()->isDictionary())
                m_object->setStructureID(StructureID::toDictionaryTransition(m_object->structureID()));
        }

        ~BatchedTransitionOptimizer()
        {
            m_object->setStructureID(StructureID::fromDictionaryTransition(m_object->structureID()));
        }

    private:
        JSObject* m_object;
    };

} // namespace KJS

#endif // BatchedTransitionOptimizer_h
