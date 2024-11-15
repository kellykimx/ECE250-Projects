#ifndef PROJECT0_ENVIRONMENT_H
#define PROJECT0_ENVIRONMENT_H

class Board {
public:

    Board(int n, int m); //constructor
    ~Board(); //destructor

    bool exitsign = false;

    void create(int n, int m);
    void move(int x, int y);
    void clear();
    bool exit();
    void point(char t, int x, int y);
    void update(double k);

    double calculatePotential(int x, int y, int xG, int yG, char t);
    void potentials(int xG, int yG, char t);
    void calculateAll();
    void clean();

private:
    int nRows;
    int mColumns;
    double **potential;
    char **item;
    double kConst;
};

#endif //PROJECT0_ENVIRONMENT_H
