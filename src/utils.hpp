//
// Created by pierre on 2/3/25.
//

#ifndef X4STATIONGENERATOR_UTILS_HPP
#define X4STATIONGENERATOR_UTILS_HPP

#include <map>
#include <QLayout>
#include <QWidget>

template<typename T, typename K>
void addMap(std::map<T, K> &dest, const std::map<T, K> &src)
{
    for (auto const &iter: src) {
        if (dest.find(iter.first) == dest.end())
            dest[iter.first] = iter.second;
        else
            dest[iter.first] += iter.second;
    }
}

template<typename T, typename K>
void addItem(std::map<T, K> &dest, std::pair<const T, K> &item)
{
    if (dest.find(item.first) == dest.end())
        dest[item.first] = item.second;
    else
        dest[item.first] += item.second;
}

void clearLayout(QLayout *layout);

#endif //X4STATIONGENERATOR_UTILS_HPP
