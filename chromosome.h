#ifndef CHROMOSOME_H
#define CHROMOSOME_H

//#include <QPoint>
class chromosome
{
public:
    chromosome();
    ~chromosome();

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


    int getIndex() const;
    void setIndex(int value);



    int getTopLeftX() const;
    void setTopLeftX(int value);

    int getTopLeftY() const;
    void setTopLeftY(int value);

    int getBottomRightX() const;
    void setBottomRightX(int value);

    int getBottomRightY() const;
    void setBottomRightY(int value);

private:
    double chromosomeLength;
    double chromosomeHeadLength;
    double chromosomeTailLength;
    double chromosomeWing1Length;
    double chromosomeWing2Length;
    int index;
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
};

#endif // CHROMOSOME_H
