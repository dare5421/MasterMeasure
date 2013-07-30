#include "chromosome.h"

chromosome::chromosome()
{
    chromosomeLength = 0;
    chromosomeHeadLength = 0;
    chromosomeTailLength = 0;
    chromosomeWing1Length = 0;
    chromosomeWing2Length = 0;
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

void chromosome::setChromosomeWing1Length(double lenght){
    chromosomeWing1Length = lenght;
}

void chromosome::setChromosomeWing2Length(double lenght){
    chromosomeWing2Length = lenght;
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

double chromosome::getChromosomeWing1Length(){
    return chromosomeWing1Length;
}

double chromosome::getChromosomeWing2Length(){
    return chromosomeWing2Length;
}

//double chromosome::operator+ (double length){
//    chromosome temp;
//    temp.setChromosomeLength(temp.getChromosomeLength()+length);
//    return temp.getChromosomeLength();
//}
