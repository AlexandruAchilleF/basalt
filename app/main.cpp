#include <iostream>
#include "Table.h"
#include <string>

int main() {
    // 2026-09-01 00:00:00 și 23:59:59 UTC, în secunde Unix
    constexpr double septemberFirstStart = 1788220800.0;
    constexpr double septemberFirstEnd = 1788307199.0;

    const std::string tripsPath = "assets/trips_september_2026_sample.csv";
    const Table trips = Table::fromCsv(tripsPath);

    std::cout << trips << "\n";

    const double septemberFirstTips = trips.sumWhere("tip_amount", "tpep_pickup_datetime", septemberFirstStart, septemberFirstEnd);
    std::cout << "Total tips for September 1, 2026: " << septemberFirstTips << "\n";

    const std::size_t skippedGroups = trips.countSkipped("tpep_pickup_datetime", septemberFirstStart, septemberFirstEnd);
    std::cout << "Number of skipped groups by zone maps for September 1, 2026: " << skippedGroups << "\n";

    const auto tipsByPayment = trips.sumBy("payment_type", "tip_amount");
    for (const auto& [paymentType, totalTip] : tipsByPayment) {
        std::cout << "Total tips for payment type " << paymentType << ": " << totalTip << "\n";
    }

    std::cout << "Compression ratio for column 'tpep_pickup_datetime': " << trips.compressionRatio("tpep_pickup_datetime") << "\n";
    std::cout << "Compression ratio for column 'payment_type': " << trips.compressionRatio("payment_type") << "\n";
    std::cout << "Dictionary helps on low-cardinality columns and hurts on near-unique ones\n";

    std::cout << "Copy check: copy-constructed, then assigned from the original table\n";
    std::cout << "Original tips for September 1, 2026: " << septemberFirstTips << "\n";
    Table snapshot = trips;
    const double copiedTips = snapshot.sumWhere("tip_amount", "tpep_pickup_datetime", septemberFirstStart, septemberFirstEnd);
    std::cout << "After copy constructor tips for September 1, 2026: " << copiedTips << "\n";
    snapshot = trips;
    const double assignedTips = snapshot.sumWhere("tip_amount", "tpep_pickup_datetime", septemberFirstStart, septemberFirstEnd);
    std::cout << "After copy assignment tips for September 1, 2026: " << assignedTips << "\n";
    return 0;
}