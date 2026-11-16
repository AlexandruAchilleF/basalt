//
// Created by Flueraru Alexandru Achille on 16/11/2026.
//
// assert() disappears when NDEBUG is defined (Release, RelWithDebInfo): the
// binary would exit 0 having checked nothing, and CI would call that a pass.
// Keep the checks on in every build type.
#undef NDEBUG
#include <cassert>
#include <stdexcept>
#include <vector>
#include <string>

#include "TestSuites.h"
#include "Table.h"

void testTable() {
    //Empty column names -> False
    bool threw = false;

    try {
        Table empty{std::vector<std::string>{}, std::vector<RowGroup>{}};
        (void) empty;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "empty column names must throw");

    //Empty row groups -> False
    threw = false;
    try {
        Table empty{std::vector<std::string>{"a", "b"}, std::vector<RowGroup>{}};
        (void) empty;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "empty row groups must throw");

    //Row groups with different column count than column names -> False
    threw = false;
    std::vector<double> sixValues{1, 2, 3, 4, 5, 6};
    std::vector<ColumnChunk> chunks1{ColumnChunk{sixValues}, ColumnChunk{sixValues}};
    std::vector<ColumnChunk> chunks2{ColumnChunk{sixValues}, ColumnChunk{sixValues}};
    std::vector<ColumnChunk> chunks3{ColumnChunk{sixValues}, ColumnChunk{sixValues}};
    std::vector<RowGroup> rowGroups{RowGroup{chunks1}, RowGroup{chunks2}, RowGroup{chunks3}};
    try {
        Table different_column_counts{std::vector<std::string>{"a", "b", "c"}, rowGroups};
        (void) different_column_counts;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "row groups with different column count than column names must throw");

    //Valid -> True
    Table valid{std::vector<std::string>{"a", "b"}, rowGroups};
    (void)  valid;
}