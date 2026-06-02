#ifndef DERIVED_FLIGHTS_H
#define DERIVED_FLIGHTS_H

#include "Flight.h"

// ── Domestic Flight ─────────────────────────────────────────────────────────
class DomesticFlight : public Flight {
private:
    double baseFarePerKm;
    double distanceKm;

public:
    DomesticFlight(const std::string& flightNumber,
                   const std::string& origin,
                   const std::string& destination,
                   const std::string& departureDateTime,
                   int totalSeats,
                   double baseFarePerKm,
                   double distanceKm);

    double calculateBaseFare() const override;
    void displayDetails() const override;
    std::string getType() const override { return "DOMESTIC"; }
    std::string getExtraData() const override;
    void setExtraData(const std::string& data) override;
};

// ── International Flight ─────────────────────────────────────────────────────
class InternationalFlight : public Flight {
private:
    double baseFare;
    bool visaRequired;
    std::string country;

public:
    InternationalFlight(const std::string& flightNumber,
                        const std::string& origin,
                        const std::string& destination,
                        const std::string& departureDateTime,
                        int totalSeats,
                        double baseFare,
                        bool visaRequired,
                        const std::string& country);

    double calculateBaseFare() const override;
    void displayDetails() const override;
    std::string getType() const override { return "INTERNATIONAL"; }
    std::string getExtraData() const override;
    void setExtraData(const std::string& data) override;

    bool isVisaRequired() const { return visaRequired; }
    std::string getCountry() const { return country; }
};

// ── Charter Flight ───────────────────────────────────────────────────────────
class CharterFlight : public Flight {
private:
    double flatFare;
    std::string contractHolder;

public:
    CharterFlight(const std::string& flightNumber,
                  const std::string& origin,
                  const std::string& destination,
                  const std::string& departureDateTime,
                  int totalSeats,
                  double flatFare,
                  const std::string& contractHolder);

    double calculateBaseFare() const override;
    void displayDetails() const override;
    std::string getType() const override { return "CHARTER"; }
    std::string getExtraData() const override;
    void setExtraData(const std::string& data) override;

    std::string getContractHolder() const { return contractHolder; }
};

#endif // DERIVED_FLIGHTS_H
