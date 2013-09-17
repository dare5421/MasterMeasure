#ifndef OVERLOADS_H
#define OVERLOADS_H
#include <QDataStream>
#include "chromosome.h"


QDataStream & operator<< (QDataStream & stream, chromosome & myChromosome);
QDataStream & operator>> (QDataStream & stream, chromosome & myChromosome);

QDataStream & operator<< (QDataStream & stream, chromosome & myChromosome){
    stream << myChromosome.getChromosomeHeadLength() << myChromosome.getChromosomeLength()
              << myChromosome.getChromosomeTailLength() << myChromosome.getChromosomeWing1Length()
                 << myChromosome.getChromosomeWing2Length() << myChromosome.getIndex();

    return stream;
}

QDataStream & operator>> (QDataStream & stream, chromosome & myChromosome){

//    myChromosome = chromosome();

//    chromosome chr = chromosome();

    double len, wing1, wing2, tail, head;
    int index;

    stream >> head >> len >> tail >> wing1 >> wing2 >> index;

    myChromosome.setChromosomeHeadLength(head);
    myChromosome.setChromosomeLength(len);
    myChromosome.setChromosomeTailLength(tail);
    myChromosome.setChromosomeWing1Length(wing1);
    myChromosome.setChromosomeWing2Length(wing2);
    myChromosome.setIndex(index);

//    chr.setChromosomeHeadLength(head);
//    chr.setChromosomeLength(len);
//    chr.setChromosomeTailLength(tail);
//    chr.setChromosomeWing1Length(wing1);
//    chr.setChromosomeWing2Length(wing2);
//    chr.setIndex(index);

//    myChromosome = chr;

    return stream;
}


#endif // OVERLOADS_H
