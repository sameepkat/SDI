#include "differentiator.h"
#include <stdexcept>

std::unique_ptr<Expr> differentiate(const Expr* e, const std::string& var){
    if(dynamic_cast<const NumberExpr*>(e)){
        return std::make_unique<NumberExpr>(0);
    }

    if(auto v=dynamic_cast<const VariableExpr*>(e)){
        return std::make_unique<NumberExpr>(v->name == var ? 1 : 0);
    }

    if(auto bin=dynamic_cast<const BinaryExpr*>(e)){
        char op = bin->op;
        if(op == '+'){
            return std::make_unique<BinaryExpr>(
                '+',
                differentiate(bin->left.get(), var),
                differentiate(bin->right.get(), var)
            );
        }
        if(op == '-'){
            return std::make_unique<BinaryExpr>(
                '-',
                differentiate(bin->left.get(), var),
                differentiate(bin->right.get(), var)
            );
        }
        if(op == '*'){
            // (u * v)' = u'v + uv'
            return std::make_unique<BinaryExpr>(
                '+',
                std::make_unique<BinaryExpr>(
                    '*',
                    differentiate(bin->left.get(), var),
                    bin->right->clone()
                ),
                std::make_unique<BinaryExpr>(
                    '*',
                    bin->left->clone(),
                    differentiate(bin->right.get(), var)
                )
            );
        }
        if(op == '/'){
            // (u / v)' = (u'v - uv') / v^2
            return std::make_unique<BinaryExpr>(
                '/',
                std::make_unique<BinaryExpr>(
                    '-',
                    std::make_unique<BinaryExpr>(
                        '*',
                        differentiate(bin->left.get(), var),
                        bin->right->clone()
                    ),
                    std::make_unique<BinaryExpr>(
                        '*',
                        bin->left->clone(),
                        differentiate(bin->right.get(), var)
                    )
                ),
                std::make_unique<BinaryExpr>(
                    '^',
                    bin->right->clone(),
                    std::make_unique<NumberExpr>(2)
                )
            );
        }
        if(op == '^'){
            if(auto exponent = dynamic_cast<NumberExpr*>(bin->right.get())){
                double n = exponent->value;
                return std::make_unique<BinaryExpr>(
                    '*',
                    std::make_unique<BinaryExpr>(
                        '*',
                        std::make_unique<NumberExpr>(n),
                        std::make_unique<BinaryExpr>(
                            '^',
                            bin->left->clone(),
                            std::make_unique<NumberExpr>(n-1)
                        )
                    ),
                    differentiate(bin->left.get(), var)
                );
            }
            throw std::runtime_error("Non-numeric exponents not supported yet");
        }
    }
    throw std::runtime_error("Unknown expression type in differentiate");
}
