#include "DerivedFlights.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// ── DomesticFlight ────────────────────────────────────────────────────────────

DomesticFlight::DomesticFlight(const std::string& flightNumber,
                               const std::string& origin,
                               const std::string& destination,
                               const std::string& departureDateTime,
                               int totalSeats,
                               double baseFarePerKm,
                               double distanceKm)
    : Flight(flightNumber, origin, destination, departureDateTime, totalSeats),
      baseFarePerKm(baseFarePerKm), distanceKm(distanceKm) {}

double DomesticFlight::calculateBaseFare() const {
    return baseFarePerKm * distanceKm;
}

void DomesticFlight::displayDetails() const {
    std::cout << *this << "\n";
    std::cout << "  Type       : Domestic\n"
              << "  Distance   : " << distanceKm << " km\n"
              << "  Fare/km    : $" << std::fixed << std::setprecision(2) << baseFarePerKm << "\n";
}

std::string DomesticFlight::getExtraData() const {
    // Format: "baseFarePerKm|distanceKm"
    std::ostringstream oss;
    oss << baseFarePerKm << "|" << distanceKm;
    return oss.str();
}

void DomesticFlight::setExtraData(const std::string& data) {
    std::istringstream iss(data);
    std::string tok;
    std::getline(iss, tok, '|'); baseFarePerKm = std::stod(tok);
    std::getline(iss, tok, '|'); distanceKm    = std::stod(tok);
    // Recalculate available seats stays as loaded; handled by Airline::loadFromFile
}

// ── InternationalFlight ───────────────────────────────────────────────────────

InternationalFlight::InternationalFlight(const std::string& flightNumber,
                                         const std::string& origin,
                                         const std::string& destination,
                                         const std::string& departureDateTime,
                                         int totalSeats,
                                         double baseFare,
                                         bool visaRequired,
                                         const std::string& country)
    : Flight(flightNumber, origin, destination, departureDateTime, totalSeats),
      baseFare(baseFare), visaRequired(visaRequired), country(country) {}

double InternationalFlight::calculateBaseFare() const {
    return baseFare * (visaRequired ? 1.15 : 1.0); // 15% surcharge if visa needed
}

void InternationalFlight::displayDetails() const {
    std::cout << *this << "\n";
    std::cout << "  Type         : International\n"
              << "  Country      : " << country << "\n"
              << "  Visa Required: " << (visaRequired ? "Yes" : "No") << "\n";
}

std::string InternationalFlight::getExtraData() const {
    std::ostringstream oss;
    oss << baseFare << "|" << (visaRequired ? 1 : 0) << "|" << country;
    return oss.str();
}

void InternationalFlight::setExtraData(const std::string& data) {
    std::istringstream iss(data);
    std::string tok;
    std::getline(iss, tok, '|'); baseFare     = std::stod(tok);
    std::getline(iss, tok, '|'); visaRequired = (tok == "1");
    std::getline(iss, tok);      country      = tok;
}

// ── CharterFlight ─────────────────────────────────────────────────────────────

CharterFlight::CharterFlight(const std::string& flightNumber,
                             const std::string& origin,
                             const std::string& destination,
                             const std::string& departureDateTime,
                             int totalSeats,
                             double flatFare,
                             const std::string& contractHolder)
    : Flight(flightNumber, origin, destination, departureDateTime, totalSeats),
      flatFare(flatFare), contractHolder(contractHolder) {}

double CharterFlight::calculateBaseFare() const {
    return flatFare;
}

void CharterFlight::displayDetails() const {
    std::cout << *this << "\n";
    std::cout << "  Type           : Charter\n"
              << "  Contract Holder: " << contractHolder << "\n"
              << "  Flat Fare      : $" << std::fixed << std::setprecision(2) << flatFare << "\n";
}

std::string CharterFlight::getExtraData() const {
    std::ostringstream oss;
    oss << flatFare << "|" << contractHolder;
    return oss.str();
}

void CharterFlight::setExtraData(const std::string& data) {
    std::istringstream iss(data);
    std::string tok;
    std::getline(iss, tok, '|'); flatFare        = std::stod(tok);
    std::getline(iss, tok);      contractHolder  = tok;
}
