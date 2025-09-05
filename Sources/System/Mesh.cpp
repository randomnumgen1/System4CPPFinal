#include "System/Mesh.hpp"


namespace System {



    void Mesh::GetNativeIndexBufferPtr(){

    }
    void Mesh::GetNativeVertexBufferPtr(){

    }

    void Mesh::GetUVs(int channel, std::vector<Vector2>& outUVs) {
        if ((channel < 0) || (channel > 7)) throw std::out_of_range("Channel must be between 0 and 7");
        if (auto* pval = std::get_if<std::vector<Vector2>>(&uvs[channel])) {
            outUVs = *pval;
        }
        else {
            throw std::runtime_error("UV channel " + std::to_string(channel) + " does not contain Vector2 data.");
        }
    }
    void Mesh::GetUVs(int channel, std::vector<Vector3>& outUVs){
        if ((channel < 0) || (channel > 7)) throw std::out_of_range("Channel must be between 0 and 7");
        if (auto* pval = std::get_if<std::vector<Vector3>>(&uvs[channel])) {
            outUVs = *pval;
        }
        else {
            throw std::runtime_error("UV channel " + std::to_string(channel) + " does not contain Vector3 data.");
        }
    }



}