#include <System/AudioClip.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

namespace System {

    int AudioClip::GetChannels() const {
        return m_channels;
    }

    int AudioClip::GetFrequency() const {
        return m_frequency;
    }

    float AudioClip::GetLength() const {
        if (m_bitsPerSample == 0) return 0.0f;
        return (float)(m_waveSize * 8) / (m_frequency * m_channels * m_bitsPerSample);
    }

    int AudioClip::GetSamples() const {
        return m_samples;
    }

    //code taken from rastertek tutorials for quickness
    bool AudioClip::LoadMonoWaveFile(std::string filename) {
        FILE* filePtr;
        RiffWaveHeaderType riffWaveFileHeader;
        SubChunkHeaderType subChunkHeader;
        FmtType fmtData;
        unsigned int count, seekSize;
        bool foundFormat, foundData;

        // Open the wave file for reading in binary.
        filePtr = fopen(filename.c_str(), "rb");
        if (filePtr == NULL) {
            std::cout << "file fail" << std::endl;
            return false;
        }

        // Read in the riff wave file header.
        count = fread(&riffWaveFileHeader, sizeof(riffWaveFileHeader), 1, filePtr);
        if (count != 1) {
            fclose(filePtr);
            std::cout << "file pointer fail" << std::endl;
            return false;
        }

        // Check that the chunk ID is the RIFF format.
        if ((riffWaveFileHeader.chunkId[0] != 'R') || (riffWaveFileHeader.chunkId[1] != 'I') || (riffWaveFileHeader.chunkId[2] != 'F') || (riffWaveFileHeader.chunkId[3] != 'F')) {
            fclose(filePtr);
            std::cout << "file riff fail" << std::endl;
            return false;
        }

        // Check that the file format is the WAVE format.
        if ((riffWaveFileHeader.format[0] != 'W') || (riffWaveFileHeader.format[1] != 'A') || (riffWaveFileHeader.format[2] != 'V') || (riffWaveFileHeader.format[3] != 'E')) {
            fclose(filePtr);
            std::cout << "file wav fail" << std::endl;
            return false;
        }
       
        // Read in the sub chunk headers until you find the format chunk.
        foundFormat = false;
        while (foundFormat == false) {
            // Read in the sub chunk header.
            count = fread(&subChunkHeader, sizeof(subChunkHeader), 1, filePtr);
            if (count != 1) {
                fclose(filePtr);
                std::cout << "" << std::endl;
                return false;
            }

            // Determine if it is the fmt header.  If not then move to the end of the chunk and read in the next one.
            if ((subChunkHeader.subChunkId[0] == 'f') && (subChunkHeader.subChunkId[1] == 'm') && (subChunkHeader.subChunkId[2] == 't') && (subChunkHeader.subChunkId[3] == ' ')) {
                foundFormat = true;
            }
            else {
                fseek(filePtr, subChunkHeader.subChunkSize, SEEK_CUR);
            }
        }

        // Read in the format data.
        count = fread(&fmtData, sizeof(fmtData), 1, filePtr);
        if (count != 1) {
            fclose(filePtr);
            return false;
        }

        // Check that the audio format is WAVE_FORMAT_PCM (1).
        if (fmtData.audioFormat != 1) {
            fclose(filePtr);
            return false;
        }

        // Check that the wave file was recorded in mono format.
        if (fmtData.numChannels != 1) {
            fclose(filePtr);
            return false;
        }

        // Check that the wave file was recorded at a sample rate of 44.1 KHz.
        if (fmtData.sampleRate != 44100) {
            fclose(filePtr);
            return false;
        } 
        // Ensure that the wave file was recorded in 16 bit format.
        if (fmtData.bitsPerSample != 16) {
            fclose(filePtr);
            return false;
        }

        // Seek up to the next sub chunk.
        seekSize = subChunkHeader.subChunkSize - 16;
        fseek(filePtr, seekSize, SEEK_CUR);

        // Read in the sub chunk headers until you find the data chunk.
        foundData = false;
        while (foundData == false) {
            // Read in the sub chunk header.
            count = fread(&subChunkHeader, sizeof(subChunkHeader), 1, filePtr);
            if (count != 1) {
                fclose(filePtr);
                return false;
            }

            // Determine if it is the data header.  If not then move to the end of the chunk and read in the next one.
            if ((subChunkHeader.subChunkId[0] == 'd') && (subChunkHeader.subChunkId[1] == 'a') && (subChunkHeader.subChunkId[2] == 't') && (subChunkHeader.subChunkId[3] == 'a')) {
                foundData = true;
            }
            else {
                fseek(filePtr, subChunkHeader.subChunkSize, SEEK_CUR);
            }
        }

        // Store the metadata.
        m_channels = fmtData.numChannels;
        m_frequency = fmtData.sampleRate;
        m_bitsPerSample = fmtData.bitsPerSample;
        m_waveSize = subChunkHeader.subChunkSize;
        m_samples = m_waveSize / (m_channels * (m_bitsPerSample / 8));

        // Create a temporary buffer to hold the wave file data.
        m_waveData = new unsigned char[m_waveSize];

        // Read in the wave file data into the newly created buffer.
        count = fread(m_waveData, 1, m_waveSize, filePtr);
        if (count != m_waveSize) {
            delete[] m_waveData;
            m_waveData = 0;
            fclose(filePtr);
            return false;
        }

        // Close the file once done reading.
        fclose(filePtr);











        return true;
    }

    bool AudioClip::LoadStereoWaveFile(std::string filename) {
        // Implementation will be moved here
        return false;
    }

    bool AudioClip::LoadMP3File(std::string filename) {
        // Implementation will be moved here
        return false;
    }

}
