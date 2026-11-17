// Main ships no test framework on purpose: plain assert / static_assert only.
// Full framework examples live on tests/gtest and tests/Boost-ext-ut branches.
// Each *_test.cpp file holds the asserts for one class; this file only runs them.

#include "TestSuites.h"

int main() {
    testZoneMap();
    testColumnChunk();
    testRowGroup();
    testTable();
    return 0;
}
