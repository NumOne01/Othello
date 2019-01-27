#include<stdio.h>  
#include<stdbool.h>

bool checkLimit(int r,int c){
	if(r>=0 && c>=0 && r<8 && c<8)     //check border limit
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
		if(field[r+(count*row)][c+(count*col)] == opposit)         //if square has opposit piece 

			if(checkLimit(r+((count+1)*row),c+((count+1)*col)) && field[r+((count+1)*row)][c+((count+1)*col)]=='0'){ 
				//if next square in this direction is empty and is in field
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
					for(col=-1;col<=1;col++){       //ckeck pieces in 8 directions
						if(row==0 && col==0);
						else if(checkLimit(i+row,j+col)){
							findDirection(field,i+row,j+col,row,col,player,move);
						}
					}
			}
}
double weight_table(char field[][8],int i, int j){
    switch(i){
        case 0:
            switch(j){
                case 0:return 1.0;            //corners have most score
                case 1:return -0.25;          //X-Squares are worst
                case 2:return 0.1;
                case 3:return 0.05;            //wall squares
                case 4:return 0.05;
                case 5:return 0.1;
                case 6:return -0.25;
                case 7:return 1.0;
                default:;
        }
        ;
        case 1:
            switch(j){
                case 0:return -0.25;
                case 1:return -0.25;
                case 2:return 0.01;
                case 3:return 0.01;
                case 4:return 0.01;
                case 5:return 0.01;
                case 6:return -0.25;
                case 7:return -0.25;
                default:;

            }
        ;
        case 2:
            switch(j){
                case 0:return 0.1;
                case 1:return 0.01;
                case 2:return 0.05;
                case 3:return 0.02;
                case 4:return 0.02;
                case 5:return 0.05;
                case 6:return 0.01;
                case 7:return 0.01;
                default:;
            }
        ;
        case 3:
            switch(j){
                case 0:return 0.05;
                case 1:return 0.01;
                case 2:return 0.05;
                case 3:return 0.02;
                case 4:return 0.02;
                case 5:return 0.05;
                case 6:return 0.01;
                case 7:return 0.1;
                default:;
            }
        ;
        case 4:
            switch(j){
                case 0:return 0.5;
                case 1:return 0.01;
                case 2:return 0.02;
                case 3:return 0.01;
                case 4:return 0.01;
                case 5:return 0.02;
                case 6:return 0.01;
                case 7:return 0.05;
                default:;
            }
        ;
        case 5:
            switch(j){
                case 0:return 0.1;
                case 1:return 0.01;
                case 2:return 0.05;
                case 3:return 0.02;
                case 4:return 0.02;
                case 5:return 0.05;
                case 6:return 0.01;
                case 7:return 0.10;
                default:;
            };
        case 6:
            switch(j){
                case 0:return -0.25;
                case 1:return -0.25;
                case 2:return 0.01;
                case 3:return 0.01;
                case 4:return 0.01;
                case 5:return 0.01;
                case 6:return -0.25;
                case 7:return -0.25;
                default:;
            };
        case 7:
            switch(j){
                case 0:return 1.0;
                case 1:return -0.25;
                case 2:return 0.1;
                case 3:return 0.05;
                case 4:return 0.05;
                case 5:return 0.10;
                case 6:return -0.25;
                case 7:return 1.0;
                default:;
            };
        default:;
    }
}