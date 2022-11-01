
/* 
* 19.10.2022: Simple adaptation
* Make if satements usable by either constartining crossover to only make legal chnages or by having a scheme to map real values to boolean values
*currently only if the value happens to be 0 the boolean will be false, else it will be true
 */

#ifndef OPIF_H
#define OPIF_H

#include "GPGOMEA/Operators/Operator.h"

class OpIf : public Operator {
public:

    OpIf() {
        arity = 3;
        name = "IF";
        type = OperatorType::opFunction;
    }

    Operator * Clone() const override {
        return new OpIf(*this);
    }

    arma::vec ComputeOutput(const arma::mat& x) override {
        arma::vec res(x.n_rows); // make a vector of size of the x rows (the number of training examples)
        //since our arity is three we should assume three columns of x coming in 
        for (size_t i = 0; i < x.n_rows; i++)
            if (x.at(i,0) > 0){  //TEMPORARY: ALL NEGATIVE VALUES ARE FALSE
                res[i] = x.at(i, 1);
            }
            else{
                res[i] = x.at(i, 2);
            }
        return res;
    }

    std::string GetHumanReadableExpression(const std::vector<std::string>& args) override {
        return "(If" + args[0] + "Then" + args[1] + "Else" + args[2] + ")";
    }


private:

};

#endif /* OPIF_H */

