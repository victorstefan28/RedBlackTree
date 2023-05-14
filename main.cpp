#include <iostream>
#include <algorithm>
#include <fstream>
#include <stack>
#include <string>
#include <climits>
std::ifstream f("../abce.in");
std::ofstream g("../abce.out");
enum e_color {
    black,
    red
};

struct nod{
    int val;
    e_color color;
    nod* dad;
    nod *left = nullptr, *right = nullptr;

    nod(int val_, e_color color_, nod* parent_ = nullptr, nod* child_l_ = nullptr, nod* child_r_ = nullptr)
        :val(val_), color(color_), dad(parent_), left(child_l_), right(child_r_){};
    std::string getColor() const
    {
        return color ? "Red" : "Black";
    }
};

class tree{
    nod *root;
    nod *null_leaf; /// fix segfault in insertfix
public:
    tree() : root(nullptr)
    {
        null_leaf = new nod(-INT_MAX, black, nullptr, nullptr, nullptr);
    } /// init null leaf
    nod* getRoot() const
    {
        return root;
    }
    void insert(int x)
    {
        if(!root)
            root = new nod(x, black, nullptr, null_leaf, null_leaf);
        else
        {
            nod* aux = getRoot();
            nod* ins = new nod(x, red, nullptr, null_leaf, null_leaf);
            nod* last = nullptr;
            while(aux != null_leaf)
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
            if(!ins->dad->dad)
                return;
            /// need to fix balancing
            insert_fix(ins);
        }
    }
    void print_inorder(nod* start)
    {
        if(start && start != null_leaf)
        {
            print_inorder(start->left);
            std::cout<<start->val<<' '<<start->getColor()<<' '<<'\n';
            print_inorder(start->right);
        }
    }
    void insert_fix(nod* ins)
    {

        while(ins->dad->color == red)
        {
            //nod* p = ins->dad;

            if(ins->dad == ins->dad->dad->left)
            {
                if(ins->dad->dad->right->color == red)
                {
                    ins->dad->dad->left->color = ins->dad->dad->right->color = black;
                    ins->dad->dad->color = red;
                    ins = ins->dad->dad;
                }
                else
                {
                    if (ins == ins->dad->right)
                    {
                        ins = ins->dad;
                        left_rotate(ins);
                    }
                    ins->dad->color = black;
                    ins->dad->dad->color = red;
                    right_rotate(ins->dad->dad);
                }
            }
            else
            {
                if(ins->dad->dad->left->color == red)
                {
                    ins->dad->dad->left->color = ins->dad->dad->right->color = black;
                    ins->dad->dad->color = red;
                    ins = ins->dad->dad;
                }
                else
                {
                    if (ins == ins->dad->left)
                    {
                        ins = ins->dad;
                        right_rotate(ins);
                    }
                    ins->dad->color = black;
                    ins->dad->dad->color = red;
                    left_rotate(ins->dad->dad);
                }
            }
            if(ins == root)
                break;
        }
        root->color = black;
    }
    void transplant(nod* a, nod* b)
    {
        if(!a->dad) /// a is root
            root = b;
        else if(a == a->dad->left)
            a->dad->left = b;
        else a->dad->right = b;
        b->dad = a->dad;
    }
    void left_rotate(nod* x)
    {
        nod* r = x->right;
        x->right = r->left;
        //x->dad = r;
        if(r->left != null_leaf)
            r->left->dad = x;
        r->dad = x->dad;
        if(!x->dad)
            this->root = r;
        else if(x == x->dad->left)
            x->dad->left = r;
        else x->dad->right = r;
        r->left = x;
        x->dad = r;
    }
    void right_rotate(nod* x)
    {
        nod* l = x->left;
        x->left = l->right;
        //x->dad = r;
        if(l->right != null_leaf)
            l->right->dad = x;
        l->dad = x->dad;
        if(!x->dad)
            this->root = l;
        else if(x == x->dad->right)
            x->dad->right = l;
        else x->dad->left = l;
        l->right = x;
        x->dad = l;
    }
    nod* min_v(nod* start, int lower_bound = -INT_MAX)
    {
        while(start->left != null_leaf && start->left->val >= lower_bound)
            start = start->left;
        return start;
    }

    void delete_value(int x)
    {
        nod* start = getRoot();
        nod* to_del = null_leaf;
        nod *aux, *y;
        ///std::cout<<"Delete";
        while(start != null_leaf)
        {
            if(start->val == x)
                to_del = start;
            if(start->val < x)
                start = start->right;
            else
                start = start->left;

        }

        if(to_del == null_leaf)
        {
            ///std::cout<<"can't find value to delete"<<'\n';
            return; // the value does not exist in the tree
        }

        e_color o_color = to_del->color;
        if(to_del->left == null_leaf)
        {
            aux = to_del->right;
            transplant(to_del, to_del->right);
        }
        else if(to_del->right == null_leaf)
        {
            aux = to_del->left;
            transplant(to_del, to_del->left);
        }
        else
        {
            y = min_v(to_del->right);
            o_color = y->color;
            aux = y->right;
            if(y->dad != to_del)
            {
                transplant(y, y->right);
                y->right = to_del->right;
                y->right->dad = y;
            }
            else
                aux->dad = y;
            transplant(to_del, y);
            y->left = to_del->left;
            y->left->dad = y;
            y->color = to_del->color;
        }

        if(o_color == black && aux != null_leaf)
            delete_balance(aux);// delete balance
    }
    void delete_balance(nod* x)
    {
        if(x == null_leaf)
            return;
        while(x!=root&&x->color == black)
        {
            if(x==x->dad->left)
            {
                nod* w = x->dad->right;

                if(w->color == red)
                {
                    w->color = black;
                    x->dad->color = red;
                    left_rotate(x->dad);
                    w = x->dad->right;
                }

                else if((w->right == nullptr && w->left == nullptr) || (w->left->color == black && w->right->color == black))
                {
                    w->color = red;
                    x = x->dad;
                }
                else{
                    if(w->right->color == black) {
                        w->left->color = black;
                        w->color = red;
                        right_rotate(w);
                        w = x->dad->right;
                    }
                    w->color = x->dad->color;
                    x->dad->color = black;
                    w->right->color = black;
                    left_rotate(x->dad);
                    x = root;
                }
            }
            else
            {
                nod* w = x->dad->left;
                if(w->color == red)
                {
                    x->dad->left->color = black;
                    x->dad->color = red;
                    right_rotate(x->dad);
                    w = x->dad->left;
                }
                if(w->right == nullptr && w->left == nullptr)
                {
                    w->color = red;
                    x = x->dad;
                }
                else if(w->left->color == black && w->right->color == black)
                {
                    w->color = red;
                    x = x->dad;
                }
                else {
                    if (w->left->color == black) {
                        w->right->color = black;
                        w->color = red;
                        left_rotate(w);
                        w = x->dad->left;
                    }

                    w->color = x->dad->color;
                    x->dad->color = black;
                    w->left->color = black;
                    right_rotate(x->dad);
                    x = root;
                }
            }
        }
        x->color = black;
    }
    bool search(int x)
    {
        return search_rec(root, x) == null_leaf ? 0 : 1;
    }
    nod* search_rec(nod* start, int x)
    {
        if(start == nullptr)
            return 0;
        if(start == null_leaf)
            return start;
        if(start->val == x)
            return start;
        else if(start->val > x)
            return search_rec(start->left, x);
        else return search_rec(start->right, x);
    }
    int min_higher(nod* start, int x)
    {
        int up = INT_MAX;
        while(start != null_leaf)
        {
            if(x<=start->val)
                up = std::min(up, start->val), start = start->left;
            else
                start = start->right;
        }
        return up;
    }
    int max_lower(nod* start, int x)
    {
        int up = -INT_MAX;
        while(start != null_leaf)
        {
            if(x>=start->val)
                up = std::max(up, start->val), start = start->right;
            else
                start = start->left;
        }
        return up;
    }
    void sort_low_high(nod* start, int x, int y, std::ostream& os)
    {
        if(start && start != null_leaf)
        {
            sort_low_high(start->left,x,y,os);
            if(start->val >= x && start->val <= y)
                os<<start->val<<' ';
            sort_low_high(start->right,x,y,os);
        }

    }
};

int main() {

    tree red_black;
    std::ifstream in("../input.in");

//    int y;
//    while(in>>y)
//        red_black.insert(y);
//    red_black.print_inorder(red_black.getRoot());
    int n; // nr operatii - Q
    f>>n;
    for(int i = 0; i<n; i++)
    {
        int tip, val;
        f>>tip;
        switch(tip){
            case 1:
                f>>val;
                red_black.insert(val);
                break;
            case 2:
                f>>val;
                red_black.delete_value(val);
                ///std::cout<<"Finished delete";
                break;
            case 3:
                f>>val;
                g<<red_black.search(val)<<'\n';
                break;
            case 4: {
                int low;
                f >> low;
                //g<<(red_black.max_v(red_black.getRoot(), low))->val;

//                auto retr = red_black.search_max(red_black.getRoot(), low, nullptr);//red_black.cerinta_4(red_black.getRoot(), low);
//                std::cout << "low is: " << low << '\n';
//                g<< retr->val<<'\n';
                //int x;
                //red_black.max_lower(red_black.getRoot(), low, false, x);
                g<<red_black.max_lower(red_black.getRoot(), low)<<'\n';

            }
                break;
            case 5:
            {
                int high;
                f >> high;

                std::cout << "high is: " << high << '\n';
                g<<red_black.min_higher(red_black.getRoot(), high)<<'\n';
            }
                //g<<(red_black.min_v(red_black.getRoot(), high))->val;
                break;
            case 6:
                int x,y;
                f>>x>>y;
                red_black.sort_low_high(red_black.getRoot(), x,y, g);
                g<<'\n';
                break;
            default:
                break;

        }


    }
    red_black.print_inorder(red_black.getRoot());
    f.close();
    g.close();
    return 0;
}
