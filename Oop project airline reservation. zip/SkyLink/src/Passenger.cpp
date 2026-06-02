#include "Passenger.h"
#include <iostream>
#include <iomanip>

// ── Passenger base ────────────────────────────────────────────────────────────

Passenger::Passenger(const std::string& passengerID,
                     const std::string& name,
                     const std::string& email,
                     const std::string& phone)
    : passengerID(passengerID), name(name), email(email), phone(phone),
      loyaltyPoints(0) {}

void Passenger::displayInfo() const {
    std::cout << "[" << getPassengerClass() << "] "
              << name << " (ID: " << passengerID << ")\n"
              << "  Email  : " << email << "\n"
              << "  Phone  : " << phone << "\n"
              << "  Baggage: " << getBaggageAllowanceKg() << " kg\n"
              << "  Loyalty: " << loyaltyPoints << " pts\n";
}

std::ostream& operator<<(std::ostream& os, const Passenger& p) {
    os << "[" << p.getPassengerClass() << "] " << p.name
       << " (ID: " << p.passengerID << ")";
    return os;
}

// ── EconomyPassenger ──────────────────────────────────────────────────────────

EconomyPassenger::EconomyPassenger(const std::string& id, const std::string& name,
                                   const std::string& email, const std::string& phone)
    : Passenger(id, name, email, phone) {}

double EconomyPassenger::getRefundPercentage(int daysBeforeDeparture) const {
    if (daysBeforeDeparture >= 14) return 80.0;
    if (daysBeforeDeparture >=  7) return 50.0;
    if (daysBeforeDeparture >=  2) return 20.0;
    return 0.0; // within 48 h — no refund
}

// ── BusinessPassenger ─────────────────────────────────────────────────────────

BusinessPassenger::BusinessPassenger(const std::string& id, const std::string& name,
                                     const std::string& email, const std::string& phone)
    : Passenger(id, name, email, phone) {}

double BusinessPassenger::getRefundPercentage(int daysBeforeDeparture) const {
    if (daysBeforeDeparture >= 7) return 90.0;
    if (daysBeforeDeparture >= 2) return 60.0;
    return 25.0; // last-minute still gets some refund
}

// ── FirstClassPassenger ───────────────────────────────────────────────────────

FirstClassPassenger::FirstClassPassenger(const std::string& id, const std::string& name,
                                         const std::string& email, const std::string& phone)
    : Passenger(id, name, email, phone) {}

double FirstClassPassenger::getRefundPercentage(int daysBeforeDeparture) const {
    if (daysBeforeDeparture >= 3) return 100.0; // full refund if 3+ days ahead
    if (daysBeforeDeparture >= 1) return  75.0;
    return 50.0; // same-day partial refund
}
