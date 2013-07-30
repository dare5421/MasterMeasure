#ifndef CHROMOSOME_H
#define CHROMOSOME_H

class chromosome
{
public:
    chromosome();

    void setChromosomeLength(double lenght);
    void setChromosomeHeadLength(double lenght);
    void setChromosomeTailLength(double lenght);
    void setChromosomeWing1Length(double lenght);
    void setChromosomeWing2Length(double lenght);

    double getChromosomeLength();
    double getChromosomeHeadLength();
    double getChromosomeTailLength();
    double getChromosomeWing1Length();
    double getChromosomeWing2Length();

//    double operator+ (double length);

private:
    double chromosomeLength;
    double chromosomeHeadLength;
    double chromosomeTailLength;
    double chromosomeWing1Length;
    double chromosomeWing2Length;
};

#endif // CHROMOSOME_H
