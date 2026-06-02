#include "Flight.h"
#include <iomanip>

Flight::Flight(const std::string& flightNumber,
               const std::string& origin,
               const std::string& destination,
               const std::string& departureDateTime,
               int totalSeats)
    : flightNumber(flightNumber), origin(origin), destination(destination),
      departureDateTime(departureDateTime), totalSeats(totalSeats),
      availableSeats(totalSeats) {}

double Flight::getOccupancyPct() const {
    if (totalSeats == 0) return 0.0;
    return 100.0 * (totalSeats - availableSeats) / totalSeats;
}

bool Flight::bookSeat() {
    if (availableSeats <= 0) return false;
    --availableSeats;
    return true;
}

void Flight::releaseSeat() {
    if (availableSeats < totalSeats) ++availableSeats;
}

std::ostream& operator<<(std::ostream& os, const Flight& f) {
    os << "[" << f.getType() << "] " << f.flightNumber
       << "  " << f.origin << " -> " << f.destination
       << "  Dep: " << f.departureDateTime
       << "  Seats: " << f.availableSeats << "/" << f.totalSeats
       << "  Base fare: $" << std::fixed << std::setprecision(2) << f.calculateBaseFare();
    return os;
}
