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
			void DeleteTree(HuffmanNode* node) {
				if (node == nullptr) {
					return;
				}
				DeleteTree(node->Left);
				DeleteTree(node->Right);
				delete node;
			}
		public:
			HuffmanTree() : root(nullptr) {}
			void DeleteTree() {
				DeleteTree(root);
			}
			~HuffmanTree() {
				DeleteTree();
			}
			void Build(const std::vector<uint8_t>& source) {
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
					if (left->Symbol > right->Symbol) {
						std::swap(left, right);
					}
					HuffmanNode* parent = new HuffmanNode{ 0, left->Frequency + right->Frequency, left, right };
					pq.push(parent);
				}

				root = pq.top();
				GenerateCodes(root, {});
			}

			inline bool IsLeaf(const HuffmanNode* node) const {
				return (node->Left == nullptr && node->Right == nullptr);
			}
			void PrintTree(HuffmanNode* node, std::string indent, bool last) {
				if (node != nullptr) {
					std::cout << indent;
					if (last) {
						std::cout << "R----";
						indent += "     ";
					}
					else {
						std::cout << "L----";
						indent += "|    ";
					}

					if (node->IsLeaf()) {
						std::cout << "'" << node->Symbol << "' (" << node->Frequency << ")" << std::endl;
					}
					else {
						std::cout << "(" << node->Frequency << ")" << std::endl;
					}

					PrintTree(node->Left, indent, false);
					PrintTree(node->Right, indent, true);
				}
			}
			void PrintTree() {
				PrintTree(root, "", true);
			}
			void PrintCodes() {
				for (auto const& [symbol, code] : codes) {
					std::cout << "'" << symbol << "' (" << (int)symbol << "): ";
					for (bool bit : code) {
						std::cout << bit;
					}
					std::cout << std::endl;
				}
			}
			std::vector<bool> Encode(const std::vector<uint8_t>& source) {
				std::vector<bool> encoded;
				for (uint8_t symbol : source) {
					encoded.insert(encoded.end(), codes[symbol].begin(), codes[symbol].end());
				}
				return encoded;
			}

			std::vector<uint8_t> Decode(const std::vector<bool>& source) {
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