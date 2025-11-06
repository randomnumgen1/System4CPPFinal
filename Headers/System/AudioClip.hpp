#ifndef _SYSTEM_AUDIOCLIP_H
#define _SYSTEM_AUDIOCLIP_H

#include <string>

namespace System {
    /// <summary>
    /// Represents a decoded PCM audio asset that can be played by an AudioSource.
    /// Stores raw waveform data and metadata such as sample rate, channel count, bit depth, and duration.
    /// Supports loading from multiple formats (e.g., WAV, MP3) as long as they decode to PCM.
    /// </summary>
    struct AudioClip {
    public:
        unsigned int bufferId;
    private:
        unsigned int m_audioBufferId, m_audioSourceId;
        unsigned char* m_waveData;
        unsigned int m_waveSize;
        int m_channels;
        int m_frequency;
        int m_samples;
        int m_bitsPerSample;
        struct RiffWaveHeaderType
        {
            char chunkId[4];
            unsigned int chunkSize;
            char format[4];
        };

        struct SubChunkHeaderType
        {
            char subChunkId[4];
            unsigned int subChunkSize;
        };

        struct FmtType
        {
            unsigned short audioFormat;
            unsigned short numChannels;
            unsigned int sampleRate;
            unsigned int bytesPerSecond;
            unsigned short blockAlign;
            unsigned short bitsPerSample;
        };

    public:
        /// <summary>
        /// Returns the number of audio channels in the clip.
        /// Common values: 1 for mono, 2 for stereo.
        /// </summary>
        /// <returns></returns>
        int GetChannels() const;
        /// <summary>
        /// Returns the sample rate of the audio clip in Hertz (Hz).
        /// </summary>
        /// <returns></returns>
        int GetFrequency() const;
        /// <summary>
        /// Returns the total duration of the audio clip in seconds.
        /// </summary>
        /// <returns></returns>
        float GetLength() const;
        /// <summary>
        ///  Returns the total number of audio samples in the clip.
        /// </summary>
        /// <returns></returns>
        int GetSamples() const;


        //code taken from rastertek tutorials for quickness
        bool LoadMonoWaveFile(std::string filename);
        bool LoadStereoWaveFile(std::string filename);
        bool LoadMP3File(std::string filename) {

        }












           


    };
}

#endif