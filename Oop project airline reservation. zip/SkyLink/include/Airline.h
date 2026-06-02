#ifndef AIRLINE_H
#define AIRLINE_H

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "Flight.h"
#include "Passenger.h"
#include "Ticket.h"

class Airline {
private:
    std::string name;
    std::vector<std::unique_ptr<Flight>> flights;
    std::vector<std::unique_ptr<Passenger>> passengers;
    std::vector<std::unique_ptr<Ticket>> tickets;

    int nextTicketID;

    // Internal helpers
    Flight*    findFlight(const std::string& flightNumber) const;
    Passenger* findPassenger(const std::string& passengerID) const;
    Ticket*    findTicket(const std::string& ticketID) const;
    std::string generateSeatNumber(const Flight* f) const;
    int daysBetween(const std::string& date1, const std::string& date2) const;
    std::string today() const;

public:
    explicit Airline(const std::string& name);

    // Flight management
    void addFlight(std::unique_ptr<Flight> flight);
    void removeFlight(const std::string& flightNumber);
    void listAllFlights() const;
    void searchFlightByNumber(const std::string& flightNumber) const;
    void searchFlightsByRoute(const std::string& origin, const std::string& destination) const;
    void searchFlightsByDate(const std::string& date) const;

    // Passenger management
    void addPassenger(std::unique_ptr<Passenger> passenger);
    void removePassenger(const std::string& passengerID);
    void viewPassengerHistory(const std::string& passengerID) const;
    void listAllPassengers() const;

    // Booking & cancellation
    std::string bookTicket(const std::string& passengerID, const std::string& flightNumber);
    double cancelTicket(const std::string& ticketID);

    // Reports
    void reportTodayDepartures() const;
    void reportOccupancy() const;
    void reportTopRevenueFlights() const;

    // Persistence
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif // AIRLINE_H
