//BTREE类

#ifndef BTREE_H
#define BTREE_H

#include "BNode.h"
#include "BNode.cpp"
#include <QRectF>
#include <algorithm>

template<class T>
class BTree
{
protected:
	BNode <T> *root;//根节点指针
	int k;	//B树为k阶
	int minKey, maxKey; //最少键值和最多键值
	int nsize;  //B数结构元素（键值）总数
	void maintainAfterInsert(BNode<T> *);	//插入元素后保持树形结构平衡
	void maintainAfterErase(BNode<T>*);  //删出元素后保持树形结构平衡
	void mergeNode(BNode<T>*, BNode<T>*, T, bool flag);//合并两个节点，flag为0时，表示合并左节点；否则合并右节点
    void deleteNode(BNode<T> *);	//删除节点空间，用于析构
    void print(BNode<T>*);	//输出节点
	BNode<T>* splitLeftNode(BNode<T>*, int);	//节点分裂得到左节点
	BNode<T>* splitRightNode(BNode<T>*, int);	//节点分裂得到右节点

	BNode<T>* findBrotherNode(BNode<T>*,T&,bool flag);//查找兄弟节点,flag为0时，查找左节点；否则为右节点
	BNode<T>* findNode(T);	//获取存在该键值的节点
    
public:
	BTree();
    BTree(const BTree<T>&other);
	~BTree();
	BTree(int k);	//构造，传入B树阶数
	bool insert(T data);	//插入
	bool erase(T key);  //删除
	void printRoot();	//输出根节点
	void printArray();	//输出排序结果
	void printTree();	//输出树型
	int size();	//获取树元素总数
    BNode<T>* findNextNode(BNode<T>*p, T, T&);  //获取后继节点及键值
    BNode<T>* findPreNode(BNode<T>*p, T, T&);	//获取前驱节点及键值
    int height();	//获取树高度
    int getK();

	BNode<T>* getRoot();	//获取根节点指针

    //qt
    QVector<BNode<T>*>v;

    vector<pair<int,T> >op;

    void Painting(int,int);
    void dfs(BNode<T>*p,int dep,qreal hd,qreal line_d);
};


#endif // BTREE_H
