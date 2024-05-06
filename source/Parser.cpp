#include "Parser.hpp"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

void translator::Parser::Prog()
{
    if (cur_lex.get_type() == LEX_PROGRAM)
        get_lex();
    else
        throw cur_lex;
    BeforeDecl();
    if (cur_lex.get_type() == LEX_SEMICOLON)
        get_lex ();
    else
        throw cur_lex;
    Begin();
    if (cur_lex.get_type() != LEX_FIN)
        throw cur_lex;
}

void translator::Parser::BeforeDecl()
{
    if (cur_lex.get_type() == LEX_VAR) {
        get_lex();
        Decl();
        while (cur_lex.get_type() == LEX_COMMA) { // ","
            get_lex();
            Decl();
        }
    }
    else {
        throw cur_lex;
    }
}

void translator::Parser::Decl()
{
    st_int.reset();
    if (cur_lex.get_type() != LEX_ID) {
        throw cur_lex;
    }  
    else {
        st_int.push(cur_lex.get_id());
        get_lex();
        while (cur_lex.get_type() == LEX_COMMA) { // ","
            get_lex();
            if (cur_lex.get_type() != LEX_ID) {
                throw cur_lex;
            }
            else {
                st_int.push(cur_lex.get_id()); 
                get_lex();
            }
        }
        if (cur_lex.get_type() != LEX_COLON)  { // ":"
            throw cur_lex;
        }
        else {
            get_lex();
            if (cur_lex.get_type() != LEX_INT && cur_lex.get_type() != LEX_BOOL && cur_lex.get_type() != LEX_BOOL)
                throw cur_lex;
            declare(cur_lex.get_type());
            get_lex();
        }
    }
}

void translator::Parser::Begin()
{
    if (cur_lex.get_type() == LEX_BEGIN) {
        get_lex();
        Master();
        while (cur_lex.get_type() == LEX_SEMICOLON) {
            get_lex();
            Master();
        }
        if (cur_lex.get_type() == LEX_END)
            get_lex();
        else 
            throw cur_lex;
    }
    else {
        throw cur_lex;
    }
}

void translator::Parser::Master()
{
    int pl0, pl1, pl2, pl3;

    if (cur_lex.get_type() == LEX_IF) {
        get_lex();
        Equation();
        eq_bool();
        pl2 = prog.get_carrier();
        prog.blank();
        prog.push_lex(Lex(POLIZ_FGO));
        if (cur_lex.get_type() == LEX_THEN) {
            get_lex();
            Master();
            pl3 = prog.get_carrier();
            prog.blank();
            prog.push_lex(Lex(POLIZ_GO));
            prog.push_lex(Lex(POLIZ_LABEL, prog.get_carrier()), pl2);
            if (cur_lex.get_type() == LEX_ELSE) {
                get_lex();
                Master();
                prog.push_lex(Lex(POLIZ_LABEL, prog.get_carrier()), pl3);
            }
            #ifdef DEBUG
            std::cout << "here" << std::endl;
            #endif
        }
        else {
            throw cur_lex;
        }
    } // end LEX_IF
    else if (cur_lex.get_type() == LEX_WHILE) {
        pl0 = prog.get_carrier();
        get_lex();
        Equation();
        eq_bool();
        pl1 = prog.get_carrier();
        prog.blank();
        prog.push_lex(Lex(POLIZ_FGO));
        if (cur_lex.get_type() == LEX_DO) {
            get_lex();
            Master();
            prog.push_lex(Lex(POLIZ_LABEL, pl0));
            prog.push_lex(Lex(POLIZ_GO));
            prog.push_lex(Lex(POLIZ_LABEL, prog.get_carrier()), pl1);
        }
        else {
            throw cur_lex;
        }
    } // end LEX_WHILE
    else if (cur_lex.get_type() == LEX_READ) {
        get_lex();
        if (cur_lex.get_type() == LEX_LPAREN) {
            get_lex();
            if (cur_lex.get_type() == LEX_ID) {
                check_id_in_read();
                prog.push_lex(Lex(POLIZ_ADDRESS, cur_lex.get_id()));
                get_lex();
            }
            else {
                throw cur_lex;
            }
            if (cur_lex.get_type() == LEX_RPAREN) {
                get_lex();
                prog.push_lex(Lex(LEX_READ));
            }
            else {
                throw cur_lex;
            }
        }
        else {
            throw cur_lex;
        }
    } // end LEX_READ
    else if (cur_lex.get_type() == LEX_WRITE) {
        get_lex();
        if (cur_lex.get_type() == LEX_LPAREN) {
            get_lex();
            Equation();
            if (cur_lex.get_type() == LEX_RPAREN) {
                get_lex();
                prog.push_lex(Lex(LEX_WRITE));
            }
            else {
                throw cur_lex;
            }
        }
        else {
            throw cur_lex;
        }
    } // end LEX_WRITE
    else if (cur_lex.get_type() == LEX_ID) {
        check_id();
        prog.push_lex(Lex(POLIZ_ADDRESS, cur_lex.get_id()));
        get_lex();
        if (cur_lex.get_type() == LEX_ASSIGN) {
            get_lex();
            Equation();
            eq_type();
            prog.push_lex(Lex(LEX_ASSIGN));
        }
        else {
            throw cur_lex;
        }
    } // end LEX_ID
    else if (cur_lex.get_type() == LEX_END) {
        return;
    }
    else {
        Begin();
    }
}

void translator::Parser::Equation()
{
    Operand1Rang();
    if (cur_lex.get_type() == LEX_EQ || 
        cur_lex.get_type() == LEX_LSS ||
        cur_lex.get_type() == LEX_GTR ||
        cur_lex.get_type() == LEX_LEQ ||
        cur_lex.get_type() == LEX_GEQ ||
        cur_lex.get_type() == LEX_NEQ) 
    {
        st_lex.push(cur_lex.get_type());
        type_of_lex relation = cur_lex.get_type();
        get_lex();
        Operand1Rang();
        check_op();
        st_lex.pop();
        st_lex.push(LEX_BOOL);
        prog.push_lex(relation);
    }
}

void translator::Parser::Operand1Rang()
{
    Operand2Rang();
    while (cur_lex.get_type() == LEX_PLUS || cur_lex.get_type() == LEX_MINUS || cur_lex.get_type() == LEX_OR) {
        type_of_lex operation = cur_lex.get_type();
        st_lex.push(cur_lex.get_type());
        get_lex();
        Operand2Rang();
        check_op();
        prog.push_lex(Lex(operation));
    }
}

void translator::Parser::Operand2Rang()
{
    Logic();
    while (cur_lex.get_type() == LEX_TIMES || cur_lex.get_type() == LEX_SLASH || cur_lex.get_type() == LEX_AND) {
        type_of_lex operation = cur_lex.get_type();
        st_lex.push(cur_lex.get_type());
        get_lex();
        Logic();
        check_op();
        prog.push_lex(Lex(operation));
    }
}

void translator::Parser::Logic()
{
    switch(cur_lex.get_type()) {
        case(LEX_ID):
            check_id();
            prog.push_lex(Lex(LEX_ID, cur_lex.get_id()));
            get_lex();
            break;
        case(LEX_NUM):
            st_lex.push(LEX_INT);
            prog.push_lex(cur_lex);
            get_lex();
            break;
        case(LEX_TRUE):
            st_lex.push(LEX_BOOL);
            prog.push_lex(Lex(LEX_TRUE, 1));
            get_lex();
            break;
        case(LEX_FALSE):
            st_lex.push(LEX_BOOL);
            prog.push_lex(Lex(LEX_FALSE, 0));
            get_lex();
            break;
        case(LEX_NOT):
            get_lex();
            Logic();
            check_not();
        case(LEX_LPAREN):
            get_lex();
            Equation();
            if (cur_lex.get_type() == LEX_RPAREN)
                get_lex();
            else    
                throw cur_lex;
            break;
        default:
            throw cur_lex;
    } // end switch
}

void translator::Parser::declare(translator::type_of_lex type)
{

    while (!st_int.empty()) {
        int i = st_int.pop();
        if (TID[i].get_declare()) {
            throw "twice";
        }
        else {
            TID[i].set_declare();
            TID[i].set_type(type);
        }      
    }
}

void translator::Parser::check_id()
{
    if (TID[cur_lex.get_id()].get_declare())
        st_lex.push(TID[cur_lex.get_id()].get_type());
    else
        throw "not_declared";
}

void translator::Parser::check_op()
{
    type_of_lex t1, t2, op;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    type_of_lex t = (op == LEX_OR || op == LEX_AND) ? LEX_BOOL : LEX_INT;
    if (t1 == t2 && t1 == t)
        st_lex.push(t);
    else
        throw "wrong_types_in_operation";
}

void translator::Parser::check_not()
{
    if (st_lex.pop() != LEX_BOOL)
        throw "wrong_type_in_not";
    else
        st_lex.push(LEX_BOOL);
}

void translator::Parser::eq_type()
{
    if (st_lex.pop() != st_lex.pop())
        throw "wrong_type_in_:=";
}

void translator::Parser::eq_bool()
{
    if(st_lex.pop() != LEX_BOOL)
        throw "expression_is_not_bool";
}

void translator::Parser::check_id_in_read()
{
    if (!TID[cur_lex.get_id()].get_declare())
        throw "not_declared";
}

void translator::Parser::get_lex() 
{
    cur_lex = scan.get_lex(TID);
}

translator::Parser::Parser(const char *program) : scan(program)
{
}

void translator::Parser::analyze()
{
    get_lex();
    Prog();
    prog.print();
}

translator::Table_id &translator::Parser::get_tid()
{
    return TID;
}
