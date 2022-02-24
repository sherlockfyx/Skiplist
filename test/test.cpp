#include <iostream>
#include <chrono>
#include <cstdlib>

#include <thread>
#include <vector>


#include "../skiplist.h"

#define NUM_THREADS 3
#define TEST_COUNT 1000000

SkipList<int, std::string> skipList(18);

void insertElement(int i) {

    int tmp = TEST_COUNT/NUM_THREADS;
    std::random_device e;
    std::uniform_int_distribution<int> dist(i * tmp, (i + 1) * tmp - 1);
	for (int j = i * tmp; j < (i + 1) * tmp; j++) {
		skipList.insert_element(dist(e), "a"); 
	}
}

void getElement(int i) {

    int tmp = TEST_COUNT/NUM_THREADS;
    std::random_device e;
    std::uniform_int_distribution<int> dist(i * tmp, (i + 1) * tmp - 1);
	for (int j = i * tmp; j < (i + 1) * tmp; j++) {
		skipList.search_element(dist(e)); 
	}
}

void delElement(int i) {

    int tmp = TEST_COUNT/NUM_THREADS;
    std::random_device e;
    std::uniform_int_distribution<int> dist(i * tmp, (i + 1) * tmp - 1);
	for (int j = i * tmp; j < (i + 1) * tmp; j++) {
		skipList.delete_element(dist(e)); 
	}
}


void insert_get_del(){

    int tmp = TEST_COUNT/NUM_THREADS;
    std::random_device e;
    {
        std::uniform_int_distribution<int> dist(0 * tmp, (0 + 1) * tmp - 1);
	    for (int j = 0 * tmp; j < (0 + 1) * tmp; j++) {
		    skipList.insert_element(dist(e),"b"); 
	    } 
    }

    {
        std::uniform_int_distribution<int> dist(1 * tmp, (1 + 1) * tmp - 1);
        for (int j = 1 * tmp; j < (1 + 1) * tmp; j++) {
            skipList.search_element(dist(e)); 
        }
    }

    {
        std::uniform_int_distribution<int> dist(2 * tmp, (2 + 1) * tmp - 1);
        for (int j = 2 * tmp; j < (2 + 1) * tmp; j++) {
            skipList.delete_element(dist(e)); 
        }       
    }

}

int main() {

    // {   //插入测试

    //     std::vector<std::thread> threads;
    //     auto start = std::chrono::high_resolution_clock::now();


    //     for(int i = 0; i < NUM_THREADS; i++ ) {
    //         std::cout << "main() : creating thread, " << i << std::endl;
    //         threads.push_back(std::thread(insertElement, i));

    //     }

    //     for(int i = 0; i < NUM_THREADS; i++) {
    //         threads[i].join();
    //     }

    //     auto end = std::chrono::high_resolution_clock::now(); 
    //     std::chrono::duration<double> total = end - start;
    //     std::cout << "insert total time:" << total.count() << std::endl;
    // }

    // skipList.dump_file();

    skipList.load_file();

    // {   //查找测试
    //     std::vector<std::thread> threads;
    //     auto start = std::chrono::high_resolution_clock::now();

    //     for( int i = 0; i < NUM_THREADS; i++ ) {
    //         std::cout << "main() : creating thread, " << i << std::endl;
    //         threads.emplace_back(std::thread(getElement, i));
    //     }

    //     for(int i = 0; i < NUM_THREADS; i++) {
    //         threads[i].join();
    //     }

    //     auto end = std::chrono::high_resolution_clock::now(); 
    //     std::chrono::duration<double> total = end - start;
    //     std::cout << "get total time:" << total.count() << std::endl;
    // }

    // {   //删除测试
    //     std::vector<std::thread> threads;
    //     auto start = std::chrono::high_resolution_clock::now();

    //     for( int i = 0; i < NUM_THREADS; i++ ) {
    //         std::cout << "main() : creating thread, " << i << std::endl;
    //         threads.emplace_back(std::thread(delElement, i));
    //     }

    //     for(int i = 0; i < NUM_THREADS; i++) {
    //         threads[i].join();
    //     }

    //     auto end = std::chrono::high_resolution_clock::now(); 
    //     std::chrono::duration<double> total = end - start;
    //     std::cout << "delete total time:" << total.count() << std::endl;
    // }

    // skipList.dump_file();

    {   //循环测试
        std::vector<std::thread> threads;
        auto start = std::chrono::high_resolution_clock::now();

        // std::cout << "main() : creating thread, " << 0 << std::endl;
        // threads.emplace_back(std::thread(insertElement, 0));
        // std::cout << "main() : creating thread, " << 1 << std::endl;
        // threads.emplace_back(std::thread(getElement, 1));
        // std::cout << "main() : creating thread, " << 2 << std::endl;
        // threads.emplace_back(std::thread(delElement, 2));

        // for(auto& t : threads) {
        //     t.join();
        // }

        insert_get_del();



        auto end = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> total = end - start;
        std::cout << "insert get del total time:" << total.count() << std::endl;
    }

    return 0;
}