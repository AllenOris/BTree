#include "BNode.h"
#include <list>
#include <iostream>
using namespace std;


template<class T>
bool BNode<T>::isLeaf()
{
	return child.empty() || (*child.begin()) == NULL;
}

template<class T>
bool BNode<T>::hasKey(T key){
    typename std::list<T>::iterator it_key;
    for(it_key=keys.begin();it_key!=keys.end();it_key++)
        if (*it_key==key)return true;
    return false;
}

template<class T>
bool BNode<T>::replace(T old_key, T new_key){
    typename std::list<T>::iterator it_key;
    for(it_key=keys.begin();it_key!=keys.end();it_key++)
        if (*it_key==old_key){
            *it_key = new_key;
            return true;
        }
    return false;
}

template<class T>
bool BNode<T>::eraseKey(T key){
    if(!hasKey(key))return false;
    typename std::list<T>::iterator it_key;
    for(it_key=keys.begin();it_key!=keys.end();it_key++)
        if (*it_key==key){
            keys.erase(it_key);
            break;
        }
    return true;
}

template<class T>
bool BNode<T>::eraseChild(BNode<T>* p){
    typename std::list<BNode<T>*>::iterator it_child;
    for(it_child=child.begin();it_child!=child.end();it_child++)
        if(*it_child==p){
            child.erase(it_child);
            return true;
        }
    return false;
}

template<class T>
BNode<T>* BNode<T>::next(T data)
{
	typename std::list<T>::iterator it_key = keys.begin();
	typename std::list<BNode<T>*>::iterator it_child = child.begin();
	while (it_key != keys.end() && data >= *it_key) {
		it_child++;
		it_key++;
	}
	return *it_child;
}

template<class T>
void BNode<T>::insertKey(T key, BNode<T> * child1, BNode<T> * child2)
{
	typename std::list<T>::iterator it_key = keys.begin();
	typename std::list<BNode*>::iterator it_child = child.begin();
	while (it_key != keys.end() && key > *it_key) {
		it_child++;
		it_key++;
	}
	keys.insert(it_key, key);
	if (!child.empty())it_child = child.erase(it_child);
	it_child = child.insert(it_child, child2);
	it_child = child.insert(it_child, child1);
}

template<class T>
void BNode<T>::updateChild(){
    typename std::list<BNode*>::iterator it_child;
    for(it_child=child.begin();it_child!=child.end();it_child++)
        if(*it_child)
            (*it_child)->fa=this;
}

template<class T>
void BNode<T>::print()
{
	typename std::list<T>::iterator it;
	for (it = keys.begin(); it != keys.end(); it++)cout << *it << ' ';
}

template<class T>
T BNode<T>::getKey(int pos)
{
	typename std::list<T>::iterator it = keys.begin();
	for (int i = 0; i < pos; ++i) {	//Òì³£´¦Àí
		++it;
	}
	return *it;
}

template<class T>
int BNode<T>::size()
{
	return keys.size();
	return 0;
}

template<class T>
BNode<T>::BNode()
{
	keys.clear();
	child.clear();
	fa = NULL;
}

template<class T>
BNode<T>::BNode(T data)
{
	keys.push_back(data);
	child.push_back(NULL);
	child.push_back(NULL);
	fa = NULL;
}

template<class T>
BNode<T>::~BNode()
{
	child.clear();
	keys.clear();
}
