#ifndef SYSTEM_INTERNAL_TRUETYPEFONT_H
#define SYSTEM_INTERNAL_TRUETYPEFONT_H

#include <stdexcept>

namespace System::Internal{
	class TrueTypeFont{
		private:
			constexpr uint32_t TTF_CHECKSUM_MAGIC = 0xB1B0AFBA;
			
			struct TableDirectory_t{
				uint32_t scaler_type;
				uint16_t numTables;
				uint16_t searchRange;
				uint16_t entrySelector;
				uint16_t rangeShift;
				void fixendian(){
					scaler_type = _byteswap_ulong(scaler_type);
					numTables = _byteswap_ushort(numTables);
					searchRange = _byteswap_ushort(searchRange);
					entrySelector = _byteswap_ushort(entrySelector);
					rangeShift = _byteswap_ushort(rangeShift);
				}
			}
			struct TableRecord_t{
				uint32_t tag;
				uint32_t checkSum;
				uint32_t offset;
				uint32_t length;
				void fixendian(){
					tag = _byteswap_ulong(tag);
					checkSum = _byteswap_ulong(checkSum);
					offset = _byteswap_ulong(offset);
					length = _byteswap_ulong(length);
				}
			}
			struct head_t{
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
				void fixendian(){
					created = _byteswap_uint64(created);
					modified = _byteswap_uint64(modified);
				}
			}
			//maximum profile
			struct maxp_t{
				
			}
			//horizontal header
			struct hhea_t{
				
			}
			//horizontal metrics
			struct hmtx_t{
				
			}
			//glyph location
			struct loca_t{
				
			}
			//glyph outline
			struct glyf_t{
				
			}
			//character code mapping
			struct cmap_t{
				
			}
		public:
			void LoadFont(std::string filepath){
				std::ifstream file(filepath, std::ios::binary);
				if (!file) {
					throw std::invalid_argument( "TrueTypeFont File Error" );
					return;
				}
				TableDirectory_t offsetHeader;
				file.read(reinterpret_cast<char*>(&offsetHeader), sizeof(offsetHeader));
				std::vector<TableRecord_t> tables(offsetHeader.numTables);
				file.read(reinterpret_cast<char*>(tables.data()), sizeof(TableRecord_t) * offsetHeader.numTables);
			}
		
	};
}
#endif