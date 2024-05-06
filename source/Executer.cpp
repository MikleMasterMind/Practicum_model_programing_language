#include "Executer.hpp"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

void translator::Executer::execute(translator::Poliz &prog, translator::Table_id &TID)
{
    Stack<int> args;
    int operand;
    for (int i = 0; i < prog.get_carrier(); ++i) {
        cur = prog[i];
        switch(cur.get_type()) {
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_NUM:
            case POLIZ_ADDRESS:
            case POLIZ_LABEL:
                args.push(cur.get_id());
                break;
            case LEX_ID:
                operand = cur.get_id();
                if (TID[operand].get_assign())
                    args.push(*(int*)TID[operand].get_value());
                else
                    throw "Poliz_bad_indefinite";
                break;
            case LEX_NOT:
                args.push(!args.pop());
                break;
            case LEX_OR:
                operand = args.pop();
                args.push(args.pop() || operand);
                break;
            case LEX_AND:
                operand = args.pop();
                args.push(args.pop() && operand);
                break;
            case POLIZ_GO:
                i = args.pop() - 1;
                break;
            case POLIZ_FGO:
                operand = args.pop();
                if (!args.pop()) 
                    i = operand - 1;
                break;
            case LEX_WRITE:
                std::cout << args.pop() << std::endl;
                break;
            case LEX_READ:
                int k;
                operand = args.pop();
                if (TID[operand].get_type() == LEX_INT) {
                    std::cin >> k;
                }
                else {
                    std::string input;
                    while(true) {
                        std::cin >> input;
                        if (input == "true") {
                            k = 1;
                            break;
                        }
                        else if (input == "false") {
                            k = 0;
                            break;
                        }
                        else {
                            std::cout << "Error input: true/false, try again" << std::endl;
                        }
                    }
                }
                TID[operand].set_value(&k);
                TID[operand].set_assign();
                break;
            case LEX_PLUS:
                args.push(args.pop() + args.pop());
                break;
            case LEX_TIMES:
                args.push(args.pop() * args.pop());
                break;
            case LEX_MINUS:
                operand = args.pop();
                args.push(args.pop() - operand);
                break;
            case LEX_SLASH:
                operand = args.pop();
                if (operand) {
                    args.push(args.pop() / operand);
                    break;
                }
                else {
                    throw "Poliz_div_by_zero";
                }
            case LEX_EQ:
                args.push(args.pop() == args.pop());
                break;
            case LEX_LSS:
                args.push(args.pop() > args.pop());
                break;
            case LEX_GTR:
                args.push(args.pop() < args.pop());
                break;
            case LEX_LEQ:
                args.push(args.pop() >= args.pop());
                break;
            case LEX_GEQ:
                args.push(args.pop() <= args.pop());
                break;
            case LEX_NEQ:
                args.push(args.pop() != args.pop());
                break;
            case LEX_ASSIGN: 
            {
                operand = args.pop();
                int addres = args.pop();
                TID[addres].set_value(&operand);
                TID[addres].set_assign();
                break;
            }
            default:
                throw "Poliz_unexpected_element";
        } // end switch
    } // end while
}