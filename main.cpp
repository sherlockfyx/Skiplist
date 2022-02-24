#include "skiplist.h"
#include<chrono>
#define FILE_PATH "./store/dumpFile"

int main() {

    SkipList<int, std::string> skipList(15);
	skipList.insert_element(1, "a"); 
	skipList.insert_element(3, "b"); 
	skipList.insert_element(7, "c"); 
	skipList.insert_element(8, "d"); 
	skipList.insert_element(9, "e"); 
    skipList.insert_element(2, "y"); 
	skipList.insert_element(5, "j"); 
	skipList.insert_element(19, "f");
	skipList.insert_element(19, "g"); 

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();   //写到磁盘

    skipList.load_file();   //将磁盘文件更新到数据库

    skipList.search_element(9);
    skipList.search_element(18);


    skipList.display_db();

    skipList.delete_element(3);
    skipList.delete_element(7);

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_db();
}