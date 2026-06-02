#include "Ticket.h"
#include <iomanip>

Ticket::Ticket(const std::string& ticketID,
               const std::string& passengerID,
               const std::string& flightNumber,
               const std::string& seatNumber,
               double farePaid,
               const std::string& bookingDate)
    : ticketID(ticketID), passengerID(passengerID), flightNumber(flightNumber),
      seatNumber(seatNumber), farePaid(farePaid), status(BookingStatus::CONFIRMED),
      bookingDate(bookingDate) {}

bool Ticket::operator==(const Ticket& other) const {
    return ticketID == other.ticketID;
}

std::ostream& operator<<(std::ostream& os, const Ticket& t) {
    os << "Ticket[" << t.ticketID << "] "
       << "Flight: " << t.flightNumber
       << "  Seat: " << t.seatNumber
       << "  Fare: $" << std::fixed << std::setprecision(2) << t.farePaid
       << "  Status: " << t.getStatusStr()
       << "  Booked: " << t.bookingDate;
    return os;
}
