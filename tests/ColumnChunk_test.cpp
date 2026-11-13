// assert() disappears when NDEBUG is defined (Release, RelWithDebInfo): the
// binary would exit 0 having checked nothing, and CI would call that a pass.
// Keep the checks on in every build type.
#undef NDEBUG
#include <cassert>
#include <stdexcept>
#include <vector>

#include "TestSuites.h"
#include "ColumnChunk.h"

void testColumnChunk() {
    bool threw = false;
    try {
        ColumnChunk empty{std::vector<double>{}};
        (void) empty;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "empty values must throw");

    ColumnChunk chunk{std::vector<double>{2, 3, 4, 5}};
    assert(chunk.canBeInside(1, 4) && "can be in interval - inside");
    assert(!chunk.canBeInside(0, 1) && "cannot be in interval - fully left");
    assert(!chunk.canBeInside(6, 8) && "cannot be in interval - fully right");
    assert(chunk.size() == 4 && "size must be correct");
    assert(chunk.valueAt(0) == 2 && "valueAt must return correct value");
    assert(chunk.valueAt(3) == 5 && "valueAt must return correct value");

    threw = false;
    try {
        chunk.valueAt(4);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw && "valueAt must throw for out-of-bounds access");
}
