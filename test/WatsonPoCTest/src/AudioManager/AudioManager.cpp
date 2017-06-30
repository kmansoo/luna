// Note to Visual Studio / Windows users: you must set the working directory manually on the project file
// to $(ProjectDir)../../../ since these settings are not saved directly in project. The loader
// will be unable to find the example assets unless the proper working directory is set.

#if defined(_MSC_VER)
#pragma comment(lib, "dsound.lib")
#endif

#include "AudioManager.h"

#include "libnyquist/AudioDecoder.h"
#include "libnyquist/WavEncoder.h"
#include "libnyquist/PostProcess.h"

#include <thread>

using namespace nqr;

int desiredSampleRate = 44100;

AudioManager::AudioManager() {
    AudioDevice::ListAudioDevices();

    my_device_ = std::make_shared<AudioDevice>(2, desiredSampleRate);
	my_device_->Open(my_device_->info.id);
}

bool AudioManager::play(const std::string& filename) {

	std::shared_ptr<AudioData> fileData = std::make_shared<AudioData>();

	NyquistIO loader;

    // Circular libnyquist testing
    loader.Load(fileData.get(), filename.c_str());

	if (fileData->sampleRate != desiredSampleRate) {
		std::cout << "[Warning - Sample Rate Mismatch] - file is sampled at " << fileData->sampleRate << " and output is " << desiredSampleRate << std::endl;
	}

	// Resample
	std::vector<float> outputBuffer;
	outputBuffer.reserve(fileData->samples.size());
	linear_resample(44100.0 / 48000.0, fileData->samples, outputBuffer, fileData->samples.size());

	std::cout << "Input Samples: " << fileData->samples.size() << std::endl;
	std::cout << "Output Samples: " << outputBuffer.size() << std::endl;

	// Convert mono to stereo for testing playback
	if (fileData->channelCount == 1) {
		std::cout << "Playing MONO for: " << fileData->lengthSeconds << " seconds..." << std::endl;
		std::vector<float> stereoCopy(fileData->samples.size() * 2);
		MonoToStereo(fileData->samples.data(), stereoCopy.data(), fileData->samples.size());

		my_device_->Play(stereoCopy);
	}
	else {
		std::cout << "Playing for: " << fileData->lengthSeconds << " seconds..." << std::endl;
		my_device_->Play(fileData->samples);
	}

	return EXIT_SUCCESS;
}
/*
catch (const UnsupportedExtensionEx & e)
{
	std::cerr << "Caught: " << e.what() << std::endl;
}
catch (const LoadPathNotImplEx & e)
{
	std::cerr << "Caught: " << e.what() << std::endl;
}
catch (const LoadBufferNotImplEx & e)
{
	std::cerr << "Caught: " << e.what() << std::endl;
}
catch (const std::exception & e)
{
	std::cerr << "Caught: " << e.what() << std::endl;
}
    return true;
}
*/

bool AudioManager::record(const std::string& filename, int seconds) {
	std::shared_ptr<AudioData> fileData = std::make_shared<AudioData>();

	NyquistIO loader;

    // Circular libnyquist testing
	fileData->samples.reserve(44100 * 5);
	fileData->channelCount = 1;
	fileData->frameSize = 32;
	fileData->lengthSeconds = (float)seconds;
	fileData->sampleRate = 44100;
	std::cout << "Starting recording ..." << std::endl;
	my_device_->Record(fileData->sampleRate * fileData->lengthSeconds, fileData->samples);

	if (fileData->sampleRate != desiredSampleRate) {
		std::cout << "[Warning - Sample Rate Mismatch] - file is sampled at " << fileData->sampleRate << " and output is " << desiredSampleRate << std::endl;
	}

	// Resample
	std::vector<float> outputBuffer;
	outputBuffer.reserve(fileData->samples.size());
	linear_resample(44100.0 / 48000.0, fileData->samples, outputBuffer, fileData->samples.size());

	std::cout << "Input Samples: " << fileData->samples.size() << std::endl;
	std::cout << "Output Samples: " << outputBuffer.size() << std::endl;

	fileData->samples = outputBuffer;
	int encoderStatus = OggOpusEncoder::WriteFile({ 1, PCM_FLT, DITHER_NONE }, fileData.get(), filename.c_str());
	std::cout << "Encoder Status: " << encoderStatus << std::endl;

	return EXIT_SUCCESS;
}
