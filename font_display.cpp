/**
 * @file     font_display.cpp
 * @brief    字体显示组件实现 — C++ class
 *
 * 功能：把字体服务取出的 16x16 点阵字模经 LCD 操作回调直接画到屏幕
 * 修改：2026-11-21 由 C API（font_display_char/font_display_string）重构为
 *       FontDisplay 静态类；字符串显示的屏宽改为参数传入（原硬编码 320）
 */
#include "font_display.h"

#include <string.h>

#include "esp_log.h"

static const char *TAG = "font_display";

FontLcdOps FontDisplay::m_ops = {};

void FontDisplay::setLcdOps(const FontLcdOps &ops)
{
    m_ops = ops;
}

void FontDisplay::displayChar(FontService &font, int x, int y, uint32_t unicode, uint16_t color)
{
    if (!isReady()) {
        ESP_LOGE(TAG, "LCD 操作回调未注入，请先调用 setLcdOps()");
        return;
    }

    uint8_t fontData[FontService::BYTES];
    if (font.getFont(unicode, fontData) != 0) {
        ESP_LOGW(TAG, "字符 0x%04lX 不存在", (unsigned long)unicode);
        return;
    }

    /* 16x16 1bpp 位图展开为 RGB565 缓冲：位为 1 画前景色，否则画黑色 */
    uint16_t buf[FontService::WIDTH * FontService::HEIGHT];
    int idx = 0;
    for (int row = 0; row < FontService::HEIGHT; row++) {
        for (int col = 0; col < FontService::WIDTH; col++) {
            int byteIndex = row * 2 + col / 8;
            int bitIndex = 7 - (col % 8);
            buf[idx++] = (fontData[byteIndex] & (1 << bitIndex)) ? color : 0x0000;
        }
    }

    m_ops.setWindow(x, y, x + FontService::WIDTH - 1, y + FontService::HEIGHT - 1);
    m_ops.writeData16(buf, FontService::WIDTH * FontService::HEIGHT);
}

void FontDisplay::displayString(FontService &font, int x, int y, const uint16_t *str,
                                uint16_t color, int maxWidth)
{
    if (str == NULL) {
        return;
    }
    if (!isReady()) {
        ESP_LOGE(TAG, "LCD 操作回调未注入，请先调用 setLcdOps()");
        return;
    }

    int curX = x;
    int curY = y;
    for (const uint16_t *p = str; *p != 0; p++) {
        /* 强制换行 */
        if (*p == '\n') {
            curX = x;
            curY += FontService::HEIGHT;
            continue;
        }
        /* 超出行宽自动换行 */
        if (curX + FontService::WIDTH > maxWidth) {
            curX = x;
            curY += FontService::HEIGHT;
        }
        displayChar(font, curX, curY, *p, color);
        curX += FontService::WIDTH;
    }
}
