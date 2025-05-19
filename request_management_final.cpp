//Mehrnaz Ranjbar

#include<iostream>
#include<string>
using namespace std;


//classes
class BSTnode
{
public:
    int id;
    string name;
    BSTnode* left;
    BSTnode* right;
    BSTnode(int id, string name): id(id), name(name), left(nullptr), right(nullptr){}
};

class heapnode
{
public:
    int id;
    int priority;
};

class BST
{
private:
    BSTnode* root;

    BSTnode* insert(BSTnode* node, int id, string name);
    bool search_bool(BSTnode* node, int id);
    BSTnode* searchNode(BSTnode* node, int id);
    BSTnode* remove(BSTnode* node, int id);
    BSTnode* min(BSTnode* node);
    void preorder(BSTnode* node);
    int count(BSTnode* node);

public:
    BST(): root(nullptr){}

    void insertRequest(int id, string name);
    bool searchRequest(int id);
    BSTnode* search_info(int id);
    void deleteRequest(int id);
    void printBST();
    int sizeBST();
};

class MaxHeap
{
private:
    heapnode* heap;
    int capacity;
    int size;

    void maxHeapify(int i);
    int findIndexById(int id);
    void heapIncreaseKey(int i, int newPriority);

public:
    MaxHeap(int capacity): capacity(capacity), size(0)
    {
        heap=new heapnode[capacity];
    }
    ~MaxHeap()
    {
        delete[] heap;
    }

    void insertHeap(int id, int priority);
    heapnode deleteMaxHeap();
    void deleteBYid(int id);
    void processHighestPriorityRequest(BST& bst);
    void increasePriority(int id, int newPriority);
    void printMaxHeap();
};

class request_management
{
private:
    BST bst;
    MaxHeap mh;

public:
    request_management(int capacity): mh(capacity){}

    void insertRequest(int id, string name, int priority);
    void searchRequest(int id);
    void deleteRequest(int id);
    void processHighestPriorityRequest();
    void increasePriority(int id, int newPriority);
    void printSystem();
};


//functions:BST
BSTnode* BST::insert(BSTnode* node, int id, string name)
{
    if(node==nullptr)
    {
        return new BSTnode(id, name);
    }
    if(id<node->id)
    {
        node->left=insert(node->left, id, name);
    }    
    else if(id>node->id)
    {
        node->right = insert(node->right, id, name);
    }
    return node;
}

bool BST::search_bool(BSTnode* node, int id)
{
    if(node==nullptr) return false;
    if(node->id==id) return true;
    if(id<node->id) return search_bool(node->left, id);
    else return search_bool(node->right, id);
}

BSTnode* BST::searchNode(BSTnode* node, int id)
{
    if(node==nullptr || node->id==id) return node;
    if(id<node->id) return searchNode(node->left, id);
    else return searchNode(node->right, id);
}

BSTnode* BST::remove(BSTnode* node, int id)
{
    if(node==nullptr) return node;
    if(id<node->id)
        node->left=remove(node->left, id);
    else if(id>node->id)
        node->right=remove(node->right, id);
    else
    {
        if(node->left==nullptr)
        {
            BSTnode* temp=node->right;
            delete node;
            return temp;
        }
        else if(node->right==nullptr)
        {
            BSTnode* temp=node->left;
            delete node;
            return temp;
        }
        BSTnode* minNode=min(node->right);
        node->id=minNode->id;
        node->name=minNode->name;
        node->right=remove(node->right, minNode->id);
    }
    return node;
}

BSTnode* BST::min(BSTnode* node)
{
    while(node->left!=nullptr)
       node=node->left;
    return node;
}

void BST::preorder(BSTnode* node)
{
    if(!node) return;
    cout<<"ID: "<<node->id<<", Name: "<<node->name<< endl;
    preorder(node->left);
    preorder(node->right);
}

int BST::count(BSTnode* node) 
{
    if(!node) return 0;
    return 1+count(node->left)+count(node->right);
}

void BST::insertRequest(int id, string name)
{
    root=insert(root, id, name);
}

bool BST::searchRequest(int id)
{
    return search_bool(root, id);
}

BSTnode* BST::search_info(int id)
{
    return searchNode(root, id);
}

void BST::deleteRequest(int id)
{
    root=remove(root, id);
}

void BST::printBST()
{
    preorder(root);
}

int BST::sizeBST()
{
    return count(root);
}


//function:MaxHeap
void MaxHeap::maxHeapify(int i)
{
    int l=2*i+1;
    int r=2*i+2;
    int largest=i;
    if(l<size && heap[l].priority>heap[largest].priority)
        largest = l;
    if(r<size && heap[r].priority>heap[largest].priority)
        largest=r;
    if(largest!=i)
    {
        swap(heap[i], heap[largest]);
        maxHeapify(largest);
    }
}

int MaxHeap::findIndexById(int id)
{
    for(int i=0; i<size; ++i)
    {
        if(heap[i].id==id)
            return i;
    }
    return -1;
}

void MaxHeap::heapIncreaseKey(int i, int newPriority)
{
    if(newPriority<heap[i].priority)
    {
        cout<<"Error: new priority is smaller!"<<endl;
        return;
    }
    heap[i].priority=newPriority;
    while(i>0 && heap[(i-1)/2].priority<heap[i].priority)
    {
        swap(heap[i], heap[(i-1)/2]);
        i=(i-1)/2;
    }
}

void MaxHeap::insertHeap(int id, int priority)
{
    if(size==capacity) return;
    int i=size++;
    heap[i].id=id;
    heap[i].priority=-1;
    heapIncreaseKey(i, priority);
}

heapnode MaxHeap::deleteMaxHeap()
{
    if(size<=0) return {-1, -1};
    heapnode root=heap[0];
    heap[0]=heap[--size];
    maxHeapify(0);
    return root;
}

void MaxHeap::deleteBYid(int id)
{
    int index=findIndexById(id);
    heapIncreaseKey(index, INT32_MAX);
    deleteMaxHeap();
}

void MaxHeap::processHighestPriorityRequest(BST& bst)
{
    heapnode max=deleteMaxHeap();
    BSTnode* node=bst.search_info(max.id);
    if(node)
        cout<<"Processing request ID: "<< max.id<<", Name: "<<node->name<<endl;
    bst.deleteRequest(max.id);
}

void MaxHeap::increasePriority(int id, int newPriority) 
{
    int index=findIndexById(id);
    if (index != -1)
        heapIncreaseKey(index, newPriority);
}

void MaxHeap::printMaxHeap()
{
    for(int i=0; i<size; i++)
        cout<<"ID: "<<heap[i].id<<", Priority: "<<heap[i].priority<<endl;
}


//functions:request management
void request_management::insertRequest(int id, string name, int priority)
{
    if(bst.searchRequest(id))
    {
        cout<<"Error: ID "<<id<<" already exists"<<endl;
        return;  
    }
    bst.insertRequest(id, name);
    mh.insertHeap(id, priority);
    cout<<"done!"<<endl;
}

void request_management::searchRequest(int id)
{
    if(bst.sizeBST()==0)
    {
        cout<<"No request yet!"<<endl;
        return;
    }
    if(!bst.searchRequest(id))
    {
        cout<<"Error: ID "<<id<<" not exists"<<endl;
        return;  
    }
    BSTnode* result=bst.search_info(id);
    cout<<"ID: "<<result->id<<" Name: "<<result->name<<endl;
}

void request_management::deleteRequest(int id)
{
    if(bst.sizeBST()==0)
    {
        cout<<"No request to delete!"<<endl;
        return;
    }
    if(!bst.searchRequest(id))
    {
        cout<<"Error: ID "<<id<<" not exists"<<endl;
        return;  
    }
    bst.deleteRequest(id);
    mh.deleteBYid(id);
    cout<<"done!"<<endl;
}

void request_management::processHighestPriorityRequest()
{
    if(bst.sizeBST()==0)
    {
        cout<<"No request to process!"<<endl;
        return;
    }
    mh.processHighestPriorityRequest(bst);
}

void request_management::increasePriority(int id, int newPriority)
{
    if(bst.sizeBST()==0)
    {
        cout<<"No request yet!"<<endl;
        return;
    }
    if(!bst.searchRequest(id))
    {
        cout<<"Error: ID "<<id<<" not exists"<<endl;
        return;  
    }
    mh.increasePriority(id, newPriority);
}

void request_management::printSystem()
{
    cout<<endl<<"BST:"<<endl;
    bst.printBST();
    cout<<endl<<"MaxHeap:"<<endl;
    mh.printMaxHeap();
}



int main()
{
    request_management system(200);
    cout<<"Commands: insert, delete, search, process, increase, print, exit"<<endl;

    while(true)
    {
        string command;
        cin>>command;

        if(command=="insert")
        {
            int id, priority;
            string name;
            cout<<"Enter ID: ";
            cin>>id;
            cout<<"Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout<<"Enter Priority: ";
            cin>>priority;
            system.insertRequest(id, name, priority);
        }
        else if(command=="delete")
        {
            int id;
            cout<<"Enter ID: ";
            cin >> id;
            system.deleteRequest(id);
        }
        else if(command=="search")
        {
            int id;
            cout <<"Enter ID: ";
            cin >> id;
            system.searchRequest(id);
        }
        else if(command=="process")
        {
            system.processHighestPriorityRequest();
        }
        else if(command=="increase")
        {
            int id, newPriority;
            cout<<"Enter ID: ";
            cin>>id;
            cout<<"Enter new Priority: ";
            cin>>newPriority;
            system.increasePriority(id, newPriority);
        }
        else if(command=="print")
        {
            system.printSystem();
        }
        else if(command=="exit")
        {
            cout<<"Exiting system..."<<endl;
            break;
        }
        else
        {
            cout<<"Error: Invalid command"<<endl;
        }
    }
    return 0;
}
