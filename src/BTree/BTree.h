//BTREE��

#ifndef BTREE_H
#define BTREE_H

#include "BNode.h"
#include "BNode.cpp"
#include <map>
using namespace std;
template<class T>
class BTree
{
protected:
	BNode <T> *root;//���ڵ�ָ��
	int k;	//B��Ϊk��
	int minKey, maxKey; //���ټ�ֵ������ֵ
	int nsize;  //B���ṹԪ�أ���ֵ������
	void maintainAfterInsert(BNode<T> *);	//����Ԫ�غ󱣳����νṹƽ��
	void maintainAfterErase(BNode<T>*);  //ɾ��Ԫ�غ󱣳����νṹƽ��
	void mergeNode(BNode<T>*, BNode<T>*, T, bool flag);//�ϲ������ڵ㣬flagΪ0ʱ����ʾ�ϲ���ڵ㣻����ϲ��ҽڵ�
	void deleteNode(BNode<T> *);	//ɾ���ڵ�ռ䣬��������
	void print(BNode<T>*);	//����ڵ�
	BNode<T>* splitLeftNode(BNode<T>*, int);	//�ڵ���ѵõ���ڵ�
	BNode<T>* splitRightNode(BNode<T>*, int);	//�ڵ���ѵõ��ҽڵ�
	BNode<T>* findBrotherNode(BNode<T>*,T&,bool flag);//�����ֵܽڵ�,flagΪ0ʱ��������ڵ㣻����Ϊ�ҽڵ�
	BNode<T>* findNode(T);	//��ȡ���ڸü�ֵ�Ľڵ�
    
public:
	BTree();
	~BTree();
	BTree(int k);	//���죬����B������
	bool insert(T data);	//����
	bool erase(T key);  //ɾ��
	void printRoot();	//������ڵ�
	void printArray();	//���������
	void printTree();	//�������
	void printWholeTree();	//�����������
	BNode<T>* findNextNode(BNode<T>*p, T, T&);  //��ȡ��̽ڵ㼰��ֵ
	BNode<T>* findPreNode(BNode<T>*p, T, T&);	//��ȡǰ���ڵ㼰��ֵ
	int size();	//��ȡ��Ԫ������
	int height();	//��ȡ���߶�
	BNode<T>* getRoot();	//��ȡ���ڵ�ָ��

private:
	int getLen(int);	//������������ȡ���ֳ���
	void GetDFS(BNode<T>*p, int d, int* dep, int *tab, map<T, int>&mp, int& cnt);	//�������������ڵݹ��ȡ������͵���Ϣ
};


#endif // BTREE_H
