#include <cstddef>
#include <cstdlib>
#include <string>

#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{
/*
template <typename K, typename V>
using Map = aisdi::TreeMap<K, V>;

void perfomTest()
{
  Map<int, std::string> map;
  map[1] = "TODO";
}*/

template <typename K, typename V>
using TreeMap = aisdi::TreeMap<K, V>;

void performTreeTest(std::size_t n, std::vector<int>& keys)
{
    std::cout << "Drzewo:" << std::endl << std::endl;
    TreeMap<int, std::string> map;

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();
    for(auto it = keys.begin(); it != keys.end(); ++it)
        map[*it] = "Value";
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Dodawanie elementow: " << elapsed_seconds.count() << "s" << std::endl;


    start = std::chrono::system_clock::now();
    for(auto it = keys.begin(); it != keys.end(); ++it)
        map[*it] = "Changed Value";
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Zmiana wartosci elementow: " << elapsed_seconds.count() << "s" << std::endl;


    start = std::chrono::system_clock::now();
    for(std::size_t i = 0; i < n; ++i)
        map.remove(map.begin());
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Usuwanie elementow: " << elapsed_seconds.count() << "s" << std::endl;

}

template <typename K, typename V>
using HashMap = aisdi::HashMap<K, V>;

void performHashTest(std::size_t n,std::vector<int>& keys)
{
    std::cout << std::endl << "Hashmapa:" << std::endl << std::endl;

    HashMap<int, std::string> map;

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();
    for(auto it = keys.begin(); it != keys.end(); ++it)
        map[*it] = "Value";
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Dodawanie elementow: " << elapsed_seconds.count() << "s" << std::endl;


    start = std::chrono::system_clock::now();
    for(auto it = keys.begin(); it != keys.end(); ++it)
        map[*it] = "Changed Value";
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Zmiana wartosci elementow: " << elapsed_seconds.count() << "s" << std::endl;


    start = std::chrono::system_clock::now();
    for(std::size_t i = 0; i < n; ++i)
        map.remove(map.begin());
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Usuwanie elementow: " << elapsed_seconds.count() << "s" << std::endl;
}


} // namespace

int main(int argc, char** argv)
{
    /*
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
  */
  srand(time(nullptr));
  const std::size_t numberOfRepeat = argc > 1 ? std::atoll(argv[1]) : 10000;

  std::vector<int> keys;
  for(size_t i= 0; i < numberOfRepeat; ++i)
    keys.push_back(i);

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle (keys.begin(), keys.end(), std::default_random_engine(seed));

  performTreeTest(numberOfRepeat,keys);
  performHashTest(numberOfRepeat, keys);

  return 0;
}
