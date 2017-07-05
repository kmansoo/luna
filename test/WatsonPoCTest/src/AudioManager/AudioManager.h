#pragma once

#include <string>
#include <memory>

#include "LunaAudioDevice.h"

class AudioManager {
public:
    AudioManager(const int output_device_id, const int input_device_id, const int channel = 2);

public:
    bool    play(const std::string& filename);
    bool    record(const std::string& filename, int seconds);

private:
    #define SHOW_LOG    0

    std::shared_ptr<LunaAudioDevice> my_device_;
};
