/**
 * @file     font_builtin.h
 * @brief    内置字库字体服务 — C++ class
 *
 * 功能：使用编译进固件的内置字库（20902 个常用汉字，16x16 点阵），
 *       Unicode 码点直接作为字模索引，查表速度最快。
 * 修改：2026-11-21 由 C API（font_builtin_get_font）重构为 FontBuiltin 类
 */
#ifndef FONT_BUILTIN_H
#define FONT_BUILTIN_H

#include "font_service.h"

/* 内置字库数据（由 longyin_column_lsb_font_data.bin 生成，见 font_builtin_data.cpp） */
extern const uint8_t builtin_font_data[];

/**
 * 内置字库字体类（单例）
 */
class FontBuiltin : public FontService {
public:
    static constexpr int CHAR_COUNT = 20902; /*!< 字符数量 */

    /* 全局唯一实例 */
    static FontBuiltin &instance();

    int getFont(uint32_t unicode, uint8_t *buf) override;
    int getMaxChars() const override { return CHAR_COUNT; }
    const char *name() const override { return "builtin"; }

private:
    FontBuiltin() = default;
};

#endif /* FONT_BUILTIN_H */
