#include "Airline.h"
#include "DerivedFlights.h"
#include "Exceptions.h"

#include <iostream>
#include <limits>
#include <string>
#include <memory>

// ── Input helpers ─────────────────────────────────────────────────────────────

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int getInt(const std::string& prompt) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) { clearInput(); return v; }
        std::cout << "Invalid input. Please enter an integer.\n";
        clearInput();
    }
}

static double getDouble(const std::string& prompt) {
    double v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) { clearInput(); return v; }
        std::cout << "Invalid input. Please enter a number.\n";
        clearInput();
    }
}

static std::string getString(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

// ── Sub-menus ─────────────────────────────────────────────────────────────────

static void addFlightMenu(Airline& airline) {
    std::cout << "\n--- Add Flight ---\n"
              << "1. Domestic\n2. International\n3. Charter\n";
    int choice = getInt("Type: ");

    std::string fn   = getString("Flight number: ");
    std::string orig = getString("Origin: ");
    std::string dest = getString("Destination: ");
    std::string dep  = getString("Departure (YYYY-MM-DD HH:MM): ");
    int seats        = getInt("Total seats: ");

    try {
        if (choice == 1) {
            double fareKm = getDouble("Base fare per km ($): ");
            double dist   = getDouble("Distance (km): ");
            airline.addFlight(std::make_unique<DomesticFlight>(fn, orig, dest, dep, seats, fareKm, dist));
        } else if (choice == 2) {
            double fare   = getDouble("Base fare ($): ");
            int visa      = getInt("Visa required? (1=Yes 0=No): ");
            std::string country = getString("Destination country: ");
            airline.addFlight(std::make_unique<InternationalFlight>(fn, orig, dest, dep, seats, fare, visa==1, country));
        } else if (choice == 3) {
            double flat   = getDouble("Flat fare per seat ($): ");
            std::string ch = getString("Contract holder: ");
            airline.addFlight(std::make_unique<CharterFlight>(fn, orig, dest, dep, seats, flat, ch));
        } else {
            std::cout << "Invalid type.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

static void addPassengerMenu(Airline& airline) {
    std::cout << "\n--- Register Passenger ---\n"
              << "1. Economy  2. Business  3. First Class\n";
    int choice = getInt("Class: ");

    std::string id    = getString("Passenger ID: ");
    std::string name  = getString("Name: ");
    std::string email = getString("Email: ");
    std::string phone = getString("Phone: ");

    try {
        if (choice == 1)
            airline.addPassenger(std::make_unique<EconomyPassenger>(id, name, email, phone));
        else if (choice == 2)
            airline.addPassenger(std::make_unique<BusinessPassenger>(id, name, email, phone));
        else if (choice == 3)
            airline.addPassenger(std::make_unique<FirstClassPassenger>(id, name, email, phone));
        else
            std::cout << "Invalid class.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

// ── Main menu ─────────────────────────────────────────────────────────────────

static void printMenu() {
    std::cout << "\n=========================================\n"
              << "       SkyLink Airways  -  Main Menu\n"
              << "=========================================\n"
              << " FLIGHTS\n"
              << "  1. Add flight\n"
              << "  2. Remove flight\n"
              << "  3. List all flights\n"
              << "  4. Search flight by number\n"
              << "  5. Search flights by route\n"
              << "  6. Search flights by date\n"
              << " PASSENGERS\n"
              << "  7. Register passenger\n"
              << "  8. Remove passenger\n"
              << "  9. View passenger booking history\n"
              << " 10. List all passengers\n"
              << " BOOKINGS\n"
              << " 11. Book ticket\n"
              << " 12. Cancel ticket\n"
              << " REPORTS\n"
              << " 13. Today's departures\n"
              << " 14. Occupancy report\n"
              << " 15. Top revenue flights (this month)\n"
              << "  0. Exit\n"
              << "-----------------------------------------\n";
}

int main() {
    const std::string SAVE_FILE = "data/skylink_state.txt";
    Airline airline("SkyLink Airways");

    airline.loadFromFile(SAVE_FILE);

    int choice;
    do {
        printMenu();
        choice = getInt("Enter choice: ");

        try {
            switch (choice) {
            case 1: addFlightMenu(airline); break;

            case 2: {
                std::string fn = getString("Flight number to remove: ");
                airline.removeFlight(fn);
                break;
            }
            case 3: airline.listAllFlights(); break;

            case 4: {
                std::string fn = getString("Flight number: ");
                airline.searchFlightByNumber(fn);
                break;
            }
            case 5: {
                std::string orig = getString("Origin: ");
                std::string dest = getString("Destination: ");
                airline.searchFlightsByRoute(orig, dest);
                break;
            }
            case 6: {
                std::string date = getString("Date (YYYY-MM-DD): ");
                airline.searchFlightsByDate(date);
                break;
            }
            case 7: addPassengerMenu(airline); break;

            case 8: {
                std::string id = getString("Passenger ID to remove: ");
                airline.removePassenger(id);
                break;
            }
            case 9: {
                std::string id = getString("Passenger ID: ");
                airline.viewPassengerHistory(id);
                break;
            }
            case 10: airline.listAllPassengers(); break;

            case 11: {
                std::string pid = getString("Passenger ID: ");
                std::string fn  = getString("Flight number: ");
                airline.bookTicket(pid, fn);
                break;
            }
            case 12: {
                std::string tid = getString("Ticket ID: ");
                airline.cancelTicket(tid);
                break;
            }
            case 13: airline.reportTodayDepartures(); break;
            case 14: airline.reportOccupancy(); break;
            case 15: airline.reportTopRevenueFlights(); break;

            case 0:
                airline.saveToFile(SAVE_FILE);
                std::cout << "Goodbye!\n";
                break;

            default: std::cout << "Unknown option.\n"; break;
            }
        } catch (const std::exception& e) {
            std::cout << "[ERROR] " << e.what() << "\n";
        }

    } while (choice != 0);

    return 0;
}
