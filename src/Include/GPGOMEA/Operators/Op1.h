#ifndef OP1_H
#define OP1_H

#include "GPGOMEA/Operators/Operator.h"

class Op1 : public Operator {
public:

    Op1() {
        arity = 0;
        name = "1";
        type = OperatorType::opFunction;
    }

    Operator * Clone() const override {
        return new Op1(*this);
    }

    arma::vec ComputeOutput(const arma::mat& x) override {
        arma::vec res(x.n_rows);

        for (size_t i = 0; i < x.n_rows; i++)
            res[i] = 1;

        return res;
    }

    std::string GetHumanReadableExpression(const std::vector<std::string>& args) override {
        return name;
    }


private:

};

#endif /* OP1_H */