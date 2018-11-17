#ifndef BNODE_H
#define BNODE_H

#include <list>
using namespace std;
#include <QRectF>
#include <QLineF>


template <class T>
class BNode
{
public:
	list <T> keys;	//键值表
	list <BNode<T>*> child;	//子节点指针表
	BNode<T>* fa;	//父节点指针
	
	BNode();
	BNode(T);
	~BNode();
	bool isLeaf();	//判断是否是叶节点
	bool hasKey(T key);  //判断是否存在某一键值
	bool replace(T old_key,T new_key);  //替换键值
	bool eraseKey(T key);  //删除键值
	bool eraseChild(BNode<T>*); //删除子节点指针
	void updateChild(); //子节点重定向
	void insertKey(T data, BNode<T>* child1, BNode<T>* child2);	//向节点插入某键值，并更新子节点指针
	void print();	//输出节点键值
	BNode<T>* next(T key);	//获取key应该对应的子节点
	T getKey(int pos);	//获取节点第pos个键值
	int size();	//获取节点所存储键值数量

    //qt
    QRectF border;
    QLineF line;
};

#endif // BNODE_H
