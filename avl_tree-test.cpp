/*
 * avl_tree-test.cc    (c) Tommy Olsson, IDA, 2007-05-02
 * Updated by filst04@ida
 */

#include <iostream>
#include "avl_tree.h"

using namespace std;


int main() {
    AVL_Tree<int>  avl_tree;

    for (int i = 1; i <= 11; i++)
        avl_tree.insert(i);

    try {
        cout << "AVL-tree after inserting 1, 2,..., 11:\n\n";
        avl_tree.print_tree(cout);
        cout << endl;
    } catch (const exception& e) {
        cout << '\n' << e.what() << endl;
        cout << "After inserting 1, 2, ..., 11, the AVL tree contains:\n\n";
        avl_tree.print(cout);
        cout << endl;
    }

    unsigned int choice;
    unsigned int value;

    while (true) {
        cout << endl;
        cout << "1 - Insert.\n";
        cout << "2 - Remove.\n";
        cout << "3 - Find value.\n";
        cout << "4 - Find minimum.\n";
        cout << "5 - Find maximum.\n";
        cout << "6 - Empty the tree.\n";
        cout << "7 - Print ordered.\n";
        cout << "8 - Print tree.\n";
        cout << "0 - Exit.\n" << endl;
        cout << "Choice: ";
        if (!(cin >> choice))
            break;
        cout << endl;

        try {
            switch (choice) {
            case 0:
                cout << "Exiting." << endl;
                return 0;
            case 1:
                cout << "Value to insert: ";
                cin >> value;
                avl_tree.insert(value);
                break;
            case 2:
                cout << "Value to remove: ";
                cin >> value;
                avl_tree.remove(value);
                break;
            case 3:
                cout << "Value to find: ";
                cin >> value;
                if (avl_tree.member(value))
                    cout << "The value " << value << " is in the tree." << endl;
                else
                    cout << "The value " << value << " is not in the tree." << endl;
                break;
            case 4:
                if (avl_tree.empty())
                    cout << "The tree is empty!" << endl;
                else
                    cout << "The minimum value in the tree is " << avl_tree.find_min() << endl;
                break;
            case 5:
                if (avl_tree.empty())
                    cout << "The tree is empty!" << endl;
                else
                    cout << "The maximum value in the tree is " << avl_tree.find_max() << endl;
                break;
            case 6:
                avl_tree.clear();
                cout << "The tree is empty!" << endl;
                break;
            case 7:
                if (avl_tree.empty()) {
                    cout << "The tree is empty!" << endl;
                } else {
                    avl_tree.print(cout);
                    cout << endl;
                }
                break;
            case 8:
                if (avl_tree.empty()) {
                    cout << "The tree is empty!" << endl;
                } else {
                    avl_tree.print_tree(cout);
                    cout << endl;
                }
                break;
            default:
                cout << "Incorrect choice!" << '\b' << endl;
                break;
            }
        } catch (const exception& e) {
            cout << e.what() << endl;
        } catch (...) {
            cout << "Caught an unknown exception." << endl;
        }
    }

    return 0;
}
