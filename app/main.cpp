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

    Table snapshot = trips;
    snapshot = Table::fromCsv(tripsPath);
    const double snapshotTips = snapshot.sumWhere("tip_amount", "tpep_pickup_datetime", septemberFirstStart, septemberFirstEnd);
    std::cout << "Copy check: snapshot is an independent copy\n";
    std::cout << "Original tips for September 1, 2026: " << septemberFirstTips << "\n";
    std::cout << "Snapshot tips for September 1, 2026: " << snapshotTips << "\n";
    return 0;
}