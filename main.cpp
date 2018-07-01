#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <string>
#include <cuckoohash_map.hh>
#include <cmath>
#include <fstream>
#include <ctime>
#include <map>
#include <hash_map>
#include <thread>
#include <mutex>

#include "hash.h"
#include "test.h"
using namespace std;
using namespace __gnu_cxx;

bool version_count[LIBCUCKOO_DEFAULT_SIZE/LIBCUCKOO_DEFAULT_SLOT_PER_BUCKET]{false};
int main() {
    runCuckoohash();//non-concurrency
    runMap();
    runHashmap();
    //runLockConcurrentCuckoohash();
    //runNonlockConcurrentCuckoohash();
    concurrent_load();
    return 0;
}
