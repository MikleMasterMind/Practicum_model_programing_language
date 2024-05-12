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
    record_flag = true;
    while (cur_lex.get_type() == LEX_RECORD)
        BeforeRec();
    record_flag = false;
    BeforeDecl();
    if (cur_lex.get_type() == LEX_SEMICOLON)
        get_lex ();
    else
        throw cur_lex;
    #ifdef DEBUG
    for (int i = 0; i < TID.size(); ++i)
        std::cout << TID[i].get_name() << ' ' << TID[i].get_type() << std::endl;
    #endif
    Begin();
    if (cur_lex.get_type() != LEX_FIN)
        throw cur_lex;
}

void translator::Parser::BeforeRec()
{
    get_lex();
    if (cur_lex.get_type() != LEX_ID)
        throw cur_lex;
    std::string this_record_name = record_name;
    get_lex();
    if (cur_lex.get_type() != LEX_BEGIN) 
        throw cur_lex;
    std::vector<std::pair<std::string, type_of_lex>> fields;
    get_lex();
    Rec(fields);
    while (cur_lex.get_type() == LEX_COMMA) {
        get_lex();
        Rec(fields);
    }
    if (cur_lex.get_type() != LEX_END)
        throw cur_lex;
    record_table[this_record_name] = fields;
    get_lex();
    if (cur_lex.get_type() != LEX_SEMICOLON)
        throw cur_lex;
    get_lex();
}

void translator::Parser::Rec(std::vector<std::pair<std::string, translator::type_of_lex>> &fields)
{
    if (cur_lex.get_type() != LEX_ID)
        throw cur_lex;
    std::vector<std::pair<std::string, type_of_lex>> buf_fields;
    buf_fields.push_back(std::make_pair(record_name, LEX_NULL));
    get_lex();
    while (cur_lex.get_type() == LEX_COMMA) {
        get_lex();
        if (cur_lex.get_type() != LEX_ID)
            throw cur_lex;
        buf_fields.push_back(std::make_pair(record_name, LEX_NULL));
        get_lex();
    }
    if (cur_lex.get_type() != LEX_COLON)
        throw cur_lex;
    get_lex();
    if (cur_lex.get_type() == LEX_INT || cur_lex.get_type() == LEX_BOOL) {
        for (int i = 0; i < buf_fields.size(); ++i)
                buf_fields[i].second = cur_lex.get_type();
    }
    else if (record_table.count(record_name)) {
        std::vector<std::pair<std::string, type_of_lex>> new_buf_fields;
        for (int i = 0; i < buf_fields.size(); ++i) {
            std::string prefix = buf_fields[i].first;
            for (auto a : record_table[record_name]) {
                std::string postfix = a.first;
                std::string field = prefix + '.' + postfix;
                new_buf_fields.push_back(std::make_pair(field, a.second));
            }
        }
        buf_fields = new_buf_fields;
    }
    else {
        throw cur_lex;
    }
    for (auto a : buf_fields)
        fields.push_back(a);
    get_lex();
}

void translator::Parser::BeforeDecl()
{
    if (cur_lex.get_type() == LEX_VAR) {
        get_lex();
        Decl();
        #ifdef DEBUG
            std::cout << "here1" << std::endl;
        #endif
        while (cur_lex.get_type() == LEX_COMMA) { // ","
            get_lex();
            Decl();
            #ifdef DEBUG
                std::cout << "here" << std::endl;
            #endif
        }
    }
    else {
        throw cur_lex;
    }
}

void translator::Parser::Decl()
{
    st_int.reset();
    st_str.reset();
    if (cur_lex.get_type() != LEX_ID)
        throw cur_lex;
    st_int.push(cur_lex.get_id());
    st_str.push(record_name);
    get_lex();
    while (cur_lex.get_type() == LEX_COMMA) { // ","
        get_lex();
        if (cur_lex.get_type() != LEX_ID)
            throw cur_lex;
        st_int.push(cur_lex.get_id()); 
        st_str.push(record_name);
        get_lex();
    }
    if (cur_lex.get_type() != LEX_COLON) // ":"
        throw cur_lex;
    get_lex();
    if (cur_lex.get_type() == LEX_INT || cur_lex.get_type() == LEX_BOOL) {
        #ifdef DEBUG
            std::cout << cur_lex << std::endl;
        #endif
        declare(cur_lex.get_type());
        get_lex();
    }
    else if (record_table.count(record_name)) {
        #ifdef DEBUG
            std::cout << record_name << std::endl;
        #endif
        declare_record(record_name);
        get_lex();
    }
    else {
        throw cur_lex;
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
            if (cur_lex.get_type() != LEX_SEMICOLON)
                throw cur_lex;
            get_lex();
            pl3 = prog.get_carrier();
            prog.blank();
            prog.push_lex(Lex(POLIZ_GO));
            prog.push_lex(Lex(POLIZ_LABEL, prog.get_carrier()), pl2);
            if (cur_lex.get_type() == LEX_ELSE) {
                get_lex();
                Master();
                prog.push_lex(Lex(POLIZ_LABEL, prog.get_carrier()), pl3);
            }
            else {
                throw cur_lex;
            }
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
        if (is_record(record_name)) { // specal assigment for record
            std::string dest_prefix = record_name;
            get_lex();
            if (cur_lex.get_type() != LEX_ASSIGN) 
                throw cur_lex;
            get_lex();
            if (cur_lex.get_type() != LEX_ID || !is_record(record_name))
                throw cur_lex;
            std::string src_prefix = record_name;
            record_assign(dest_prefix, src_prefix);
            get_lex();
        } else { // usual var assigment
            check_id();
            prog.push_lex(Lex(POLIZ_ADDRESS, cur_lex.get_id()));
            get_lex();
            if (cur_lex.get_type() != LEX_ASSIGN)
                throw cur_lex;
            get_lex();
            Equation();
            eq_type();
            prog.push_lex(Lex(LEX_ASSIGN));
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
        case(LEX_MINUS):
            st_lex.push(LEX_UNARY_MINUS);
            get_lex();
            Logic();
            check_op();
            prog.push_lex(Lex(LEX_UNARY_MINUS));
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

void translator::Parser::declare_record(std::string record)
{
    while (!st_str.empty()) {
        std::string prefix = st_str.pop();
        #ifdef DEBUG
            std::cout << "declare_record " << prefix << std::endl;
        #endif
        int index = TID.push(prefix);
        TID[index].set_declare();
        TID[index].set_type(LEX_RECORD);
        for (auto a : record_table[record]) {
            std::string postfix = a.first;
            std::string var = prefix + '.' + postfix;
            int index = TID.push(var);
            TID[index].set_type(a.second);
            TID[index].set_declare();
            
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
    if (op == LEX_UNARY_MINUS) {
        if (t2 == LEX_INT || t2 == LEX_ID) 
            st_lex.push(t2);
        else 
            throw "wrong_types_in_operation";
    } else {
        t1 = st_lex.pop();
        type_of_lex t = (op == LEX_OR || op == LEX_AND) ? LEX_BOOL : LEX_INT;
        if (t1 == t2 && t1 == t)
            st_lex.push(t);
        else
            throw "wrong_types_in_operation";
    }
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

void translator::Parser::record_assign(std::string dest, std::string src)
{
    
    std::vector<Identificator> dest_list = find_fields(dest);
    std::vector<Identificator> src_list = find_fields(src);
    if (dest_list.size() != src_list.size())
        throw "assign different reports";
    for (int i = 0; i < dest_list.size(); ++i) {
        if (get_postfix(dest_list[i].get_name()) != get_postfix(src_list[i].get_name()) || dest_list[i].get_type() != src_list[i].get_type())
            throw "assign different reports";
        int dest_id = TID.push(dest_list[i].get_name());
        int src_id = TID.push(src_list[i].get_name());
        prog.push_lex(Lex(POLIZ_ADDRESS, dest_id));
        prog.push_lex(Lex(LEX_ID, src_id));
        prog.push_lex(Lex(LEX_ASSIGN));
    }
}

std::string translator::Parser::get_prefix(std::string str)
{
    std::string result;
    for (auto c : str) {
        if (c == '.')
            break;
        result += c;
    }
    return result;
}

std::string translator::Parser::get_postfix(std::string str)
{
    std::string result;
    int i;
    for (i = 0; i < str.length(); ++i)
        if (str[i] == '.') {
            ++i;
            break;
        }
    for (; i < str.length(); ++i)
        result += str[i];
    return result;
}

std::vector<translator::Identificator> translator::Parser::find_fields(std::string prefix)
{
    std::vector<translator::Identificator> result;
    for (int i = 0; i < TID.size(); ++i)
        if (get_prefix(TID[i].get_name()) == prefix && TID[i].get_name() != prefix)
            result.push_back(TID[i]);
    return result;
}

bool translator::Parser::is_record(std::string var)
{
//     if (var.find('.') != std::string::npos)
//         return false;
    std::vector<translator::Identificator> buf = find_fields(var);
    #ifdef DEBUG
    std::cout << var << std::endl;
    for (auto a : buf)
        std::cout << a.get_name() << std::endl;
    #endif
    for (auto a : buf)
        if (a.get_name().find('.') != std::string::npos)
            return true;
    return false;
}

void translator::Parser::get_lex()
{
    std::string buf = record_name;
    cur_lex = scan.get_lex(record_name);
    if (cur_lex.get_type() == LEX_ID && !record_flag && !record_table.count(record_name))
        cur_lex = Lex(LEX_ID, TID.push(record_name));
    else if (buf == record_name)
        record_name = "";
}

translator::Parser::Parser(std::string program) : scan(program)
{
}

void translator::Parser::analyze()
{
    get_lex();
    Prog();
    //prog.print();
}

translator::Table_id &translator::Parser::get_tid()
{
    return TID;
}
