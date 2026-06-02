# SkyLink Airways — Airline Reservation & Flight Management System

A console-based C++ application implementing an airline reservation system for the OOP PBL assignment.  
Demonstrates all four OOP pillars: **encapsulation, inheritance, polymorphism, and abstraction**.

---

## Project Structure

```
SkyLink/
├── include/
│   ├── Exceptions.h        # Custom exception classes
│   ├── Flight.h            # Abstract base class
│   ├── DerivedFlights.h    # DomesticFlight, InternationalFlight, CharterFlight
│   ├── Passenger.h         # Passenger base + Economy/Business/FirstClass
│   ├── Ticket.h            # Ticket class
│   ├── Airline.h           # Aggregator / main logic
│   └── SearchUtils.h       # Generic template search utilities
├── src/
│   ├── Flight.cpp
│   ├── DerivedFlights.cpp
│   ├── Passenger.cpp
│   ├── Ticket.cpp
│   ├── Airline.cpp
│   └── main.cpp            # Menu-driven UI
├── data/
│   └── skylink_state.txt   # Persistence file (auto-created; sample provided)
├── Makefile
└── README.md
```

---

## Build Requirements

| Tool    | Minimum version |
|---------|----------------|
| g++     | 7.0 (C++17)    |
| make    | Any            |
| Linux / macOS / WSL | — |

---

## How to Build

```bash
# From the SkyLink/ directory:
make
```

This compiles all sources with `-std=c++17 -Wall -Wextra` and produces the `skylink` executable.

To build **and** run immediately:
```bash
make run
```

To clean all build artefacts:
```bash
make clean
```

---

## How to Run

```bash
# Make sure the data/ directory exists (make run does this automatically):
mkdir -p data
./skylink
```

A sample `data/skylink_state.txt` is included with **10 flights** and **8 passengers** so you can
explore the system immediately without entering data manually.

---

## Quick-Start Walkthrough

1. **Start** — the program loads state from `data/skylink_state.txt` automatically.
2. **List flights** — option **3** to see all 10 pre-loaded flights.
3. **List passengers** — option **10** to see all 8 pre-loaded passengers.
4. **Book a ticket** — option **11**; enter `P001` and `SL101`.
5. **View history** — option **9**; enter `P001`.
6. **Cancel a ticket** — option **12**; enter the ticket ID printed during booking (e.g. `TKT1`).
7. **Reports** — options **13**, **14**, **15**.
8. **Exit** — option **0** saves state; re-running restores it.

---

## OOP Features Demonstrated

| Feature | Where |
|---|---|
| Abstract class + pure virtual | `Flight` (calculateBaseFare, displayDetails, getType) |
| Inheritance | `DomesticFlight`, `InternationalFlight`, `CharterFlight` ← `Flight`; `Economy/Business/FirstClass` ← `Passenger` |
| Runtime polymorphism | Fare & refund calculation, `displayDetails()`, `operator<<` |
| Encapsulation | All data members `private`/`protected`; access via getters |
| Operator overloading | `operator<<` for `Flight`, `Passenger`, `Ticket`; `operator==` for `Ticket` |
| Function template | `searchAll<T>()`, `findFirst<T>()` in `SearchUtils.h` |
| Custom exceptions | `FlightFullException`, `InvalidCancellationException`, `FlightNotFoundException`, `PassengerNotFoundException`, `DuplicateBookingException` |
| STL containers | `std::vector` (flights/passengers/tickets), `std::map` (revenue report) |
| STL algorithms | `std::sort`, `std::find_if` |
| Smart pointers | `std::unique_ptr` throughout — no raw `new`/`delete`, no memory leaks |
| File persistence | Full save/load on exit/startup via `data/skylink_state.txt` |

---

## Passenger Classes & Refund Policy

| Class        | Baggage | Loyalty Mult. | Refund (≥14d / ≥7d / ≥2d / <2d) |
|---|---|---|---|
| Economy      | 20 kg   | 1×            | 80% / 50% / 20% / 0%             |
| Business     | 40 kg   | 2×            | 90% / 60% / — / 25%              |
| First Class  | 60 kg   | 3×            | 100% (≥3d) / 75% (≥1d) / 50%    |

---

## Flight Types

| Type          | Fare Logic                               | Extra Attributes               |
|---|---|---|
| Domestic      | `baseFarePerKm × distanceKm`             | Distance, fare per km          |
| International | `baseFare × 1.15` if visa required       | Country, visa requirement      |
| Charter       | Flat fare per seat                       | Contract holder name           |

---

## Notes & Known Limitations

- Date/time is stored as plain strings (`YYYY-MM-DD HH:MM`). No timezone handling.
- Seat numbering is sequential (1A–1F, 2A–2F …) — no seat-map selection.
- The loyalty points multiplier also acts as the fare multiplier (higher class pays more).
- The save file uses `|` as a delimiter; fields must not contain `|`.
- Tested on Ubuntu 22.04 with g++ 11.4 and macOS 14 with Apple Clang 15.
