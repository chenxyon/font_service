/**
 * @file     font_service.h
 * @brief    统一字体服务组件 — C++ class
 *
 * 功能：提供统一的字体服务抽象基类与工厂，支持内置字库（BUILTIN）、
 *       跃宋字库（YUESONG）、W25Qxx 外挂 Flash 字库（W25Q）三种字体类型。
 *       所有字体统一 16x16 点阵、每字 32 字节（1bpp 位图）。
 * 修改：2026-11-21 由 C API（font_service_create_font 等）重构为 C++ class；
 *       移除无数据文件的 GBK 字体与未被引用的 font_chinese_longyin 死代码；
 *       W25Q 字体由空壳补全为经 W25qxx 驱动真实读 Flash 的实现
 */
#ifndef FONT_SERVICE_H
#define FONT_SERVICE_H

#include <stdint.h>

/**
 * 字体类型
 */
enum class FontType : uint8_t {
    BUILTIN = 0, /* 内置字库（编译进固件，20902 字） */
    YUESONG = 1, /* 跃宋字库（编译进固件，20902 字，需定义 FONT_SERVICE_ENABLE_YUESONG） */
    W25Q    = 2, /* W25Qxx 外挂 Flash 字库（21003 字，需挂接 W25qxx 驱动） */
    MAX
};

/**
 * 字体服务抽象基类
 *
 * 功能：统一「按 Unicode 取 16x16 点阵字模」的接口，
 *       子类实现各自的数据来源（固件数组 / 外挂 Flash）
 */
class FontService {
public:
    /* ---- 统一字形参数 ---- */
    static constexpr int WIDTH  = 16; /*!< 字形宽度（像素） */
    static constexpr int HEIGHT = 16; /*!< 字形高度（像素） */
    static constexpr int BYTES  = 32; /*!< 每字字模字节数（16x16 1bpp） */

    virtual ~FontService() = default;

    /**
     * @brief  获取指定 Unicode 字符的字模数据
     *
     * @param unicode 字符 Unicode 码点
     * @param buf     输出缓冲，至少 BYTES 字节
     * @return 0 成功；-1 字符不存在或参数错误
     */
    virtual int getFont(uint32_t unicode, uint8_t *buf) = 0;

    /** 字体尺寸（像素） */
    virtual int getFontSize() const { return HEIGHT; }

    /** 字体支持的最大字符数 */
    virtual int getMaxChars() const = 0;

    /** 字体名称（日志用） */
    virtual const char *name() const = 0;

    /**
     * @brief  工厂方法：按类型创建字体服务实例
     *
     * 功能：返回对应类型的单例指针（静态存储，无需释放）
     *
     * @param type 字体类型
     * @return 成功返回实例指针；类型非法返回 nullptr
     */
    static FontService *create(FontType type);
};

#endif /* FONT_SERVICE_H */
