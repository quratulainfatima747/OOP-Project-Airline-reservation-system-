#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <iostream>

class Passenger {
protected:
    std::string passengerID;
    std::string name;
    std::string email;
    std::string phone;
    int loyaltyPoints;

public:
    Passenger(const std::string& passengerID,
              const std::string& name,
              const std::string& email,
              const std::string& phone);

    virtual ~Passenger() = default;

    // Pure virtual
    virtual double getBaggageAllowanceKg() const = 0;
    virtual double getLoyaltyMultiplier() const = 0;
    virtual double getRefundPercentage(int daysBeforeDeparture) const = 0;
    virtual std::string getPassengerClass() const = 0;

    // Getters
    std::string getPassengerID() const { return passengerID; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    int getLoyaltyPoints() const { return loyaltyPoints; }

    void addLoyaltyPoints(int pts) { loyaltyPoints += pts; }
    void displayInfo() const;

    friend std::ostream& operator<<(std::ostream& os, const Passenger& p);
};

// ── Economy Passenger ────────────────────────────────────────────────────────
class EconomyPassenger : public Passenger {
public:
    EconomyPassenger(const std::string& id, const std::string& name,
                     const std::string& email, const std::string& phone);

    double getBaggageAllowanceKg() const override { return 20.0; }
    double getLoyaltyMultiplier() const override { return 1.0; }
    double getRefundPercentage(int daysBeforeDeparture) const override;
    std::string getPassengerClass() const override { return "ECONOMY"; }
};

// ── Business Passenger ────────────────────────────────────────────────────────
class BusinessPassenger : public Passenger {
public:
    BusinessPassenger(const std::string& id, const std::string& name,
                      const std::string& email, const std::string& phone);

    double getBaggageAllowanceKg() const override { return 40.0; }
    double getLoyaltyMultiplier() const override { return 2.0; }
    double getRefundPercentage(int daysBeforeDeparture) const override;
    std::string getPassengerClass() const override { return "BUSINESS"; }
};

// ── First Class Passenger ─────────────────────────────────────────────────────
class FirstClassPassenger : public Passenger {
public:
    FirstClassPassenger(const std::string& id, const std::string& name,
                        const std::string& email, const std::string& phone);

    double getBaggageAllowanceKg() const override { return 60.0; }
    double getLoyaltyMultiplier() const override { return 3.0; }
    double getRefundPercentage(int daysBeforeDeparture) const override;
    std::string getPassengerClass() const override { return "FIRST"; }
};

#endif // PASSENGER_H
