#pragma once

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

namespace state {
/// Initial state of the FSM
struct Start {};
/// State if any transition was wrong, no matter the token, the FSM will stay in the invalid state
struct Invalid {};
/// If the sequence of tokens is correct the FSM are in the valid state, wrong tokens put the FSM
/// into the invalid state, only the semicolon is allowed.
struct Valid {};

struct SelectStmt {};

struct AllColumns {};

struct NamedColumn {};

struct MoreColumns {};

struct FromClause {};

struct TableName {};
// TODO: all new states go between here...

} // namespace state

/// variant of all possible states of our finite machine
/// TODO: Add all the possible states to the variant
using State =
    std::variant<state::Start, state::Invalid, state::Valid, state::SelectStmt, state::AllColumns, state::NamedColumn, state::MoreColumns, state::FromClause, state::TableName>;

/// Transition from the `Start` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Start, const Token &token);

/// Transition from the `Valid` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Valid, const Token &token);

/// Transition from the `Invalid` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Invalid, const Token &token);

State transition(state::SelectStmt, const Token &token);

State transition(state::AllColumns, const Token &token);

State transition(state::NamedColumn, const Token &token);

State transition(state::MoreColumns, const Token &token);

State transition(state::FromClause, const Token &token);

State transition(state::TableName, const Token &token);
// TODO: all of the transition functions from the newly created states go
// between here...
// ... and here

/// Our finite state machine.
/// The initial state is `Start` and based on the given tokens it will move to
/// the next state. You can query if the given sequence of tokens was valid with
/// the `is_valid` member function.
class SqlValidator {
public:
    SqlValidator() = default;
    
    /// TODO: Implement this member function
    /// Returns `true` iff the variant is in the `Valid` state
    [[nodiscard]]
    bool is_valid() const;

    /// TODO: Implement this member function
    /// Moves from one state to the next given the token.
    void handle(const Token &token);

private:
    State state_ = state::Start{};
};

/// TODO: Implement this function!
///
/// Given a sequence of tokens, this functions returns true, if it is a valid
/// (simplified) select clause of SQL
///
/// - Example of a valid token sequence:
///   - "SELECT * FROM table;"
/// - Example of a invalid token sequence:
///   - "SELECT Col1 Col2 FROM table;" (Missing comma)
///   - "SELECT Col1, * FROM table;" (Either select specific columns or all)
///   - "SELECT Col1 FROM table" (Missing Semicolon)
///   - "FROM * SELECT Col1;" (Select should be first)
///
/// These sequences must be given in a `std::vector<Token>`
[[nodiscard]]
bool is_valid_sql_query(const std::vector<Token> &tokens);
} // namespace sql
