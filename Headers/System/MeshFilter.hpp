#ifndef _SYSTEM_MESHFILTER_HPP
#define _SYSTEM_MESHFILTER_HPP

#include <System/Component.hpp>
#include <System/Mesh.hpp>

namespace System {
    class MeshFilter : public Component {
    public:
        Mesh* mesh;

        Component* Clone() const override {
            MeshFilter* newMeshFilter = new MeshFilter();
            newMeshFilter->mesh = mesh;
            return newMeshFilter;
        }
    };
}

#endif
