#pragma once

#include <vector>

class LunaAudioDevice
{
public:
	LunaAudioDevice() {}
	virtual ~LunaAudioDevice() {}

public:
	virtual bool open(const int output_device_id, const int input_device_id) = 0;
	virtual bool play(const std::vector<float> & data) = 0;
	virtual bool record(const uint32_t lengthInSamples, std::vector<float> & recordingBuffer) = 0;
};
