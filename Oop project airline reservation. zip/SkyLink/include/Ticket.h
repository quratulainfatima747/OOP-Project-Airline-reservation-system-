#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <iostream>

enum class BookingStatus { CONFIRMED, CANCELLED };

class Ticket {
private:
    std::string ticketID;
    std::string passengerID;
    std::string flightNumber;
    std::string seatNumber;
    double farePaid;
    BookingStatus status;
    std::string bookingDate; // "YYYY-MM-DD"

public:
    Ticket(const std::string& ticketID,
           const std::string& passengerID,
           const std::string& flightNumber,
           const std::string& seatNumber,
           double farePaid,
           const std::string& bookingDate);

    // Getters
    std::string getTicketID() const { return ticketID; }
    std::string getPassengerID() const { return passengerID; }
    std::string getFlightNumber() const { return flightNumber; }
    std::string getSeatNumber() const { return seatNumber; }
    double getFarePaid() const { return farePaid; }
    BookingStatus getStatus() const { return status; }
    std::string getBookingDate() const { return bookingDate; }
    std::string getStatusStr() const {
        return status == BookingStatus::CONFIRMED ? "CONFIRMED" : "CANCELLED";
    }

    void cancel() { status = BookingStatus::CANCELLED; }

    // Operator overloads
    bool operator==(const Ticket& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Ticket& t);
};

#endif // TICKET_H
