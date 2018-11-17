#include "BTree.h"
#include <list>
#include <queue>
#include <set>
#include <iostream>
using namespace std;

#include <QVector>
#include <QQueue>
#include <QDebug>
#include <QLineF>
#include <QRectF>

template<class T>
void BTree<T>::Painting(int w, int h)
{
    if(this->root==NULL)return;
    qreal W=(qreal)w;
    qreal H=(qreal)h;
    QQueue<BNode<T>*>q;
    v.clear();
    q.enqueue(root);
    while(!q.empty()){
        BNode<T>* p=q.dequeue();
        if(p->isLeaf()){
            v.push_back(p);
            continue;
        }
        typename std::list<BNode<T>*>::iterator it;
        for(it = p->child.begin(); it != p->child.end(); it++) {
            q.enqueue(*it);
        }
    }
    int ln=v.size();//最底层节点数
    //层数
    int hn=1;
    BNode<T>*p=root;
    while(!p->isLeaf()){
        ++hn;
        p=*(p->child.begin());
    }
    qreal wd=W/ln;
    qreal hd=min(min(H/(2*ln-1),wd/1.5),H/6.0);
    qreal line_d=hn!=1?(H-hd*hn)/(hn-1):0;
    for(int i=0;i<ln;++i){
        qreal left=i*wd;
        qreal right=left+wd;
        qreal up=(hd+line_d)*(hn-1);
        v[i]->border=QRectF(left+wd*0.1,up,wd*0.8,hd);
    }
    dfs(root,1,hd,line_d);
}

template<class T>
void BTree<T>::dfs(BNode<T> *p, int dep,qreal hd,qreal line_d)
{
    if(p->isLeaf())return;
    v.push_back(p);
    typename std::list<BNode<T>*>::iterator it=p->child.begin();
    qreal x=0;
    qreal d=0;
    int n=p->child.size();
    for(;it!=p->child.end();it++){
        dfs(*it,dep+1,hd,line_d);
        x+=((*it)->border.left()+(*it)->border.right())/2.0;
        d=(*it)->border.width();
    }
    x/=qreal(n);
    qreal up=(line_d+hd)*(dep-1);
    p->border=QRectF(x-d/2.0,up,d,hd);
    int i=0;
    for(it=p->child.begin();it!=p->child.end();it++){
        qreal xx=((*it)->border.left()+(*it)->border.right())/2.0;
        qreal yy=(*it)->border.top();
        (*it)->line=QLineF(xx,yy,p->border.left()+i*(d/p->size()),p->border.bottom());
        ++i;
    }
}

template<class T>
bool BTree<T>::insert(T key)
{
	BNode<T> *p = root;
    op.push_back(pair<int,T>(0,key));
	if (root == NULL) {
		root = new BNode<T>(key);
		nsize++;
		return 1;
	}
	while (!p->isLeaf()) {
        if(p->hasKey(key))return false;
		p = p->next(key);
	}
	if(p->hasKey(key))return false;
	p->insertKey(key, NULL, NULL);
	maintainAfterInsert(p);
	nsize++;
	return 1;
}

template<class T>
bool BTree<T>::erase(T key){
    BNode<T>* p = findNode(key);
    if(!p)return false; //未找到key
    op.push_back(pair<int,T>(1,key));
    if(!p->isLeaf()){
        T next_key;
        BNode<T>* next_node = findNextNode(p, key, next_key);
        p->replace(key, next_key);
        key = next_key;
        p = next_node;
    }
    p->eraseKey(key);
    p->eraseChild(NULL);
    this->maintainAfterErase(p);
    nsize--;
    return true;
}

template<class T>
void BTree<T>::maintainAfterErase(BNode<T>*p){
	BNode<T>* fa = p->fa;
    if(p==root){
        if(p->size()==0){
            root=*(root->child.begin());
			if (root) root->fa = NULL;
        }
        return;
    }
    if(p->size()>=minKey)return;
    T father_key;
    BNode<T>* brother_node;
    brother_node = findBrotherNode(p, father_key,0);//0左，1右

    if(brother_node)mergeNode(p,brother_node,father_key,0);//存在左节点则合并左节点
    else{
        brother_node = findBrotherNode(p, father_key,1);//0左，1右
        mergeNode(p,brother_node,father_key,1);
    }
    this->maintainAfterErase(fa);
}

template<class T>
void BTree<T>::mergeNode(BNode<T>*p1,BNode<T>*p2, T father_key, bool flag){
    BNode<T>*fa = p1->fa;
	if (p2->size() > minKey) {
		int replace_key = flag ? *(p2->keys.begin()) : *(p2->keys.rbegin());
		BNode<T>* replace_child = flag ? *(p2->child.begin()) : *(p2->child.rbegin());
		p2->eraseKey(replace_key);
		p2->eraseChild(replace_child);
		fa->replace(father_key, replace_key);
		if (flag == 0) {
			p1->keys.push_front(father_key);
			p1->child.push_front(replace_child);
		}
		else {
			p1->keys.push_back(father_key);
			p1->child.push_back(replace_child);
		}
		p1->updateChild();
    }
    else{
        fa->eraseKey(father_key);
        fa->eraseChild(p1);
        if(flag==0){
            p2->keys.push_back(father_key);
            p2->keys.splice(p2->keys.end(),p1->keys);
            p2->child.splice(p2->child.end(),p1->child);
        }
        else{
            p2->keys.push_front(father_key);
            p2->keys.splice(p2->keys.begin(),p1->keys);
            p2->child.splice(p2->child.begin(),p1->child);
        }
        fa->updateChild();
        p2->updateChild();
        delete p1;
    }
}

template<class T>
BNode<T>* BTree<T>::findBrotherNode(BNode<T>* p,T& key, bool flag){
    BNode<T>* fa = p->fa;
    typename std::list<T>::iterator it_key = fa->keys.begin();
	typename std::list<BNode<T>*>::iterator it_child = fa->child.begin();
    while(*it_child!=p){
        it_child++;it_key++;
    }
    if(flag==0){//左兄弟
        if(it_child==fa->child.begin())return NULL;
        key=*(--it_key);
        return *(--it_child);
    }
    else{
        if((++it_child)==fa->child.end())return NULL;
        key=*it_key;
        return  *it_child;
    }
}

template<class T>
BNode<T>* BTree<T>::findNextNode(BNode<T>*p, T key, T& next_key) {
	if (!p)return NULL;
	typename std::list<T>::iterator it_key;
	typename std::list<BNode<T>*>::iterator it_child;
	do {
		it_key = p->keys.begin();
		it_child = p->child.begin();
		while (it_key != p->keys.end()) {
			if (key < *it_key)break;
			it_child++; it_key++;
		}
		if (it_key != p->keys.end())next_key = *it_key;
		if (*it_child == NULL) 
			return p;
	} while (p = *it_child);
}

template<class T>
BNode<T>* BTree<T>::findPreNode(BNode<T>* p, T key, T &pre_key)
{
	if (!p)return NULL;
	typename std::list<T>::reverse_iterator it_key;
	typename std::list<BNode<T>*>::reverse_iterator it_child;
	do {
		it_key = p->keys.rbegin();
		it_child = p->child.rbegin();
		while (it_key != p->keys.rend()) {
			if (*it_key < key)break;
			it_child++; it_key++;
		}
		if (it_key != p->keys.rend())pre_key = *it_key;
		if (*it_child == NULL) 
			return p;
	} while (p = *it_child);
}


template<class T>
BNode<T>* BTree<T>::findNode(T key){
    BNode<T>* p = root;
    while(p&&!(p->hasKey(key))){
        p=p->next(key);
    }
    return p;
}

template<class T>
void BTree<T>::maintainAfterInsert(BNode<T> *p)
{
	while (p->size() >= k) {
		BNode<T>* left_node = splitLeftNode(p, (k - 1) / 2);
		BNode<T>* right_node = splitRightNode(p, k - (k - 1) / 2 - 1);
		T key = p->getKey((k - 1) / 2);	//上移的值
		delete p;
		p = left_node->fa;	//父节点
		if (!p) {
			root = p = new BNode<T>();
			left_node->fa = right_node->fa = root;
		}
		p->insertKey(key, left_node, right_node);
	}
}

template<class T>
BNode<T> * BTree<T>::splitLeftNode(BNode<T> * p, int pos)
{
	BNode<T>* node = new BNode<T>();
	node->fa = p->fa;
	typename std::list<T>::iterator it_key = p->keys.begin();
	typename std::list<BNode<T>*>::iterator it_child = p->child.begin();
	node->child.push_back(*it_child);
	int i = 0;
	while (i++<pos) {
		it_child++;
		node->keys.push_back(*it_key);
		node->child.push_back(*it_child);
		it_key++;
	}
	node->updateChild();
	return node;
}

template<class T>
BNode<T> * BTree<T>::splitRightNode(BNode<T> *p, int pos)
{
	BNode<T>* node = new BNode<T>();
	node->fa = p->fa;
	typename std::list<T>::reverse_iterator it_key = p->keys.rbegin();
	typename std::list<BNode<T>*>::reverse_iterator it_child = p->child.rbegin();
	node->child.push_front(*it_child);
	int i = 0;
	while (i++<pos) {
		it_child++;
		node->keys.push_front(*it_key);
		node->child.push_front(*it_child);
		it_key++;
	}
	node->updateChild();
	return node;
}

template <class T>
int BTree<T>::size(){
    return nsize;
}

template<class T>
int BTree<T>::getK()
{
    return this->k;
}

template<class T>
int BTree<T>::height()
{
    int i = 0;
    BNode<T> *p = root;
    while (p) {
        p = *(p->child.begin());
        ++i;
    }
    return i;
}

template<class T>
BNode<T>* BTree<T>::getRoot()
{
	return this->root;
}

template<class T>
void BTree<T>::deleteNode(BNode<T>*p)
{
	if (!p)return;
	typename std::list<BNode<T>*>::iterator it;
	for (it = p->child.begin(); it != p->child.end(); it++) {
		deleteNode(*it);
	}
	delete p;
}

template <class T>
BTree<T>::BTree()
{
	k = 3;
	nsize=0;
	maxKey = k - 1;
	minKey = (k - 1) / 2;
    root = NULL;
}

template<class T>
BTree<T>::BTree(const BTree<T>& other)
{
    root=NULL;
    minKey=other.minKey;maxKey=other.maxKey;
    k=other.k;
    nsize=0;
    //op.assign(other.op.begin(),other.op.end());
    for(auto p:other.op){
        if(p.first==0)insert(p.second);
        else erase(p.second);
    }
}

template<class T>
BTree<T>::BTree(int k) :k(k)
{
    nsize=0;
    maxKey = k - 1;
	minKey = (k - 1) / 2;
	root = NULL;
}

template<class T>
void BTree<T>::printArray(){
    //cout<<"root:";printRoot();cout<<endl;;
    print(root);
    cout<<endl;
}

template<class T>
void BTree<T>::print(BNode<T>*p){
    if(p==NULL)return;
    typename std::list<T>::iterator it_key = p->keys.begin();
	typename std::list<BNode<T>*>::iterator it_child = p->child.begin();
    print(*it_child);
    for(it_child++;it_child!=p->child.end();it_child++,it_key++){
        cout<<*it_key<<' ';
        print(*it_child);
    }
}

template<class T>
void BTree<T>::printRoot()
{
	if (!root)return;
	root->print();
	cout << endl;
}

template<class T>
void BTree<T>::printTree(){
	if (!root) {
		cout << "空树~" << endl;
		return;
	}
    queue<BNode<T>*>q;
    set<BNode<T>*>s;s.clear();s.insert(root);
    q.push(root);
    while(!q.empty()){
        BNode<T>* p=q.front();q.pop();
		cout << '|';p->print(); cout << '|';
        if(p->isLeaf())continue;
        typename std::list<BNode<T>*>::iterator it;
        for(it = p->child.begin(); it != p->child.end(); it++) {
            q.push(*it);
        }
        if(s.find(p)!=s.end()){
            s.insert((*p->child.rbegin()));
            cout<<endl;
        }
    }
    cout<<endl<<endl;
}

template<class T>
BTree<T>::~BTree()
{
	deleteNode(root);
}
