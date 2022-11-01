/*
 Function I used to create boston train and test 

 arma::mat Fitness::ReadFitnessCases(std::string filepath) {
    arma::mat X;
    X.load(filepath);
    cout << "FILE SIZE" << X.n_rows;
    arma::ivec num_entries = arma::linspace<arma::ivec>(0,(int)X.n_rows-1,(int)X.n_rows);
    num_entries = arma::shuffle(num_entries);
    int train_size = (int)(2*X.n_rows/3);
    int test_size = X.n_rows - train_size;
    arma::mat training_set(train_size,X.n_cols, arma::fill::zeros);
    arma::mat testing_set(test_size,X.n_cols, arma::fill::zeros);

    for (int i = 0; i <train_size; i++){
        training_set.row(i) = X.row(num_entries(i));
    }

    for (int i =0; i < test_size; i++){
        testing_set.row(i) = X.row(num_entries(i+train_size));
    }
    cout << "__________"<< endl;
    cout << "MY OWN  train: ( " << training_set.n_rows << "x" << training_set.n_cols - 1 << " )" << endl;
    cout << "MY OWN  test: ( " << testing_set.n_rows << "x" << testing_set.n_cols - 1 << " )" << endl;
    training_set.save("Train_Boston.dat", arma_ascii);
    testing_set.save("Test_Boston.dat", arma_ascii);
    return X;
}


 */

/* 
 * File:   Fitness.cpp
 * Author: virgolin
 * 
 * Created on June 27, 2018, 6:14 PM
 */

#include "GPGOMEA/Fitness/Fitness.h"

using namespace std;
using namespace arma;

Fitness::Fitness() {
}

Fitness::Fitness(const Fitness& orig) {
}

Fitness::~Fitness() {
}

double_t Fitness::ComputeFitness(Node* n, bool use_caching) {
    throw NotImplementedException("Fitness::ComputeFitness not implemented");
}

arma::mat Fitness::ReadFitnessCases(std::string filepath) {
    arma::mat X;
    X.load(filepath);
    return X;
}

void Fitness::SetFitnessCases(const arma::mat& X, FitnessCasesType fct) {

    arma::vec Y = X.col(X.n_cols - 1); // target
    arma::mat Xx = X;
    Xx.shed_col(Xx.n_cols - 1);//Xx features

    if (fct == FitnessCasesTRAIN) { // i data from training set
        TrainY = Y; 
        TrainX = Xx;

        trainY_mean = arma::mean(TrainY); // calculate mean target
        trainY_std = arma::stddev(TrainY, 1); // calculate std target

        var_comp_trainY = Y - trainY_mean; // distance from target mean

    } else if (fct == FitnessCasesTEST) {
        TestY = Y;
        TestX = Xx;
    } else if (fct == FitnessCasesVALIDATION) {
        ValidationY = Y;
        ValidationX = Xx;
    } else {
        throw std::runtime_error("Fitness::SetFitnessCases invalid fitness cases type provided.");
    }

}

arma::vec Fitness::GetPopulationFitness(const std::vector<Node*>& population, bool compute, bool use_caching) {

    vec fitnesses(population.size());

    if (compute) {
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < population.size(); i++) {
            fitnesses[i] = ComputeFitness(population[i], use_caching);
        }
    } else {
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < population.size(); i++) {
            fitnesses[i] = population[i]->cached_fitness;
        }
    }

    return fitnesses;
}

Node* Fitness::GetBest(const std::vector<Node*>& population, bool compute, bool use_caching) {

    vec fitnesses = GetPopulationFitness(population, compute, use_caching);
    Node * best = population[ index_min(fitnesses) ];
    return best;

}

double_t Fitness::GetTestFit(Node * n) {
    throw NotImplementedException("Fitness::GetTestFit not implemented.");
}

double_t Fitness::GetValidationFit(Node* n) {
    throw NotImplementedException("Fitness::GetValidationFit not implemented.");
}


