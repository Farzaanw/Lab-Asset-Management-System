## How To Build
-Clone the repository
-Run this command: g++ main.cpp -o Test_Executable
-main.cpp includes all the skeleton code header files

## Supported User Stories — Assets
Assets are responsible for all of the information needed about the equipment.  Whether that be details about type, condition, and availability. It exists to provide structure to each asset in the lab.
Supports operations like updating, adding, or removing assets.

---
### Epic E-301 — Resource Access & Scheduling
- [UR-302: Suspend or Escalate User Privileges](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/51)

---
### Epic E-302 — Equipment Oversight & Inventory Management
- [UR-320: Global dashboard view](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/56)
- [UR-321: Monitor Asset Misuse & Damage](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/57)

---
### Epic E-303 — High-Level Reporting and Maintenance
- [UR-331: Oversee Budgets and Maintenance using Data](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/59)

---
### Epic E-401 — Asset Updating
- [UR-401: Remove user accounts](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/21)
- [UR-410: Add assets to inventory](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/25)
- [UR-411: Read lab assets](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/26)
- [UR-413: Update the inventory when assets are retired or serviced](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/28)
- [UR-414: Flag assets](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/29)
- [UR-415: Run inventory checks](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/66)

---
### Epic E-402 — Asset Ordering
- [UR-421: Trigger replenishment orders](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/32)

---
### Epic E-403 — Document Updating
- [UR-430: Read a list of documents](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/34)
- [UR-432: Update documents](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/67)


## Supported User Stories — Documents

The documents class will manage all files, records, or reports associated with the research and lab operations.
This can include usage reports, maintenance logs, instruction manuals, project assignments, etc. The several possible documents will all share similar attributes, and that is what this class intends to hold.

---
### Epic E-101 — Student Resource Usage Supervision
- [UR-102: Generate group-level resource usage reports](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/63)

---
### Epic E-302 — Equipment Oversight & Inventory Management
- [UR-320: Global dashboard view](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/56)

---
### Epic E-303 — High-Level Reporting and Maintenance
- [UR-330: Generate comprehensive reports](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/58)

---
### Epic E-403 — Document Updating
- [UR-430: Read a list of documents](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/34)
- [UR-431: Upload documents](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/35)
- [UR-432: Update documents](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/67)


## Supported User Stories — Faculty Researcher

The Faculty Researcher is responsible for conducting research and supervising the research students. They need to reserve and use assets for projects, oversee the students’ use of equipment,
track patterns of lab equipment for their specific research group, and create reports to describe the use of assets.
They must also book equipment and software for their group, manage group members’ reservations, access dashboards with a summary of the usage data, and generate reports for resource usage. 

---

### Epic E-101 — Student Resource Usage Supervision
- [UR-101: View What Equipment Is In Use](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/62)
- [UR-102: Generate Reports](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/63)

---

### Epic E-102 — Advanced Capabilities
- [UR-103: High Level Clearance For Faculty Researcher](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/64)
- [UR-104: Report Issues](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/65)


## Supported User Stories — Lab Asset Manager

The lab asset manager is responsible for everything that has to do with accounts. This includes account creation, management, and role assigning. This role is also responsible for asset management,
which provides for adding assets to the lab inventory, updating, deleting, and running inventory checks. Lastly, this role is responsible for uploading and modifying documents,
including software licenses, and ensuring everything is up to date.

---

### Epic E-400 — User Account Management
- [UR-400: Create user accounts](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/20)
- [UR-401: Remove user accounts](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/21)
- [UR-402: Update and modify user accounts](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/22)
- [UR-403: Read user accounts](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/23)

---

### Epic E-401 — Asset Updating
- [UR-410: Add assets to the inventory](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/25)
- [UR-411: Read lab assets](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/26)
- [UR-412: Update product metadata](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/27)
- [UR-413: Update inventory when assets are retired or serviced](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/28)
- [UR-414: Flag assets as out of service](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/29)
- [UR-415: Run inventory checks](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/66)

---

### Epic E-402 — Asset Ordering
- [UR-420: Track consumable levels](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/31)
- [UR-421: Trigger replenishment orders](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/32)

---

### Epic E-403 — Document Updating
- [UR-430: Read list of documents](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/34)
- [UR-431: Upload documents](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/35)
- [UR-432: Update and edit documents](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/67)


## Supported User Stories — Lab Manager

The Lab Manager is responsible for overseeing the daily operations of the lab, ensuring that all equipment, accounts, and reservations are functioning correctly.
They will also maintain the overall scheduling of lab resources, verify that maintenance tasks are completed, and ensure everyone complies with lab safety and policy standards.
Lastly, they will monitor the users' activities and provide support to both faculty and students using the system.

---

### Epic E-300 — User & Access Management
- [UR-300: Grant or revoke administrative rights](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/49)
- [UR-301: Approve high-level system policies](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/50)
- [UR-302: Suspend or escalate user privileges](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/51)
- [UR-303: Manage lab-wide policies for system](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/52)

---

### Epic E-301 — Resource Access & Scheduling
- [UR-310: Manage scheduling conflicts](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/53)
- [UR-311: Approve special access requests](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/54)
- [UR-312: Approve high-value equipment requests](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/55)

---

### Epic E-302 — Equipment Oversight & Inventory Management
- [UR-320: Global dashboard view of lab assets](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/56)
- [UR-321: Monitor asset misuse and damage](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/57)

---

### Epic E-303 — High-Level Reporting & Maintenance
- [UR-330: Generate comprehensive lab reports](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/58)
- [UR-331: Oversee budgets and maintenance using data](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/59)


## Supported User Stories — Research Student

The primary responsibility of this class is to create research student objects and be a parent class to all users for our software.  The class will include attributes such as username and password,
which all other user classes will inherit.  This class will give a base level of privileges, allowing users to reserve non-restricted equipment and view what equipment is currently available.

---

### Epic E-200 — Reserve & Return of Lab Equipment
- [UR-201: Reserve/Return Lab Equipment](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/42)
- [UR-202: Return Notifications](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/80)

---

### Epic E-210 — Request Software Access
- [UR-211: Request Software](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/83)

---

### Epic E-220 — Check Equipment Status
- [UR-221: Check Equipment Status](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/85)

---

### Epic E-230 — Report Equipment Issues
- [UR-231: Report Equipment Issues](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/84)


## Supported User Stories — Reservations

The reservations class is responsible for all the equipment and asset bookings made by faculty and students. It ensures equipment is properly maintained and recorded as checked out/checked in along
with the date and time of each reservation. Permissions will be permitted to actors, which will determine who can make certain reservations and see what reservations have been made.

---

### Epic E-101 — Student Resource Usage Supervision
- [UR-101: Student Resource Usage Supervision](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/60)

---

### Epic E-200 — Reserve & Return of Lab Equipment
- [UR-201: Reserve/Return Lab Equipment](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/42)

---

### Epic E-301 — Resource Access & Scheduling
- [UR-310: Manage Scheduling Conflicts](https://github.com/CS361-2025-Fall-Term/MicroCat/issues/53)
