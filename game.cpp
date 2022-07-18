#include <bits/stdc++.h>
#include <chrono>
using namespace std;
#define vint vector<int>
#define f(i,n) for(int i=0;i<n;i++)

const char player_marker = 'X',ai_marker = 'O';
class board{
	public:
	char b[3][3];
	
	board(){
		f(i,3)f(j,3){this->b[i][j]='_';}
	}
	void print_board(){
		f(i,3){
			f(j,3){
				if(j==2){cout<<this->b[i][j]<<"\n"; continue;}
                else cout<<this->b[i][j]<<" | ";
			}
		}
		cout<<"\n";
	}
	
	int evaluate(){
		//checking rows
		f(i,3){
			if(b[i][0]==b[i][1] && b[i][1]==b[i][2] && b[i][0]!='_'){
				return (b[i][0]==player_marker)? +20 : -20 ;
			}
		}
		//checking columns
		f(j,3){
			if(b[0][j]==b[1][j] && b[1][j]==b[2][j] && b[0][j]!='_'){
				return (b[0][j]==player_marker)? +20 : -20 ;
			}
		}
		//checking diagonals
		if(b[0][0]==b[1][1] && b[1][1]==b[2][2] && b[1][1]!='_'){
			return (b[1][1]==player_marker)? +20 : -20 ;
		}

		if(b[0][2]==b[1][1] && b[1][1]==b[2][0] && b[1][1]!='_'){
			return (b[1][1]==player_marker)? +20 : -20 ;
		}
		//no one won yet
		return 0;
	}
	int cal_depth(){
		int d=0;
		f(i,3){f(j,3){
				if(b[i][j]!='_'){	d++;	}
		}}
		return d;
	}
	//returns the best value of evaluation function along with the position which makes the best move
	//first value >0 implies player/maximizer wins, first value<0 implies ai/minimizer wins 
	pair<int,int> minimax(int depth,bool is_this_max_turn){ 
		int score = evaluate();
		// cout<<score<<" "<<depth<<"\n";
		if(score!=0){
			return make_pair(	(score>0)? score-depth : score+depth ,-1);
		}
		if(depth==9){
			return make_pair(score,-1);
		}
		if(is_this_max_turn){
			int max_value = INT_MIN,pos =-1;
			f(i,3){
				f(j,3){
					if(b[i][j]=='_'){
						b[i][j] = 'X';
						int possible_score = minimax(depth+1,!is_this_max_turn).first;
						if(possible_score>max_value){
							max_value = possible_score;
							pos = (i*3) + j;
						}
						b[i][j] = '_';
					}
				}
			}
			return make_pair(max_value,pos);
		}
		else{
			int min_value = INT_MAX,pos = -1;
			f(i,3){
				f(j,3){
					if(b[i][j]=='_'){
						b[i][j] = 'O';
						int possible_score  = minimax(depth+1,!is_this_max_turn).first;
						if(possible_score<min_value){
							min_value = possible_score;
							pos = (3*i) + j; 
						}
						b[i][j] = '_';
					}
				}
			}
			return make_pair(min_value,pos);
		}
	}

	pair<int,int> minimax_with_ABpruning(int depth,bool is_this_max_turn,int alpha, int beta){ 
		int score = evaluate();
		// cout<<score<<" "<<depth<<"\n";
		if(score!=0){
			return make_pair(	(score>0)? score-depth : score+depth ,-1);
		}
		if(depth==9){
			return make_pair(score,-1);
		}
		if(is_this_max_turn){
			int max_value = INT_MIN,pos =-1;
			f(i,3){
				f(j,3){
					if(b[i][j]=='_'){
						b[i][j] = 'X';
						// int possible_score = minimax(depth+1,!is_this_max_turn).first;
						int possible_score = minimax_with_ABpruning(depth+1,!is_this_max_turn,alpha,beta).first;
						if(possible_score>max_value){
							max_value = possible_score;
							pos = (i*3) + j;
							alpha = max(alpha,max_value);
							if(beta<=alpha){
								break;
							}
						}
						b[i][j] = '_';
					}
				}
				if(beta<=alpha){
					break;
				}
			}
			return make_pair(max_value,pos);
		}
		else{
			int min_value = INT_MAX,pos = -1;
			f(i,3){
				f(j,3){
					if(b[i][j]=='_'){
						b[i][j] = 'O';
						// int possible_score  = minimax(depth+1,!is_this_max_turn).first;
						int possible_score  = minimax_with_ABpruning(depth+1,!is_this_max_turn,alpha,beta).first;
						if(possible_score<min_value){
							min_value = possible_score;
							pos = (3*i) + j; 
							beta = min(min_value,beta);
							if(beta<=alpha){
								break;
							}
						}
						b[i][j] = '_';
					}
				}
				if(beta<=alpha){
					break;
				}
			}
			return make_pair(min_value,pos);
		}
	}

	int valid_move(int x,int y,char t){
		if(x>-1 && x<3 && y>-1 && y<3 && this->b[x][y]=='_'){
			return 1;
		}
		else{
			if(x<0 || x>2 || y>2 || y<0)
				cout<<"Invalid input, select valid cell\n";
			else{
				cout<<"Already Occupied position, select some other location\n";
			}
			return 0;
		}
	}
	bool play_game(){
		int d=this->cal_depth(),e= this->evaluate();
		if((d!=9 && e==0)){
			return true;
		}
		else{
			return false;
		}
	}
};

int main(){
	board* tic = new board();
	tic->print_board();
	int turn = 0;
	while (tic->play_game()){
		if(turn%2==0){
			int x,y;
			cout<<"Enter your desired location <X> :: ";
			cin>>x>>y;
			if(tic->valid_move(x-1,y-1,player_marker)){
				tic->b[x-1][y-1] = player_marker;
			}
			else{
				continue;
			}
		}
		else{
			auto start = chrono::high_resolution_clock::now(); 

			pair<int,int> best_move = tic->minimax(turn,false);
			// pair<int,int> best_move = tic->minimax_with_ABpruning(turn,false,INT_MIN,INT_MAX);

			auto end = chrono::high_resolution_clock::now();
			auto time_taken = chrono::duration_cast<chrono::nanoseconds>(end-start).count();
			cout<<"Time taken by computer = "<<fixed<<setprecision(9)<<time_taken*(1e-9)<<" ns and selected "<<(best_move.second/3)+1<<" "<<(best_move.second%3)+1<<"\n";
			
			tic->b[best_move.second/3][best_move.second%3] = ai_marker;
		}
		turn++;
		tic->print_board();
	}

	if(tic->evaluate()>0){
		cout<<"******* GAME OVER *********\n\t\tYou <X> won\n";
	}
	else if(tic->evaluate()==0){
		cout<<"******** DRAW **********\n";
	}
	else{
		cout<<"*******  GAME OVER *********\n\t\tComputer <O> won";
	}

}