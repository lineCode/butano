/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_ALIGNMENT_H
#define BTN_ALIGNMENT_H

/**
 * @file
 * Alignment functions header file.
 *
 * @ingroup memory
 */

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Indicates if the given pointer is aligned to the given Bytes.
     *
     * @ingroup memory
     */
    template<int Bytes>
    [[nodiscard]] bool aligned(const void* pointer)
    {
        static_assert(Bytes > 0);

        return uintptr_t(pointer) % unsigned(Bytes) == 0;
    }

    /**
     * @brief Indicates if the given reference to a Type object is aligned to the given Bytes.
     *
     * @ingroup memory
     */
    template<int Bytes, typename Type>
    [[nodiscard]] bool aligned(const Type& reference)
    {
        static_assert(Bytes > 0);

        return alignof(Type) % unsigned(Bytes) == 0 ||
                uintptr_t(&reference) % unsigned(Bytes) == 0;
    }
}

#endif
