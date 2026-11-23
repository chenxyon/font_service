# Font Service 组件 v1.0（C++ class 版）

## 一、组件概述

Font Service 是统一字体服务组件，提供字体抽象基类 + 工厂，支持多种字体实现与运行时切换。

### 特性

- **统一接口**：所有字体实现继承 `FontService` 基类，上层按统一接口取字模
- **工厂模式**：`FontService::create(FontType)` 统一获取字体实例（静态单例，无需释放）
- **三种字库**：内置字库（BUILTIN）、跃宋字库（YUESONG，可选编译）、W25Qxx 外挂 Flash 字库（W25Q）
- **字模直绘**：`FontDisplay` 经 LCD 回调把 16x16 字模直接画到屏幕，支持自动换行

### 字体规格

- 点阵大小：16x16
- 每个字模：32 字节（16 行 × 2 字节/行，1bpp）
- 内置/跃宋字库：20902 个常用汉字
- W25Q 字库：21003 字，索引 = Unicode - 0x4E00

### 编译策略

| 字体 | 编译条件 | Flash 占用 |
|------|----------|-----------|
| BUILTIN | 始终编译（默认回退字体） | 约 653KB |
| YUESONG | `build_flags` 定义 `FONT_SERVICE_ENABLE_YUESONG` | 约 700KB |
| W25Q | 始终编译（字库在外挂 Flash，固件零占用） | 0 |

> 原 GBK 字体因数据文件缺失（引用了不存在的 examples 路径）已移除；
> 原 `font_chinese_longyin` 为无人引用的死代码，一并移除。

---

## 二、目录结构

```
font_service/
├── font_service.h          # FontType 枚举 + FontService 抽象基类
├── font_service.cpp        # 工厂实现
├── font_builtin.h/.cpp     # 内置字库字体类（FontBuiltin）
├── font_builtin_data.cpp   # 内置字库数据（自动生成，653KB）
├── font_yuesong.h/.cpp     # 跃宋字库字体类（FontYuesong，可选编译）
├── font_yuesong_data.cpp   # 跃宋字库数据（自动生成，可选编译）
├── font_w25q.h/.cpp        # W25Qxx 外挂字库字体类（FontW25q）
├── font_display.h/.cpp     # 字模直绘显示类（FontDisplay）
├── tools/
│   └── generate_gbk_font.py # TTF 字体转字模工具（生成原始 .bin 数据）
├── library.json            # PlatformIO 库描述
└── README.md               # 开发文档（本文件）
```

---

## 三、API 参考

### 3.1 工厂创建

```cpp
#include "font_service.h"

FontService *font = FontService::create(FontType::BUILTIN);
if (font != nullptr) {
    uint8_t buf[FontService::BYTES];
    if (font->getFont(U'汉', buf) == 0) {
        // buf 内即 16x16 字模
    }
}
```

### 3.2 各字体类

```cpp
#include "font_builtin.h"
#include "font_yuesong.h"
#include "font_w25q.h"

/* 内置字库：单例直接用 */
FontBuiltin::instance().getFont(U'汉', buf);

/* 跃宋字库：需先在 build_flags 定义 FONT_SERVICE_ENABLE_YUESONG，否则恒返回 -1 */
FontYuesong::instance().getFont(U'汉', buf);

/* W25Q 字库：先挂接已初始化的 W25qxx 驱动 */
W25qxx flash;
// flash.init(cfg); ...
FontW25q::instance().attach(flash);              // 默认基地址 0x000000
FontW25q::instance().attach(flash, 0x100000);    // 或指定基地址
FontW25q::instance().getFont(U'汉', buf);
```

### 3.3 字模直绘显示

```cpp
#include "font_display.h"

/* 注入 LCD 回调（由显示驱动实现） */
FontLcdOps ops = {
    .setWindow  = [](int x1, int y1, int x2, int y2) { /* 设置窗口 */ },
    .writeData16 = [](const uint16_t *data, int len) { /* 写像素 */ },
};
FontDisplay::setLcdOps(ops);

/* 画单字 */
FontDisplay::displayChar(FontBuiltin::instance(), 0, 0, U'汉', 0xFFFF);

/* 画字符串（UTF-16 码点数组，超宽自动换行） */
const uint16_t text[] = { u'你', u'好', u'世', u'界', 0 };
FontDisplay::displayString(FontBuiltin::instance(), 0, 0, text, 0xFFFF, 320);
```

### 3.4 FontService 基类接口

| 方法 | 说明 |
|------|------|
| `getFont(unicode, buf)` | 取字模，成功返回 0；字符不存在/参数错误返回 -1 |
| `getFontSize()` | 字体尺寸（16） |
| `getMaxChars()` | 字符数量 |
| `name()` | 字体名（日志用） |
| 常量 `WIDTH / HEIGHT / BYTES` | 16 / 16 / 32 |

---

## 四、W25Q 字库 Flash 布局

| 项 | 值 |
|----|----|
| 基地址 | 默认 0x000000（attach 可改） |
| 字模 0 对应 Unicode | 0x4E00（CJK 统一汉字区起点） |
| 每字 | 32 字节，连续存放 |
| 字数 | 21003 |

字模地址 = 基地址 + (Unicode - 0x4E00) × 32。

---

## 五、版本历史

| 版本 | 日期 | 说明 |
|------|------|------|
| v1.0.0 | 2026-11-21 | 重构为 C++ class；移除无数据的 GBK 字体与死代码 font_chinese_longyin；FontW25q 由空壳补全为经 W25qxx 驱动真实读 Flash；字库 .bin 转为内嵌 C 数组（PlatformIO 无 binary embed）；displayString 屏宽改为参数 |
| v1.3.094 | - | ESP-IDF C 版本（Kconfig 条件编译） |

## 许可证

MIT License
