#ifndef BNODE_H
#define BNODE_H

#include <list>
using namespace std;

template <class T>
class BNode
{
public:
	list <T> keys;	//��ֵ��
	list <BNode<T>*> child;	//�ӽڵ�ָ���
	BNode<T>* fa;	//���ڵ�ָ��

	BNode();
	BNode(T);
	~BNode();
	bool isLeaf();	//�ж��Ƿ���Ҷ�ڵ�
	bool hasKey(T key);  //�ж��Ƿ����ĳһ��ֵ
	bool replace(T old_key, T new_key);  //�滻��ֵ
	bool eraseKey(T key);  //ɾ����ֵ
	bool eraseChild(BNode<T>*); //ɾ���ӽڵ�ָ��
	void updateChild(); //�ӽڵ��ض���
	void insertKey(T data, BNode<T>* child1, BNode<T>* child2);	//��ڵ����ĳ��ֵ���������ӽڵ�ָ��
	void print();	//����ڵ��ֵ
	BNode<T>* next(T key);	//��ȡkeyӦ��ָ���ӽڵ�ķ���
	T getKey(int pos);	//��ȡ�ڵ��pos����ֵ
	int size();	//��ȡ�ڵ����洢��ֵ����
};

#endif // BNODE_H
