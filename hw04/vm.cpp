#include "vm.h"

#include <iostream>
#include <limits>

#include "util.h"


namespace vm {


vm_state create_vm(bool debug) {
    vm_state state;

    // enable vm debugging
    state.debug = debug;


    register_instruction(state, "PRINT", [](vm_state& vmstate, const item_t /*arg*/) {
        std::cout << vmstate.stack.top() << std::endl;
        return true;
    });

    register_instruction(state, "LOAD_CONST", [](vm_state& vmstate, const item_t var) {
        
        vmstate.stack.push(var);
        
        return true;
    });

    register_instruction(state, "ADD", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is add");
        if(vmstate.stack.top() < 0)
            throw vm_stackfail("arg is negative");
        item_t popped = vmstate.stack.top();
        vmstate.stack.pop();
        vmstate.stack.top() += popped; 
        return true;
    });

    register_instruction(state, "EXIT", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is exit");
        return false;
    });

    register_instruction(state, "POP", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is pop");
        
        vmstate.stack.pop();
  
        return true;
    });

    register_instruction(state, "DIV", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is div");
        if(vmstate.pc == 1)
            throw vm_stackfail("tos1 is missing");
        
        item_t popped = vmstate.stack.top();
        if(popped == 0){
            throw div_by_zero{"div by zero"};
            return false;
        }
        vmstate.stack.pop();
        vmstate.stack.top() /= popped; 
        
        return true;
    });

    register_instruction(state, "EQ", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is eq");
        if(vmstate.pc == 1)
            throw vm_stackfail("tos1 is missing");

        item_t TOS = vmstate.stack.top();
        vmstate.stack.pop();
        item_t TOS1 = vmstate.stack.top();
        if(TOS == TOS1){
            vmstate.stack.top() = 1;
            std::cout << vmstate.stack.top();
        }else{
            vmstate.stack.top() = 0;
        }
         
        return true;
    });

    register_instruction(state, "NEQ", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is neq");
        if(vmstate.pc == 1)
            throw vm_stackfail("tos1 is missing");
        
        item_t TOS = vmstate.stack.top();
        vmstate.stack.pop();
        item_t TOS1 = vmstate.stack.top();
        if(TOS != TOS1){
            vmstate.stack.top() = 1;
        }else{
            vmstate.stack.top() = 0;
        }
         
        return true;
    });

    register_instruction(state, "DUP", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is dup");
        vmstate.stack.push(vmstate.stack.top());
        return true;
    });

    register_instruction(state, "JMP", [](vm_state& vmstate, const item_t var) {
        vmstate.pc = var;
        return true;
    });

    register_instruction(state, "JMPZ", [](vm_state& vmstate, const item_t var) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is jmpz");
        
        if(vmstate.stack.top() == 0){
            vmstate.stack.pop();
            vmstate.pc = var;
            
        }else{
            
            vmstate.stack.pop();
            ++vmstate.pc;
        }
        return true;
    });

    register_instruction(state, "WRITE_CHAR", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is WRITE_CHAR");
        
        return true;
    });

    register_instruction(state, "WRITE", [](vm_state& vmstate, const item_t) {
        if(vmstate.pc == 0)
            throw vm_stackfail("only op is WRITE");

        return true;
    });
    // TODO create instructions

    return state;
}


void register_instruction(vm_state& state, std::string_view name,
                          const op_action_t& action) {
    size_t op_id = state.next_op_id;
    state.instruction_names[op_id] = name.data();
    state.instruction_ids[name.data()] = op_id;
    state.instruction_actions[op_id] = action;
    ++state.next_op_id;
    // TODO make instruction available to vm
}


code_t assemble(const vm_state& state, std::string_view input_program) {
    code_t code;

    // convert each line separately
    for (auto& line : util::split(input_program, '\n')) {

        auto line_words = util::split(line, ' ');

        // only support instruction and one argument
        if (line_words.size() >= 3) {
            throw invalid_instruction{std::string{"more than one instruction argument: "} + line};
        }

        // look up instruction id
        auto& op_name = line_words[0];
        auto find_op_id = state.instruction_ids.find(op_name);
        if (find_op_id == std::end(state.instruction_ids)) {
            throw invalid_instruction{std::string{"unknown instruction: "} + op_name};
        }
        op_id_t op_id = find_op_id->second;

        // parse the argument
        item_t argument{0};
        if (line_words.size() == 2) {
            argument = std::stoll(line_words[1]);
        }

        // and save the instruction to the code store
        code.emplace_back(op_id, argument);
    }

    return code;
}


std::tuple<item_t, std::string> run(vm_state& vm, const code_t& code) {
    // to help you debugging the code!
    if (vm.debug) {
        std::cout << "=== running vm ======================" << std::endl;
        std::cout << "disassembly of run code:" << std::endl;
        for (const auto &[op_id, arg] : code) {
            if (not vm.instruction_names.contains(op_id)) {
                std::cout << "could not disassemble - op_id unknown..." << std::endl;
                std::cout << "turning off debug mode." << std::endl;
                vm.debug = false;
                break;
            }
            std::cout << vm.instruction_names[op_id] << " " << arg << std::endl;
        }
        std::cout << "=== end of disassembly" << std::endl << std::endl;
    }
    std::string result = "";
    // execution loop for the machine
    while (true) {
        
        auto& [op_id, arg] = code[vm.pc];
        
        if (vm.debug) {
            std::cout << "-- exec " << vm.instruction_names[op_id] << " arg=" << arg << " at pc=" << vm.pc << std::endl;
        }
        bool isRun = vm.instruction_actions[op_id](vm,arg);

        if(!isRun){
            break;
            
        }else if(vm.instruction_names[op_id] == "JMP"){
            if(code.size() - 1 < arg){
                throw vm_segfault{"segfault"};
            }else{
                continue;
            }
        }else if(vm.instruction_names[op_id] == "JMPZ"){
            if(code.size() - 1 < arg){
                throw vm_segfault{"segfault"};
            }else{
                continue;
            }
        }else if(vm.instruction_names[op_id] == "WRITE"){
            result += std::to_string(vm.stack.top());
            
            ++vm.pc;
        }else if(vm.instruction_names[op_id] == "WRITE_CHAR"){
            char ascii = vm.stack.top();
            result += ascii;
            ++vm.pc;
        }else{
            ++vm.pc;
        }
        
        
        // increase the program counter here so its value can be overwritten
        // by the instruction when it executes!
        // TODO execute instruction and stop if the action returns false.
    }
    
    return {vm.stack.top(), result}; // TODO: return tuple(exit value, output text)
}


} // namespace vm
