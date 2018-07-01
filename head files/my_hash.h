
//
// Created by wangluobin on 18-6-26.
//
#ifndef HASH_H
#define HASH_H

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
#include <pthread.h>
#include <auto_ptr.h>
#include <shared_mutex>
using namespace std;
using namespace __gnu_cxx;

const char file_name[] = "/media/wangluobin/Story/mac24_port4_address40000000_repeated1.txt";
using key = int64_t;
using value = int;
int batch_size = 64;
mutex mutex_instream;
mutex mutex_cuckoohash[LIBCUCKOO_DEFAULT_SIZE];
int CORES = thread::hardware_concurrency();
int64_t hashFn(key k){    return k%LIBCUCKOO_DEFAULT_SIZE;}

void print_result(const char name[],time_t total_time,time_t read_time,time_t find_time,time_t insert_time,time_t update_time,
                  int find_num,int insert_num,int update_num){
    cout << name << ":" << endl;
    printf("total time: %.3fs\tread time: %.3fs\n",total_time/1e6,read_time/1e6);
    printf("find time:  %.3fs\tfind num:  %d\n",find_time/1e6,find_num);
    printf("insert time:%.3fs\tinsert num:%d\n",insert_time/1e6,insert_num);
    printf("update time:%.3fs\tupdate num:%d\n",update_time/1e6,update_num);
}
void _runCuckoohash(cuckoohash_map<key , value>& Table){
    key mac;
    value port;
    int maximum;
    time_t t1,t2,read_time = 0,find_time = 0,insert_time = 0,update_time = 0;
    time_t older,newer;
    ifstream in;
    older = clock();
    in.open(file_name);
    in >> maximum;
    int find_num = 0 , insert_num = 0, update_num = 0;
    for (int i = 0; i < maximum; i++) {
        //if(i % (maximum/10) == 0) cout << i << endl;
        //t1 = clock();
        in >> mac >> port;
        //t2 = clock();
        //read_time += t2-t1;
        int temp = -1;
        //t1 = clock();
        if(Table.find(mac,temp)){
            //t2 = clock();
            //find_time += t2 - t1;
            if(temp == port){
                find_num ++;
            }
            else{
                //t1 = clock();
                Table.update(mac,port);
                //t2 = clock();
                //update_time += t2-t1;
                update_num ++;
            }
        }
        else{
            //t2 = clock();
            //find_time += t2-t1;
            //t1 = clock();
            Table.insert(mac,port);
            //t2 = clock();
            //insert_time += t2-t1;
            insert_num ++;
        }
    }
    newer = clock();
    print_result("Cuckoohash",newer - older,read_time,find_time,insert_time,update_time,
                 find_num,insert_num,update_num);
    in.close();
}
void runCuckoohash(){
    cuckoohash_map<key , value> Table;
    _runCuckoohash(Table);
    _runCuckoohash(Table);
}

void runMap(){
    key mac;
    value port;
    int maximum;
    map<key,value> Map;
    time_t t1,t2,read_time = 0,find_time = 0,insert_time = 0,update_time = 0;
    time_t older,newer;
    ifstream in;
    in.open(file_name);
    in >> maximum;
    int find_num = 0 , insert_num = 0, update_num = 0;
    older = clock();
    for (int i = 0; i < maximum; i++) {
        //if(i % (maximum/10) == 0) cout << i << endl;
        t1 = clock();
        in >> mac >> port;
        t2 = clock();
        read_time += t2-t1;
        t1 = clock();
        _Rb_tree_iterator<pair<const int64_t,int>> it = Map.find(mac);

        if(it != Map.end()) {
            t2 = clock();
            find_time += t2-t1;
            if(it->second != port){
                it->second = port;
                update_num ++;
            }
            else find_num ++;
        }
        else {
            t2 = clock();
            find_time += t2-t1;
            t1 = clock();
            Map.insert(map<int64_t, int> :: value_type(mac, port));
            t2 = clock();
            insert_time += t2-t1;
            insert_num ++;
        }
    }
    newer = clock();
    print_result("Map",newer - older,read_time,find_time,insert_time,update_time,
                 find_num,insert_num,update_num);
    in.close();
    in.open(file_name);
    in >> maximum;
    read_time = 0,find_time = 0,insert_time = 0,update_time = 0;
    find_num = 0 ; insert_num = 0; update_num = 0;
    older = clock();
    for (int i = 0; i < maximum; i++) {
        //if(i % (maximum/10) == 0) cout << i << endl;
        t1 = clock();
        in >> mac >> port;
        t2 = clock();
        read_time += t2-t1;
        t1 = clock();
        _Rb_tree_iterator<pair<const int64_t,int>> it = Map.find(mac);

        if(it != Map.end()) {
            t2 = clock();
            find_time += t2-t1;
            if(it->second != port){
                it->second = port;
                update_num ++;
            }
            else find_num ++;
        }
        else {
            t2 = clock();
            find_time += t2-t1;
            t1 = clock();
            Map.insert(map<int64_t, int> :: value_type(mac, port));
            t2 = clock();
            insert_time += t2-t1;
            insert_num ++;
        }
    }
    newer = clock();
    print_result("Map",newer - older,read_time,find_time,insert_time,update_time,
                 find_num,insert_num,update_num);
    in.close();
}

void runHashmap(){
    hash_map <key, value> Table;
    hash_map<key,value>::const_iterator h_iterator;
    key mac;
    value port;
    int maximum;
    time_t t1,t2,read_time = 0,find_time = 0,insert_time = 0,update_time = 0;
    time_t older,newer;
    ifstream in;
    older = clock();
    in.open(file_name);
    in >> maximum;
    int find_num = 0 , insert_num = 0, update_num = 0;
    for (int i = 0; i < maximum; i++) {
        //if(i % (maximum/10) == 0) cout << i << endl;
        t1 = clock();
        in >> mac >> port;
        t2 = clock();
        read_time += t2-t1;
        t1 = clock();
        h_iterator = Table.find(mac);
        if(h_iterator != Table.end()){
            t2 = clock();
            find_time += t2 - t1;
            if(h_iterator->second == port){
                find_num ++;
            }
            else{
                t1 = clock();
                Table[mac] = port;
                t2 = clock();
                update_time += t2-t1;
                update_num ++;
            }
        }
        else{
            t2 = clock();
            find_time += t2-t1;
            t1 = clock();
            Table.insert(pair<int64_t, int>(mac,port));
            t2 = clock();
            insert_time += t2-t1;
            insert_num ++;
        }
    }
    newer = clock();
    print_result("Hashmap",newer - older,read_time,find_time,insert_time,update_time,
                 find_num,insert_num,update_num);
    in.close();
    older = clock();
    in.open(file_name);
    in >> maximum;
    read_time = 0,find_time = 0,insert_time = 0,update_time = 0;
    find_num = 0; insert_num = 0; update_num = 0;
    for (int i = 0; i < maximum; i++) {
        //if(i % (maximum/10) == 0) cout << i << endl;
        t1 = clock();
        in >> mac >> port;
        t2 = clock();
        read_time += t2-t1;
        t1 = clock();
        if(Table.find(mac) != Table.end()){
            t2 = clock();
            find_time += t2 - t1;
            if(Table[mac] == port){
                find_num ++;
            }
            else{
                t1 = clock();
                Table[mac] = port;
                t2 = clock();
                update_time += t2-t1;
                update_num ++;
            }
        }
        else{
            t2 = clock();
            find_time += t2-t1;
            t1 = clock();
            Table.insert(pair<int64_t, int>(mac,port));
            t2 = clock();
            insert_time += t2-t1;
            insert_num ++;
        }
    }
    newer = clock();
    print_result("Hashmap",newer - older,read_time,find_time,insert_time,update_time,
                 find_num,insert_num,update_num);
    in.close();
}

void runConcurrentBatchDeal(){
    //mutex_instream.lock();

    //mutex_instream.unlock();
//    value temp;
//    for(int i = 0 ; i < batch ; i ++) {
//        if(Table.find(mini_table[i].first,temp)){
//            if(temp == mini_table[i].second){
//            }
//            else{
//                Table.update(mini_table[i].first,mini_table[i].second);
//            }
//        }
//        else{
//            Table.insert(mini_table[i].first,mini_table[i].second);
//        }
//    }
}
void _runLockConcurrentCuckoohash(cuckoohash_map<key , value>& Table){
    key mac;
    value port;
    int maximum;
    time_t t1,t2,read_time = 0,find_time = 0,insert_time = 0,update_time = 0;
    time_t older,newer;
    int find_num = 0 , insert_num = 0, update_num = 0;
    ifstream in;
    older = clock();
    in.open(file_name);
    in >> maximum;
    int batch_to_core = (maximum)/CORES;

    for(int y = 0 ; y < CORES ; y ++){
        pair<key,value> mini_table[batch_to_core];
        for(int i = 0 ; i < batch_to_core ; i ++){
            in >> mini_table[i].first >> mini_table[i].second;
        }

    }

    thread t[CORES];
    vector<std::thread> threads;

//    for(int i=0; i < CORES; ++i)
//        runConcurrentBatchDeal(in,batch_to_core,Table);
}
    

void runLockConcurrentCuckoohash(){
    cuckoohash_map<key , value> Table;
    _runCuckoohash(Table);
    time_t newer,older;
    older = clock();
    _runLockConcurrentCuckoohash(Table);
    newer = clock();
    print_result("concurrent cuckoohash", newer - older, 0, 0, 0, 0, 0, 0, 0);
}
void runNonlockConcurrentCuckoohash(){

}



#endif
