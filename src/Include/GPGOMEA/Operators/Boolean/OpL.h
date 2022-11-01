/*
 


 */


#ifndef OPL_H
#define OPL_H

#include "GPGOMEA/Operators/Operator.h"

class OpL : public Operator {
public:

    OpL() {
        arity = 2;
        name = "<";
        type = OperatorType::opFunction;
    }

    Operator * Clone() const override {
        return new OpL(*this);
    }

    arma::vec ComputeOutput(const arma::mat& x) override {
        arma::vec res(x.n_rows);

        for (size_t i = 0; i < x.n_rows; i++){
            if (x.at(i, 0) <  x.at(i, 1)){
                res[i] = 1;
            }
            else{
                res[i] = 0;
            }
        }

        return res;
    }

    std::string GetHumanReadableExpression(const std::vector<std::string>& args) override {
        return "(" + args[0] + name + args[1] + ")";
    }


private:

};

#endif /* OPL_H */

