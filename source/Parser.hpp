#ifndef _PARSER_H_
#define _PARSER_H_

#include "Lex.hpp"
#include "Scanner.hpp"
#include "Poliz.hpp"
#include "Stack.hpp"
#include "Table_id.hpp"

namespace translator 
{

    class Parser 
    {
        Lex cur_lex; // текущая лексема
        Scanner scan;
        Stack<int> st_int;
        Stack<type_of_lex> st_lex;
        Table_id TID;
        // процедуры РС-метода
        void Prog(); 
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
        void check_id();
        void check_op();
        void check_not();
        void eq_type();
        void eq_bool();
        void check_id_in_read();
        //
        void get_lex(); // получить очередную лексему
        
    public:
        Poliz prog; // внутреннее представление программы
        Parser(const char *program);
        void analyze(); // анализатор с действиями
        Table_id& get_tid();
    };
}

#endif // ! _PARSER_H_