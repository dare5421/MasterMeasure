#include "chromosome.h"

chromosome::chromosome()
{
    chromosomeLength = 0;
    chromosomeHeadLength = 0;
    chromosomeTailLength = 0;
    chromosomeWing1 = 0;
    chromosomeWing2 = 0;
}


void chromosome::setChromosomeLength(double lenght){
    chromosomeLength = lenght;
}

void chromosome::setChromosomeHeadLength(double lenght){
    chromosomeHeadLength = lenght;
}

void chromosome::setChromosomeTailLength(double lenght){
    chromosomeTailLength = lenght;
}

void chromosome::setChromosomeWing1(double lenght){
    chromosomeWing1 = lenght;
}

void chromosome::setChromosomeWing2(double lenght){
    chromosomeWing2 = lenght;
}

double chromosome::getChromosomeLength(){
    return chromosomeLength;
}

double chromosome::getChromosomeHeadLength(){
    return chromosomeHeadLength;
}

double chromosome::getChromosomeTailLength(){
    return chromosomeTailLength;
}

double chromosome::getChromosomeWing1(){
    return chromosomeWing1;
}

double chromosome::getChromosomeWing2(){
    return chromosomeWing2;
}

double chromosome::operator+ (double length){
    chromosome temp;
    temp.setChromosomeLength(temp.getChromosomeLength()+length);
    return temp.getChromosomeLength();
}
