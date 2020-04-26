#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

//H_n huerisitic functions for A*

int Misplaced_Tile(vector<vector <int> > puzzle_state){

   int H_n = 0;//H(n) value to be plugged into f(n) = H(n) + g(n)

   for(int i = 0; i < puzzle_state.size(); i++){
       for(int j = 0; j < puzzle_state.at(i).size(); j++){
           if((puzzle_state.at(i).at(j) == 0)){
           }else if(((i*puzzle_state.size() + j + 1) != puzzle_state.at(i).at(j))){
               H_n++;
           }else{
           }
       }
   }
   return H_n;
};

void actual_position(int& val, int& x, int& y) {

	switch (val) {
	case 1:
		x = 0;
		y = 0;
		break;

	case 2:
		x = 0;
		y = 1;
		break;

	case 3:
		x = 0;
		y = 2;
		break;

	case 4:
		x = 1;
		y = 2;
		break;

	case 5:
		x = 2;
		y = 2;
		break;

	case 6:
		x = 2;
		y = 1;
		break;

	case 7:
		x = 2;
		y = 0;
		break;

	case 8:
		x = 1;
		y = 0;
		break;

	}
};



int Manhattan_Distance(vector<vector <int> > puzzle_state){

    int H_n = 0;
    int act_x, act_y;

    for(int i = 0; i < puzzle_state.size(); i++){
       for(int j = 0; j < puzzle_state.at(i).size(); j++){
           if(puzzle_state.at(i).at(j) != 0){
               //H_n += abs(   ((puzzle_state.at(i).at(j) - 1) / (puzzle_state.at(i).size())) - i   )  + abs(   ((puzzle_state.at(i).at(j) - 1) % (puzzle_state.at(i).size())) - j   );

               //H_n +=   abs(((puzzle_state.at(i).at(j) - 1) / (puzzle_state.at(i).size())) - i)  +   abs(((puzzle_state.at(i).at(j) - 1) % (puzzle_state.at(i).size())) - j);
                actual_position(puzzle_state.at(i).at(j),act_x,act_y);
                H_n += abs(i-act_x) +abs(j-act_y);

           }

       }

   }
    return H_n;
};

//Node struct and node comparison for the Priority Queue

struct Node{

    vector <vector <int> > puzzle_box;

    int G_n;//g(n) is effectively the depth
    int H_n;//H(n) is iether 1, manhatan distance or mispalce tile huerisitcs

    Node(vector <vector <int> > puzzle_box, int G_n, int H_n): puzzle_box(puzzle_box), G_n(G_n), H_n(H_n){

    }
};



struct Compare_Nodes{

    bool operator()(Node const &a, Node const &b){
        return (a.G_n + a.H_n) >  (b.G_n + b.H_n);
    }

};



bool Compare_Puzzles(Node a, Node b){

//check if puzzle is same
    bool puzzles_are_the_same = true;

    for(int i = 0; i < a.puzzle_box.size(); i++){
        for(int j = 0; j < a.puzzle_box.at(i).size(); j++){
            if(a.puzzle_box.at(i).at(j) != b.puzzle_box.at(i).at(j)){

                puzzles_are_the_same = false;

            }
        }
    }

    return puzzles_are_the_same;
};



bool Puzzle_Repeated(vector<Node> &node_list, Node a){

    for(int i = 0; i < node_list.size(); i++){
        if(Compare_Puzzles(a, node_list.at(i))){
            return true;
        }
    }
    return false;
}



int get_H_n(int puzzle_heuristic, vector< vector<int> > puzzle_board){

    if( puzzle_heuristic == 1){

        return Misplaced_Tile(puzzle_board);

    }else if( puzzle_heuristic == 2){

        return Manhattan_Distance(puzzle_board);

    }else{

        cout<<"ERROR: PUZZLE HEURISITIC NO WITHIN RANGE. Exiting";

        return 0;

    }

}


void Generate_Next_Nodes( priority_queue<Node, vector<Node>, Compare_Nodes> &node_queue, Node &a,vector<Node> &node_list, int puzzle_heuristic){

    int zero_x;
    int zero_y;

    for(int i = 0; i <a.puzzle_box.size();i++){

        for(int j = 0; j < a.puzzle_box.at(i).size(); j++){

            if(a.puzzle_box.at(i).at(j) == 0){

                zero_x = j;
                zero_y = i;

            }

        }

    }

    int temp_swap;

    vector< vector <int> > up = a.puzzle_box;
    vector< vector <int> > down = a.puzzle_box;
    vector< vector <int> > left = a.puzzle_box;
    vector< vector <int> > right = a.puzzle_box;

    //move empty tile up

    if(zero_y-1 >=0){

        //temp_swap = new_puzzle_box.at(zero_y).at(zero_x);

        up.at(zero_y).at(zero_x) = up.at(zero_y -1).at(zero_x);
        up.at(zero_y-1).at(zero_x) = 0;

        Node x = Node(up, a.G_n + 1, get_H_n(puzzle_heuristic, up));

        if(!Puzzle_Repeated(node_list,x)){

            node_queue.push(x);
            node_list.push_back(x);

        }

    }

    //move empty tile down

    if(zero_y+1<=2){

        down.at(zero_y).at(zero_x) = down.at(zero_y +1).at(zero_x);
        down.at(zero_y+1).at(zero_x) = 0;

        Node x1 = Node(down, a.G_n + 1, get_H_n(puzzle_heuristic, down));

        if(!Puzzle_Repeated(node_list, x1)){

            node_queue.push(x1);
            node_list.push_back(x1);

        }

    }

    //move empty tile left

    if(zero_x-1 >=0){

        left.at(zero_y).at(zero_x) = left.at(zero_y).at(zero_x-1);
        left.at(zero_y).at(zero_x-1) = 0;

        Node x2 = Node(left, a.G_n + 1, get_H_n(puzzle_heuristic, left));

        if(!Puzzle_Repeated(node_list, x2)){

            node_queue.push(x2);
            node_list.push_back(x2);

        }

    }

    //move empty tile right

    if(zero_x+1 <=2){

        right.at(zero_y).at(zero_x) = right.at(zero_y).at(zero_x+1);
        right.at(zero_y).at(zero_x+1) = 0;

        Node x3 = Node(right, a.G_n + 1, get_H_n(puzzle_heuristic, right));

        if(!Puzzle_Repeated(node_list, x3)){

            node_queue.push(x3);
            node_list.push_back(x3);

        }

    }
    return;
};



void Print_Node(Node a){

    for(int i = 0; i < a.puzzle_box.size(); i++){

        for(int j = 0; j < a.puzzle_box.at(i).size(); j++){

            cout<<a.puzzle_box.at(i).at(j)<<" ";
        }

        cout<<endl;
    }

    return;
}







int main(){

    string string_parse;

    int int_parse;
    int puzzle_heuristic;
    int max_nodes_in_queue = 0;
    int goal_node_depth;
    int num_of_expanded_nodes = 0;

    vector<vector<int> > input_puzzle (3);//TODO: change hardcoding to softcoding hwere size of array can be anything
    vector<Node> node_list;
    vector< vector<int> > default_puzzle (3);

    default_puzzle.at(0).push_back(1);
    default_puzzle.at(0).push_back(2);
    default_puzzle.at(0).push_back(3);
    default_puzzle.at(1).push_back(4);
    default_puzzle.at(1).push_back(0);
    default_puzzle.at(1).push_back(6);
    default_puzzle.at(2).push_back(7);
    default_puzzle.at(2).push_back(5);
    default_puzzle.at(2).push_back(8);

    vector<vector<int> > goal_puzzle (3);

    goal_puzzle.at(0).push_back(1);
    goal_puzzle.at(0).push_back(2);
    goal_puzzle.at(0).push_back(3);
    goal_puzzle.at(1).push_back(8);
    goal_puzzle.at(1).push_back(0);
    goal_puzzle.at(1).push_back(4);
    goal_puzzle.at(2).push_back(7);
    goal_puzzle.at(2).push_back(6);
    goal_puzzle.at(2).push_back(5);

    Node goal_node = Node(goal_puzzle, 0, 0);

    priority_queue<Node, vector<Node>, Compare_Nodes> node_queue;


    //TODO: sanitize and error check input

    cout<<"*****Welcome to our 8-puzzle solver using A* Algorithm.*****\n\n";
    cout<<"Enter your puzzle, use a zero to represent the blank\n\n";
    cout<<"Enter the first row, use space or tabs between numbers\t\t";

    getline(cin,string_parse);

    stringstream ss1(string_parse);

    while(ss1>>int_parse){

        input_puzzle.at(0).push_back(int_parse);

    }



    cout<<"Enter the second row, use space or tabs between numbers\t\t";

    getline(cin,string_parse);

    stringstream ss(string_parse);

    while(ss>>int_parse){

        input_puzzle.at(1).push_back(int_parse);

    }



    cout<<"Enter the third row, use space or tabs between numbers\t\t";

    getline(cin,string_parse);

    stringstream ss2(string_parse);

    while(ss2>>int_parse){

        input_puzzle.at(2).push_back(int_parse);

    }




    cout<<"\n\tEnter your choice of A* Algorithm Heuristic";
    cout<<"\n\t1. A* with the Misplaced Tile heurisitic.(H1)\n";
    cout<<"\n\t2. A* with the Manhattan Distance heuristic.\n";
    //cout<<"\n\t3. A* with

    getline(cin,string_parse);

    stringstream(string_parse) >> puzzle_heuristic;


    cout<<"\nExpanding State\n\n";

    vector< vector <int> > puzzle_board;

    int heuristic_value;
    int depth = 0;

    puzzle_board = input_puzzle;

    heuristic_value = get_H_n(puzzle_heuristic, puzzle_board);

    Node given_puzzle = Node(puzzle_board, depth ,heuristic_value);

    Print_Node(given_puzzle);

    node_queue.push(given_puzzle);

    while(!node_queue.empty()){

        Node min = node_queue.top();

        if(Compare_Puzzles(goal_node, min)){

            goal_node = min;
            break;

        }

        if(num_of_expanded_nodes == 0){

            //Nothing to do

        }else{

            cout<<"\nThe best state to expand with a g(n) = "<<min.G_n<<" and h(n) = "<<min.H_n<<" is..."<<endl;

        Print_Node(min);

        cout<<"Expanding this node...\n";

        }

        num_of_expanded_nodes++;

        node_queue.pop();

        Generate_Next_Nodes(node_queue, min, node_list ,puzzle_heuristic);

        if(node_queue.size()>max_nodes_in_queue){

            max_nodes_in_queue = node_queue.size();

        }

    }

    cout << "\n";

    Print_Node(node_queue.top()); // displays finished Node

    cout<<"\nDone!\n";
    cout<<"\nTo solve this problem the search algorithm expanded a total of "<<num_of_expanded_nodes<<" nodes.\n";
    cout<<"The maximum number of nodes in the queue at any one time was "<<max_nodes_in_queue<<".\n";
    cout<<"The depth of the goal node was "<<goal_node.G_n<<".\n";

    return 0;

}
