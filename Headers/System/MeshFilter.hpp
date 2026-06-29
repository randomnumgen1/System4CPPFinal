#ifndef _SYSTEM_MESHFILTER_HPP
#define _SYSTEM_MESHFILTER_HPP

#include <System/Component.hpp>
#include <System/Mesh.hpp>

namespace System {
    class MeshFilter : public Component {
    public:
        Mesh* mesh = nullptr;


        Mesh* m_SharedMesh = nullptr;  
        Mesh* m_InstanceMesh = nullptr; 


        void SetMesh(Mesh* newMesh) {
            mesh = newMesh;
        }
        void GetMesh(Mesh* newMesh) {
            mesh = newMesh;
        }

        void Setsharedmesh(Mesh* newMesh) {
            mesh = newMesh;
        }
        void Getsharedmesh(Mesh* newMesh) {
            mesh = newMesh;
        }

        //void
        Component* Clone() const override {
            MeshFilter* newMeshFilter = new MeshFilter();
            newMeshFilter->mesh = mesh;
            return newMeshFilter;
        }
    };
}

#endif
