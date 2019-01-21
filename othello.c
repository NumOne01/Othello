#include<stdio.h>  
#include<stdbool.h>
void findDirection(char [][8],int,int,int,int,char,int*);
bool checkLimit(int,int);
void findMine(char [][8],char,int*);
void printField(char [][8]);
int count_piece(char [][8],char);
void copy(char [][8],char [][8]);
int stable_pieces(char [][8],char);
int minimax(char [][8],int,char [2],char);
int count_corners(char [][8],char);
int evaluation(char [][8],char);
int interial_pieces(int,int,char [][8]);
int main(int argc,char* argv[]){
    char field[8][8],player;
	char best_chose[2];
    int i,j,move=0;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
            field[i][j]=argv[i+1][j];
	player='1';	
	minimax(field,0,best_chose,player);
	//printf("**%c\n",player);
	//findMine(field,player,&move);
	printf("%d %d",best_chose[1],best_chose[0]);
	//printField(field);
return 0;
}
bool checkLimit(int r,int c){
	if(r>=0 && c>=0 && r<8 && c<8)
		return true;
	else
		return false;	
}

void findDirection(char field[][8],int r,int c,int row,int col,char player,int *move){
	int count=0,opposit;
	if(player=='1')
		opposit='2';
	else
		opposit='1';	
	while(checkLimit(r+(count*row),c+(count*col))){
		if(field[r+(count*row)][c+(count*col)] == '0' || field[r+(count*row)][c+(count*col)] == '*'){
			break;
		}
		if(field[r+(count*row)][c+(count*col)] == opposit)                     
			if(checkLimit(r+((count+1)*row),c+((count+1)*col)) && field[r+((count+1)*row)][c+((count+1)*col)]=='0'){
                field[r+((count+1)*row)][c+((count+1)*col)]='*';
				(*move)++;	
			}
        count++;
	}
}

void findMine(char field[][8],char player,int *move){
	int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			if(field[i][j]==player){
				int row,col;
				for(row=-1;row<=1;row++)
					for(col=-1;col<=1;col++){
						if(row==0 && col==0);
						else if(checkLimit(i+row,j+col)){
							findDirection(field,i+row,j+col,row,col,player,move);
						}
					}
			}
}

void printField(char field[][8]){
    int i,j;
    for(i=0;i<8;i++){
        for(j=0;j<8;j++)
            printf("%c ",field[i][j]);
        puts("");
    }
}
int minimax(char field[][8],int best_score,char best_chose[2],char player){	
		int move=0;
		findMine(field,player,&move);	
		if(move==0)
			return evaluation(field,player);
		else{
			char temp_field[8][8];
			for(int i=0;i<8;i++)
				for(int j=0;j<8;j++)
					if(field[i][j]=='*'){
						copy(temp_field,field);
						temp_field[i][j]=player;
						int score=evaluation(temp_field,player);
						if(score>best_score){
							best_score=score;
							best_chose[0]=i;
							best_chose[1]=j;
						}
					}
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
	findMine(temp_field1,player,&count_mobility1);
	if(player=='1')
		opposit='2';
	else
		opposit='1';
	int count_mobility2=0;
	char temp_field2[8][8];
	copy(temp_field2,field);
	findMine(temp_field2,opposit,&count_mobility2);
	if(count_mobility1==0)
		score-=50-64;
	if(count_mobility2==0)
		score+=50+64;	
	int count_piece1=count_piece(field,player);
	int count_piece2=count_piece(field,opposit);
	int player_in = 0; int opposit_in = 0; // counts for open spaces neighboring a player/comp's pieces
	for (int i=1; i<9;i++) 
		for (int j=1; j<9; j++) {
			if (field[i][j] == player) {
				//add count to sc
				player_in += interial_pieces(i,j,field);
			}
			if (field[i][j] == opposit) {
				//add count to sp
				opposit_in += interial_pieces(i,j,field);
			}
		}

	score+=(opposit_in-player_in+64)*10;
	int count_corners1=count_corners(field,player);
	int count_corners2=count_corners(field,opposit);
	score+=(count_corners1-count_corners2+64)*200+(count_mobility1-count_mobility2+64)*20+(count_piece1-count_piece2+64)*10;
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
int count_corners(char field[][8],char player){
	int count=0;
	if(field[0][0]==player)
		count++;
	if(field[0][7]==player)
		count++;	
	if(field[0][0]==player)
		count++;
	if(field[0][0]==player)
		count++;
	return count;
}

int interial_pieces(int i, int j,char field[][8]) {
	int count = 0;
	if ((i+1)>0 && j>0 && (i+1)<9 && j<9 && field[i+1][j] == 0)
		count++;
	if ((i+1)>0 && (j-1)>0 && (i+1)<9 && (j-1)<9 && field[i+1][j+1] == 0)
		count++;
	if (i>0 && (j-1)>0 && i<9 && (j-1)<9 && field[i][j-1] == 0)
		count++;
	if ((i-1)>0 && (j-1)>0 && (i-1)<9 && (j-1)<9 && field[i-1][j-1] == 0)
		count++;
	if ((i-1)>0 && j>0 && (i-1)<9 && j<9 && field[i-1][j] == 0)
		count++;
	if ((i-1)>0 && (j+1)>0 && (i-1)<9 && (j+1)<9 && field[i-1][j+1] == 0)
		count++;
	if (i>0 && (j+1)>0 && i<9 && (j+1)<9 && field[i][j+1] == 0)
		count++;
	if ((i+1)>0 && (j+1)>0 && (i+1)<9 && (j+1)<9 && field[i+1][j+1] == 0)
		count++;

	return count;
	}

