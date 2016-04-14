struct bin_tree {
    char index[4]; //CGH
    int *values; //posicoes de inicio de linha que é de congonhas
    int nrValues; // nr de posicoes em values
    struct bin_tree * right, * left;
};
typedef struct bin_tree node;

