/**
 * @file     font_yuesong.h
 * @brief    跃宋字库字体服务 — C++ class
 *
 * 功能：使用编译进固件的跃宋字库（20902 个常用汉字，16x16 点阵），
 *       通过 Unicode -> GBK 索引映射表查字，字形为宋体风格。
 * 修改：2026-11-21 由 C API（font_yuesong_get_font）重构为 FontYuesong 类
 *
 * @note  数据文件约 700KB，默认不参与编译；需要时在 platformio.ini 的
 *        build_flags 中加 -DFONT_SERVICE_ENABLE_YUESONG
 */
#ifndef FONT_YUESONG_H
#define FONT_YUESONG_H

#include "font_service.h"

/* 跃宋字库数据（由 yuesong_font_data.bin / yuesong_unicode_to_gbk.bin 生成，
 * 见 font_yuesong_data.cpp） */
extern const uint8_t  yuesong_font_data[];
extern const uint16_t yuesong_unicode_to_gbk[];

/**
 * 跃宋字库字体类（单例）
 */
class FontYuesong : public FontService {
public:
    static constexpr int CHAR_COUNT = 20902; /*!< 字符数量 */

    /* 全局唯一实例 */
    static FontYuesong &instance();

    int getFont(uint32_t unicode, uint8_t *buf) override;
    int getMaxChars() const override { return CHAR_COUNT; }
    const char *name() const override { return "yuesong"; }

private:
    FontYuesong() = default;
};

#endif /* FONT_YUESONG_H */
