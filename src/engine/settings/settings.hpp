#pragma once
#ifndef TME_SETTINGS
#define TME_SETTINGS

////////////////////////////////////
// Note(Appy): Default configuration for engine start up.

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "engine/common/common.hpp"
#include "engine/window/window_context.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#ifndef TME_WINDOW_WIDTH
#define TME_WINDOW_WIDTH 1920
#endif

#ifndef TME_WINDOW_HEIGHT
#define TME_WINDOW_HEIGHT 1080
#endif

#ifndef TME_WINDOW_FRAMES_LIM
#define TME_WINDOW_FRAMES_LIM 144
#endif

#ifndef TME_WINDOW_FULLSCREEN
#define TME_WINDOW_FULLSCREEN 0
#endif

#ifndef TME_VERTICAL_SYNC
#define TME_VERTICAL_SYNC 0
#endif

#ifndef TME_WINDOW_TITLE
#define TME_WINDOW_TITLE "Two Mans' Game Engine"
#endif

// Load settings from a JSON file.
inline std::optional<tme::WindowContext> load_settings(const std::string& filename)
{
    std::ifstream ifs{filename};

    if (!ifs.is_open())
    {
        std::cerr << "[Error] Settings error: No such file exits: " << filename << '\n';
        return std::nullopt;
    }

    tme::WindowContext        setting_config;
    rapidjson::IStreamWrapper isw{ifs};
    rapidjson::Document       doc{};
    doc.ParseStream(isw);

    rapidjson::StringBuffer                    buffer{};
    rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
    doc.Accept(writer);

    if (doc.HasParseError())
    {
        std::cerr << "[Error] : " << doc.GetParseError() << '\n';
        std::cout << "Offset : " << doc.GetErrorOffset() << '\n';
        return std::nullopt;
    }

    if (doc.HasMember("width"))
    {
        tme::ushort width    = cast(tme::ushort, doc["width"].GetUint());
        setting_config.width = width;
    }
    else
    {
        setting_config.width = TME_WINDOW_WIDTH;
    }

    if (doc.HasMember("height"))
    {
        tme::ushort height    = cast(tme::ushort, doc["height"].GetUint());
        setting_config.height = height;
    }
    else
    {
        setting_config.width = TME_WINDOW_HEIGHT;
    }

    if (doc.HasMember("title"))
    {
        std::string title    = cast(std::string, doc["title"].GetString());
        setting_config.title = title;
    }
    else
    {
        setting_config.title = TME_WINDOW_TITLE;
    }

    if (doc.HasMember("target_fps"))
    {
        // ...
    }
    else
    {
        setting_config.target_fps = TME_WINDOW_FRAMES_LIM;
    }

    if (doc.HasMember("vsync"))
    {
        // ...
    }
    else
    {
        setting_config.vsync = TME_VERTICAL_SYNC;
    }

    if (doc.HasMember("fullscreen"))
    {
        // ...
    }
    else
    {
        setting_config.fullscreen = TME_WINDOW_FULLSCREEN;
    }

    return setting_config;
}

#endif /* TME_SETTINGS */