// assert() disappears when NDEBUG is defined (Release, RelWithDebInfo): the
// binary would exit 0 having checked nothing, and CI would call that a pass.
// Keep the checks on in every build type.
#undef NDEBUG
#include <cassert>
#include <stdexcept>

#include "TestSuites.h"
#include "ZoneMap.h"

void testZoneMap() {
    ZoneMap z{18.0, 24.0};
    assert(z.canBeInside(20.0, 22.0) && "can be in interval - inside");
    assert(z.canBeInside(24.0, 32.0) && "can be in interval - touches max edge");
    assert(!z.canBeInside(30.0, 32.0) && "cannot be in interval - fully right");
    assert(!z.canBeInside(10.0, 17.0) && "cannot be in interval - fully left");
    assert(!z.canBeInside(30.0, 24.0) && "cannot be in interval - invalid interval");

    bool threw = false;
    try {
        ZoneMap broken{24.0, 18.0};
        (void) broken;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "the constructor must not hand out a broken object either");
}
