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
#include <cmath>
#include <sstream>

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

    assert(valid.sumWhere("a", "b", 1.0, 3.0) == 18.0);

    std::vector<ColumnChunk> chunks4{ColumnChunk{std::vector<double>{12, 55, 30, 70}}, ColumnChunk{std::vector<double>{2, 10, 0, 15}}};
    std::vector<ColumnChunk> chunks5{ColumnChunk{std::vector<double>{100, 120}}, ColumnChunk{std::vector<double>{20, 25}}};
    std::vector<RowGroup> rowGroups2{RowGroup{chunks4}, RowGroup{chunks5}};
    Table trips = Table{std::vector<std::string>{"fare", "tip"}, rowGroups2};
    assert(trips.sumWhere("tip", "fare", 50.0, 110.0) == 45.0);

    assert(trips.sumWhere("tip", "fare", 1000.0, 2000.0) == 0.0);

    threw = false;
    try {
        (void) trips.sumWhere("tip", "nu_exista", 1.0, 3.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "sumWhere must throw for invalid column names");

    assert(trips.countSkipped("fare", 50.0, 110.0) == 0);
    assert(trips.countSkipped("fare", 100.0, 120.0) == 1);
    assert(trips.countSkipped("fare", 1000.0, 2000.0) == 2);

    Table taxi = Table::fromCsv("assets/trips_september_2026_sample.csv");
    // 2026-09-01 00:00:00 UTC – 23:59:59 UTC
    assert(taxi.countSkipped("tpep_pickup_datetime", 1788220800.0, 1788307199.0) == 19);
    assert(std::abs(taxi.sumWhere("tip_amount", "tpep_pickup_datetime", 1788220800.0, 1788307199.0) - 2368.15) < 1e-6);

    std::ostringstream oss;
    oss << taxi;
    assert(oss.str().find("20 row groups") != std::string::npos);
    assert(oss.str().find("ZoneMap") != std::string::npos);

    Table copy = trips;
    assert(copy.sumWhere("tip", "fare", 50.0, 110.0) == 45.0);

    copy = taxi;
    assert(copy.countSkipped("tpep_pickup_datetime", 1788220800.0, 1788307199.0) == 19);

    assert(trips.sumWhere("tip", "fare", 50.0, 110.0) == 45.0);

    std::vector<double> payment{1,2,1};
    std::vector<double> tip{5, 0, 3};
    std::vector<ColumnChunk> chunks6{ColumnChunk{payment}, ColumnChunk{tip}};
    RowGroup rowGroup3{chunks6};
    std::vector<double> payment2{1,2,2};
    std::vector<double> tip2{2, 0, 1};
    std::vector<ColumnChunk> chunks7{ColumnChunk{payment2}, ColumnChunk{tip2}};
    RowGroup rowGroup4{chunks7};
    Table table{{"payment", "tip"}, {rowGroup3, rowGroup4}};
    const std::map<double, double> expected{{1, 10}, {2, 1}};
    assert(table.sumBy("payment", "tip") == expected);
}
