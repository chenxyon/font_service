/**
 * @file     font_yuesong.cpp
 * @brief    跃宋字库字体服务实现 — C++ class
 *
 * 功能：通过 Unicode -> GBK 索引映射表从跃宋字库取字模
 * 修改：2026-11-21 由 C API（font_yuesong_get_font）重构为 FontYuesong 类
 */
#include "font_yuesong.h"

/* 跃宋字库约 700KB，默认不编译；数据与实现统一由本开关控制 */
#ifdef FONT_SERVICE_ENABLE_YUESONG

#include <string.h>

FontYuesong &FontYuesong::instance()
{
    static FontYuesong inst;
    return inst;
}

int FontYuesong::getFont(uint32_t unicode, uint8_t *buf)
{
    if (buf == NULL) {
        return -1;
    }

    /* Unicode 超出映射表范围 */
    if (unicode >= (uint32_t)CHAR_COUNT) {
        return -1;
    }

    /* Unicode -> GBK 索引 */
    uint16_t gbkIndex = yuesong_unicode_to_gbk[unicode];
    if (gbkIndex == 0xFFFF) {
        return -1; /* 该字符不存在 */
    }

    memcpy(buf, &yuesong_font_data[gbkIndex * BYTES], BYTES);
    return 0;
}

#else /* !FONT_SERVICE_ENABLE_YUESONG */

FontYuesong &FontYuesong::instance()
{
    static FontYuesong inst;
    return inst;
}

int FontYuesong::getFont(uint32_t unicode, uint8_t *buf)
{
    (void)unicode;
    (void)buf;
    /* 字库未编译进固件 */
    return -1;
}

#endif /* FONT_SERVICE_ENABLE_YUESONG */
