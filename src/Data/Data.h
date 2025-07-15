//
// Created by pierre on 7/15/25.
//

#ifndef WARE_H
#define WARE_H
#include <string>

struct Price {
    unsigned int min;
    unsigned int avg;
    unsigned int max;
};

struct Ware {
    static Ware load(const std::string &filename);

    const std::string id;
    const std::string name;

    const unsigned int volume;
    const Price price;

    const std::string description;
    const std::string transport;
};

struct Module {

};


#endif //WARE_H
