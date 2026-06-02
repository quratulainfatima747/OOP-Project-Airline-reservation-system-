#include "Airline.h"
#include "DerivedFlights.h"
#include "SearchUtils.h"
#include "Exceptions.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <stdexcept>

// ── Constructor ───────────────────────────────────────────────────────────────

Airline::Airline(const std::string& name) : name(name), nextTicketID(1) {}

// ── Private helpers ───────────────────────────────────────────────────────────

Flight* Airline::findFlight(const std::string& flightNumber) const {
    for (const auto& f : flights)
        if (f->getFlightNumber() == flightNumber) return f.get();
    return nullptr;
}

Passenger* Airline::findPassenger(const std::string& passengerID) const {
    for (const auto& p : passengers)
        if (p->getPassengerID() == passengerID) return p.get();
    return nullptr;
}

Ticket* Airline::findTicket(const std::string& ticketID) const {
    for (const auto& t : tickets)
        if (t->getTicketID() == ticketID) return t.get();
    return nullptr;
}

std::string Airline::generateSeatNumber(const Flight* f) const {
    int booked = f->getTotalSeats() - f->getAvailableSeats() + 1;
    int row    = (booked - 1) / 6 + 1;
    char col   = 'A' + (booked - 1) % 6;
    return std::to_string(row) + col;
}

std::string Airline::today() const {
    std::time_t t  = std::time(nullptr);
    std::tm*    tm = std::localtime(&t);
    char buf[11];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
    return std::string(buf);
}

// Returns difference date2 - date1 in days (date format YYYY-MM-DD)
int Airline::daysBetween(const std::string& date1, const std::string& date2) const {
    auto toTm = [](const std::string& s) -> std::tm {
        std::tm tm{};
        tm.tm_year = std::stoi(s.substr(0,4)) - 1900;
        tm.tm_mon  = std::stoi(s.substr(5,2)) - 1;
        tm.tm_mday = std::stoi(s.substr(8,2));
        tm.tm_isdst = -1;
        return tm;
    };
    std::tm t1 = toTm(date1);
    std::tm t2 = toTm(date2);
    std::time_t tt1 = std::mktime(&t1);
    std::time_t tt2 = std::mktime(&t2);
    return static_cast<int>((tt2 - tt1) / 86400);
}

// ── Flight management ─────────────────────────────────────────────────────────

void Airline::addFlight(std::unique_ptr<Flight> flight) {
    if (findFlight(flight->getFlightNumber()))
        throw std::invalid_argument("Flight " + flight->getFlightNumber() + " already exists.");
    std::cout << "Flight " << flight->getFlightNumber() << " added.\n";
    flights.push_back(std::move(flight));
}

void Airline::removeFlight(const std::string& flightNumber) {
    auto it = std::find_if(flights.begin(), flights.end(),
        [&](const std::unique_ptr<Flight>& f){ return f->getFlightNumber() == flightNumber; });
    if (it == flights.end())
        throw FlightNotFoundException(flightNumber);
    flights.erase(it);
    std::cout << "Flight " << flightNumber << " removed.\n";
}

void Airline::listAllFlights() const {
    if (flights.empty()) { std::cout << "No flights registered.\n"; return; }
    std::cout << "\n===== All Flights (" << flights.size() << ") =====\n";
    for (const auto& f : flights) std::cout << *f << "\n";
}

void Airline::searchFlightByNumber(const std::string& flightNumber) const {
    Flight* f = findFlight(flightNumber);
    if (!f) throw FlightNotFoundException(flightNumber);
    f->displayDetails();
}

void Airline::searchFlightsByRoute(const std::string& origin,
                                   const std::string& destination) const {
    bool found = false;
    for (const auto& f : flights)
        if (f->getOrigin() == origin && f->getDestination() == destination) {
            std::cout << *f << "\n";
            found = true;
        }
    if (!found) std::cout << "No flights found for route " << origin << " -> " << destination << "\n";
}

void Airline::searchFlightsByDate(const std::string& date) const {
    bool found = false;
    for (const auto& f : flights)
        if (f->getDepartureDateTime().substr(0,10) == date) {
            std::cout << *f << "\n";
            found = true;
        }
    if (!found) std::cout << "No flights found on " << date << "\n";
}

// ── Passenger management ──────────────────────────────────────────────────────

void Airline::addPassenger(std::unique_ptr<Passenger> passenger) {
    if (findPassenger(passenger->getPassengerID()))
        throw std::invalid_argument("Passenger " + passenger->getPassengerID() + " already exists.");
    std::cout << "Passenger " << passenger->getName() << " registered.\n";
    passengers.push_back(std::move(passenger));
}

void Airline::removePassenger(const std::string& passengerID) {
    auto it = std::find_if(passengers.begin(), passengers.end(),
        [&](const std::unique_ptr<Passenger>& p){ return p->getPassengerID() == passengerID; });
    if (it == passengers.end())
        throw PassengerNotFoundException(passengerID);
    passengers.erase(it);
    std::cout << "Passenger " << passengerID << " removed.\n";
}

void Airline::listAllPassengers() const {
    if (passengers.empty()) { std::cout << "No passengers registered.\n"; return; }
    std::cout << "\n===== Passengers (" << passengers.size() << ") =====\n";
    for (const auto& p : passengers) p->displayInfo();
}

void Airline::viewPassengerHistory(const std::string& passengerID) const {
    Passenger* p = findPassenger(passengerID);
    if (!p) throw PassengerNotFoundException(passengerID);
    std::cout << "\nBooking history for " << *p << ":\n";
    bool any = false;
    for (const auto& t : tickets)
        if (t->getPassengerID() == passengerID) {
            std::cout << "  " << *t << "\n";
            any = true;
        }
    if (!any) std::cout << "  No bookings.\n";
}

// ── Booking & cancellation ────────────────────────────────────────────────────

std::string Airline::bookTicket(const std::string& passengerID,
                                const std::string& flightNumber) {
    Passenger* p = findPassenger(passengerID);
    if (!p) throw PassengerNotFoundException(passengerID);

    Flight* f = findFlight(flightNumber);
    if (!f) throw FlightNotFoundException(flightNumber);

    if (f->getAvailableSeats() <= 0)
        throw FlightFullException(flightNumber);

    // Check duplicate booking
    for (const auto& t : tickets)
        if (t->getPassengerID() == passengerID &&
            t->getFlightNumber() == flightNumber &&
            t->getStatus() == BookingStatus::CONFIRMED)
            throw DuplicateBookingException(passengerID + " already booked on " + flightNumber);

    // Calculate fare with class multiplier
    double fare = f->calculateBaseFare() * p->getLoyaltyMultiplier();

    // Book seat
    f->bookSeat();
    std::string seat = generateSeatNumber(f);

    std::string ticketID = "TKT" + std::to_string(nextTicketID++);
    tickets.push_back(std::make_unique<Ticket>(ticketID, passengerID, flightNumber,
                                               seat, fare, today()));

    // Award loyalty points (1 pt per dollar)
    int pts = static_cast<int>(fare * p->getLoyaltyMultiplier());
    p->addLoyaltyPoints(pts);

    std::cout << "Booking confirmed! " << ticketID << "  Seat: " << seat
              << "  Fare: $" << std::fixed << std::setprecision(2) << fare
              << "  Loyalty pts awarded: " << pts << "\n";
    return ticketID;
}

double Airline::cancelTicket(const std::string& ticketID) {
    Ticket* t = findTicket(ticketID);
    if (!t) throw std::invalid_argument("Ticket " + ticketID + " not found.");

    if (t->getStatus() == BookingStatus::CANCELLED)
        throw InvalidCancellationException("Ticket " + ticketID + " is already cancelled.");

    Flight* f = findFlight(t->getFlightNumber());
    if (!f) throw InvalidCancellationException("Associated flight no longer exists.");

    Passenger* p = findPassenger(t->getPassengerID());
    if (!p) throw InvalidCancellationException("Associated passenger no longer exists.");

    // Calculate days before departure
    std::string depDate = f->getDepartureDateTime().substr(0,10);
    int days = daysBetween(today(), depDate);
    if (days < 0)
        throw InvalidCancellationException("Cannot cancel a past flight.");

    double refundPct   = p->getRefundPercentage(days);
    double refundAmount = t->getFarePaid() * refundPct / 100.0;

    t->cancel();
    f->releaseSeat();

    std::cout << "Ticket " << ticketID << " cancelled. Refund: $"
              << std::fixed << std::setprecision(2) << refundAmount
              << " (" << refundPct << "% of $" << t->getFarePaid() << ")\n";
    return refundAmount;
}

// ── Reports ───────────────────────────────────────────────────────────────────

void Airline::reportTodayDepartures() const {
    std::string td = today();
    std::cout << "\n===== Today's Departures (" << td << ") =====\n";
    bool any = false;
    for (const auto& f : flights)
        if (f->getDepartureDateTime().substr(0,10) == td) {
            std::cout << *f << "\n";
            any = true;
        }
    if (!any) std::cout << "No departures today.\n";
}

void Airline::reportOccupancy() const {
    std::cout << "\n===== Occupancy Report =====\n";
    // Sort by occupancy descending (copy pointers for sorting)
    std::vector<Flight*> sorted;
    for (const auto& f : flights) sorted.push_back(f.get());
    std::sort(sorted.begin(), sorted.end(),
              [](const Flight* a, const Flight* b){
                  return a->getOccupancyPct() > b->getOccupancyPct();
              });
    for (const Flight* f : sorted)
        std::cout << f->getFlightNumber() << "  " << f->getOrigin()
                  << " -> " << f->getDestination()
                  << "  Occupancy: " << std::fixed << std::setprecision(1)
                  << f->getOccupancyPct() << "%\n";
}

void Airline::reportTopRevenueFlights() const {
    std::cout << "\n===== Top 5 Revenue Flights (This Month) =====\n";
    // Build revenue map per flight
    std::map<std::string, double> revenue;
    std::string currMonth = today().substr(0,7); // "YYYY-MM"

    for (const auto& t : tickets) {
        if (t->getStatus() != BookingStatus::CONFIRMED) continue;
        // Filter tickets booked this month
        if (t->getBookingDate().substr(0,7) != currMonth) continue;
        revenue[t->getFlightNumber()] += t->getFarePaid();
    }

    // Collect & sort
    std::vector<std::pair<std::string,double>> vec(revenue.begin(), revenue.end());
    std::sort(vec.begin(), vec.end(),
              [](const auto& a, const auto& b){ return a.second > b.second; });

    int count = 0;
    for (const auto& [fn, rev] : vec) {
        if (++count > 5) break;
        std::cout << count << ". " << fn << "  Revenue: $"
                  << std::fixed << std::setprecision(2) << rev << "\n";
    }
    if (vec.empty()) std::cout << "No revenue data for this month.\n";
}

// ── Persistence ───────────────────────────────────────────────────────────────

void Airline::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot open file for saving: " + filename);

    out << nextTicketID << "\n";

    // Flights
    out << flights.size() << "\n";
    for (const auto& f : flights) {
        out << f->getType() << "|"
            << f->getFlightNumber() << "|"
            << f->getOrigin() << "|"
            << f->getDestination() << "|"
            << f->getDepartureDateTime() << "|"
            << f->getTotalSeats() << "|"
            << f->getAvailableSeats() << "|"
            << f->getExtraData() << "\n";
    }

    // Passengers
    out << passengers.size() << "\n";
    for (const auto& p : passengers) {
        out << p->getPassengerClass() << "|"
            << p->getPassengerID() << "|"
            << p->getName() << "|"
            << p->getEmail() << "|"
            << p->getPhone() << "|"
            << p->getLoyaltyPoints() << "\n";
    }

    // Tickets
    out << tickets.size() << "\n";
    for (const auto& t : tickets) {
        out << t->getTicketID() << "|"
            << t->getPassengerID() << "|"
            << t->getFlightNumber() << "|"
            << t->getSeatNumber() << "|"
            << t->getFarePaid() << "|"
            << t->getStatusStr() << "|"
            << t->getBookingDate() << "\n";
    }

    std::cout << "State saved to " << filename << "\n";
}

void Airline::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "No save file found (" << filename << "). Starting fresh.\n";
        return;
    }

    flights.clear(); passengers.clear(); tickets.clear();

    in >> nextTicketID; in.ignore();

    // Flights
    int numFlights; in >> numFlights; in.ignore();
    for (int i = 0; i < numFlights; ++i) {
        std::string line; std::getline(in, line);
        std::istringstream ss(line);
        std::string type, fn, orig, dest, dep, tok;
        int total, avail;

        std::getline(ss, type, '|');
        std::getline(ss, fn,   '|');
        std::getline(ss, orig, '|');
        std::getline(ss, dest, '|');
        std::getline(ss, dep,  '|');
        std::getline(ss, tok,  '|'); total = std::stoi(tok);
        std::getline(ss, tok,  '|'); avail = std::stoi(tok);
        std::string extra; std::getline(ss, extra);

        std::unique_ptr<Flight> f;
        if (type == "DOMESTIC") {
            f = std::make_unique<DomesticFlight>(fn, orig, dest, dep, total, 0.0, 0.0);
        } else if (type == "INTERNATIONAL") {
            f = std::make_unique<InternationalFlight>(fn, orig, dest, dep, total, 0.0, false, "");
        } else {
            f = std::make_unique<CharterFlight>(fn, orig, dest, dep, total, 0.0, "");
        }
        f->setExtraData(extra);
        // Restore available seats: book (total - avail) seats
        for (int s = 0; s < total - avail; ++s) f->bookSeat();

        flights.push_back(std::move(f));
    }

    // Passengers
    int numPass; in >> numPass; in.ignore();
    for (int i = 0; i < numPass; ++i) {
        std::string line; std::getline(in, line);
        std::istringstream ss(line);
        std::string cls, id, nm, email, phone, tok;
        int pts;

        std::getline(ss, cls,   '|');
        std::getline(ss, id,    '|');
        std::getline(ss, nm,    '|');
        std::getline(ss, email, '|');
        std::getline(ss, phone, '|');
        std::getline(ss, tok);        pts = std::stoi(tok);

        std::unique_ptr<Passenger> p;
        if      (cls == "ECONOMY")  p = std::make_unique<EconomyPassenger>(id, nm, email, phone);
        else if (cls == "BUSINESS") p = std::make_unique<BusinessPassenger>(id, nm, email, phone);
        else                        p = std::make_unique<FirstClassPassenger>(id, nm, email, phone);
        p->addLoyaltyPoints(pts);
        passengers.push_back(std::move(p));
    }

    // Tickets
    int numTickets; in >> numTickets; in.ignore();
    for (int i = 0; i < numTickets; ++i) {
        std::string line; std::getline(in, line);
        std::istringstream ss(line);
        std::string tid, pid, fn, seat, tok, status, bdate;
        double fare;

        std::getline(ss, tid,    '|');
        std::getline(ss, pid,    '|');
        std::getline(ss, fn,     '|');
        std::getline(ss, seat,   '|');
        std::getline(ss, tok,    '|'); fare = std::stod(tok);
        std::getline(ss, status, '|');
        std::getline(ss, bdate);

        auto t = std::make_unique<Ticket>(tid, pid, fn, seat, fare, bdate);
        if (status == "CANCELLED") t->cancel();
        tickets.push_back(std::move(t));
    }

    std::cout << "State loaded from " << filename << ".\n";
}
