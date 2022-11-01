/* 
 * File:   PopulationInitializer.cpp
 * Author: virgolin
 * 
 * Created on June 28, 2018, 11:32 AM
 */

#include "GPGOMEA/Evolution/PopulationInitializer.h"

using namespace arma;
using namespace std;

std::vector<Node*> PopulationInitializer::InitializeTreePopulation(const ConfigurationOptions & conf, const TreeInitializer & tree_initializer, const Fitness & fitness) {

    std::vector<Node *> pop;
    pop.reserve(conf.population_size);

    size_t init_tries = 0;

    TreeInitShape tis = TreeInitShape::TreeInitFULL;
    for (size_t i = 0; i < conf.population_size; i++) {

        if (i > conf.population_size / 2)
            tis = TreeInitShape::TreeInitGROW;

        Node * n = tree_initializer.InitializeRandomTree(tis, conf.initial_maximum_tree_height, conf.functions, conf.terminals);

        // check for uniqueness
        //semantic uniqueness
        if (conf.semantic_uniqueness_tries > 0 && init_tries < conf.semantic_uniqueness_tries) {
            n = EnsureSemanticUniqueness(n, pop, conf, tree_initializer, conf.initial_maximum_tree_height, init_tries, fitness);
            if (init_tries == conf.semantic_uniqueness_tries)
                cout << " % warning: maximum number of tries for syntactic unique initialization reached " << endl;
        } else if (conf.syntactic_uniqueness_tries > 0 && init_tries < conf.syntactic_uniqueness_tries) { // syntactic uniqueness
            n = EnsureSyntacticUniqueness(n, pop, conf, tree_initializer, conf.initial_maximum_tree_height, init_tries);
            if (init_tries == conf.syntactic_uniqueness_tries)
                cout << " % warning: maximum number of tries for syntactic unique initialization reached " << endl;
        }

        pop.push_back(n);
    }

    return pop;
}

Node * PopulationInitializer::EnsureSyntacticUniqueness(Node* n, const std::vector<Node*> & pop, const ConfigurationOptions & conf, const TreeInitializer & tree_initializer, size_t height, size_t & tries) {
    //input: pointer to node, vector of pointers to nodes of the rest of the population, configuration options (constant), tree initialiser used, height and tries
    do {
        vector<Node*> o_nodes;
        vector<Node*> n_nodes = n->GetSubtreeNodes(true);
        bool identical_found = false;

        for (Node * o : pop) { // for each tree in the population
            o_nodes = o->GetSubtreeNodes(true); // get vector of their nodes
            if (o_nodes.size() == n_nodes.size()) { //if the trees are of equal size
                for (size_t k = 0; k < o_nodes.size(); k++) { // for all nodes in population tree
                    if (!o_nodes[k]->GetValue().compare(n_nodes[k]->GetValue())) // if the value(name) is not the same - not identical
                        break;
                    identical_found = true;
                }
            }
        }

        if (!identical_found)
            break;

        // identical found, retry
        n->ClearSubtree();
        n = tree_initializer.InitializeRandomTree(TreeInitShape::TreeInitFULL, height, conf.functions, conf.terminals);
        tries++;

    } while (tries < conf.syntactic_uniqueness_tries); // while we have tries left

    return n; // return pointer to node

}

Node * PopulationInitializer::EnsureSemanticUniqueness(Node* n, const std::vector<Node*> & pop, const ConfigurationOptions & conf, const TreeInitializer & tree_initializer, size_t height, size_t& tries, const Fitness & fitness) {

    do {

        vec o_output; 
        vec n_output = n->GetOutput(fitness.TrainX, conf.caching); // get output of tree for all training instances
        bool identical_found = false;
        for (Node * o : pop) {
            o_output = o->GetOutput(fitness.TrainX, conf.caching); 

            for (size_t i = 0; i < n_output.n_elem; i++) {
                if (o_output[i] != n_output[i]) { // if not all outputs of the two trees are the same, they must be semantically different
                    break;
                }
                identical_found = true;
            }

            if (identical_found)
                break;
        }

        if (!identical_found)
            break;

        // identical found, retry
        n->ClearSubtree();
        n = tree_initializer.InitializeRandomTree(TreeInitShape::TreeInitFULL, height, conf.functions, conf.terminals);
        tries++;

    } while (tries < conf.semantic_uniqueness_tries);

    return n;
}
