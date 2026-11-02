// Main ships no test framework on purpose: plain assert / static_assert only.
// Full framework examples live on tests/gtest and tests/Boost-ext-ut branches.

// assert() disappears when NDEBUG is defined (Release, RelWithDebInfo): the
// binary would exit 0 having checked nothing, and CI would call that a pass.
// Keep the checks on in every build type.
#undef NDEBUG
#include <cassert>
#include <stdexcept>

#include "Example.h"      // the library under test; include/ comes from oop
#include "ZoneMap.h"

static_assert(Example::limit > 0, "the invariant below assumes a positive limit");

// checked through the public interface only: the helper behind the invariant is
// file-local in src/Example.cpp and cannot even be named here
int main() {
    Example e;
    assert(e.value() == 0);

    e.add(7);
    assert(e.value() == 7);
    e.add(-7);
    assert(e.value() == 0);

    bool threw = false;
    try {
        e.add(-1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "add() must refuse to go below 0");
    assert(e.value() == 0 && "a failed call leaves the object untouched");

    threw = false;
    try {
        e.add(Example::limit + 1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "add() must refuse to go above limit");

    threw = false;
    try {
        Example broken{Example::limit + 1};
        (void) broken;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "the constructor must not hand out a broken object either");

    ZoneMap z{18.0, 24.0};
    assert(z.canBeInside(20.0, 22.0) && "can be in interval - inside");
    assert(z.canBeInside(24.0, 32.0) && "can be in interval - touches max edge");
    assert(!z.canBeInside(30.0, 32.0) && "cannot be in interval - fully right");
    assert(!z.canBeInside(10.0, 17.0) && "cannot be in interval - fully left");
    assert(!z.canBeInside(30.0, 24.0) && "cannot be in interval - invalid interval");

    threw = false;
    try {
        ZoneMap broken{24.0, 18.0};
        (void) broken;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "the constructor must not hand out a broken object either");

    return 0;
}
