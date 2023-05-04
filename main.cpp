#include <iostream>
#include <algorithm>
#include <fstream>
#include <stack>

enum e_color{
    black,
    red
};

struct nod{
    nod* dad;
    nod *left = nullptr, *right = nullptr;
    e_color color;
    int val;
    nod(int val_, e_color color_, nod* parent_ = nullptr, nod* child_l_ = nullptr, nod* child_r_ = nullptr)
        :val(val_), color(color_), dad(parent_), left(child_l_), right(child_r_){};

};

class tree{
    nod *root;
public:
    tree() : root(nullptr){} /// init root
    nod* getRoot() const
    {
        return root;
    }
    void insert(int x)
    {
        if(!root)
            root = new nod(x, black);
        else
        {
            nod* aux = getRoot();
            nod* ins = new nod(x, red);
            nod* last = nullptr;
            while(aux)
            {
                last = aux;
                if (x > aux->val)
                    aux = aux->right;
                else aux = aux->left;
            }
            ins->dad = last;
            if(last->val < x)
                last->right = ins;
            else last->left = ins;
            /// need to fix balancing

        }
    }
    void print_inorder(nod* start)
    {
        if(start)
        {
            print_inorder(start->left);
            std::cout<<start->val<<' ';
            print_inorder(start->right);
        }
    }
    void insert_fix(nod* ins)
    {
        nod* p = ins->dad;
        while(p->color == red)
        {
            nod* gD = p->dad;
            if(p == gD->left)
            {
                if(gD->right->color == red)
                {
                    gD->left->color = gD->right->color = black;
                    gD->color = red;
                    ins = gD;
                }
                else if()

            }

        }
    }
    void left_rotate(nod* x)
    {
        nod* r = x->right;
        x->right = r->left;
        //x->dad = r;
        r->dad = x->dad;

    }
};

int main() {
//    std::cout << "Hello, World!" << std::endl;
    tree red_black;
    std::ifstream in("../input.in");
    int y;
    while(in>>y)
        red_black.insert(y);
    red_black.print_inorder(red_black.getRoot());
    return 0;
}
