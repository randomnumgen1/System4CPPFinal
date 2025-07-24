


#include <vector>
#include <cstdint>
#include <map>

namespace System {
	namespace Tools{
	struct HuffmanNode{
		public:
			uint8_t Symbol;
			int Frequency;
			HuffmanNode* Left;
			HuffmanNode* Right;
			
			inline bool IsLeaf() const{
				return (Left == nullptr && Right == nullptr);
			}		
	};

	class HuffmanTree{
		public:
			void Build(std::vector<uint8_t> source){
				
				
			}
			inline bool IsLeaf(HuffmanNode* node) const{
				return (node->Left == nullptr && node->Right == nullptr);
			}
	};
}
}