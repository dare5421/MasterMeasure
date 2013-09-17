#include "chromosome.h"


chromosome::chromosome()
{
    chromosomeLength = 0;
    chromosomeHeadLength = 0;
    chromosomeTailLength = 0;
    chromosomeWing1Length = 0;
    chromosomeWing2Length = 0;
    index = 0;

    topLeftX=0;
    topLeftY=0;
    bottomRightX=0;
    bottomRightY=0;


}

chromosome::~chromosome()
{

}


int chromosome::getTopLeftX() const
{
    return topLeftX;
}

void chromosome::setTopLeftX(int value)
{
    topLeftX = value;
}

int chromosome::getTopLeftY() const
{
    return topLeftY;
}

void chromosome::setTopLeftY(int value)
{
    topLeftY = value;
}

int chromosome::getBottomRightX() const
{
    return bottomRightX;
}

void chromosome::setBottomRightX(int value)
{
    bottomRightX = value;
}

int chromosome::getBottomRightY() const
{
    return bottomRightY;
}

void chromosome::setBottomRightY(int value)
{
    bottomRightY = value;
}



int chromosome::getIndex() const
{
    return index;
}

void chromosome::setIndex(int value)
{
    index = value;
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


