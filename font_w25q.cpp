/**
 * @file     font_w25q.cpp
 * @brief    W25Qxx 外挂 Flash 字库字体服务实现 — C++ class
 *
 * 功能：按 Unicode 偏移从外挂 W25Qxx Flash 实时读取字模
 * 修改：2026-11-21 由 C API（font_w25q_get_font，恒返回 -1 的空壳）
 *       重构为 FontW25q 类，并补全为经 W25qxx 驱动真实读 Flash 的实现
 */
#include "font_w25q.h"

#include "esp_log.h"
#include "w25qxx.h"

static const char *TAG = "font_w25q";

FontW25q &FontW25q::instance()
{
    static FontW25q inst;
    return inst;
}

void FontW25q::attach(W25qxx &flash, uint32_t baseAddr)
{
    m_flash = &flash;
    m_baseAddr = baseAddr;
    ESP_LOGI(TAG, "已挂接 W25Qxx 字库，基地址 0x%06lX，共 %lu 字",
             (unsigned long)m_baseAddr, (unsigned long)CHAR_COUNT);
}

int FontW25q::getFont(uint32_t unicode, uint8_t *buf)
{
    if (buf == NULL) {
        return -1;
    }
    if (m_flash == NULL) {
        ESP_LOGE(TAG, "未挂接 W25qxx 驱动，请先调用 attach()");
        return -1;
    }

    /* CJK 区外的字符不在字库中 */
    if (unicode < GLYPH_BASE_UNICODE) {
        return -1;
    }
    uint32_t index = unicode - GLYPH_BASE_UNICODE;
    if (index >= CHAR_COUNT) {
        return -1;
    }

    /* 字模地址 = 基地址 + 索引 * 每字字节数 */
    uint32_t addr = m_baseAddr + index * BYTES;
    if (!m_flash->read(addr, buf, BYTES)) {
        ESP_LOGE(TAG, "读 Flash 字模失败：地址 0x%06lX", (unsigned long)addr);
        return -1;
    }
    return 0;
}
