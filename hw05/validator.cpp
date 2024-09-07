#include "validator.h"

#include <variant>
#include <vector>
#include <iostream>
#include "token.h"

namespace sql {

bool SqlValidator::is_valid() const {
    // TODO: Implement this
    
    return std::holds_alternative<state::Valid>(state_);
}

void SqlValidator::handle(const Token &token) {
    // TODO: Implement this
    //tried to do this with operatiors inside SqlValidator but i cannot get the transition in it or i dunno maybe im dumb
    auto lambda_for_visit = [&token] (auto statehinho) {return transition(statehinho,token);};
    state_ = std::visit(lambda_for_visit, state_);
}

struct TransitionFromStartVisitor {
    // Only the `Select` token advances the FSM from the `Start` state
    // TODO: Correct the behaviour
    State operator()(token::Select) const { return state::SelectStmt{}; }

    // All the other tokens, put it in the invalid state
    State operator()(token::From) const { return state::Invalid{}; }
    State operator()(token::Comma) const { return state::Invalid{}; }
    State operator()(token::Asterisks) const { return state::Invalid{}; }
    State operator()(token::Semicolon) const { return state::Invalid{}; }
    State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::Start, const Token &token) {
    
    return std::visit(TransitionFromStartVisitor{}, token.value());
}

State transition(state::Valid, const Token &token) {
    // TODO: Implement
    if(std::holds_alternative<token::Semicolon>(token.value())){
        return state::Valid{};
    }else{
        return state::Invalid{}; 
    }
    
}

State transition(state::Invalid, const Token &token) {
    // TODO: Implement
    return state::Invalid{};
}

State transition(state::SelectStmt, const Token &token){
    if(std::holds_alternative<token::Select>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::From>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Comma>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Semicolon>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Asterisks>(token.value())){
        return state::AllColumns{};
    }else if(std::holds_alternative<token::Identifier>(token.value())){
        return state::NamedColumn{};
    }
};

State transition(state::AllColumns, const Token &token){
    if(std::holds_alternative<token::Select>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Comma>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::From>(token.value())){
        return state::FromClause{};
    }else if(std::holds_alternative<token::Asterisks>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Semicolon>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Identifier>(token.value())){
        return state::Invalid{};
    }
};

State transition(state::NamedColumn, const Token &token){
    if(std::holds_alternative<token::Select>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::From>(token.value())){
        return state::FromClause{};
    }else if(std::holds_alternative<token::Comma>(token.value())){
        return state::MoreColumns{};
    }else if(std::holds_alternative<token::Asterisks>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Semicolon>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Identifier>(token.value())){
        return state::Invalid{};
    }
};

State transition(state::MoreColumns, const Token &token){
    if(std::holds_alternative<token::Select>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::From>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Comma>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Asterisks>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Semicolon>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Identifier>(token.value())){
        return state::NamedColumn{};
    }
};

State transition(state::FromClause, const Token &token){
    if(std::holds_alternative<token::Select>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::From>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Comma>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Asterisks>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Semicolon>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Identifier>(token.value())){
        return state::TableName{};
    }
};

State transition(state::TableName, const Token &token){
    if(std::holds_alternative<token::Select>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::From>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Comma>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Asterisks>(token.value())){
        return state::Invalid{};
    }else if(std::holds_alternative<token::Semicolon>(token.value())){
        return state::Valid{};
    }else if(std::holds_alternative<token::Identifier>(token.value())){
        return state::Invalid{};
    }
};

bool is_valid_sql_query(const std::vector<Token> &tokens){
    auto validate = SqlValidator{};
    for (size_t i = 0; i < tokens.size(); i++)
    {
        validate.handle(tokens.at(i));
    }
    
    return validate.is_valid();
};

} // namespace sql
