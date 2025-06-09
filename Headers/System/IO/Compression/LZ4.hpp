namespace System {
namespace IO {
namespace Compression {

	class LZ4{
		private:
		bool isLZ4Frame(const std::vector<uint8_t>& data) {
			const uint32_t LZ4_MAGIC = 0x184D2204;
			if (data.size() < 4)
				return false;
			uint32_t magic = (static_cast<uint32_t>(data[0])) |
							 (static_cast<uint32_t>(data[1]) << 8) |
							 (static_cast<uint32_t>(data[2]) << 16) |
							 (static_cast<uint32_t>(data[3]) << 24);
			return magic == LZ4_MAGIC;
		}
		public:
		
		std::vector<uint8_t> Decompress(std::vector<uint8_t>){
			if (!isLZ4Frame(compressedData)){
				throw std::runtime_error("LZ4 Frame Magic error!");
			}

			
		}
		
		
		
	};
}
}
}