/**
 * @file     font_w25q.h
 * @brief    W25Qxx 外挂 Flash 字库字体服务 — C++ class
 *
 * 功能：字库存储在外挂 W25Qxx SPI NOR Flash 中（21003 字，16x16 点阵），
 *       按 Unicode 偏移实时读 Flash，固件零字库占用。
 *       使用前须经 attach() 挂接已初始化的 W25qxx 驱动实例。
 * 修改：2026-11-21 由 C API（font_w25q_get_font，恒返回 -1 的空壳）
 *       重构为 FontW25q 类，并补全为经 W25qxx 驱动真实读 Flash 的实现
 *
 * @note  Flash 字库布局：基地址起连续存放 21003 个字模，每字 32 字节，
 *        索引 = Unicode - 0x4E00（CJK 统一汉字区起始）
 */
#ifndef FONT_W25Q_H
#define FONT_W25Q_H

#include "font_service.h"

class W25qxx; /* w25qxx_driver 组件前向声明 */

/**
 * W25Qxx 外挂字库字体类（单例）
 */
class FontW25q : public FontService {
public:
    static constexpr uint32_t CHAR_COUNT       = 21003;  /*!< 字符数量 */
    static constexpr uint32_t GLYPH_BASE_UNICODE = 0x4E00; /*!< 字模 0 对应的 Unicode */
    static constexpr uint32_t DEFAULT_BASE_ADDR  = 0x000000; /*!< 字库在 Flash 中的默认基地址 */

    /* 全局唯一实例 */
    static FontW25q &instance();

    /**
     * @brief  挂接已初始化的 W25qxx 驱动
     *
     * @param flash    W25qxx 实例（须已 init 成功）
     * @param baseAddr 字库在 Flash 中的基地址
     */
    void attach(W25qxx &flash, uint32_t baseAddr = DEFAULT_BASE_ADDR);

    /** 是否已挂接 Flash 驱动 */
    bool isAttached() const { return m_flash != nullptr; }

    int getFont(uint32_t unicode, uint8_t *buf) override;
    int getMaxChars() const override { return (int)CHAR_COUNT; }
    const char *name() const override { return "w25q"; }

private:
    FontW25q() = default;

    W25qxx  *m_flash    = nullptr;
    uint32_t m_baseAddr = DEFAULT_BASE_ADDR;
};

#endif /* FONT_W25Q_H */
