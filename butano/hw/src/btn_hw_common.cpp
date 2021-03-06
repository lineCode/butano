/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/btn_hw_common.h"

#include "btn_assert.h"

extern "C"
{
    // Disable __aeabi_atexit to reduce IWRAM usage:
    // https://arobenko.gitbooks.io/bare_metal_cpp/content/compiler_output/static.html
    int __aeabi_atexit(void*, void (*)(void*), void*)
    {
        return 0;
    }

    // Override __cxa_pure_virtual to reduce IWRAM usage:
    // https://stackoverflow.com/questions/14689639/can-i-disable-exceptions-for-when-a-pure-virtual-function-is-called
    void __cxa_pure_virtual()
    {
        BTN_ERROR("Pure virtual function call");
    }
}
