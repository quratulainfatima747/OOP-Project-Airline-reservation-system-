#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <iostream>

class Flight {
protected:
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureDateTime; // "YYYY-MM-DD HH:MM"
    int totalSeats;
    int availableSeats;

public:
    Flight(const std::string& flightNumber,
           const std::string& origin,
           const std::string& destination,
           const std::string& departureDateTime,
           int totalSeats);

    virtual ~Flight() = default;

    // Pure virtual functions
    virtual double calculateBaseFare() const = 0;
    virtual void displayDetails() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getExtraData() const = 0;       // for persistence
    virtual void setExtraData(const std::string& data) = 0; // for persistence

    // Getters
    std::string getFlightNumber() const { return flightNumber; }
    std::string getOrigin() const { return origin; }
    std::string getDestination() const { return destination; }
    std::string getDepartureDateTime() const { return departureDateTime; }
    int getTotalSeats() const { return totalSeats; }
    int getAvailableSeats() const { return availableSeats; }
    double getOccupancyPct() const;

    // Seat management
    bool bookSeat();
    void releaseSeat();

    // Operator overloads
    friend std::ostream& operator<<(std::ostream& os, const Flight& f);
};

#endif // FLIGHT_H
