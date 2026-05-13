#ifndef QUERY_ENGINE_H
#define QUERY_ENGINE_H

#include <string>
#include "indexing/HashIndex.h"
#include "indexing/SortedIndex.h"

// Main query dispatcher - parses input and executes corresponding query
void executeQuery(const std::string &input, const HashIndex &hashIdx, const SortedIndex &sortedIdx);

#endif // QUERY_ENGINE_H
