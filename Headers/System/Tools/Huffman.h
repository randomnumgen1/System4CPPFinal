#ifndef _SYSTEM_TOOLS_HUFFMAN_H
#define _SYSTEM_TOOLS_HUFFMAN_H

#include <vector>
#include <cstdint>
#include <map>
#include <queue>

namespace System {
	namespace Tools {
		struct HuffmanNode {
		public:
			uint8_t Symbol;
			int Frequency;
			HuffmanNode* Left;
			HuffmanNode* Right;

			inline bool IsLeaf() const {
				return (Left == nullptr && Right == nullptr);
			}
		};

		class HuffmanTree {
		private:
			HuffmanNode* root;
			std::map<uint8_t, std::vector<bool>> codes;

			struct CompareNodes {
				bool operator()(HuffmanNode* a, HuffmanNode* b) {
					return a->Frequency > b->Frequency;
				}
			};

			void GenerateCodes(HuffmanNode* node, std::vector<bool> code) {
				if (node->IsLeaf()) {
					codes[node->Symbol] = code;
					return;
				}
				std::vector<bool> leftCode = code;
				leftCode.push_back(false);
				GenerateCodes(node->Left, leftCode);

				std::vector<bool> rightCode = code;
				rightCode.push_back(true);
				GenerateCodes(node->Right, rightCode);
			}

		public:
			void Build(std::vector<uint8_t> source) {
				std::map<uint8_t, int> frequencies;
				for (uint8_t symbol : source) {
					frequencies[symbol]++;
				}

				std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;
				for (auto const& [symbol, frequency] : frequencies) {
					pq.push(new HuffmanNode{ symbol, frequency, nullptr, nullptr });
				}

				while (pq.size() > 1) {
					HuffmanNode* left = pq.top();
					pq.pop();
					HuffmanNode* right = pq.top();
					pq.pop();
					HuffmanNode* parent = new HuffmanNode{ 0, left->Frequency + right->Frequency, left, right };
					pq.push(parent);
				}

				root = pq.top();
				GenerateCodes(root, {});
			}

			inline bool IsLeaf(HuffmanNode* node) const {
				return (node->Left == nullptr && node->Right == nullptr);
			}

			std::vector<bool> Encode(std::vector<uint8_t> source) {
				std::vector<bool> encoded;
				for (uint8_t symbol : source) {
					encoded.insert(encoded.end(), codes[symbol].begin(), codes[symbol].end());
				}
				return encoded;
			}

			std::vector<uint8_t> Decode(std::vector<bool> source) {
				std::vector<uint8_t> decoded;
				HuffmanNode* currentNode = root;
				for (bool bit : source) {
					if (bit) {
						currentNode = currentNode->Right;
					}
					else {
						currentNode = currentNode->Left;
					}
					if (currentNode->IsLeaf()) {
						decoded.push_back(currentNode->Symbol);
						currentNode = root;
					}
				}
				return decoded;
			}
		};
	}
}
#endif