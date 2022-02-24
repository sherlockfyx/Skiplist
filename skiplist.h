#pragma once

#include <cstdlib>
#include <cmath>
#include <time.h>
#include <cstring>
#include <fstream>

#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#define STORE_FILE "store/dumpFile"

std::mutex mtx;
std::string delimiter = ":";

//=======================Node=========================//
template<typename K, typename V>
class Node {
public:
    Node() = default;

    Node(const K& key, const V& value, int level);

    ~Node();

    K get_key() const;

    V get_value() const;

    void set_value(V val);

    std::vector<Node<K, V>*> next_;

private:

    K key_;

    V value_;
};

// 构造函数
template<typename K, typename V>
Node<K, V>::Node(const K& key, const V& value, int level) : 
    next_(level + 1, nullptr), //node 传入参数是 level
    key_(key), 
    value_(value) {
    
}

// 析构函数
template<typename K, typename V>
Node<K, V>::~Node() {

}

// 工具函数
template<typename K, typename V>
K Node<K, V>::get_key() const {
    return key_;
}

template<typename K, typename V>
V Node<K, V>::get_value() const {
    return value_;
}

template<typename K, typename V>
void Node<K, V>::set_value(V val) {
    value_ = val;
}

//========================= Skip List ============================//

template<typename K, typename V>
class SkipList {
public:
    SkipList(int max_level = 15);

    ~SkipList();

    int get_random_level();

    void display_db();

    //增, 删, 查
    bool insert_element(K key, V value);

    bool delete_element(K key);

    V search_element(K key);

    int size() const {
        return element_count_;
    }

    void load_file();

    void dump_file();
private:

    void get_key_value_from_string(const std::string& str, std::string& key, std::string& value);

    bool is_valid_string(const std::string& str);

private:
    int max_level_;

    int cur_level_;

    int element_count_;

    Node<K, V>* header_;  //头节点

    std::ifstream file_reader_; // 读
    
    std::ofstream file_writer_; // 写

};

// 构造函数
template<typename K, typename V> 
SkipList<K, V>::SkipList(int max_level) :
    max_level_(max_level),
    cur_level_(0),
    element_count_(0) {

    header_ = new Node<K, V>({}, {}, max_level); 

};

// 析构函数
template<typename K, typename V>
SkipList<K, V>::~SkipList() {
    if (file_reader_.is_open()) {
        file_reader_.close();
    }   
    if (file_writer_.is_open()) {
        file_writer_.close();
    }

    Node<K, V>* cur = header_->next_[0];

    while(cur != nullptr) {
        Node<K, V>* node = cur->next_[0];
        delete cur;
        cur = node;
    }

    delete header_;
}

// 随机 0~ max_level
template<typename K, typename V>
int SkipList<K, V>::get_random_level() {
    int k = 0;
    std::random_device e;
    std::uniform_int_distribution<int> dist(0, 1);
    while (dist(e) % 2) {
        k++;
    }
    k = (k < max_level_) ? k : max_level_;
    return k;
};

//插入元素
template<typename K, typename V>
bool SkipList<K, V>::insert_element(K key, V value) {

    std::unique_lock<std::mutex> lk(mtx);

    Node<K, V>* cur = header_;

    std::vector<Node<K, V>*> prev(max_level_ + 1, nullptr);
    //从上向下遍历
    for(int i = cur_level_; i >= 0; i--) {
        while(cur->next_[i] != nullptr && cur->next_[i]->get_key() < key) {
            cur = cur->next_[i];
        }
        prev[i] = cur;
    }
    bool flag = true;

    cur = cur->next_[0];
    
    //插入
    if(cur != nullptr && cur->get_key() == key) {
        //更新
        flag = false;
    } else {
        int random_level = get_random_level();
        
        if (random_level > cur_level_) {
            for (int i = cur_level_ + 1; i < random_level + 1; i++) {
                prev[i] = header_;
            }
            cur_level_ = random_level;
        }

        Node<K, V>* inserted_node = new Node<K, V>(key, value, random_level);   //新节点

        for(int i = 0; i <= random_level; i++) {
            inserted_node->next_[i] = prev[i]->next_[i];
            prev[i]->next_[i] = inserted_node;
        }

        element_count_++;

    }
    //std::cout <<std::this_thread::get_id() << " : insert------"<<flag<<std::endl;

    return flag;
}

//删除元素
template<typename K, typename V> 
bool SkipList<K, V>::delete_element(K key) {

    std::lock_guard<std::mutex> lk(mtx);

    bool flag = false; 

    Node<K, V> *cur = header_; 
    std::vector<Node<K, V>*> prev(cur_level_ + 1, nullptr);


    //  从上到下遍历
    for (int i = cur_level_; i >= 0; i--) {
        while (cur->next_[i] != nullptr && cur->next_[i]->get_key() < key) {
            cur = cur->next_[i];
        }
        prev[i] = cur;
    }
    cur = cur->next_[0];

    if (cur != nullptr && cur->get_key() == key) {
       
        for (int i = 0; i <= cur_level_; i++) {

            if (prev[i]->next_[i] != cur) 
                break;

            prev[i]->next_[i] = cur->next_[i];
        }

        while (cur_level_ > 0 && header_->next_[cur_level_] == nullptr) {
            cur_level_--; 
        }
        delete cur;
        flag = true;
        element_count_--;
    }
    //std::cout <<std::this_thread::get_id() << " : delete------"<<flag<<std::endl;

    return flag;
}

// 查找元素
template<typename K, typename V> 
V SkipList<K, V>::search_element(K key) {

    std::lock_guard<std::mutex> lk(mtx);

    Node<K, V> *cur = header_;
    V res {};
    int flag = false;

    for (int i = cur_level_; i >= 0; i--) {
        while (cur->next_[i] != nullptr && cur->next_[i]->get_key() < key) {
            cur = cur->next_[i];
        }
    }

    cur = cur->next_[0];

    if (cur != nullptr && cur->get_key() == key) {
        res = cur->get_value();
        flag = true;
    } 

    //std::cout <<std::this_thread::get_id() << " : search------"<<flag<<std::endl;
    return res;
}


// load_file and print
template<typename K, typename V> 
void SkipList<K, V>::load_file() {

    file_reader_.open(STORE_FILE);
    std::cout << "*-----------------load_file-----------------*" << std::endl;
    std::string line;
    std::string key = "";
    std::string value = "";
    while (getline(file_reader_, line)) {
        get_key_value_from_string(line, key, value);
        if (key.empty() || value.empty()) {
            continue;
        }
        if(insert_element(atoi(key.c_str()), value)) {
            // std::cout << "key:" << key << "value:" << value << std::endl;
        }
        
    }
    file_reader_.close();
    std::cout << "*----------------load finish----------------*" << std::endl;

}

//读文件时从字符串中获取K, V
template<typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string& key, std::string& value) {

    if(!is_valid_string(str)) {
        return;
    }
    key = str.substr(0, str.find(delimiter));
    value = str.substr(str.find(delimiter)+1, str.length());
}

//字符串是否有效
template<typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {

    if (str.empty() || str.find(delimiter) == std::string::npos) {
        return false;
    }

    return true;
}

// dump_file
template<typename K, typename V> 
void SkipList<K, V>::dump_file() {

    std::cout << "*-----------------dump_file-----------------*" << std::endl;
    file_writer_.open(STORE_FILE);
    Node<K, V> *node = header_->next_[0]; 
    {   
        std::lock_guard<std::mutex> lk(mtx);
        while (node != nullptr) {
            file_writer_ << node->get_key() << ":" << node->get_value() << "\n";
            std::cout << node->get_key() << ":" << node->get_value() << ";\n";
            node = node->next_[0];
        }
    }
   

    file_writer_.flush();
    file_writer_.close();
    return ;
}

//打印数据
template<typename K, typename V> 
void SkipList<K, V>::display_db() {

    std::cout << "\n*=======================Skip List Content=======================*"<<"\n"; 
    for (int i = cur_level_; i >= 0; i--) {
        Node<K, V> *node = header_->next_[i]; 
        std::cout << "Level " << i << ": ";
        while (node != nullptr) {
            std::cout <<"("<<node->get_key() << ": " << node->get_value() << "); ";
            node = node->next_[i];
        }
        std::cout << std::endl;
    }
    std::cout << "\n*=======================      End        =======================*"<<"\n"; 
}


