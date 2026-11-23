/**
 * @file     font_display.h
 * @brief    字体显示组件 — C++ class
 *
 * 功能：把字体服务取出的 16x16 点阵字模经 LCD 操作回调直接画到屏幕，
 *       支持单字与字符串（自动换行）显示。
 *       LCD 操作回调由调用方注入，与具体显示驱动解耦。
 * 修改：2026-11-21 由 C API（font_display_char/font_display_string）重构为
 *       FontDisplay 静态类；字符串显示的屏宽改为参数传入（原硬编码 320）
 */
#ifndef FONT_DISPLAY_H
#define FONT_DISPLAY_H

#include <stdint.h>

#include "font_service.h"

/**
 * LCD 操作回调（由显示驱动实现并注入）
 */
struct FontLcdOps {
    /** 设置绘制窗口（含边界），坐标为像素 */
    void (*setWindow)(int x1, int y1, int x2, int y2);
    /** 写 16bit 像素数据（RGB565） */
    void (*writeData16)(const uint16_t *data, int len);
};

/**
 * 字体显示类（纯静态工具类）
 *
 * 用法：
 *   FontDisplay::setLcdOps(ops);
 *   FontDisplay::displayString(FontBuiltin::instance(), 0, 0, u"你好", 0xFFFF);
 */
class FontDisplay {
public:
    /** 注入 LCD 操作回调（须在显示前调用） */
    static void setLcdOps(const FontLcdOps &ops);

    /** 是否已注入 LCD 操作回调 */
    static bool isReady() { return m_ops.setWindow != nullptr && m_ops.writeData16 != nullptr; }

    /**
     * @brief  显示单个字符
     *
     * 功能：取字模 -> 逐位展开为 RGB565 缓冲 -> 经回调写屏
     *
     * @param font    字体服务实例
     * @param x       左上角 X（像素）
     * @param y       左上角 Y（像素）
     * @param unicode 字符 Unicode 码点
     * @param color   前景色 RGB565；背景固定黑色
     */
    static void displayChar(FontService &font, int x, int y, uint32_t unicode, uint16_t color);

    /**
     * @brief  显示字符串（UTF-16 码点数组，0 结尾）
     *
     * 功能：逐字显示，超出 maxWidth 自动换行，'\n' 强制换行
     *
     * @param font     字体服务实例
     * @param x        起始 X（像素）
     * @param y        起始 Y（像素）
     * @param str      Unicode 码点数组，0 结尾
     * @param color    前景色 RGB565
     * @param maxWidth 行宽上限（像素），超出换行
     */
    static void displayString(FontService &font, int x, int y, const uint16_t *str,
                              uint16_t color, int maxWidth);

private:
    static FontLcdOps m_ops;
};

#endif /* FONT_DISPLAY_H */
