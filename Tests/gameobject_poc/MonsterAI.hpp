#ifndef _SYSTEM_MONSTERAI_H
#define _SYSTEM_MONSTERAI_H

#include <System/Component.hpp>

namespace System {
    struct MonsterAI : public Component {
        void Awake() override;
        void Start() override;
        void Update();
        Component* Clone() const override;
    };
}

#endif
