#ifndef _SYSTEM_AUDIOCLIP_H
#define _SYSTEM_AUDIOCLIP_H



namespace System {
    /// <summary>
    /// Represents a loaded audio asset, typically a WAV file, that can be played by an AudioSource.
    /// Handles raw waveform data and metadata such as sample rate, channel count, and duration.
    /// </summary>
    struct AudioClip {
    private:
        unsigned int m_audioBufferId, m_audioSourceId;
        unsigned char* m_waveData;
        unsigned int m_waveSize;
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
        int GetChannels() const{
            return 0;
        }
        int GetFrequency() const {
            return 0;
        }
        float GetLength() const {
            return 0.0f;
        }
        int GetSamples() const {
            return 0;
        }


        //code taken from rastertek tutorials for quickness
          bool LoadStereoWaveFile(std::string filename) {
         
              FILE* filePtr;
              RiffWaveHeaderType riffWaveFileHeader;
              SubChunkHeaderType subChunkHeader;
              FmtType fmtData;
              unsigned int count, seekSize;
              bool foundFormat, foundData;


              // Open the wave file for reading in binary.
              filePtr = fopen(filename.c_str(), "rb");
              if (filePtr == NULL) {
                  return false;
              }

              // Read in the riff wave file header.
              count = fread(&riffWaveFileHeader, sizeof(riffWaveFileHeader), 1, filePtr);
              if (count != 1) {
                  return false;
              }

              // Check that the chunk ID is the RIFF format.
              if ((riffWaveFileHeader.chunkId[0] != 'R') || (riffWaveFileHeader.chunkId[1] != 'I') || (riffWaveFileHeader.chunkId[2] != 'F') || (riffWaveFileHeader.chunkId[3] != 'F')) {
                  return false;
              }

              // Check that the file format is the WAVE format.
              if ((riffWaveFileHeader.format[0] != 'W') || (riffWaveFileHeader.format[1] != 'A') || (riffWaveFileHeader.format[2] != 'V') || (riffWaveFileHeader.format[3] != 'E')) {
                  return false;
              }

              // Read in the sub chunk headers until you find the format chunk.
              foundFormat = false;
              while (foundFormat == false) {
                  // Read in the sub chunk header.
                  count = fread(&subChunkHeader, sizeof(subChunkHeader), 1, filePtr);
                  if (count != 1) {
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
                  return false;
              }

              // Check that the audio format is WAVE_FORMAT_PCM (1).
              if (fmtData.audioFormat != 1) {
                  return false;
              }
              // Check that the wave file was recorded in mono format.
              if (fmtData.numChannels != 1) {
                  return false;
              }

              // Check that the wave file was recorded at a sample rate of 44.1 KHz.
              if (fmtData.sampleRate != 44100) {
                  return false;
              }

              // Ensure that the wave file was recorded in 16 bit format.
              if (fmtData.bitsPerSample != 16) {
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

              // Store the size of the data chunk.
              m_waveSize = subChunkHeader.subChunkSize;

              // Create a temporary buffer to hold the wave file data.
              m_waveData = new unsigned char[m_waveSize];

              // Read in the wave file data into the newly created buffer.
              count = fread(m_waveData, 1, m_waveSize, filePtr);
              if (count != m_waveSize) {
                  return false;
              }

              // Close the file once done reading.
              fclose(filePtr);

              return true;
          }














           


    };
}

#endif