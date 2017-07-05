// Note to Visual Studio / Windows users: you must set the working directory
// manually on the project file
// to $(ProjectDir)../../../ since these settings are not saved directly in
// project. The loader
// will be unable to find the example assets unless the proper working directory
// is set.

#if defined(_MSC_VER)
#pragma comment(lib, "dsound.lib")
#endif

#include "AudioManager.h"

#include "./rtaudio/RtAudioDevice.h"

#include "libnyquist/AudioDecoder.h"
#include "libnyquist/PostProcess.h"
#include "libnyquist/WavEncoder.h"

#include <thread>

using namespace nqr;

int desiredSampleRate = 48000;

AudioManager::AudioManager(const int output_device_id, const int input_device_id, const int channel) {
  RtAudioDevice::listAudioDevices();

  my_device_ = std::make_shared<RtAudioDevice>(channel, desiredSampleRate);
  my_device_->open(output_device_id, input_device_id);
}

bool AudioManager::play(const std::string &filename) {

  std::shared_ptr<AudioData> fileData = std::make_shared<AudioData>();

  NyquistIO loader;

  // Circular libnyquist testing
  loader.Load(fileData.get(), filename.c_str());

  if (fileData->sampleRate != desiredSampleRate) {
    std::cout << "[Warning - Sample Rate Mismatch] - file is sampled at "
              << fileData->sampleRate << " and output is " << desiredSampleRate
              << std::endl;
  }

  // Resample
  std::vector<float> outputBuffer;
  outputBuffer.reserve(fileData->samples.size());
  linear_resample(desiredSampleRate / 48000.0, fileData->samples, outputBuffer,
                  fileData->samples.size());

#if (LUNA_SHOW_DEBUG_LOG == 1)
  std::cout << "Input Samples: " << fileData->samples.size() << std::endl;
  std::cout << "Output Samples: " << outputBuffer.size() << std::endl;
#endif

  // Convert mono to stereo for testing playback
  if (fileData->channelCount == 1) {

#if (LUNA_SHOW_DEBUG_LOG == 1)
    std::cout << "Playing MONO for: " << fileData->lengthSeconds
              << " seconds..." << std::endl;
#endif

    std::vector<float> stereoCopy(fileData->samples.size() * 2);
    MonoToStereo(fileData->samples.data(), stereoCopy.data(),
                 fileData->samples.size());

    my_device_->play(stereoCopy);
  } 
  else {
#if (LUNA_SHOW_DEBUG_LOG == 1)
    std::cout << "Playing for: " << fileData->lengthSeconds << " seconds..."
              << std::endl;
#endif

    my_device_->play(fileData->samples);
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

bool AudioManager::record(const std::string &filename, int seconds) {
  std::shared_ptr<AudioData> fileData = std::make_shared<AudioData>();

  NyquistIO loader;

  // Circular libnyquist testing
  fileData->samples.reserve(desiredSampleRate * 5);
  fileData->channelCount = 1;
  fileData->frameSize = 32;
  fileData->lengthSeconds = (float)seconds;
  fileData->sampleRate = desiredSampleRate;

#if (LUNA_SHOW_DEBUG_LOG == 1)
  std::cout << "Starting recording ..." << std::endl;
#endif

  my_device_->record(fileData->sampleRate * fileData->lengthSeconds,
                     fileData->samples);

  if (fileData->sampleRate != desiredSampleRate) {
#if (LUNA_SHOW_DEBUG_LOG == 1)
    std::cout << "[Warning - Sample Rate Mismatch] - file is sampled at "
              << fileData->sampleRate << " and output is " << desiredSampleRate
              << std::endl;
#endif
  }

  // Resample
  std::vector<float> outputBuffer;
  outputBuffer.reserve(fileData->samples.size());
  linear_resample(desiredSampleRate / 48000.0, fileData->samples, outputBuffer,
                  fileData->samples.size());

#if (LUNA_SHOW_DEBUG_LOG == 1)
  std::cout << "Input Samples: " << fileData->samples.size() << std::endl;
  std::cout << "Output Samples: " << outputBuffer.size() << std::endl;
#endif

  fileData->samples = outputBuffer;

  int encoderStatus = OggOpusEncoder::WriteFile(
      {1, PCM_FLT, DITHER_NONE}, fileData.get(), filename.c_str());

#if (LUNA_SHOW_DEBUG_LOG == 1)
  std::cout << "Encoder Status: " << encoderStatus << std::endl;
#endif

  return EXIT_SUCCESS;
}
