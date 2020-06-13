# 编译原理课程设计

C-minus语言的词法分析和语法分析

运行：
g++ -c main.cpp
g++ -c lexer.cpp
g++ -c parser.cpp
g++ main.o lexer.o parser.o -o test
./test


C-minus的EBNF(消除左递归):
* program->declaration_list
* declaration_list -> declaration{declaration}
* declaration -> type_specifier "ID" diff_declaration
* diff_declaration -> [ "[" "NUM" "]"] ; |  ( params )  compound_stmt
* var_declaration -> type_specifier "ID" [ "[" "NUM" "]"] ;
* type_specifier -> "INT"  |  "VOID"
* params -> param_list  |  "VOID"
* param_list -> param {, param}
* param -> "INT"  ID  ["[ ]"]
* compound_stmt -> "{" local_declarations statement_list "}"
* local_declarations ->{var_declaration}
* statement_list ->{statement}
* statement -> expression_stmt  |  compound_stmt  |  selection_stmt  |iteration_stmt  |  return_stmt
* expression_stmt -> {expression ;};
* selection_stmt ->"IF" ( expression ) statement ["ELSE" statement]
* iteration_stmt -> "WHILE" "(" expression ")" statement
* return_stmt -> RETURN  [expression];
* expression -> "ID" left_expression | simple_expression3
* left_expression->[ "[" expression "]" ] left_part | simple_expression2
* left_part-> "=" expression | simple_expression1
* simple_expression1 -> additive_expression1 [relop additive_expression]
* additive_expression1 -> term1 { addop term }
* term1->{mulop factor}
* simple_expression2 -> additive_expression2 [relop additive_expression]
* additive_expression2 -> term2 { addop term }
* term2->"(" args ")"{mulop factor}
* simple_expression3-> additive_expression3 [relop additive_expression]
* additive_expression3 -> term3 { addop term }
* term3->factor3 {mulop factor}
* factor3->"("expression")"| "NUM"
* simple_expression -> additive_expression [relop additive_expression]
* additive_expression -> term { addop term }
* term->factor{mulop factor}
* factor->"ID" factor_part|factor3
* factor_part->[ "[" expression "]" ]|"(" args ")"
* relop -> <=  |  <  |  >  |  >=  |  ==  |  != 
* addop → + | -  
* mulop ->"*" | "/"
* args->[arg_list]
* arg_list-> expression{, expression}


C-minus的EBNF:
* program->declaration_list
* declaration_list -> declaration{declaration}
* declaration -> type_specifier "ID" diff_declaration
* diff_declaration -> [ "[" "NUM" "]"] ; |  ( params )  compound_stmt
* var_declaration -> type_specifier "ID" [ "[" "NUM" "]"] ;
* type_specifier -> "INT"  |  "VOID"
* fun_declaration -> type_specifier "ID" ( params )  compound_stmt
* params -> param_list  |  "VOID"
* param_list -> param {, param}
* param -> "INT"  ID  ["[ ]"]
* compound_stmt -> "{" local_declarations statement_list "}"
* local_declarations ->{var_declaration}
* statement_list ->{statement}
* statement -> expression_stmt  |  compound_stmt  |  selection_stmt  |iteration_stmt  |  return_stmt
* expression_stmt -> {expression ;};
* selection_stmt ->"IF" ( expression ) statement ["ELSE" statement]
* iteration_stmt -> "WHILE" "(" expression ")" statement
* return_stmt -> RETURN  [expression]
* expression -> var "=" expression | simple_expression
* var -> "ID" [ "[" expression "]" ]
* simple_expression -> additive_expression [relop additive_expression]
* relop -> <=  |  <  |  >  |  >=  |  ==  |  != 
* additive_expression -> term { addop term }
* addop → + | -  
* term->factor{mulop factor}
* mulop ->"*" | "/"
* factor->"("expression")"| var | call | "NUM"
* call->"ID""(" args ")"
* args->arg_list | empty
* arg_list-> expression{, expression}

