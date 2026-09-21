#include <iostream>
#include <vector>
#include <optional>
#include <memory>
#include <tuple>
#include <utility>
#include "debug_derive.hpp"

// ============================================================================
// Example 1: Basic Intrusive Structs with Nested Hierarchy
// ============================================================================

struct Address {
    std::string city;
    int zip;

    DEBUG_FIELDS(city, zip)
};

struct User {
    std::string name;
    int age;
    Address address;

    DEBUG_FIELDS(name, age, address)
};

// ============================================================================
// Example 2: Container and Optional Fields
// ============================================================================

struct Company {
    std::string name;
    std::vector<User> employees;
    std::optional<Address> headquarters;

    DEBUG_FIELDS(name, employees, headquarters)
};

// ============================================================================
// Example 3: Non-Intrusive External Struct Registration (DEBUG_STRUCT)
// ============================================================================

struct GeoCoordinate {
    double latitude;
    double longitude;
};
DEBUG_STRUCT(GeoCoordinate, latitude, longitude)

// ============================================================================
// Example 4: Class with Private Members
// ============================================================================

class BankAccount {
private:
    std::string account_id;
    int64_t balance_cents;
    bool is_frozen;

public:
    BankAccount(std::string id, int64_t balance, bool frozen = false)
        : account_id(std::move(id)), balance_cents(balance), is_frozen(frozen) {}

    // DEBUG_FIELDS placed anywhere (even inside private or public) has access to private fields!
    DEBUG_FIELDS(account_id, balance_cents, is_frozen)
};

// ============================================================================
// Example 5: Enums (Scoped and Unscoped)
// ============================================================================

enum class ServerState {
    Starting,
    Running,
    Stopped
};
DEBUG_ENUM(ServerState, Starting, Running, Stopped)

enum Priority {
    Low,
    Medium,
    High
};

// ============================================================================
// Example 6: Recursive Linked-List / Graph Node with Cycle Detection
// ============================================================================

struct Node {
    int value;
    std::shared_ptr<Node> next;

    DEBUG_FIELDS(value, next)
};

// ============================================================================
// Main Demonstration
// ============================================================================

int main() {
    std::cout << "=========================================================\n";
    std::cout << " C++20 #[derive(Debug)] Demonstration\n";
    std::cout << "=========================================================\n\n";

    // 1. Exact user request example
    std::cout << "--- 1. User & Address Debug Print ---\n";
    User user{"Alice", 30, {"Amsterdam", 1011}};
    debug_print(user);

    // 2. Containers and nested optionals
    std::cout << "\n--- 2. Company with Vector of Structs & Optional ---\n";
    Company company{
        "TechCorp Europe",
        {
            {"Alice", 30, {"Amsterdam", 1011}},
            {"Bob", 25, {"Rotterdam", 3011}}
        },
        Address{"The Hague", 2500}
    };
    debug_print(company);

    // 3. Optional with None
    std::cout << "\n--- 3. Optional with None ---\n";
    Company remote_company{
        "RemoteOnly Inc",
        {
            {"Charlie", 35, {"Berlin", 10115}}
        },
        std::nullopt
    };
    debug_print(remote_company);

    // 4. Non-intrusive registration
    std::cout << "\n--- 4. Non-Intrusive External Registration (DEBUG_STRUCT) ---\n";
    GeoCoordinate coord{52.3676, 4.9041};
    debug_print(coord);

    // 5. Private members
    std::cout << "\n--- 5. Encapsulated Class with Private Members ---\n";
    BankAccount account{"NL91ABNA0417164300", 254000, false};
    debug_print(account);

    // 6. Enums (registered and unregistered)
    std::cout << "\n--- 6. Enums ---\n";
    ServerState state = ServerState::Running;
    Priority priority = High;
    std::cout << "Registered enum:   ";
    debug_print(state);
    std::cout << "Unregistered enum: ";
    debug_print(priority);

    // 7. Tuples and Pairs
    std::cout << "\n--- 7. Pairs and Tuples ---\n";
    auto user_tuple = std::make_tuple(101, std::string("TupleUser"), true, 3.14159);
    debug_print(user_tuple);

    // 8. Cycle and Depth Protection
    std::cout << "\n--- 8. Cycle & Depth Protection ---\n";
    auto node1 = std::make_shared<Node>(Node{1, nullptr});
    auto node2 = std::make_shared<Node>(Node{2, nullptr});
    node1->next = node2;
    node2->next = node1; // Form a cyclic reference!

    std::cout << "Printing cyclic graph (Node 1 -> Node 2 -> Node 1):\n";
    debug_print(*node1);

    // 9. to_debug_string string return
    std::cout << "\n--- 9. String Formatting via to_debug_string() ---\n";
    std::string serialized = to_debug_string(user);
    std::cout << "Stored in std::string (length " << serialized.size() << " bytes):\n"
              << serialized << "\n";

    return 0;
}
