#include<stdio.h>  
#include<stdbool.h>

#define MAX 900000
#define MIN -90000

void findDirection(char [][8],int,int,int,int,char,int*);
bool checkLimit(int,int);
void findMine(char [][8],char,int*);
int count_piece(char [][8],char);
void copy(char [][8],char [][8]);
int minimax(int,int,bool,char [][8],int*,int*,int [],char);
int evaluation(char [][8],char);
void apply_move(char [][8],char,int,int);
double weight_table(char [][8],int,int);

int main(int argc,char* argv[]){
    char field[8][8],player;
	int best_chose[2];
    int i,j,move=0;
	int *alpha,*beta,max,min;
	max=9000000;
	min=-9000000;
	alpha=&min;
	beta=&max;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
            field[i][j]=argv[i+1][j];
	player=argv[9][0];	
	minimax(1,5,true,field,alpha,beta,best_chose,player);
	printf("%d %d",best_chose[1],best_chose[0]);
return 0;
}
int minimax(int depth,int max_depth,bool is_player,char field[][8],int *alpha,int *beta,int best_chose[2],char player){	
        if(depth==max_depth)
            return evaluation(field,player);
        if(is_player){
            int move=0;
		    findMine(field,player,&move);	
		    if(move==0)
			    return evaluation(field,player);
            int best_score=MIN,score;
            char temp_field[8][8];
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(field[i][j]=='*'){         //ckeck posiibles moves
						copy(temp_field,field);
						apply_move(temp_field,player,i,j);
                        score=minimax(depth+1,max_depth,false,temp_field,alpha,beta,best_chose,'2');

                        if(score>=best_score){          //player wants to increase score
							if(depth==1){
								best_chose[0]=i;
						    	best_chose[1]=j;
							}
                            best_score=score;
						}
                        if(*alpha<=best_score)
                            *alpha=best_score; 
				        if(*beta<=*alpha)      //alpha-beta Pruning
                    		break;
                    }  
                }
            }
            return best_score;
        }
		else{
            int move=0;
            findMine(field,player,&move);	
		    if(move==0)
			    return evaluation(field,'1');
            int best_score=MAX,score;
            char temp_field[8][8];
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(field[i][j]=='*'){    //ckeck posiibles moves
						copy(temp_field,field);
						apply_move(temp_field,player,i,j);
                        score=minimax(depth+1,max_depth,true,temp_field,alpha,beta,best_chose,'1');

                    	if(score<=best_score)       //opposit wants to decrease score
                        	best_score=score;

                    	if(*beta>=best_score)
                        	*beta=best_score; 
						if(*beta<=*alpha)         //alpha-beta Pruning
                    		break;
                    }  
                }
            }
		    return best_score;
        }
}
void copy(char temp[][8],char field[][8]){
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++){
			if(field[i][j]=='*')
				temp[i][j]='0';
			else
				temp[i][j]=field[i][j];
		}
}
int evaluation(char field[][8],char player){
	int count_mobility1=0,score=0;
	char opposit;
	char temp_field1[8][8];
	copy(temp_field1,field);                
	findMine(temp_field1,player,&count_mobility1);           //count possible moves for player
	if(player=='1')
		opposit='2';
	else
		opposit='1';
	int count_mobility2=0;
	char temp_field2[8][8];
	copy(temp_field2,field);
	findMine(temp_field2,opposit,&count_mobility2);               //count possible moves for opposit
	if(count_mobility1==0)
		score-=50-64;
	if(count_mobility2==0)
		score+=50+64;	
    if(count_mobility1==0 && count_mobility2==0){               //if game is ended
        if(count_piece(field,player)>=count_piece(field,opposit))   //if we have more pieces
            score+=9999999999;
    }
	double weight_player=0,weight_opposit=0;
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++){
			if(field[i][j]==player)
				weight_player+=weight_table(field,i,j);      //add up squares values
			else if(field[i][j]==opposit)
				weight_opposit+=weight_table(field,i,j);
		}
	score+=(weight_player-weight_opposit+10)*2000;
	return score;
}
int count_piece(char field[][8],char player){
	int count_piece=0;
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			if(field[i][j]==player)
				count_piece++;
	return count_piece;			
}
void apply_move(char field[][8],char player,int i,int j){
	char opposit;
	if(player=='1')
		opposit='2';
	else
		opposit='1';
	
	field[i][j]=player;
	for(int k=-1;k<=1;k++){
		for(int m=-1;m<=1;m++){
			if(i==j)
				continue;
			int count=1;
				int is=0;
			if(field[i+(count*k)][j+(count*m)] == '0' || field[i+(count*k)][j+(count*m)] == '*')
				continue;
			
			if(field[i+(count*k)][j+(count*m)] == opposit)   {      
				int count1=2;
				while(checkLimit(i+(count1*k),j+(count1*m))){
					if(checkLimit(i+((count1)*k),j+((count1)*m)) && field[i+((count1)*k)][j+((count1)*m)]==player){
						is=1;
						break;
					}
					count1++;
				}
				if(is)
					while(checkLimit(i+(count*k),j+(count*m)) && field[i+((count)*k)][j+((count)*m)]!=player){ 
                		field[i+((count)*k)][j+((count)*m)]=player;
						count++;	
						}
			}
			
			
		}
	}
}