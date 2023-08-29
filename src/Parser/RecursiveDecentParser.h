#pragma once

#include "Parser.h"

namespace Jack {
ParserResult parseClass(ParseTree&, std::vector<Token>::const_iterator&,
                        std::vector<Token>::const_iterator);
ParserResult parseClassVarDec(ParseTree&, std::vector<Token>::const_iterator&,
                              std::vector<Token>::const_iterator);
ParserResult parseSubroutine(ParseTree&, std::vector<Token>::const_iterator&,
                             std::vector<Token>::const_iterator);
ParserResult parseParameterList(ParseTree&, std::vector<Token>::const_iterator&,
                                std::vector<Token>::const_iterator);
ParserResult parseSubroutineBody(ParseTree&,
                                 std::vector<Token>::const_iterator&,
                                 std::vector<Token>::const_iterator);
ParserResult parseVarDec(ParseTree&, std::vector<Token>::const_iterator&,
                         std::vector<Token>::const_iterator);
ParserResult parseStatements(ParseTree&, std::vector<Token>::const_iterator&,
                             std::vector<Token>::const_iterator);
ParserResult parseLetStatement(ParseTree&, std::vector<Token>::const_iterator&,
                               std::vector<Token>::const_iterator);
ParserResult parseDoStatement(ParseTree&, std::vector<Token>::const_iterator&,
                              std::vector<Token>::const_iterator);
ParserResult parseIfStatement(ParseTree&, std::vector<Token>::const_iterator&,
                              std::vector<Token>::const_iterator);
ParserResult parseWhileStatement(ParseTree&,
                                 std::vector<Token>::const_iterator&,
                                 std::vector<Token>::const_iterator);
ParserResult parseReturnStatement(ParseTree&,
                                  std::vector<Token>::const_iterator&,
                                  std::vector<Token>::const_iterator);
ParserResult parseExpression(ParseTree&, std::vector<Token>::const_iterator&,
                             std::vector<Token>::const_iterator);
ParserResult parseTerm(ParseTree&, std::vector<Token>::const_iterator&,
                       std::vector<Token>::const_iterator);
ParserResult parseExpressionList(ParseTree&,
                                 std::vector<Token>::const_iterator&,
                                 std::vector<Token>::const_iterator);
}  // namespace Jack
