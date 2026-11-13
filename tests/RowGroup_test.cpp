//
// Created by Flueraru Alexandru Achille on 13/11/2026.
//
// assert() disappears when NDEBUG is defined (Release, RelWithDebInfo): the
// binary would exit 0 having checked nothing, and CI would call that a pass.
// Keep the checks on in every build type.
#undef NDEBUG
#include <cassert>
#include <stdexcept>
#include <vector>

#include "TestSuites.h"
#include "RowGroup.h"

void testRowGroup() {
    //Empty column chunk -> False
    bool threw = false;
    try {
        RowGroup empty{std::vector<ColumnChunk>{}};
        (void) empty;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "empty row groups must throw");

    //Variable size column chunks -> False
    threw = false;
    std::vector<double> sixValues{1, 2, 3, 4, 5, 6};
    std::vector<double> fiveValues{1, 2, 3, 4, 5};
    std::vector<ColumnChunk> chunks{ColumnChunk{sixValues}, ColumnChunk{fiveValues}};
    try {
        RowGroup variable_size{chunks};
        (void) variable_size;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "variable size row groups must throw");

    //Same size column chunks -> True
    threw = false;
    std::vector<double> sixValues2{1, 2, 3, 4, 5, 6};
    std::vector<double> sixValues3{7, 8, 9, 10, 11, 12};
    std::vector<ColumnChunk> chunks2{ColumnChunk{sixValues2}, ColumnChunk{sixValues3}};
    try {
        RowGroup same_size{chunks2};
        (void) same_size;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(!threw && "same size row groups must not throw");

    //canBeInside with invalid column index
    threw = false;
    RowGroup rowGroup{chunks2};
    try {
        rowGroup.canBeInside(5, 0.0, 10.0);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw && "out of range row groups must throw");

    //canBeInside with valid column index and lo/hi values
    assert(rowGroup.canBeInside(0, 2.0, 3.0) && "can be inside with valid column index and lo/hi values");

    //canBeInside with valid column index and out of range lo/hi values
    assert(!rowGroup.canBeInside(0, 10.0, 20.0) && "cannot be inside with valid column index and out of range lo/hi values");

    // sumWhere: tip sum for fares in [lo, hi])
    std::vector<double> fares{12, 55, 30, 70};
    std::vector<double> tips{2, 10, 0, 15};
    std::vector<ColumnChunk> chunks3{ColumnChunk{fares}, ColumnChunk{tips}};
    RowGroup rowGroup2{chunks3};
    assert(rowGroup2.sumWhere(1, 0, 50, 100) == 25 && "sumWhere must return correct value");
    assert(rowGroup2.sumWhere(1, 0, 200, 300) == 0 && "sumWhere must return correct value");


}