#ifndef SYSTEM_INTERNAL_TRUETYPEFONT_H
#define SYSTEM_INTERNAL_TRUETYPEFONT_H
//file format spec: https://learn.microsoft.com/en-us/typography/opentype/spec/glyf
#include <stdexcept>

namespace System::Internal{
	class TrueTypeFont{
		private:
			constexpr uint32_t TTF_CHECKSUM_MAGIC = 0xB1B0AFBA;
			//static vector<font_t> fonts;
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
  version: reader.getFixed(),
				uint16_t numGlyphs;
				uint16_t maxPoints;
				uint16_t maxContours;
				uint16_t maxCompositePoints;
				uint16_t maxCompositeContours;
				uint16_t maxZones;
				uint16_t maxTwilightPoints;
				uint16_t maxStorage;
				uint16_t maxFunctionDefs;
				uint16_t maxInstructionDefs;
				uint16_t maxStackElements;
				uint16_t maxSizeOfInstructions;
				uint16_t maxComponentElements;
				uint16_t maxComponentDepth;			
			}
			//horizontal header
			struct hhea_t{
				uint16_t majorVersion;
				uint16_t minorVersion;
				FWORD ascender;
				FWORD descender;
				FWORD lineGap;
				UFWORD advanceWidthMax;
				FWORD minLeftSideBearing;
				FWORD minRightSideBearing;
				FWORD xMaxExtent;
				int16_t	caretSlopeRise;
				int16_t	caretSlopeRun;
				int16_t	caretOffset;
				int16_t	reserved1;
				int16_t	reserved2;
				int16_t	reserved3;
				int16_t	reserved4;
				int16_t	metricDataFormat;
				uint16_t numberOfHMetrics;
			}
			//horizontal metrics
			struct hmtx_t{
				
			}
			//glyph location
			struct loca_t{
				
			}
			//glyph outline
			struct glyf_t{
				int16	numberOfContours;
				int16	xMin
				int16	yMin
				int16	xMax
				int16	yMax
				
			}
			//character code mapping
			struct cmap_t{
				
			}
			static bool ends_with(const std::string& str, const std::string& suffix){
				return str.size() >= suffix.size() && str.compare(str.size()-suffix.size(), suffix.size(), suffix) == 0;
			}
			static bool starts_with(const std::string& str, const std::string& prefix){
				return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
			} 

		public:
			void LoadFont(std::string filepath){
				if (!ends_with(filepath, ".ttf") && !ends_with(filepath, ".TTF")){
					throw std::invalid_argument( "TrueTypeFont [Error] Filename not .ttf");
					return;
				}
				std::ifstream file(filepath, std::ios::binary);
				if (!file) {
					throw std::invalid_argument( "TrueTypeFont [Error] Cannot open file");
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