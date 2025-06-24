#ifndef SYSTEM_INTERNAL_TRUETYPEFONT_H
#define SYSTEM_INTERNAL_TRUETYPEFONT_H

#include <stdexcept>

namespace System::Internal{
	class TrueTypeFont{
		private:
			struct OffsetSubTable{
				uint32_t scaler_type;
				uint16_t numTables;
				uint16_t searchRange;
				uint16_t entrySelector;
				uint16_t rangeShift;
			}
			struct TableDirectory{
				uint32_t tag;
				uint32_t checkSum;
				uint32_t offset;
				uint32_t length;
			}
			struct head{
				uint16_t majorVersion;
				uint16_t minorVersion;
				uint32_t fontRevision;
				uint32_t checksumAdjustment;
				uint32_t magicNumber;
				uint16_t flags;
				uint16_t unitsPerEm;
				uint64_t created;
				uint64_t modified;
				uint16_t xMin;
				uint16_t yMin;
				uint16_t xMax;
				uint16_t yMax;
				uint16_t macStyle;
				uint16_t lowestRecPPEM;
				uint16_t fontDirectionHint;
				uint16_t indexToLocFormat;
				uint16_t glyphDataFormat;
			}
		public:
			void LoadFont(std::string filepath){
				std::ifstream file(filepath, std::ios::binary);
				if (!file) {
					throw std::invalid_argument( "TrueTypeFont File Error" );
					return;
				}
				OffsetSubTable offsetHeader;
				file.read(reinterpret_cast<char*>(&offsetHeader), sizeof(offsetHeader));
				std::vector<TableDirectory> tables(offsetHeader.numTables);
				file.read(reinterpret_cast<char*>(tables.data()), sizeof(TableDirectory) * offsetHeader.numTables);
			}
		
	};
}
#endif