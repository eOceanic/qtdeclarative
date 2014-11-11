/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QV4IDENTIFIERTABLE_H
#define QV4IDENTIFIERTABLE_H

#include "qv4identifier_p.h"
#include "qv4string_p.h"
#include "qv4engine_p.h"
#include <limits.h>

QT_BEGIN_NAMESPACE

namespace QV4 {

struct IdentifierTable
{
    ExecutionEngine *engine;

    int alloc;
    int size;
    int numBits;
    Heap::String **entries;

    void addEntry(Heap::String *str);

public:

    IdentifierTable(ExecutionEngine *engine);
    ~IdentifierTable();

    Heap::String *insertString(const QString &s);

    Identifier *identifier(const Heap::String *str) {
        if (str->identifier)
            return str->identifier;
        return identifierImpl(str);
    }
    Identifier *identifier(const QV4::String *str) {
        return identifier(str->d());
    }

    Identifier *identifier(const QString &s);
    Identifier *identifier(const char *s, int len);

    Identifier *identifierImpl(const Heap::String *str);

    void mark(ExecutionEngine *e) {
        for (int i = 0; i < alloc; ++i) {
            Heap::String *entry = entries[i];
            if (!entry || entry->markBit)
                continue;
            entry->markBit = 1;
            Q_ASSERT(entry->internalClass->vtable->markObjects);
            entry->internalClass->vtable->markObjects(entry, e);
        }
    }
};

}

QT_END_NAMESPACE

#endif
