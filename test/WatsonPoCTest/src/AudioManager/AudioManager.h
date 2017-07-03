#pragma once

#include <string>
#include <memory>

#include "AudioDevice.h"

class AudioManager {
public:
    AudioManager(const int output_device_id, const int input_device_id, const int channel = 2);

public:
    bool    play(const std::string& filename);
    bool    record(const std::string& filename, int seconds);

private:
    #define SHOW_LOG    0

    std::shared_ptr<AudioDevice> my_device_;
};
