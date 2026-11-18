#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

// ============================================================
// HILLSTONE GRAND HOTEL
// HOTEL RESERVATION & MANAGEMENT SYSTEM
//
// Systems & Programming Concepts Assignment
// June 2026
// ============================================================

// ============================================================
// CONSTANTS
// ============================================================

const int FLOORS = 5;
const int ROOMS_PER_FLOOR = 5;

const int ROOM_AVAILABLE = 0;
const int ROOM_RESERVED = 1;
const int ROOM_OCCUPIED = 2;

// ============================================================
// STRUCTURES
// ============================================================

struct Customer
{
    string customerID;
    string name;
    string phone;
    string email;
    int loyaltyPoints = 0;
};

struct Room
{
    int roomNumber;
    string roomType;
    double pricePerNight;
};

struct Reservation
{
    string bookingID;
    string customerID;
    int roomNumber;
    int numberOfNights;
    string checkInDate;
    string checkOutDate;
    string status;
    bool paid = false;
};

struct RoomServiceOrder
{
    string orderID;
    string bookingID;
    string itemName;
    int quantity;
    double totalAmount;
};

struct Payment
{
    string paymentID;
    string bookingID;
    double roomCharge;
    double serviceCharge;
    double discount;
    double totalAmount;
    double paymentAmount;
    double change;
    string paymentMethod;
};

// ============================================================
// GLOBAL DATA
// ============================================================

vector<Customer> customers;
vector<Reservation> reservations;
vector<RoomServiceOrder> roomServiceOrders;
vector<Payment> payments;

int roomStatus[FLOORS][ROOMS_PER_FLOOR];

Room hotelRooms[FLOORS][ROOMS_PER_FLOOR];

int customerCounter = 1;
int bookingCounter = 1;
int paymentCounter = 1;
int orderCounter = 1;

// ============================================================
// FUNCTION PROTOTYPES
// ============================================================

// GENERAL
void clearInput();
void pauseScreen();
void printLine();
void printHeader(const string &title);

int getValidatedInt(
    const string &message,
    int min,
    int max
);

double getValidatedDouble(
    const string &message,
    double min
);

bool isValidName(const string &name);
bool isValidPhone(const string &phone);
bool isValidEmail(const string &email);

string generateID(
    const string &prefix,
    int number
);

// INITIALIZATION
void initializeRooms();

// DISPLAY
void displayIntroduction();
void displayMainMenu();

// CUSTOMER
void customerMenu();
void addCustomer();
void searchCustomer();
void updateCustomer();
void deleteCustomer();
void displayAllCustomers();

int findCustomerIndex(
    const string &customerID
);

// RESERVATION
void reservationMenu();
void displayAllRooms();
void displayAvailableRooms();
void searchRoom();
void makeReservation();
void searchReservation();
void cancelReservation();
void displayAllReservations();

int findRoomIndex(int roomNumber);
int findReservationIndex(
    const string &bookingID
);

string getRoomStatus(int status);

double calculateRoomCharge(
    int roomNumber,
    int numberOfNights
);

// CHECK-IN / CHECK-OUT
void checkInOutMenu();
void checkInGuest();
void checkOutGuest();
void searchGuestStay();
void displayOccupiedRooms();

// PAYMENT
void paymentMenu();
void makePayment();
void searchPayment();
void displayAllPayments();
double calculateServiceCharge(
    const string &bookingID
);

// ROOM SERVICE
void roomServiceMenu();
void displayRoomServiceMenu();
void orderRoomService();
void viewRoomServiceOrders();
void cancelRoomServiceOrder();

int findOrderIndex(
    const string &orderID
);

// LOYALTY
void loyaltyMenu();
void viewLoyaltyPoints();
void redeemLoyaltyPoints();
void displayLoyaltyMembers();

// REPORTS
void reportMenu();
void roomOccupancyReport();
void revenueReport();
void customerReport();
void reservationReport();

// ============================================================
// GENERAL FUNCTIONS
// ============================================================

void clearInput()
{
    cin.clear();

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
}

// ------------------------------------------------------------

void pauseScreen()
{
    cout << "\nPress ENTER to continue...";

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
}

// ------------------------------------------------------------

void printLine()
{
    cout << "============================================================\n";
}

// ------------------------------------------------------------

void printHeader(const string &title)
{
    cout << "\n";

    printLine();

    cout << " " << title << endl;

    printLine();
}

// ============================================================
// INTEGER VALIDATION
// ============================================================

int getValidatedInt(
    const string &message,
    int min,
    int max
)
{
    int value;

    while (true)
    {
        cout << message;

        cin >> value;

        if (cin.fail())
        {
            cout << "\nInvalid input. Please enter a number.\n";

            clearInput();
        }
        else if (value < min || value > max)
        {
            cout << "\nInvalid choice. Please enter a value between "
                 << min << " and " << max << ".\n";

            clearInput();
        }
        else
        {
            clearInput();

            return value;
        }
    }
}

// ============================================================
// DOUBLE VALIDATION
// ============================================================

double getValidatedDouble(
    const string &message,
    double min
)
{
    double value;

    while (true)
    {
        cout << message;

        cin >> value;

        if (cin.fail() || value < min)
        {
            cout << "\nInvalid amount. Please enter a valid value.\n";

            clearInput();
        }
        else
        {
            clearInput();

            return value;
        }
    }
}

// ============================================================
// NAME VALIDATION
// ============================================================

bool isValidName(const string &name)
{
    if (name.empty())
    {
        return false;
    }

    for (char ch : name)
    {
        if (!isalpha(static_cast<unsigned char>(ch)) &&
            ch != ' ')
        {
            return false;
        }
    }

    return true;
}

// ============================================================
// PHONE VALIDATION
// ============================================================

bool isValidPhone(const string &phone)
{
    if (phone.length() < 9 ||
        phone.length() > 15)
    {
        return false;
    }

    for (char ch : phone)
    {
        if (!isdigit(static_cast<unsigned char>(ch)) &&
            ch != '-')
        {
            return false;
        }
    }

    return true;
}

// ============================================================
// EMAIL VALIDATION
// ============================================================

bool isValidEmail(const string &email)
{
    size_t atPosition =
        email.find('@');

    size_t dotPosition =
        email.find('.', atPosition);

    if (atPosition == string::npos)
    {
        return false;
    }

    if (dotPosition == string::npos)
    {
        return false;
    }

    if (atPosition == 0)
    {
        return false;
    }

    if (dotPosition <= atPosition + 1)
    {
        return false;
    }

    return true;
}

// ============================================================
// GENERATE ID
// ============================================================

string generateID(
    const string &prefix,
    int number
)
{
    string result = prefix;

    if (number < 10)
    {
        result += "00";
    }
    else if (number < 100)
    {
        result += "0";
    }

    result += to_string(number);

    return result;
}

// ============================================================
// INITIALIZE ROOMS
// ============================================================

void initializeRooms()
{
    for (int floor = 0;
         floor < FLOORS;
         floor++)
    {
        for (int room = 0;
             room < ROOMS_PER_FLOOR;
             room++)
        {
            hotelRooms[floor][room].roomNumber =
                (floor + 1) * 100 + room + 1;

            if (room == 0)
            {
                hotelRooms[floor][room].roomType =
                    "Single";

                hotelRooms[floor][room].pricePerNight =
                    120.00;
            }
            else if (room == 1)
            {
                hotelRooms[floor][room].roomType =
                    "Double";

                hotelRooms[floor][room].pricePerNight =
                    180.00;
            }
            else if (room == 2)
            {
                hotelRooms[floor][room].roomType =
                    "Deluxe";

                hotelRooms[floor][room].pricePerNight =
                    250.00;
            }
            else if (room == 3)
            {
                hotelRooms[floor][room].roomType =
                    "Suite";

                hotelRooms[floor][room].pricePerNight =
                    400.00;
            }
            else
            {
                hotelRooms[floor][room].roomType =
                    "Family";

                hotelRooms[floor][room].pricePerNight =
                    300.00;
            }

            roomStatus[floor][room] =
                ROOM_AVAILABLE;
        }
    }
}

// ============================================================
// INTRODUCTION
// ============================================================

void displayIntroduction()
{
    printLine();

    cout << "             WELCOME TO HILLSTONE GRAND HOTEL\n";
    cout << "         HOTEL RESERVATION & MANAGEMENT SYSTEM\n";

    printLine();

    cout << "\nThis system provides the following services:\n\n";

    cout << "1. Customer Management\n";
    cout << "2. Room & Reservation Management\n";
    cout << "3. Check-In & Check-Out Management\n";
    cout << "4. Payment & Billing\n";
    cout << "5. Reports & Statistics\n";
    cout << "6. Room Service\n";
    cout << "7. Loyalty Points Program\n";
}

// ============================================================
// MAIN MENU DISPLAY
// ============================================================

void displayMainMenu()
{
    cout << "\n";

    printLine();

    cout << "                    MAIN MENU\n";

    printLine();

    cout << "1. Customer Management\n";
    cout << "2. Room & Reservation Management\n";
    cout << "3. Check-In & Check-Out Management\n";
    cout << "4. Payment & Billing\n";
    cout << "5. Reports & Statistics\n";
    cout << "6. Room Service\n";
    cout << "7. Loyalty Points Program\n";
    cout << "8. Exit System\n";

    printLine();
}

// ============================================================
// CUSTOMER MANAGEMENT
// ============================================================

// ============================================================
// FIND CUSTOMER
// ============================================================

int findCustomerIndex(
    const string &customerID
)
{
    for (int i = 0;
         i < static_cast<int>(customers.size());
         i++)
    {
        if (customers[i].customerID ==
            customerID)
        {
            return i;
        }
    }

    return -1;
}

// ============================================================
// ADD CUSTOMER
// ============================================================

void addCustomer()
{
    printHeader("ADD NEW CUSTOMER");

    Customer newCustomer;

    newCustomer.customerID =
        generateID("C", customerCounter);

    cout << "Generated Customer ID: "
         << newCustomer.customerID
         << "\n\n";

    while (true)
    {
        cout << "Enter customer name: ";

        getline(cin, newCustomer.name);

        if (isValidName(newCustomer.name))
        {
            break;
        }

        cout << "Invalid name.\n";
        cout << "Name must contain letters and spaces only.\n";
    }

    while (true)
    {
        cout << "Enter phone number: ";

        getline(cin, newCustomer.phone);

        if (isValidPhone(newCustomer.phone))
        {
            break;
        }

        cout << "Invalid phone number.\n";
        cout << "Please enter 9-15 digits.\n";
    }

    while (true)
    {
        cout << "Enter email address: ";

        getline(cin, newCustomer.email);

        if (isValidEmail(newCustomer.email))
        {
            break;
        }

        cout << "Invalid email address.\n";
        cout << "Example: customer@gmail.com\n";
    }

    newCustomer.loyaltyPoints = 0;

    customers.push_back(newCustomer);

    customerCounter++;

    cout << "\n";

    printLine();

    cout << "CUSTOMER SUCCESSFULLY REGISTERED!\n";

    printLine();

    cout << "Customer ID : "
         << newCustomer.customerID
         << "\n";

    cout << "Name        : "
         << newCustomer.name
         << "\n";

    cout << "Phone       : "
         << newCustomer.phone
         << "\n";

    cout << "Email       : "
         << newCustomer.email
         << "\n";

    cout << "Loyalty     : "
         << newCustomer.loyaltyPoints
         << " points\n";

    pauseScreen();
}

// ============================================================
// SEARCH CUSTOMER
// ============================================================

void searchCustomer()
{
    printHeader("SEARCH CUSTOMER");

    if (customers.empty())
    {
        cout << "No customer records available.\n";

        pauseScreen();

        return;
    }

    string customerID;

    cout << "Enter Customer ID: ";

    getline(cin, customerID);

    int index =
        findCustomerIndex(customerID);

    if (index == -1)
    {
        cout << "\nCustomer not found.\n";
    }
    else
    {
        printLine();

        cout << "CUSTOMER INFORMATION\n";

        printLine();

        cout << "Customer ID    : "
             << customers[index].customerID
             << "\n";

        cout << "Name           : "
             << customers[index].name
             << "\n";

        cout << "Phone          : "
             << customers[index].phone
             << "\n";

        cout << "Email          : "
             << customers[index].email
             << "\n";

        cout << "Loyalty Points : "
             << customers[index].loyaltyPoints
             << "\n";

        printLine();
    }

    pauseScreen();
}

// ============================================================
// UPDATE CUSTOMER
// ============================================================

void updateCustomer()
{
    printHeader("UPDATE CUSTOMER");

    if (customers.empty())
    {
        cout << "No customer records available.\n";

        pauseScreen();

        return;
    }

    string customerID;

    cout << "Enter Customer ID to update: ";

    getline(cin, customerID);

    int index =
        findCustomerIndex(customerID);

    if (index == -1)
    {
        cout << "\nCustomer not found.\n";

        pauseScreen();

        return;
    }

    cout << "\nCurrent customer information:\n\n";

    cout << "Name  : "
         << customers[index].name
         << "\n";

    cout << "Phone : "
         << customers[index].phone
         << "\n";

    cout << "Email : "
         << customers[index].email
         << "\n";

    cout << "\nEnter new information.\n";

    while (true)
    {
        string newName;

        cout << "New name: ";

        getline(cin, newName);

        if (isValidName(newName))
        {
            customers[index].name =
                newName;

            break;
        }

        cout << "Invalid name. Try again.\n";
    }

    while (true)
    {
        string newPhone;

        cout << "New phone: ";

        getline(cin, newPhone);

        if (isValidPhone(newPhone))
        {
            customers[index].phone =
                newPhone;

            break;
        }

        cout << "Invalid phone number. Try again.\n";
    }

    while (true)
    {
        string newEmail;

        cout << "New email: ";

        getline(cin, newEmail);

        if (isValidEmail(newEmail))
        {
            customers[index].email =
                newEmail;

            break;
        }

        cout << "Invalid email address. Try again.\n";
    }

    cout << "\nCustomer information successfully updated.\n";

    pauseScreen();
}

// ============================================================
// DELETE CUSTOMER
// ============================================================

void deleteCustomer()
{
    printHeader("DELETE CUSTOMER");

    if (customers.empty())
    {
        cout << "No customer records available.\n";

        pauseScreen();

        return;
    }

    string customerID;

    cout << "Enter Customer ID to delete: ";

    getline(cin, customerID);

    int index =
        findCustomerIndex(customerID);

    if (index == -1)
    {
        cout << "\nCustomer not found.\n";

        pauseScreen();

        return;
    }

    cout << "\nCustomer found:\n";

    cout << "Name: "
         << customers[index].name
         << "\n";

    int confirmation =
        getValidatedInt(
            "Are you sure? (1 = Yes, 2 = No): ",
            1,
            2
        );

    if (confirmation == 1)
    {
        customers.erase(
            customers.begin() + index
        );

        cout << "\nCustomer successfully deleted.\n";
    }
    else
    {
        cout << "\nDelete operation cancelled.\n";
    }

    pauseScreen();
}

// ============================================================
// DISPLAY ALL CUSTOMERS
// ============================================================

void displayAllCustomers()
{
    printHeader("ALL CUSTOMER RECORDS");

    if (customers.empty())
    {
        cout << "No customer records available.\n";

        pauseScreen();

        return;
    }

    cout << left
         << setw(12)
         << "ID"
         << setw(25)
         << "Name"
         << setw(17)
         << "Phone"
         << setw(30)
         << "Email"
         << setw(10)
         << "Points"
         << "\n";

    printLine();

    for (const Customer &customer : customers)
    {
        cout << left
             << setw(12)
             << customer.customerID
             << setw(25)
             << customer.name
             << setw(17)
             << customer.phone
             << setw(30)
             << customer.email
             << setw(10)
             << customer.loyaltyPoints
             << "\n";
    }

    pauseScreen();
}

// ============================================================
// CUSTOMER MENU
// ============================================================

void customerMenu()
{
    int choice;

    do
    {
        printHeader("CUSTOMER MANAGEMENT");

        cout << "1. Add New Customer\n";
        cout << "2. Search Customer\n";
        cout << "3. Update Customer\n";
        cout << "4. Delete Customer\n";
        cout << "5. Display All Customers\n";
        cout << "6. Return to Main Menu\n";

        printLine();

        choice =
            getValidatedInt(
                "Enter your choice (1-6): ",
                1,
                6
            );

        switch (choice)
        {
            case 1:
                addCustomer();
                break;

            case 2:
                searchCustomer();
                break;

            case 3:
                updateCustomer();
                break;

            case 4:
                deleteCustomer();
                break;

            case 5:
                displayAllCustomers();
                break;

            case 6:
                cout << "\nReturning to Main Menu...\n";
                break;
        }

    } while (choice != 6);
}

// ============================================================
// RESERVATION MANAGEMENT
// ============================================================

// ============================================================
// FIND ROOM
// ============================================================

int findRoomIndex(int roomNumber)
{
    for (int floor = 0;
         floor < FLOORS;
         floor++)
    {
        for (int room = 0;
             room < ROOMS_PER_FLOOR;
             room++)
        {
            if (hotelRooms[floor][room].roomNumber ==
                roomNumber)
            {
                return floor * ROOMS_PER_FLOOR + room;
            }
        }
    }

    return -1;
}

// ============================================================
// GET ROOM STATUS
// ============================================================

string getRoomStatus(int status)
{
    if (status == ROOM_AVAILABLE)
    {
        return "Available";
    }
    else if (status == ROOM_RESERVED)
    {
        return "Reserved";
    }
    else
    {
        return "Occupied";
    }
}

// ============================================================
// DISPLAY ALL ROOMS
// ============================================================

void displayAllRooms()
{
    printHeader("ALL HOTEL ROOMS");

    cout << left
         << setw(12)
         << "Room"
         << setw(15)
         << "Type"
         << setw(18)
         << "Price/Night"
         << setw(15)
         << "Status"
         << "\n";

    printLine();

    for (int floor = 0;
         floor < FLOORS;
         floor++)
    {
        for (int room = 0;
             room < ROOMS_PER_FLOOR;
             room++)
        {
            cout << left
                 << setw(12)
                 << hotelRooms[floor][room].roomNumber
                 << setw(15)
                 << hotelRooms[floor][room].roomType
                 << "RM "
                 << setw(15)
                 << fixed
                 << setprecision(2)
                 << hotelRooms[floor][room].pricePerNight
                 << setw(15)
                 << getRoomStatus(
                        roomStatus[floor][room]
                    )
                 << "\n";
        }
    }

    pauseScreen();
}

// ============================================================
// DISPLAY AVAILABLE ROOMS
// ============================================================

void displayAvailableRooms()
{
    printHeader("AVAILABLE ROOMS");

    bool found = false;

    cout << left
         << setw(12)
         << "Room"
         << setw(15)
         << "Type"
         << setw(18)
         << "Price/Night"
         << "\n";

    printLine();

    for (int floor = 0;
         floor < FLOORS;
         floor++)
    {
        for (int room = 0;
             room < ROOMS_PER_FLOOR;
             room++)
        {
            if (roomStatus[floor][room] ==
                ROOM_AVAILABLE)
            {
                found = true;

                cout << left
                     << setw(12)
                     << hotelRooms[floor][room].roomNumber
                     << setw(15)
                     << hotelRooms[floor][room].roomType
                     << "RM "
                     << setw(15)
                     << fixed
                     << setprecision(2)
                     << hotelRooms[floor][room].pricePerNight
                     << "\n";
            }
        }
    }

    if (!found)
    {
        cout << "\nNo rooms are currently available.\n";
    }

    pauseScreen();
}

// ============================================================
// SEARCH ROOM
// ============================================================

void searchRoom()
{
    printHeader("SEARCH ROOM");

    int roomNumber =
        getValidatedInt(
            "Enter room number (101-505): ",
            101,
            505
        );

    int index =
        findRoomIndex(roomNumber);

    if (index == -1)
    {
        cout << "\nRoom "
             << roomNumber
             << " does not exist.\n";

        pauseScreen();

        return;
    }

    int floor =
        index / ROOMS_PER_FLOOR;

    int room =
        index % ROOMS_PER_FLOOR;

    printLine();

    cout << "ROOM INFORMATION\n";

    printLine();

    cout << "Room Number : "
         << hotelRooms[floor][room].roomNumber
         << "\n";

    cout << "Room Type   : "
         << hotelRooms[floor][room].roomType
         << "\n";

    cout << "Price/Night : RM "
         << fixed
         << setprecision(2)
         << hotelRooms[floor][room].pricePerNight
         << "\n";

    cout << "Status      : "
         << getRoomStatus(roomStatus[floor][room])
         << "\n";

    printLine();

    pauseScreen();
}

// ============================================================
// CALCULATE ROOM CHARGE
// ============================================================

double calculateRoomCharge(
    int roomNumber,
    int numberOfNights
)
{
    int index =
        findRoomIndex(roomNumber);

    if (index == -1)
    {
        return 0.0;
    }

    int floor =
        index / ROOMS_PER_FLOOR;

    int room =
        index % ROOMS_PER_FLOOR;

    return hotelRooms[floor][room].pricePerNight
           * numberOfNights;
}

// ============================================================
// MAKE RESERVATION
// ============================================================

void makeReservation()
{
    printHeader("MAKE NEW RESERVATION");

    if (customers.empty())
    {
        cout << "No customer records found.\n";
        cout << "Please register a customer first.\n";

        pauseScreen();

        return;
    }

    string customerID;

    cout << "Enter Customer ID: ";

    getline(cin, customerID);

    int customerIndex =
        findCustomerIndex(customerID);

    if (customerIndex == -1)
    {
        cout << "\nCustomer ID not found.\n";

        pauseScreen();

        return;
    }

    cout << "\nCustomer Found:\n";

    cout << "Name: "
         << customers[customerIndex].name
         << "\n";

    displayAvailableRooms();

    int roomNumber =
        getValidatedInt(
            "\nEnter room number: ",
            101,
            505
        );

    int roomIndex =
        findRoomIndex(roomNumber);

    if (roomIndex == -1)
    {
        cout << "\nInvalid room number.\n";

        pauseScreen();

        return;
    }

    int floor =
        roomIndex / ROOMS_PER_FLOOR;

    int room =
        roomIndex % ROOMS_PER_FLOOR;

    if (roomStatus[floor][room] !=
        ROOM_AVAILABLE)
    {
        cout << "\nRoom "
             << roomNumber
             << " is not available.\n";

        cout << "Current status: "
             << getRoomStatus(
                    roomStatus[floor][room]
                )
             << "\n";

        pauseScreen();

        return;
    }

    int nights =
        getValidatedInt(
            "Enter number of nights (1-30): ",
            1,
            30
        );

    string checkInDate;
    string checkOutDate;

    cout << "Enter check-in date (DD/MM/YYYY): ";

    getline(cin, checkInDate);

    cout << "Enter check-out date (DD/MM/YYYY): ";

    getline(cin, checkOutDate);

    if (checkInDate.empty() ||
        checkOutDate.empty())
    {
        cout << "\nInvalid date.\n";

        pauseScreen();

        return;
    }

    double roomCharge =
        calculateRoomCharge(
            roomNumber,
            nights
        );

    Reservation newReservation;

    newReservation.bookingID =
        generateID(
            "B",
            bookingCounter
        );

    newReservation.customerID =
        customerID;

    newReservation.roomNumber =
        roomNumber;

    newReservation.numberOfNights =
        nights;

    newReservation.checkInDate =
        checkInDate;

    newReservation.checkOutDate =
        checkOutDate;

    newReservation.status =
        "Reserved";

    newReservation.paid =
        false;

    reservations.push_back(
        newReservation
    );

    roomStatus[floor][room] =
        ROOM_RESERVED;

    bookingCounter++;

    cout << "\n";

    printLine();

    cout << "        RESERVATION SUCCESSFULLY CREATED\n";

    printLine();

    cout << "Booking ID       : "
         << newReservation.bookingID
         << "\n";

    cout << "Customer ID      : "
         << newReservation.customerID
         << "\n";

    cout << "Customer Name    : "
         << customers[customerIndex].name
         << "\n";

    cout << "Room Number      : "
         << newReservation.roomNumber
         << "\n";

    cout << "Room Type        : "
         << hotelRooms[floor][room].roomType
         << "\n";

    cout << "Check-In Date    : "
         << newReservation.checkInDate
         << "\n";

    cout << "Check-Out Date   : "
         << newReservation.checkOutDate
         << "\n";

    cout << "Number of Nights : "
         << newReservation.numberOfNights
         << "\n";

    cout << "Room Charge      : RM "
         << fixed
         << setprecision(2)
         << roomCharge
         << "\n";

    cout << "Status           : Reserved\n";
    cout << "Payment Status   : Unpaid\n";

    printLine();

    pauseScreen();
}

// ============================================================
// FIND RESERVATION
// ============================================================

int findReservationIndex(
    const string &bookingID
)
{
    for (int i = 0;
         i < static_cast<int>(reservations.size());
         i++)
    {
        if (reservations[i].bookingID ==
            bookingID)
        {
            return i;
        }
    }

    return -1;
}

// ============================================================
// SEARCH RESERVATION
// ============================================================

void searchReservation()
{
    printHeader("SEARCH RESERVATION");

    if (reservations.empty())
    {
        cout << "No reservations available.\n";

        pauseScreen();

        return;
    }

    string bookingID;

    cout << "Enter Booking ID: ";

    getline(cin, bookingID);

    int index =
        findReservationIndex(bookingID);

    if (index == -1)
    {
        cout << "\nReservation not found.\n";

        pauseScreen();

        return;
    }

    Reservation &reservation =
        reservations[index];

    int roomIndex =
        findRoomIndex(
            reservation.roomNumber
        );

    int customerIndex =
        findCustomerIndex(
            reservation.customerID
        );

    if (roomIndex == -1)
    {
        cout << "\nRoom information not found.\n";

        pauseScreen();

        return;
    }

    int floor =
        roomIndex / ROOMS_PER_FLOOR;

    int room =
        roomIndex % ROOMS_PER_FLOOR;

    printLine();

    cout << "RESERVATION INFORMATION\n";

    printLine();

    cout << "Booking ID       : "
         << reservation.bookingID
         << "\n";

    cout << "Customer ID      : "
         << reservation.customerID
         << "\n";

    if (customerIndex != -1)
    {
        cout << "Customer Name    : "
             << customers[customerIndex].name
             << "\n";
    }

    cout << "Room Number      : "
         << reservation.roomNumber
         << "\n";

    cout << "Room Type        : "
         << hotelRooms[floor][room].roomType
         << "\n";

    cout << "Check-In Date    : "
         << reservation.checkInDate
         << "\n";

    cout << "Check-Out Date   : "
         << reservation.checkOutDate
         << "\n";

    cout << "Number of Nights : "
         << reservation.numberOfNights
         << "\n";

    cout << "Room Charge      : RM "
         << fixed
         << setprecision(2)
         << calculateRoomCharge(
                reservation.roomNumber,
                reservation.numberOfNights
            )
         << "\n";

    cout << "Status           : "
         << reservation.status
         << "\n";

    cout << "Payment Status   : "
         << (reservation.paid
                 ? "Paid"
                 : "Unpaid")
         << "\n";

    printLine();

    pauseScreen();
}

// ============================================================
// CANCEL RESERVATION
// ============================================================

void cancelReservation()
{
    printHeader("CANCEL RESERVATION");

    if (reservations.empty())
    {
        cout << "No reservations available.\n";

        pauseScreen();

        return;
    }

    string bookingID;

    cout << "Enter Booking ID to cancel: ";

    getline(cin, bookingID);

    int index =
        findReservationIndex(bookingID);

    if (index == -1)
    {
        cout << "\nBooking ID not found.\n";

        pauseScreen();

        return;
    }

    Reservation &reservation =
        reservations[index];

    if (reservation.status ==
        "Cancelled")
    {
        cout << "\nThis reservation is already cancelled.\n";

        pauseScreen();

        return;
    }

    if (reservation.status ==
        "Checked-In")
    {
        cout << "\nThis guest has already checked in.\n";
        cout << "Check-out must be completed first.\n";

        pauseScreen();

        return;
    }

    cout << "\nReservation found.\n";

    cout << "Customer ID : "
         << reservation.customerID
         << "\n";

    cout << "Room Number : "
         << reservation.roomNumber
         << "\n";

    cout << "Status      : "
         << reservation.status
         << "\n";

    int confirmation =
        getValidatedInt(
            "\nConfirm cancellation? (1 = Yes, 2 = No): ",
            1,
            2
        );

    if (confirmation == 1)
    {
        int roomIndex =
            findRoomIndex(
                reservation.roomNumber
            );

        if (roomIndex != -1)
        {
            int floor =
                roomIndex / ROOMS_PER_FLOOR;

            int room =
                roomIndex % ROOMS_PER_FLOOR;

            roomStatus[floor][room] =
                ROOM_AVAILABLE;
        }

        reservation.status =
            "Cancelled";

        cout << "\nReservation successfully cancelled.\n";

        cout << "Room "
             << reservation.roomNumber
             << " is now available.\n";
    }
    else
    {
        cout << "\nCancellation operation cancelled.\n";
    }

    pauseScreen();
}

// ============================================================
// DISPLAY ALL RESERVATIONS
// ============================================================

void displayAllReservations()
{
    printHeader("ALL RESERVATIONS");

    if (reservations.empty())
    {
        cout << "No reservation records available.\n";

        pauseScreen();

        return;
    }

    cout << left
         << setw(12)
         << "Booking ID"
         << setw(12)
         << "Customer"
         << setw(10)
         << "Room"
         << setw(10)
         << "Nights"
         << setw(15)
         << "Check-In"
         << setw(15)
         << "Check-Out"
         << setw(15)
         << "Status"
         << "\n";

    printLine();

    for (const Reservation &reservation :
         reservations)
    {
        cout << left
             << setw(12)
             << reservation.bookingID
             << setw(12)
             << reservation.customerID
             << setw(10)
             << reservation.roomNumber
             << setw(10)
             << reservation.numberOfNights
             << setw(15)
             << reservation.checkInDate
             << setw(15)
             << reservation.checkOutDate
             << setw(15)
             << reservation.status
             << "\n";
    }

    pauseScreen();
}

// ============================================================
// RESERVATION MENU
// ============================================================

void reservationMenu()
{
    int choice;

    do
    {
        printHeader(
            "ROOM & RESERVATION MANAGEMENT"
        );

        cout << "1. Display All Rooms\n";
        cout << "2. Display Available Rooms\n";
        cout << "3. Search Room\n";
        cout << "4. Make New Reservation\n";
        cout << "5. Search Reservation\n";
        cout << "6. Cancel Reservation\n";
        cout << "7. Display All Reservations\n";
        cout << "8. Return to Main Menu\n";

        printLine();

        choice =
            getValidatedInt(
                "Enter your choice (1-8): ",
                1,
                8
            );

        switch (choice)
        {
            case 1:
                displayAllRooms();
                break;

            case 2:
                displayAvailableRooms();
                break;

            case 3:
                searchRoom();
                break;

            case 4:
                makeReservation();
                break;

            case 5:
                searchReservation();
                break;

            case 6:
                cancelReservation();
                break;

            case 7:
                displayAllReservations();
                break;

            case 8:
                cout << "\nReturning to Main Menu...\n";
                break;
        }

    } while (choice != 8);
}

// ============================================================
// CHECK-IN & CHECK-OUT
// ============================================================

// ============================================================
// CHECK-IN GUEST
// ============================================================

void checkInGuest()
{
    printHeader("CHECK-IN GUEST");

    if (reservations.empty())
    {
        cout << "No reservations available.\n";

        pauseScreen();

        return;
    }

    string bookingID;

    cout << "Enter Booking ID for check-in: ";

    getline(cin, bookingID);

    int reservationIndex =
        findReservationIndex(bookingID);

    if (reservationIndex == -1)
    {
        cout << "\nBooking ID not found.\n";

        pauseScreen();

        return;
    }

    Reservation &reservation =
        reservations[reservationIndex];

    if (reservation.status == "Cancelled")
    {
        cout << "\nThis reservation has been cancelled.\n";

        pauseScreen();

        return;
    }

    if (reservation.status == "Checked-In")
    {
        cout << "\nThis guest has already checked in.\n";

        pauseScreen();

        return;
    }

    if (reservation.status == "Checked-Out")
    {
        cout << "\nThis reservation has already been checked out.\n";

        pauseScreen();

        return;
    }

    int roomIndex =
        findRoomIndex(
            reservation.roomNumber
        );

    if (roomIndex == -1)
    {
        cout << "\nRoom information not found.\n";

        pauseScreen();

        return;
    }

    int floor =
        roomIndex / ROOMS_PER_FLOOR;

    int room =
        roomIndex % ROOMS_PER_FLOOR;

    if (roomStatus[floor][room] !=
        ROOM_RESERVED)
    {
        cout << "\nRoom is not currently reserved.\n";

        pauseScreen();

        return;
    }

    int customerIndex =
        findCustomerIndex(
            reservation.customerID
        );

    printLine();

    cout << "CHECK-IN INFORMATION\n";

    printLine();

    cout << "Booking ID : "
         << reservation.bookingID
         << "\n";

    cout << "Customer ID: "
         << reservation.customerID
         << "\n";

    if (customerIndex != -1)
    {
        cout << "Guest Name : "
             << customers[customerIndex].name
             << "\n";
    }

    cout << "Room       : "
         << reservation.roomNumber
         << "\n";

    cout << "Room Type  : "
         << hotelRooms[floor][room].roomType
         << "\n";

    cout << "Check-In   : "
         << reservation.checkInDate
         << "\n";

    cout << "Check-Out  : "
         << reservation.checkOutDate
         << "\n";

    printLine();

    int confirmation =
        getValidatedInt(
            "Confirm check-in? (1 = Yes, 2 = No): ",
            1,
            2
        );

    if (confirmation == 1)
    {
        reservation.status =
            "Checked-In";

        roomStatus[floor][room] =
            ROOM_OCCUPIED;

        cout << "\n";
        printLine();

        cout << "GUEST SUCCESSFULLY CHECKED IN!\n";

        printLine();

        cout << "Room Status: Occupied\n";
    }
    else
    {
        cout << "\nCheck-in operation cancelled.\n";
    }

    pauseScreen();
}

// ============================================================
// CHECK-OUT GUEST
// ============================================================

void checkOutGuest()
{
    printHeader("CHECK-OUT GUEST");

    if (reservations.empty())
    {
        cout << "No reservations available.\n";

        pauseScreen();

        return;
    }

    string bookingID;

    cout << "Enter Booking ID for check-out: ";

    getline(cin, bookingID);

    int reservationIndex =
        findReservationIndex(bookingID);

    if (reservationIndex == -1)
    {
        cout << "\nBooking ID not found.\n";

        pauseScreen();

        return;
    }

    Reservation &reservation =
        reservations[reservationIndex];

    if (reservation.status == "Reserved")
    {
        cout << "\nGuest has not checked in yet.\n";

        pauseScreen();

        return;
    }

    if (reservation.status == "Checked-Out")
    {
        cout << "\nGuest has already checked out.\n";

        pauseScreen();

        return;
    }

    if (reservation.status == "Cancelled")
    {
        cout << "\nThis reservation was cancelled.\n";

        pauseScreen();

        return;
    }

    int roomIndex =
        findRoomIndex(
            reservation.roomNumber
        );

    if (roomIndex == -1)
    {
        cout << "\nRoom information not found.\n";

        pauseScreen();

        return;
    }

    int floor =
        roomIndex / ROOMS_PER_FLOOR;

    int room =
        roomIndex % ROOMS_PER_FLOOR;

    int customerIndex =
        findCustomerIndex(
            reservation.customerID
        );

    printLine();

    cout << "CHECK-OUT INFORMATION\n";

    printLine();

    cout << "Booking ID : "
         << reservation.bookingID
         << "\n";

    cout << "Customer ID: "
         << reservation.customerID
         << "\n";

    if (customerIndex != -1)
    {
        cout << "Guest Name : "
             << customers[customerIndex].name
             << "\n";
    }

    cout << "Room       : "
         << reservation.roomNumber
         << "\n";

    cout << "Status     : "
         << reservation.status
         << "\n";

    printLine();

    int confirmation =
        getValidatedInt(
            "Confirm check-out? (1 = Yes, 2 = No): ",
            1,
            2
        );

    if (confirmation == 1)
    {
        reservation.status =
            "Checked-Out";

        roomStatus[floor][room] =
            ROOM_AVAILABLE;

        cout << "\n";
        printLine();

        cout << "GUEST SUCCESSFULLY CHECKED OUT!\n";

        printLine();

        cout << "Room Status: Available\n";

        cout << "\nThank you for staying at "
             << "Hillstone Grand Hotel.\n";
    }
    else
    {
        cout << "\nCheck-out operation cancelled.\n";
    }

    pauseScreen();
}

// ============================================================
// SEARCH GUEST STAY
// ============================================================

void searchGuestStay()
{
    printHeader("SEARCH GUEST STAY");

    if (reservations.empty())
    {
        cout << "No reservation records available.\n";

        pauseScreen();

        return;
    }

    string bookingID;

    cout << "Enter Booking ID: ";

    getline(cin, bookingID);

    int reservationIndex =
        findReservationIndex(bookingID);

    if (reservationIndex == -1)
    {
        cout << "\nBooking ID not found.\n";

        pauseScreen();

        return;
    }

    Reservation &reservation =
        reservations[reservationIndex];

    int customerIndex =
        findCustomerIndex(
            reservation.customerID
        );

    int roomIndex =
        findRoomIndex(
            reservation.roomNumber
        );

    if (roomIndex == -1)
    {
        cout << "\nRoom information not found.\n";

        pauseScreen();

        return;
    }

    int floor =
        roomIndex / ROOMS_PER_FLOOR;

    int room =
        roomIndex % ROOMS_PER_FLOOR;

    printLine();

    cout << "GUEST STAY INFORMATION\n";

    printLine();

    cout << "Booking ID        : "
         << reservation.bookingID
         << "\n";

    cout << "Customer ID       : "
         << reservation.customerID
         << "\n";

    if (customerIndex != -1)
    {
        cout << "Guest Name        : "
             << customers[customerIndex].name
             << "\n";
    }

    cout << "Room Number       : "
         << reservation.roomNumber
         << "\n";

    cout << "Room Type         : "
         << hotelRooms[floor][room].roomType
         << "\n";

    cout << "Check-In Date     : "
         << reservation.checkInDate
         << "\n";

    cout << "Check-Out Date    : "
         << reservation.checkOutDate
         << "\n";

    cout << "Number of Nights  : "
         << reservation.numberOfNights
         << "\n";

    cout << "Reservation Status: "
         << reservation.status
         << "\n";

    cout << "Room Status       : "
         << getRoomStatus(
                roomStatus[floor][room]
            )
         << "\n";

    cout << "Payment Status    : "
         << (reservation.paid
                 ? "Paid"
                 : "Unpaid")
         << "\n";

    printLine();

    pauseScreen();
}

// ============================================================
// DISPLAY OCCUPIED ROOMS
// ============================================================

void displayOccupiedRooms()
{
    printHeader("CURRENT OCCUPIED ROOMS");

    bool found = false;

    cout << left
         << setw(12)
         << "Room"
         << setw(15)
         << "Type"
         << setw(18)
         << "Price/Night"
         << setw(15)
         << "Status"
         << "\n";

    printLine();

    for (int floor = 0;
         floor < FLOORS;
         floor++)
    {
        for (int room = 0;
             room < ROOMS_PER_FLOOR;
             room++)
        {
            if (roomStatus[floor][room] ==
                ROOM_OCCUPIED)
            {
                found = true;

                cout << left
                     << setw(12)
                     << hotelRooms[floor][room].roomNumber
                     << setw(15)
                     << hotelRooms[floor][room].roomType
                     << "RM "
                     << setw(15)
                     << fixed
                     << setprecision(2)
                     << hotelRooms[floor][room].pricePerNight
                     << setw(15)
                     << "Occupied"
                     << "\n";
            }
        }
    }

    if (!found)
    {
        cout << "\nNo rooms are currently occupied.\n";
    }

    pauseScreen();
}

// ============================================================
// CHECK-IN & CHECK-OUT MENU
// ============================================================

void checkInOutMenu()
{
    int choice;

    do
    {
        printHeader(
            "CHECK-IN & CHECK-OUT MANAGEMENT"
        );

        cout << "1. Check-In Guest\n";
        cout << "2. Check-Out Guest\n";
        cout << "3. Search Guest Stay\n";
        cout << "4. Display Occupied Rooms\n";
        cout << "5. Return to Main Menu\n";

        printLine();

        choice =
            getValidatedInt(
                "Enter your choice (1-5): ",
                1,
                5
            );

        switch (choice)
        {
            case 1:
                checkInGuest();
                break;

            case 2:
                checkOutGuest();
                break;

            case 3:
                searchGuestStay();
                break;

            case 4:
                displayOccupiedRooms();
                break;

            case 5:
                cout << "\nReturning to Main Menu...\n";
                break;
        }

    } while (choice != 5);
}

// ============================================================
// PAYMENT & BILLING
// ============================================================

// ============================================================
// CALCULATE ROOM SERVICE CHARGE
// ============================================================

double calculateServiceCharge(
    const string &bookingID
)
{
    double total = 0.0;

    for (const RoomServiceOrder &order :
         roomServiceOrders)
    {
        if (order.bookingID == bookingID)
        {
            total += order.totalAmount;
        }
    }

    return total;
}

// ============================================================
// MAKE PAYMENT
// ============================================================

void makePayment()
{
    printHeader("MAKE PAYMENT");

    if (reservations.empty())
    {
        cout << "No reservations available.\n";

        pauseScreen();

        return;
    }

    string bookingID;

    cout << "Enter Booking ID: ";

    getline(cin, bookingID);

    int reservationIndex =
        findReservationIndex(bookingID);

    if (reservationIndex == -1)
    {
        cout << "\nBooking ID not found.\n";

        pauseScreen();

        return;
    }

    Reservation &reservation =
        reservations[reservationIndex];

    if (reservation.status == "Cancelled")
    {
        cout << "\nCancelled reservations cannot be paid.\n";

        pauseScreen();

        return;
    }

    if (reservation.paid)
    {
        cout << "\nThis reservation has already been paid.\n";

        pauseScreen();

        return;
    }

    double roomCharge =
        calculateRoomCharge(
            reservation.roomNumber,
            reservation.numberOfNights
        );

    double serviceCharge =
        calculateServiceCharge(
            bookingID
        );

    double discount = 0.0;

    int customerIndex =
        findCustomerIndex(
            reservation.customerID
        );

    if (customerIndex != -1)
    {
        if (customers[customerIndex].loyaltyPoints >= 100)
        {
            discount =
                roomCharge * 0.10;
        }
    }

    double totalAmount =
        roomCharge +
        serviceCharge -
        discount;

    printLine();

    cout << "PAYMENT SUMMARY\n";

    printLine();

    cout << "Booking ID     : "
         << bookingID
         << "\n";

    cout << "Room Charge    : RM "
         << fixed
         << setprecision(2)
         << roomCharge
         << "\n";

    cout << "Service Charge : RM "
         << serviceCharge
         << "\n";

    cout << "Discount       : RM "
         << discount
         << "\n";

    cout << "Total Amount   : RM "
         << totalAmount
         << "\n";

    printLine();

    cout << "\nPayment Method:\n";
    cout << "1. Cash\n";
    cout << "2. Credit/Debit Card\n";
    cout << "3. Online Banking\n";
    cout << "4. E-Wallet\n";

    int method =
        getValidatedInt(
            "Select payment method (1-4): ",
            1,
            4
        );

    string paymentMethod;

    if (method == 1)
    {
        paymentMethod = "Cash";
    }
    else if (method == 2)
    {
        paymentMethod = "Credit/Debit Card";
    }
    else if (method == 3)
    {
        paymentMethod = "Online Banking";
    }
    else
    {
        paymentMethod = "E-Wallet";
    }

    double paymentAmount =
        getValidatedDouble(
            "Enter payment amount: RM ",
            totalAmount
        );

    double change =
        paymentAmount - totalAmount;

    Payment newPayment;

    newPayment.paymentID =
        generateID(
            "P",
            paymentCounter
        );

    newPayment.bookingID =
        bookingID;

    newPayment.roomCharge =
        roomCharge;

    newPayment.serviceCharge =
        serviceCharge;

    newPayment.discount =
        discount;

    newPayment.totalAmount =
        totalAmount;

    newPayment.paymentAmount =
        paymentAmount;

    newPayment.change =
        change;

    newPayment.paymentMethod =
        paymentMethod;

    payments.push_back(newPayment);

    paymentCounter++;

    reservation.paid = true;

    // Add loyalty points
    if (customerIndex != -1)
    {
        int pointsEarned =
            static_cast<int>(totalAmount / 10);

        customers[customerIndex].loyaltyPoints +=
            pointsEarned;

        cout << "\nLoyalty points earned: "
             << pointsEarned
             << "\n";
    }

    printLine();

    cout << "PAYMENT SUCCESSFUL!\n";

    printLine();

    cout << "Payment ID     : "
         << newPayment.paymentID
         << "\n";

    cout << "Payment Method : "
         << newPayment.paymentMethod
         << "\n";

    cout << "Total Amount   : RM "
         << newPayment.totalAmount
         << "\n";

    cout << "Payment Amount : RM "
         << newPayment.paymentAmount
         << "\n";

    cout << "Change         : RM "
         << newPayment.change
         << "\n";

    cout << "Payment Status : Paid\n";

    printLine();

    pauseScreen();
}

// ============================================================
// SEARCH PAYMENT
// ============================================================

void searchPayment()
{
    printHeader("SEARCH PAYMENT");

    if (payments.empty())
    {
        cout << "No payment records available.\n";

        pauseScreen();

        return;
    }

    string paymentID;

    cout << "Enter Payment ID: ";

    getline(cin, paymentID);

    bool found = false;

    for (const Payment &payment : payments)
    {
        if (payment.paymentID == paymentID)
        {
            found = true;

            printLine();

            cout << "PAYMENT INFORMATION\n";

            printLine();

            cout << "Payment ID     : "
                 << payment.paymentID
                 << "\n";

            cout << "Booking ID     : "
                 << payment.bookingID
                 << "\n";

            cout << "Room Charge    : RM "
                 << fixed
                 << setprecision(2)
                 << payment.roomCharge
                 << "\n";

            cout << "Service Charge : RM "
                 << payment.serviceCharge
                 << "\n";

            cout << "Discount       : RM "
                 << payment.discount
                 << "\n";

            cout << "Total Amount   : RM "
                 << payment.totalAmount
                 << "\n";

            cout << "Payment Amount : RM "
                 << payment.paymentAmount
                 << "\n";

            cout << "Change         : RM "
                 << payment.change
                 << "\n";

            cout << "Method         : "
                 << payment.paymentMethod
                 << "\n";

            printLine();

            break;
        }
    }

    if (!found)
    {
        cout << "\nPayment not found.\n";
    }

    pauseScreen();
}

// ============================================================
// DISPLAY ALL PAYMENTS
// ============================================================

void displayAllPayments()
{
    printHeader("ALL PAYMENT RECORDS");

    if (payments.empty())
    {
        cout << "No payment records available.\n";

        pauseScreen();

        return;
    }

    cout << left
         << setw(12)
         << "Payment ID"
         << setw(12)
         << "Booking ID"
         << setw(15)
         << "Total"
         << setw(20)
         << "Method"
         << "\n";

    printLine();

    for (const Payment &payment : payments)
    {
        cout << left
             << setw(12)
             << payment.paymentID
             << setw(12)
             << payment.bookingID
             << "RM "
             << setw(12)
             << fixed
             << setprecision(2)
             << payment.totalAmount
             << setw(20)
             << payment.paymentMethod
             << "\n";
    }

    pauseScreen();
}

// ============================================================
// PAYMENT MENU
// ============================================================

void paymentMenu()
{
    int choice;

    do
    {
        printHeader("PAYMENT & BILLING");

        cout << "1. Make Payment\n";
        cout << "2. Search Payment\n";
        cout << "3. Display All Payments\n";
        cout << "4. Return to Main Menu\n";

        printLine();

        choice =
            getValidatedInt(
                "Enter your choice (1-4): ",
                1,
                4
            );

        switch (choice)
        {
            case 1:
                makePayment();
                break;

            case 2:
                searchPayment();
                break;

            case 3:
                displayAllPayments();
                break;

            case 4:
                cout << "\nReturning to Main Menu...\n";
                break;
        }

    } while (choice != 4);
}

// ============================================================
// ROOM SERVICE
// ============================================================

void displayRoomServiceMenu()
{
    printHeader("ROOM SERVICE MENU");

    cout << "1. Fried Rice          RM 12.00\n";
    cout << "2. Chicken Burger      RM 15.00\n";
    cout << "3. Club Sandwich       RM 14.00\n";
    cout << "4. Nasi Goreng         RM 13.00\n";
    cout << "5. Spaghetti           RM 16.00\n";
    cout << "6. Coffee              RM  6.00\n";
    cout << "7. Orange Juice        RM  7.00\n";
    cout << "8. Mineral Water       RM  3.00\n";

    printLine();
}

// ============================================================
// FIND ORDER
// ============================================================

int findOrderIndex(
    const string &orderID
)
{
    for (int i = 0;
         i < static_cast<int>(roomServiceOrders.size());
         i++)
    {
        if (roomServiceOrders[i].orderID ==
            orderID)
        {
            return i;
        }
    }

    return -1;
}

// ============================================================
// ORDER ROOM SERVICE
// ============================================================

void orderRoomService()
{
    printHeader("ORDER ROOM SERVICE");

    bool validBooking = false;

    string bookingID;

    cout << "Enter Booking ID: ";

    getline(cin, bookingID);

    int reservationIndex =
        findReservationIndex(bookingID);

    if (reservationIndex == -1)
    {
        cout << "\nBooking ID not found.\n";

        pauseScreen();

        return;
    }

    Reservation &reservation =
        reservations[reservationIndex];

    if (reservation.status != "Checked-In")
    {
        cout << "\nRoom service is only available "
             << "for checked-in guests.\n";

        pauseScreen();

        return;
    }

    validBooking = true;

    if (!validBooking)
    {
        return;
    }

    displayRoomServiceMenu();

    int item =
        getValidatedInt(
            "Select food/drink (1-8): ",
            1,
            8
        );

    string itemName;
    double price;

    if (item == 1)
    {
        itemName = "Fried Rice";
        price = 12.00;
    }
    else if (item == 2)
    {
        itemName = "Chicken Burger";
        price = 15.00;
    }
    else if (item == 3)
    {
        itemName = "Club Sandwich";
        price = 14.00;
    }
    else if (item == 4)
    {
        itemName = "Nasi Goreng";
        price = 13.00;
    }
    else if (item == 5)
    {
        itemName = "Spaghetti";
        price = 16.00;
    }
    else if (item == 6)
    {
        itemName = "Coffee";
        price = 6.00;
    }
    else if (item == 7)
    {
        itemName = "Orange Juice";
        price = 7.00;
    }
    else
    {
        itemName = "Mineral Water";
        price = 3.00;
    }

    int quantity =
        getValidatedInt(
            "Enter quantity (1-20): ",
            1,
            20
        );

    double totalAmount =
        price * quantity;

    RoomServiceOrder newOrder;

    newOrder.orderID =
        generateID(
            "O",
            orderCounter
        );

    newOrder.bookingID =
        bookingID;

    newOrder.itemName =
        itemName;

    newOrder.quantity =
        quantity;

    newOrder.totalAmount =
        totalAmount;

    roomServiceOrders.push_back(
        newOrder
    );

    orderCounter++;

    cout << "\n";

    printLine();

    cout << "ROOM SERVICE ORDER SUCCESSFUL!\n";

    printLine();

    cout << "Order ID     : "
         << newOrder.orderID
         << "\n";

    cout << "Booking ID   : "
         << newOrder.bookingID
         << "\n";

    cout << "Item         : "
         << newOrder.itemName
         << "\n";

    cout << "Quantity     : "
         << newOrder.quantity
         << "\n";

    cout << "Total Amount : RM "
         << fixed
         << setprecision(2)
         << newOrder.totalAmount
         << "\n";

    printLine();

    pauseScreen();
}

// ============================================================
// VIEW ROOM SERVICE ORDERS
// ============================================================

void viewRoomServiceOrders()
{
    printHeader("ROOM SERVICE ORDERS");

    if (roomServiceOrders.empty())
    {
        cout << "No room service orders available.\n";

        pauseScreen();

        return;
    }

    cout << left
         << setw(12)
         << "Order ID"
         << setw(12)
         << "Booking"
         << setw(22)
         << "Item"
         << setw(10)
         << "Qty"
         << setw(15)
         << "Total"
         << "\n";

    printLine();

    for (const RoomServiceOrder &order :
         roomServiceOrders)
    {
        cout << left
             << setw(12)
             << order.orderID
             << setw(12)
             << order.bookingID
             << setw(22)
             << order.itemName
             << setw(10)
             << order.quantity
             << "RM "
             << setw(12)
             << fixed
             << setprecision(2)
             << order.totalAmount
             << "\n";
    }

    pauseScreen();
}

// ============================================================
// CANCEL ROOM SERVICE ORDER
// ============================================================

void cancelRoomServiceOrder()
{
    printHeader("CANCEL ROOM SERVICE ORDER");

    if (roomServiceOrders.empty())
    {
        cout << "No room service orders available.\n";

        pauseScreen();

        return;
    }

    string orderID;

    cout << "Enter Order ID: ";

    getline(cin, orderID);

    int index =
        findOrderIndex(orderID);

    if (index == -1)
    {
        cout << "\nOrder ID not found.\n";

        pauseScreen();

        return;
    }

    cout << "\nOrder found:\n";

    cout << "Item     : "
         << roomServiceOrders[index].itemName
         << "\n";

    cout << "Quantity : "
         << roomServiceOrders[index].quantity
         << "\n";

    cout << "Total    : RM "
         << fixed
         << setprecision(2)
         << roomServiceOrders[index].totalAmount
         << "\n";

    int confirmation =
        getValidatedInt(
            "Cancel this order? (1 = Yes, 2 = No): ",
            1,
            2
        );

    if (confirmation == 1)
    {
        roomServiceOrders.erase(
            roomServiceOrders.begin() + index
        );

        cout << "\nRoom service order cancelled.\n";
    }
    else
    {
        cout << "\nCancellation cancelled.\n";
    }

    pauseScreen();
}

// ============================================================
// ROOM SERVICE MENU
// ============================================================

void roomServiceMenu()
{
    int choice;

    do
    {
        printHeader("ROOM SERVICE");

        cout << "1. Order Room Service\n";
        cout << "2. View Room Service Orders\n";
        cout << "3. Cancel Room Service Order\n";
        cout << "4. Return to Main Menu\n";

        printLine();

        choice =
            getValidatedInt(
                "Enter your choice (1-4): ",
                1,
                4
            );

        switch (choice)
        {
            case 1:
                orderRoomService();
                break;

            case 2:
                viewRoomServiceOrders();
                break;

            case 3:
                cancelRoomServiceOrder();
                break;

            case 4:
                cout << "\nReturning to Main Menu...\n";
                break;
        }

    } while (choice != 4);
}

// ============================================================
// LOYALTY POINTS
// ============================================================

void viewLoyaltyPoints()
{
    printHeader("VIEW LOYALTY POINTS");

    if (customers.empty())
    {
        cout << "No customer records available.\n";

        pauseScreen();

        return;
    }

    string customerID;

    cout << "Enter Customer ID: ";

    getline(cin, customerID);

    int index =
        findCustomerIndex(customerID);

    if (index == -1)
    {
        cout << "\nCustomer not found.\n";

        pauseScreen();

        return;
    }

    cout << "\n";

    printLine();

    cout << "LOYALTY INFORMATION\n";

    printLine();

    cout << "Customer ID : "
         << customers[index].customerID
         << "\n";

    cout << "Customer Name: "
         << customers[index].name
         << "\n";

    cout << "Points       : "
         << customers[index].loyaltyPoints
         << "\n";

    printLine();

    pauseScreen();
}

// ============================================================
// REDEEM LOYALTY POINTS
// ============================================================

void redeemLoyaltyPoints()
{
    printHeader("REDEEM LOYALTY POINTS");

    if (customers.empty())
    {
        cout << "No customer records available.\n";

        pauseScreen();

        return;
    }

    string customerID;

    cout << "Enter Customer ID: ";

    getline(cin, customerID);

    int index =
        findCustomerIndex(customerID);

    if (index == -1)
    {
        cout << "\nCustomer not found.\n";

        pauseScreen();

        return;
    }

    cout << "\nCurrent points: "
         << customers[index].loyaltyPoints
         << "\n";

    cout << "\nRedemption rule:\n";
    cout << "100 points = RM 10 discount\n";

    if (customers[index].loyaltyPoints < 100)
    {
        cout << "\nInsufficient points.\n";
        cout << "You need at least 100 points.\n";

        pauseScreen();

        return;
    }

    int points =
        getValidatedInt(
            "Enter points to redeem (100-1000): ",
            100,
            1000
        );

    if (points >
        customers[index].loyaltyPoints)
    {
        cout << "\nYou do not have enough points.\n";

        pauseScreen();

        return;
    }

    if (points % 100 != 0)
    {
        cout << "\nPoints must be redeemed in multiples of 100.\n";

        pauseScreen();

        return;
    }

    int discount =
        (points / 100) * 10;

    customers[index].loyaltyPoints -= points;

    cout << "\n";
    printLine();

    cout << "LOYALTY POINTS REDEEMED!\n";

    printLine();

    cout << "Points Used : "
         << points
         << "\n";

    cout << "Discount    : RM "
         << discount
         << "\n";

    cout << "Remaining Points: "
         << customers[index].loyaltyPoints
         << "\n";

    printLine();

    pauseScreen();
}

// ============================================================
// DISPLAY LOYALTY MEMBERS
// ============================================================

void displayLoyaltyMembers()
{
    printHeader("LOYALTY MEMBERS");

    if (customers.empty())
    {
        cout << "No customer records available.\n";

        pauseScreen();

        return;
    }

    cout << left
         << setw(12)
         << "Customer ID"
         << setw(25)
         << "Name"
         << setw(15)
         << "Points"
         << "\n";

    printLine();

    for (const Customer &customer :
         customers)
    {
        cout << left
             << setw(12)
             << customer.customerID
             << setw(25)
             << customer.name
             << setw(15)
             << customer.loyaltyPoints
             << "\n";
    }

    pauseScreen();
}

// ============================================================
// LOYALTY MENU
// ============================================================

void loyaltyMenu()
{
    int choice;

    do
    {
        printHeader("LOYALTY POINTS PROGRAM");

        cout << "1. View Loyalty Points\n";
        cout << "2. Redeem Loyalty Points\n";
        cout << "3. Display Loyalty Members\n";
        cout << "4. Return to Main Menu\n";

        printLine();

        choice =
            getValidatedInt(
                "Enter your choice (1-4): ",
                1,
                4
            );

        switch (choice)
        {
            case 1:
                viewLoyaltyPoints();
                break;

            case 2:
                redeemLoyaltyPoints();
                break;

            case 3:
                displayLoyaltyMembers();
                break;

            case 4:
                cout << "\nReturning to Main Menu...\n";
                break;
        }

    } while (choice != 4);
}

// ============================================================
// REPORTS & STATISTICS
// ============================================================

// ============================================================
// ROOM OCCUPANCY REPORT
// ============================================================

void roomOccupancyReport()
{
    printHeader("ROOM OCCUPANCY REPORT");

    int available = 0;
    int reserved = 0;
    int occupied = 0;

    for (int floor = 0;
         floor < FLOORS;
         floor++)
    {
        for (int room = 0;
             room < ROOMS_PER_FLOOR;
             room++)
        {
            if (roomStatus[floor][room] ==
                ROOM_AVAILABLE)
            {
                available++;
            }
            else if (roomStatus[floor][room] ==
                     ROOM_RESERVED)
            {
                reserved++;
            }
            else
            {
                occupied++;
            }
        }
    }

    int totalRooms =
        FLOORS * ROOMS_PER_FLOOR;

    double occupancyRate =
        static_cast<double>(occupied) /
        totalRooms * 100.0;

    printLine();

    cout << "ROOM STATISTICS\n";

    printLine();

    cout << "Total Rooms     : "
         << totalRooms
         << "\n";

    cout << "Available Rooms : "
         << available
         << "\n";

    cout << "Reserved Rooms  : "
         << reserved
         << "\n";

    cout << "Occupied Rooms  : "
         << occupied
         << "\n";

    cout << "Occupancy Rate  : "
         << fixed
         << setprecision(2)
         << occupancyRate
         << "%\n";

    printLine();

    pauseScreen();
}

// ============================================================
// REVENUE REPORT
// ============================================================

void revenueReport()
{
    printHeader("REVENUE REPORT");

    double roomRevenue = 0.0;
    double serviceRevenue = 0.0;
    double totalRevenue = 0.0;

    for (const Payment &payment :
         payments)
    {
        roomRevenue +=
            payment.roomCharge -
            payment.discount;

        serviceRevenue +=
            payment.serviceCharge;

        totalRevenue +=
            payment.totalAmount;
    }

    printLine();

    cout << "REVENUE SUMMARY\n";

    printLine();

    cout << "Room Revenue    : RM "
         << fixed
         << setprecision(2)
         << roomRevenue
         << "\n";

    cout << "Service Revenue : RM "
         << serviceRevenue
         << "\n";

    cout << "Total Revenue   : RM "
         << totalRevenue
         << "\n";

    cout << "Total Payments  : "
         << payments.size()
         << "\n";

    printLine();

    pauseScreen();
}

// ============================================================
// CUSTOMER REPORT
// ============================================================

void customerReport()
{
    printHeader("CUSTOMER REPORT");

    cout << "Total Customers : "
         << customers.size()
         << "\n";

    int totalPoints = 0;

    for (const Customer &customer :
         customers)
    {
        totalPoints +=
            customer.loyaltyPoints;
    }

    cout << "Total Loyalty Points: "
         << totalPoints
         << "\n";

    printLine();

    if (!customers.empty())
    {
        cout << "\nCUSTOMER LIST\n\n";

        cout << left
             << setw(12)
             << "ID"
             << setw(25)
             << "Name"
             << setw(15)
             << "Points"
             << "\n";

        printLine();

        for (const Customer &customer :
             customers)
        {
            cout << left
                 << setw(12)
                 << customer.customerID
                 << setw(25)
                 << customer.name
                 << setw(15)
                 << customer.loyaltyPoints
                 << "\n";
        }
    }

    pauseScreen();
}

// ============================================================
// RESERVATION REPORT
// ============================================================

void reservationReport()
{
    printHeader("RESERVATION REPORT");

    int reserved = 0;
    int checkedIn = 0;
    int checkedOut = 0;
    int cancelled = 0;

    for (const Reservation &reservation :
         reservations)
    {
        if (reservation.status == "Reserved")
        {
            reserved++;
        }
        else if (reservation.status == "Checked-In")
        {
            checkedIn++;
        }
        else if (reservation.status == "Checked-Out")
        {
            checkedOut++;
        }
        else if (reservation.status == "Cancelled")
        {
            cancelled++;
        }
    }

    printLine();

    cout << "RESERVATION STATISTICS\n";

    printLine();

    cout << "Total Reservations : "
         << reservations.size()
         << "\n";

    cout << "Reserved           : "
         << reserved
         << "\n";

    cout << "Checked-In         : "
         << checkedIn
         << "\n";

    cout << "Checked-Out        : "
         << checkedOut
         << "\n";

    cout << "Cancelled          : "
         << cancelled
         << "\n";

    printLine();

    pauseScreen();
}

// ============================================================
// REPORT MENU
// ============================================================

void reportMenu()
{
    int choice;

    do
    {
        printHeader("REPORTS & STATISTICS");

        cout << "1. Room Occupancy Report\n";
        cout << "2. Revenue Report\n";
        cout << "3. Customer Report\n";
        cout << "4. Reservation Report\n";
        cout << "5. Return to Main Menu\n";

        printLine();

        choice =
            getValidatedInt(
                "Enter your choice (1-5): ",
                1,
                5
            );

        switch (choice)
        {
            case 1:
                roomOccupancyReport();
                break;

            case 2:
                revenueReport();
                break;

            case 3:
                customerReport();
                break;

            case 4:
                reservationReport();
                break;

            case 5:
                cout << "\nReturning to Main Menu...\n";
                break;
        }

    } while (choice != 5);
}

// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{
    int choice;

    initializeRooms();

    displayIntroduction();

    pauseScreen();

    do
    {
        displayMainMenu();

        choice =
            getValidatedInt(
                "Enter your choice (1-8): ",
                1,
                8
            );

        switch (choice)
        {
            case 1:
                customerMenu();
                break;

            case 2:
                reservationMenu();
                break;

            case 3:
                checkInOutMenu();
                break;

            case 4:
                paymentMenu();
                break;

            case 5:
                reportMenu();
                break;

            case 6:
                roomServiceMenu();
                break;

            case 7:
                loyaltyMenu();
                break;

            case 8:

                printHeader(
                    "SYSTEM TERMINATION"
                );

                cout << "Thank you for using "
                     << "Hillstone Grand Hotel System.\n";

                cout << "Have a pleasant day!\n";

                break;
        }

    } while (choice != 8);

    return 0;
}