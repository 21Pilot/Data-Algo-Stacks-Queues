// Hyomin Seo  
// ECE264 Project 1
// Nov.2020

// The purpose of the program is to recieve an input file which is a list of three 
// commands : create, push and pop, to and from two classes : stack and queue,
// and to produce an output file which is a overview of such command processing.

// The program heavily follows the advice stated in the given intruction
// Refer to attached pdf file for detailed instruction

// Set up 
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

// Use the EXACT casing and spacing 
using namespace std;

// Create Base class : SimpleList 
template <typename Object>
class SimpleList {
        private:
                // initialize SimpleList
                void init() {
                        set_size(0);
                        this->head = NULL;
                        this->tail = NULL;
                }

                // Private Nested class: Node  
                // contains a field for data and a pointer to the next Node
                class Node {
                        public:
                                // Node Constructor
                                Node(Object d, Node *ptr) {
                                        this->data = d;
                                        this->next = ptr;
                                }
                                Object data;
                                Node *next;
                };

                // Declared : Insert Node at start and end of the list 
                Node *head;
                Node *tail;

                // Declared: SimpleList data : name and size 
                string name;
                int size;
        protected:
                // Non-virtual function :
                // Command : "push" 
                // Class : Stack  
                void insert_head(const Object &d) {
                        if(get_size() == 0) {
                                Node *node = new Node(d, NULL);
                                this->tail = node;
                                this->head = node;
                        }
                        else {
                                Node *node = new Node(d, this->head);
                                this->head = node;
                        }
                }

                // Non-virtual function 
                // Command : "push" 
                // Class : Queue 
                void insert_tail(Object d) {
                        Node *node = new Node(d, NULL);
                        if(get_size() == 0) {
                                this->tail = node;
                                this->head = node;
                        }
                        else {
                              this->tail->next = node;
                              this->tail = node;  
                        }
                }

                // Non-virtual function 
                // Comman : "pop" 
                // Class : Stack and Queue
                Object delete_head() {
                        Node *node = this->head;
                        this->head = node->next;
                        Object data = node->data;
                        delete node;
                        return data;
                }
        public:

                // Public member function :
                // retrieve SimpleList 
                SimpleList(const string &n) {this->name = n; init();}

                // Pure-virtual functions :
                // Command : "push" and "pop"
                virtual void push(Object d) = 0;
                virtual Object pop() = 0;

                // Public functions :
                // to edit or access private data
                string get_name() {
                        return name;
                }
                int get_size() {
                        return size;
                }
                void set_size(int s) {
                        size = s;
                }
};

// Derived 
// Class : Stack
template <typename Object>
class Stack : public SimpleList<Object> {
        public:
                // Construct Stack
                // Using SimpleList Constructor
                Stack(const string &n) : SimpleList<Object>(n) {}
                
                // Pure-virtual functions :
                // Command : "push" and "pop"
                void push(Object d) {
                        this->insert_head(d);
                        int n = this->get_size();
                        this->set_size(n+1);
                }
                Object pop(){
                        int n = this->get_size();
                        this->set_size(n-1);
                        return this->delete_head();
                }
};

// CLass derivation

// Derived 
// Class : Queue
template <typename Object>
class Queue : public SimpleList<Object> {
        public:
                // Construct Queue 
                // Using SimpleList Constructor
                Queue(const string &n) : SimpleList<Object>(n) {}
                
                // Pure-virtual functions :
                // Command : "push" and "pop"
                void push(Object d) {
                        this->insert_tail(d);
                        int n = this->get_size();
                        this->set_size(n+1);
                }
                Object pop() {
                        int n = this->get_size();
                        this->set_size(n-1);
                        return this->delete_head();
                }
};

// Program functionality :
// Search function : search and return 
// a pointer to the desired Stack or Queue or a NULL pointer
template <typename Object>
SimpleList<Object> * search_list(list<SimpleList<Object> *> *l, const string &name) {
        // Iterator :
        // iterate through the list of pointers until desired Stack or Queue has been found
        // Loop through SimpleList
        for (typename list<SimpleList<Object> *>::const_iterator itr = l->begin(); itr != l->end(); ++itr) {
            
                // IF : Return pointer once desired SimpleList is found
                if((*itr)->get_name() == name) {
                        return *itr;
                }
        }

        //ELSE : return NULL pointer
        return NULL;
}

// Program functionality :
// Add function : add
// add a SimpleList to the list of pointers 
template <typename Object>
void create_list(list<SimpleList<Object> *> *l, const string &name, const string &type, ofstream &outFile) {
        // IF : Return Error if desired Stack or Queue already exists
        if(search_list(l, name) != nullptr) {
                outFile << "ERROR: This name already exists!" << endl;
        }


        // ELSE : check if SimpleList to be created is Stack or Queue and create new SimpleList accordingly
        else {
                // Stack
                if(type == "stack") {
                        SimpleList<Object> *stack = new Stack<Object>(name);
                        l->push_back(stack);
                }
                // Queue
                else {
                        SimpleList<Object> *queue = new Queue<Object>(name);
                        l->push_back(queue);
                }
        }
}
// Program functionality :
// Push function : push
// data to a specific SimpleList in the list of pointers to SimpleLists
template <typename Object>
void push_value(list<SimpleList<Object> *> *l, const string &name, Object data, ofstream &outFile) {
        SimpleList<Object> *temp = search_list(l, name);

        // IF : push the data to Stack or Queue if desired class exists
        // if not NULL 
        if(temp != NULL) {
                temp->push(data);
        }
        
        // ELSE : Return Error 
        // if NULL 
        else {
                outFile << "ERROR: This name does not exist!" << endl;
        }
}

// Program functionality :
// Pop function : pop
// data from a specific SimpleList in the list of pointers to SimpleLists
template <typename Object>
void pop_value(list<SimpleList<Object> *> *l, const string &name, ofstream &outFile) {
        SimpleList<Object> *temp = search_list(l, name);

        // IF : check the size of the SimpleList if desired class exists
        // if not NULL
        if(search_list(l, name) != NULL) {

                // IF : the SimpleList is not empty, retrieve the popped value 
                if(temp->get_size() != 0) {
                        outFile << "Value popped: " << temp->pop() << endl;
                }

                // ELSE : Return Error 
                // if SimpleList is empty
                else {
                        outFile << "ERROR: This list is empty!" << endl;
                }
        }

        // ELSE : Return Error 
        // if NULL 
        else {
                outFile << "ERROR: This name does not exist!" << endl;
        }
}

// Program functionality :
// Parse function ("Large Function") : parse
// through input file and process all the commands
void parse_file(const string &input, const string &output) {
    
        // Three list :
        // Lists of pointers - integers, doubles, and strings
        // all integer, double, string Stacks and Queues
        list<SimpleList<int> *> listSLi; 
        list<SimpleList<double> *> listSLd;
        list<SimpleList<string> *> listSLs;
        
        // Open the input file and output file 
        ifstream inFile(input);
        ofstream outFile(output);

        // IF : parse through the file if it is opened/exists
        if(inFile.is_open()) {
            
                // Variables 
                string action, name, data, commands;
                char type;
                
                // Process Algorithm 
                while(getline(inFile, commands)) {
                    
                        // Seperate lines into strings
                        istringstream iss(commands);

                        // First string
                        // Stored : in action 
                        iss >> action;

                        // Second string 
                        // Stored : in name
                        iss >> name;

                        // First character from 'name' string
                        // Stored : name type
                        type = name.at(0);
                        
                        // After each comman is readable output the string 
                        outFile << "PROCESSING COMMAND: " << commands << endl;

                        // IF : action is "create", 
                        // check the SimpleList type 
                        // call the according function to add a new SimpleList to the list of pointers 
                        if(action == "create") {
                                iss >> data;
                                
                                // integer
                                if(type == 'i') {
                                        create_list<int>(&listSLi, name, data, outFile);
                                }
                                // double
                                else if(type == 'd') {
                                        create_list<double>(&listSLd, name, data, outFile);
                                }
                                // string 
                                else {
                                        create_list<string>(&listSLs, name, data, outFile);
                                }
                        }
                        
                        // IF : action is "push", 
                        // check the SimpleList type 
                        // call the according function to add a new SimpleList to the list of pointers 
                        else if(action == "push") {
                                iss >> data;
                                
                                // integer
                                if(type == 'i') {
                                        push_value<int>(&listSLi, name, stoi(data), outFile);
                                }
                                // double
                                else if(type == 'd') {
                                        push_value<double>(&listSLd, name, stod(data), outFile);
                                }
                                // string 
                                else {
                                        push_value<string>(&listSLs, name, data, outFile);
                                }
                        }
                        
                        // IF : action is "pop", 
                        // check the SimpleList type 
                        // call the function to retrieve the value popped and modify the SimpleList
                        else {
                                // integer
                                if(type == 'i') {
                                        pop_value<int>(&listSLi, name, outFile);
                                }
                                // double
                                else if(type == 'd') {
                                        pop_value<double>(&listSLd, name, outFile);
                                }
                                // string
                                else {
                                        pop_value<string>(&listSLs, name, outFile);
                                }
                        }
                }

                // Save/Close input and output files 
                inFile.close();
                outFile.close();
        }

        // ELSE : Return Error 
        // input file does not exist
        else {
                outFile << "ERROR: The input file does not exist!" << endl;
        }
}

// Retrieve function
// Retrieve : name of the input file
string get_input() {
        string input;
        cout << "Enter name of input file: ";
        cin >> input;
        return input;
}
// Retrieve function
// Retrieve : name of the output file
string get_output() {
        string output;
        cout << "Enter name of output file: ";
        cin >> output;
        return output;
}

int main() {
        // String1 : input file names
        string s1 = get_input();
        // String2 : output file names
        string s2 = get_output();

        // Call the parse function 
        // Parameter : String1 and String2
        parse_file(s1, s2);

        return 0;
}