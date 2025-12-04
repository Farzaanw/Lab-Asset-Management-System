// CS-7: Reservations.cpp
// Auth: Dunlap, Jack, Rustic
// Supports: US-101, UR-201, UR-310
// Collaborators: FacultyResearcher[*], ResearchStudent[*], Asset[1], Documents[*]

#include "Reservations.h"
#include "json.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>   
#include <vector>

// Constructors 
Reservations::Reservations() {
    reservationID = 0;
    assetID = 0;
    User = "";
    description = "";
    startDate = "";
    endDate = "";
    permissionApproved = false;
    confirmed = false;
    overdueFlag = false;
    status = PENDING;
}

// Parameterized constructor
Reservations::Reservations(int resID, int assetID_, const std::string& user,
                           const std::string& desc, const std::string& start,
                           const std::string& end, bool permission)
    : reservationID(resID),
      assetID(assetID_),
      User(user),
      description(desc),
      startDate(start),
      endDate(end),
      permissionApproved(permission),
      confirmed(false),
      overdueFlag(false),
      status(permission ? APPROVED : PENDING)
{}

using json = nlohmann::json;

static std::vector<Reservations> reservationsDB;
static int nextReservationID = 1;

static bool parseDate(const std::string& dateStr, std::tm& out) {
    std::istringstream ss(dateStr);
    ss >> std::get_time(&out, "%Y-%m-%d %H:%M");
    return !ss.fail();
}

static bool overlaps(const std::string& aStart, const std::string& aEnd,
                     const std::string& bStart, const std::string& bEnd) {
    std::tm as{}, ae{}, bs{}, be{};
    if (!parseDate(aStart, as) || !parseDate(aEnd, ae)) return true;  // invalid new range -> treat as conflicting
    if (!parseDate(bStart, bs) || !parseDate(bEnd, be)) return false; // invalid stored range -> ignore

    time_t aS = mktime(&as), aE = mktime(&ae);
    time_t bS = mktime(&bs), bE = mktime(&be);
    return (aS < bE) && (aE > bS);
}

static bool hasAssetOverlap(int assetID,
                            const std::string& start,
                            const std::string& end) {
    for (const auto& r : reservationsDB) {
        if (r.assetID == assetID && r.status != ReservationStatus::CANCELLED) {
            if (overlaps(start, end, r.startDate, r.endDate)) return true;
        }
    }
    return false;
}

static const char* statusName(ReservationStatus s) {
    switch (s) {
        case PENDING:   return "PENDING";
        case APPROVED:  return "APPROVED";
        case CONFIRMED: return "CONFIRMED";
        case CANCELLED: return "CANCELLED";
        default:        return "UNKNOWN";
    }
}

bool Reservations::makeReservation(bool permissionApproved_,
                                   std::string description_,
                                   std::string startDate_,
                                   std::string endDate_,
                                   std::string User_) {
    // NOTICE: caller must set this->assetID before calling makeReservation(...)
    this->reservationID     = nextReservationID++;
    this->permissionApproved = permissionApproved_;
    this->description        = std::move(description_);
    this->startDate          = std::move(startDate_);
    this->endDate            = std::move(endDate_);
    this->User               = std::move(User_);
    this->overdueFlag        = false;

    // Initial status
    this->status = permissionApproved ? APPROVED : PENDING;

    // If permission pre-approved AND no time overlap, confirm & decrement stock
    if (permissionApproved && !hasAssetOverlap(this->assetID, this->startDate, this->endDate)) {
        this->status = CONFIRMED;
        this->confirmed = true;
        update_asset_stock_count(std::to_string(this->assetID), -1);
    } else {
        this->confirmed = false;
    }

    reservationsDB.push_back(*this);
    return true;
}

bool Reservations::cancelReservation(int reservationID_) {
    for (auto& r : reservationsDB) {
        if (r.reservationID == reservationID_ && r.status != CANCELLED) {
            r.status = CANCELLED;
            r.confirmed = false;
            update_asset_stock_count(std::to_string(r.assetID), +1);
            return true;
        }
    }
    return false;
}

std::string Reservations::getReservation(int reservationID_) {
    for (const auto& r : reservationsDB) {
        if (r.reservationID == reservationID_) {
            std::ostringstream ss;
            ss << "Reservation ID: " << r.reservationID << "\n"
               << "Asset ID     : " << r.assetID << "\n"
               << "User         : " << r.User << "\n"
               << "Description  : " << r.description << "\n"
               << "Start        : " << r.startDate << "\n"
               << "End          : " << r.endDate << "\n"
               << "Status       : " << statusName(r.status) << "\n"
               << "Confirmed    : " << (r.confirmed ? "Yes" : "No") << "\n"
               << "Overdue      : " << (r.overdueFlag ? "Yes" : "No") << "\n";
            return ss.str();
        }
    }
    return "Reservation not found.";
}

bool Reservations::loadFromJson(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return false;

    json j;
    in >> j;

    reservationsDB.clear();
    nextReservationID = 1;

    for (const auto& it : j) {
        Reservations r;
        r.reservationID      = it.value("reservationID", nextReservationID++);
        r.assetID            = it.value("assetID", 0);
        r.User               = it.value("User", "");
        r.description        = it.value("description", "");
        r.startDate          = it.value("startDate", "");
        r.endDate            = it.value("endDate", "");
        r.permissionApproved = it.value("permissionApproved", false);
        r.confirmed          = it.value("confirmed", false);
        r.overdueFlag        = it.value("overdueFlag", false);

        int s = it.value("status", 0);
        if      (s == 0) r.status = PENDING;
        else if (s == 1) r.status = APPROVED;
        else if (s == 2) r.status = CONFIRMED;
        else             r.status = CANCELLED;

        reservationsDB.push_back(std::move(r));
    }
    return true;
}

bool Reservations::saveToJson(const std::string& path) {
    json j = json::array();
    for (const auto& r : reservationsDB) {
        int s = 0;
        switch (r.status) {
            case PENDING:   s = 0; break;
            case APPROVED:  s = 1; break;
            case CONFIRMED: s = 2; break;
            case CANCELLED: s = 3; break;
        }
        j.push_back({
            {"reservationID",      r.reservationID},
            {"assetID",            r.assetID},
            {"User",               r.User},
            {"description",        r.description},
            {"startDate",          r.startDate},
            {"endDate",            r.endDate},
            {"permissionApproved", r.permissionApproved},
            {"confirmed",          r.confirmed},
            {"overdueFlag",        r.overdueFlag},
            {"status",             s}
        });
    }

    std::ofstream out(path);
    if (!out.is_open()) return false;
    out << j.dump(2);
    return true;
}

void Reservations::update_asset_stock_count(std::string assetID, int quantity) {
    // TODO: Replace with real Asset subsystem call (e.g., SystemController/AssetRepo)
    std::cout << "[Stock Update] Asset " << assetID << " adjusted by " << quantity << "\n";
}
