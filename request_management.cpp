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

    BSTnode* remove(BSTnode* node, int id);
    BSTnode* min(BSTnode* node);
    void preorder(BSTnode* node);
    int count(BSTnode* node);
    
public:
    BST(): root(nullptr){}

    void insertRequest(int id, string name);
    bool searchRequest(int id);
    void deleteRequest(int id);
    void printBST();
    bool isEmptyBST();
    int sizeBST();
};

class MaxHeap
{
private:
    heapnode* heap;
    int capacity;
    int size;
    
    void maxHeapify(int i);
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
    bool deleteBYid(int id);
    void processHighestPriorityRequest(BST& bst);
    void increasePriority(int id, int newPriority);
    void printMaxHeap();
    bool isEmptyHeap();
    int sizeMaxHeap();
};

class request_management
{
private:
    BST bst;
    MaxHeap maxHeap;
    
public:
    request_management(int capacity): maxHeap(capacity){}

    void insertRequest(int id, string name, int priority);
    bool searchRequest(int id);
    void deleteRequest(int id);
    void processHighestPriorityRequest();
    void increasePriority(int id, int newPriority);
    void printSystem();
};


//functions:BST
BSTnode* BST::remove(BSTnode* node, int id)
{
    if (node == nullptr) return node;

    if(id<node->id)
    {
        node->left=remove(node->left, id);
    } 
    else if(id>node->id)
    {
        node->right=remove(node->right, id);
    }
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
            
        BSTnode* temp=min(node->right);
        node->id=temp->id;
        node->name=temp->name;
        node->right=remove(node->right, temp->id);
    }
    return node;
}

BSTnode* BST::min(BSTnode* node)
{
    BSTnode* current=node;
    while(current&&current->left!=nullptr)
    {
        current=current->left;
    }
    return current;
}

void BST::preorder(BSTnode* node)
{
    if(node==nullptr)
    {
    return;
    }
    cout<<"ID: "<<node->id<<", Name: "<<node->name<<endl;
    preorder(node->left);
    preorder(node->right);
}

int BST::count(BSTnode* node)
{
    if(node==nullptr) return 0;
    return 1+count(node->left)+count(node->right);
}

void BST::insertRequest(int id, string name)
{
    if(root==nullptr)
    {
        root=new BSTnode(id, name);
        return;
    }
    BSTnode* current=root;
    BSTnode* parent=nullptr;
    while(current!=nullptr)
    {
        parent=current;
        if(id<current->id)
        {
            current=current->left;
        }
        else
        {
            current=current->right;
        }
    }
    BSTnode* new_node=new BSTnode(id, name);
    if(id<parent->id)
    {
        parent->left=new_node;
    }
    else
    {
        parent->right=new_node;
    }
}

bool BST::searchRequest(int id)
{
    BSTnode* current=root;
    while(current!=nullptr)
    {
        if(id==current->id)
        {
            return true;
        }
        else if(id<current->id)
        {
            current=current->left;
        }
        else
        {
            current=current->right;
        }
    }
    return false;
}

void BST::deleteRequest(int id)
{
    root=remove(root, id);
}

void BST::printBST()
{
    cout<<"BST:" <<endl;
    preorder(root);
}

bool BST::isEmptyBST()
{
    return root==nullptr;
}

int BST::sizeBST()
{
    return count(root);
}


//function:MaxHeap
void MaxHeap::maxHeapify(int i)
{
    int largest=i;
    int left=2*i+1;
    int right=2*i+2;
        
    if(left<size && heap[left].priority>heap[largest].priority)
    {
        largest=left;
    }
        
    if(right<size && heap[right].priority>heap[largest].priority)
    {
        largest=right;
    }
    
    if(largest!=i)
    {
        swap(heap[i], heap[largest]);
        maxHeapify(largest);
    }
}
    
void MaxHeap::heapIncreaseKey(int i, int newPriority)
{
    if(newPriority<heap[i].priority)
    {
        cout<<"New priority is smaller than current priority"<<endl;
        return;
    }
        
    heap[i].priority=newPriority;
    while(i>0 && heap[(i-1)/2].priority<heap[i].priority)
    {
        swap(heap[i], heap[(i-1)/2]);
        i=(i-1)/2;
    }
    cout<<"done!"<<endl;
}

void MaxHeap::insertHeap(int id, int priority)
{
    if(size==capacity)
    {
        cout<<"Heap is full"<<endl;
        return;
    }
        
    size++;
    heap[size-1].id=id;
    heap[size-1].priority=-1;
    heapIncreaseKey(size-1, priority);
}
    
heapnode MaxHeap::deleteMaxHeap()
{
    if(size<1)
    {
        cout<<"heap is empty!"<<endl;
        return {-1, -1};
    }
    heapnode max=heap[0];
    heap[0]=heap[size-1];
    size--;
    maxHeapify(0);
    return max;
}

bool MaxHeap::deleteBYid(int id)
{
    int index=-1;
    for(int i=0; i<size; i++)
    {
        if(heap[i].id==id)
        {
            index=i;
            break;
        }
    }
    
    if(index==-1) return false;
    
    heap[index]=heap[size - 1];
    size--;
    
    if(index<size)
    {
        if(index>0 && heap[index].priority>heap[(index-1)/2].priority)
        {
            while(index>0 && heap[(index-1)/2].priority<heap[index].priority)
            {
                swap(heap[index], heap[(index-1)/2]);
                index=(index-1)/2;
            }
        }
        else
        {
            maxHeapify(index);
        }
    }
    
    return true;
}

void MaxHeap::processHighestPriorityRequest(BST& bst)
{
    if(isEmptyHeap())
    {
        cout<<"no requests to process"<<endl;
        return;
    }
        
    heapnode max=deleteMaxHeap();
    if(max.id==-1 && max.priority==-1)
    {
        return;
    }
    cout<<"Processing request - ID: "<<max.id<<", Priority: "<< max.priority<<endl;
    bst.deleteRequest(max.id);
}
    
void MaxHeap::increasePriority(int id, int newPriority)
{
    for(int i=0; i<size; i++)
    {
        if(heap[i].id==id)
        {
            heapIncreaseKey(i, newPriority);
            return;
        }
    }
    cout<<"ID not found in heap"<<endl;
}

void MaxHeap::printMaxHeap()
{
    cout<<"MaxHeap:"<<endl;
    for(int i=0; i<size; i++)
    {
        cout<<"ID: "<<heap[i].id<<", Priority: "<<heap[i].priority<< endl;
    }
}

bool MaxHeap::isEmptyHeap()
{
    return size==0;
}

int MaxHeap::sizeMaxHeap()
{
    return size;
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
    maxHeap.insertHeap(id, priority);
}

bool request_management::searchRequest(int id)
{
    return bst.searchRequest(id);
}

void request_management::deleteRequest(int id)
{
    if(bst.searchRequest(id))
    {
        bst.deleteRequest(id);
        if(!maxHeap.deleteBYid(id))
        {
            cout <<"Request found in BST but not in Heap"<< endl;
        }
        else
        {
            cout<<"Request deleted from both BST and Heap"<< endl;
        }
    }
    else
    {
        cout <<"Request not found"<<endl;
    }
}

void request_management::processHighestPriorityRequest()
{
    maxHeap.processHighestPriorityRequest(bst);
}

void request_management::increasePriority(int id, int newPriority)
{
    maxHeap.increasePriority(id, newPriority);
}

void request_management::printSystem()
{
    bst.printBST();
    cout<<endl;
    maxHeap.printMaxHeap();
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
            if (system.searchRequest(id))
            {
                cout<<"Request found in system."<<endl;
            }
            else
            {
                cout<<"Request not found."<<endl;
            }
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
        else if(command == "print")
        {
            system.printSystem();
        }
        else if(command == "exit")
        {
            cout<<"Exiting system..."<<endl;
            break;
        }
        else
        {
            cout<<"Invalid command. Try again."<<endl;
        }
    }
    return 0;
}