/**
 * @file     font_builtin.cpp
 * @brief    内置字库字体服务实现 — C++ class
 *
 * 功能：使用编译进固件的内置字库，Unicode 码点直接作为字模索引
 * 修改：2026-11-21 由 C API（font_builtin_get_font）重构为 FontBuiltin 类
 */
#include "font_builtin.h"

#include <string.h>

FontBuiltin &FontBuiltin::instance()
{
    static FontBuiltin inst;
    return inst;
}

int FontBuiltin::getFont(uint32_t unicode, uint8_t *buf)
{
    if (buf == NULL) {
        return -1;
    }

    /* 内置字库按 Unicode 码点直接索引 */
    if (unicode >= (uint32_t)CHAR_COUNT) {
        return -1;
    }

    memcpy(buf, &builtin_font_data[unicode * BYTES], BYTES);
    return 0;
}
