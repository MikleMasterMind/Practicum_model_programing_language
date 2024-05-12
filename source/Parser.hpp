#ifndef _PARSER_H_
#define _PARSER_H_

#include "Lex.hpp"
#include "Scanner.hpp"
#include "Poliz.hpp"
#include "Stack.hpp"
#include "Table_id.hpp"
#include <string>
#include <map>
#include <stack>

namespace translator 
{

    class Parser 
    {
        Lex cur_lex; // текущая лексема
        Scanner scan;
        Stack<int> st_int;
        Stack<std::string> st_str;
        std::string record_name;
        std::map<std::string, std::vector<std::pair<std::string, type_of_lex>>> record_table;
        Stack<type_of_lex> st_lex;
        Table_id TID;
        bool record_flag;
        // процедуры РС-метода
        void Prog(); 
        void BeforeRec();
        void Rec(std::vector<std::pair<std::string, type_of_lex>>& fields);
        void BeforeDecl();
        void Decl();
        void Begin();
        void Master();
        void Equation();
        void Operand1Rang();
        void Operand2Rang();
        void Logic();
        // семантичиеские действия
        void declare(type_of_lex type);
        void declare_record(std::string record);
        void check_id();
        void check_op();
        void check_not();
        void eq_type();
        void eq_bool();
        void check_id_in_read();
        void record_assign(std::string dest, std::string src);
        // auxiliary functions
        std::string get_prefix(std::string str);
        std::string get_postfix(std::string str);
        std::vector<Identificator> find_fields(std::string prefix);
        bool is_record(std::string var);
        void get_lex(); // получить очередную лексему
        
    public:
        Poliz prog; // внутреннее представление программы
        Parser(std::string program);
        void analyze(); // анализатор с действиями
        Table_id& get_tid();
    };
}

#endif // ! _PARSER_H_