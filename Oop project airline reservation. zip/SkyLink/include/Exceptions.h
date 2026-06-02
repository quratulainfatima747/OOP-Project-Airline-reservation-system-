#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class FlightFullException : public std::runtime_error {
public:
    explicit FlightFullException(const std::string& flightNumber)
        : std::runtime_error("Flight " + flightNumber + " is fully booked.") {}
};

class InvalidCancellationException : public std::runtime_error {
public:
    explicit InvalidCancellationException(const std::string& reason)
        : std::runtime_error("Invalid cancellation: " + reason) {}
};

class FlightNotFoundException : public std::runtime_error {
public:
    explicit FlightNotFoundException(const std::string& id)
        : std::runtime_error("Flight not found: " + id) {}
};

class PassengerNotFoundException : public std::runtime_error {
public:
    explicit PassengerNotFoundException(const std::string& id)
        : std::runtime_error("Passenger not found: " + id) {}
};

class DuplicateBookingException : public std::runtime_error {
public:
    explicit DuplicateBookingException(const std::string& msg)
        : std::runtime_error("Duplicate booking: " + msg) {}
};

#endif // EXCEPTIONS_H
