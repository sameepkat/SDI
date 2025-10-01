#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H
#include "parser.h"

std::unique_ptr<Expr> differentiate(const Expr* e, const std::string& var);

#endif
