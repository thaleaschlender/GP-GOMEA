#ifndef OP0_H
#define OP0_H

#include "GPGOMEA/Operators/Operator.h"

class Op0 : public Operator {
public:

    Op0() {
        arity = 0;
        name = "0";
        type = OperatorType::opFunction;
    }

    Operator * Clone() const override {
        return new Op0(*this);
    }

    arma::vec ComputeOutput(const arma::mat& x) override {
        arma::vec res(x.n_rows);

        for (size_t i = 0; i < x.n_rows; i++)
            res[i] = 0;

        return res;
    }

    std::string GetHumanReadableExpression(const std::vector<std::string>& args) override {
        return name;
    }


private:

};

#endif /* OP0_H */

