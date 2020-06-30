#include "stdlib.h"
#include <string.h>
#include <tsl/htrie_map.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
//#include <boost/beast/core/detail/base64.hpp>
#include "base64.hpp"
#include <chrono>

using namespace std;
using clock1 = std::chrono::system_clock;
using sec1 = std::chrono::duration<double>;

//typedef google::dense_hash_map<long, long> hash_map;


// README: Scale test for Tessil Hat-trie
// https://github.com/Tessil/hat-trie
// Dependencies include boost beast base_64 encode/decode api.
// To install Tessil Hat trie : see installation instruction in ReadMe of Tessil Hat-trie
// GCC version needed 4.8.4, clang version 3.5.0
// This need cmake to be installed.
// To install cmake with command see: https://cliutils.gitlab.io/modern-cmake/chapters/intro/installing.html

int parseLine(char* line){
  // This assumes that a digit will be found and the line ends in " Kb".
  int i = strlen(line);
  const char* p = line;
  while (*p <'0' || *p > '9') p++;
  line[i-3] = '\0';
  i = atoi(p);
  return i;
}

int getValue(){ //Note: this value is in KB!
  FILE* file = fopen("/proc/self/status", "r");
  int result = -1;
  char line[128];

  while (fgets(line, 128, file) != NULL){
    if (strncmp(line, "VmSize:", 7) == 0){
      result = parseLine(line);
      break;
    }
  }
  fclose(file);
  return result;
}

int main() {
  // for milliseconds, use using ms = std::chrono::duration<double, std::milli>;
  tsl::htrie_map<char, int> map;
  //struct rusage usage;
  //struct timeval start, end;

  //  getrusage(RUSAGE_SELF, &usage);
  //start = usage.ru_utime;
  // google::dense_hash_map<long, long> hm;
  // hm.set_empty_key(-1);

  //    hash_map hm;
  //hm.set_deleted_key(NULL);
  const auto before = clock1::now();

  cout<< "Memory begin:" << getValue() << endl;

  for (int i = 1; i <= 100000000; ++i) {
    //hm[rand()%100000000 + 1] = i;

    map[boost::beast::detail::base64_encode((uint8_t*)&i, 4)] = i;
  }
  const sec1 duration = clock1::now() - before;
  //cout<< sizeof(map) <<endl;
  cout<< "Memory End" << getValue() << endl;
  std::cout << "It took " << duration.count() << "s" << std::endl;
  //getrusage(RUSAGE_SELF, &usage);
  //end = usage.ru_utime;

  //printf("Started at: %ld.%lds\n", start.tv_sec, start.tv_usec);
  //printf("Ended at: %ld.%lds\n", end.tv_sec, end.tv_usec); 
 return 0;
}
