/**
 * @file     font_service.cpp
 * @brief    统一字体服务工厂实现 — C++ class
 *
 * 功能：按字体类型返回对应字体服务单例
 * 修改：2026-11-21 由 C API（font_service_create_font）重构为 FontService::create
 */
#include "font_service.h"

#include "esp_log.h"
#include "font_builtin.h"
#include "font_w25q.h"
#include "font_yuesong.h"

static const char *TAG = "font_service";

FontService *FontService::create(FontType type)
{
    switch (type) {
        case FontType::BUILTIN:
            return &FontBuiltin::instance();

        case FontType::YUESONG:
            return &FontYuesong::instance();

        case FontType::W25Q:
            return &FontW25q::instance();

        default:
            ESP_LOGE(TAG, "无效的字体类型：%d", (int)type);
            return nullptr;
    }
}

#include "font_service_version.h"
#include "version_manager.h"
COMPONENT_VERSION_REGISTER(font_service, FONT_SERVICE_VERSION_STRING)
