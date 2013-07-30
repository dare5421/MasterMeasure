#ifndef CHROMOSOME_H
#define CHROMOSOME_H

class chromosome
{
public:
    chromosome();

    void setChromosomeLength(double lenght);
    void setChromosomeHeadLength(double lenght);
    void setChromosomeTailLength(double lenght);
    void setChromosomeWing1(double lenght);
    void setChromosomeWing2(double lenght);

    double getChromosomeLength();
    double getChromosomeHeadLength();
    double getChromosomeTailLength();
    double getChromosomeWing1();
    double getChromosomeWing2();

    double operator+ (double length);

private:
    double chromosomeLength;
    double chromosomeHeadLength;
    double chromosomeTailLength;
    double chromosomeWing1;
    double chromosomeWing2;
};

#endif // CHROMOSOME_H
