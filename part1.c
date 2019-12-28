#include <stdio.h>
#include <stdbool.h>

#define MAX 900000
#define MIN -900000

void findDirection(char[][8], int, int, int, int, char, int *);
bool checkLimit(int, int);
void findMine(char[][8], char, int *);
int countPiece(char[][8], char);
void copy(char[][8], char[][8]);
int minimax(int, int, bool, char[][8], int *, int *, int[], char);
int evaluation(char[][8], char);
void applyMove(char[][8], char, int, int);
double weightTable(int, int);
char getOpposite(int);

double weight[][8] = {
    {1.0, -0.25, 0.1, 0.05, 0.05, 0.1, -0.25, 1.0},
    {-0.25, -0.25, 0.01, 0.01, 0.01, 0.01, -0.25, -0.25},
    {0.1, 0.01, 0.05, 0.02, 0.02, 0.05, 0.01, 0.01},
    {0.05, 0.01, 0.05, 0.02, 0.02, 0.05, 0.01, 0.1},
    {0.5, 0.01, 0.02, 0.01, 0.01, 0.02, 0.01, 0.05},
    {0.1, 0.01, 0.05, 0.02, 0.02, 0.05, 0.01, 0.10},
    {-0.25, -0.25, 0.01, 0.01, 0.01, 0.01, -0.25, -0.25},
    {1.0, -0.25, 0.1, 0.05, 0.05, 0.1, -0.25, 1}};

int main(int argc, char *argv[])
{
    char field[8][8], player;
    int bestChoose[2];
    int i, j, move = 0;
    int *alpha, *beta, max, min;
    max = MAX;
    min = -MIN;
    alpha = &min;
    beta = &max;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            field[i][j] = argv[i + 1][j];
    player = argv[9][0];
    minimax(1, 5, true, field, alpha, beta, bestChoose, player);
    printf("%d %d", bestChoose[1], bestChoose[0]);
    return 0;
}

int minimax(int depth, int maxDepth, bool isPlayer, char field[][8], int *alpha, int *beta, int best_chose[2], char player)
{
    if (depth == maxDepth)
        return evaluation(field, player);
    if (isPlayer)
    {
        int move = 0;
        findMine(field, player, &move);
        if (move == 0)
            return evaluation(field, player);
        int bestScore = MIN, score;
        char temp_field[8][8];
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (field[i][j] == '*')
                { //ckeck posiibles moves
                    copy(temp_field, field);
                    applyMove(temp_field, player, i, j);
                    score = minimax(depth + 1, maxDepth, false, temp_field, alpha, beta, best_chose, getOpposite(player));

                    if (score >= bestScore)
                    { //player wants to increase score
                        if (depth == 1)
                        {
                            best_chose[0] = i;
                            best_chose[1] = j;
                        }
                        bestScore = score;
                    }
                    if (*alpha <= bestScore)
                        *alpha = bestScore;
                    if (*beta <= *alpha) //alpha-beta Pruning
                        break;
                }
            }
        }
        return bestScore;
    }
    else
    {
        int move = 0;
        findMine(field, player, &move);
        if (move == 0)
            return evaluation(field, player);
        int bestScore = MAX, score;
        char temp_field[8][8];
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (field[i][j] == '*')
                { //ckeck posiibles moves
                    copy(temp_field, field);
                    applyMove(temp_field, player, i, j);
                    score = minimax(depth + 1, maxDepth, true, temp_field, alpha, beta, best_chose, getOpposite(player));

                    if (score <= bestScore) //opposit wants to decrease score
                        bestScore = score;

                    if (*beta >= bestScore)
                        *beta = bestScore;
                    if (*beta <= *alpha) //alpha-beta Pruning
                        break;
                }
            }
        }
        return bestScore;
    }
}

char getOpposite(int player)
{
    return player == 1 ? '2' : '1';
}

bool checkLimit(int r, int c)
{
    if (r >= 0 && c >= 0 && r < 8 && c < 8) //check border limit
        return true;
    else
        return false;
}

void findDirection(char field[][8], int r, int c, int row, int col, char player, int *move)
{
    int count = 0, opposit = getOpposite(player);
    while (checkLimit(r + (count * row), c + (count * col)))
    {

        if (field[r + (count * row)][c + (count * col)] == '0' || field[r + (count * row)][c + (count * col)] == '*')
        {
            break;
        }
        if (field[r + (count * row)][c + (count * col)] == opposit) //if square has opposit piece

            if (checkLimit(r + ((count + 1) * row), c + ((count + 1) * col)) && field[r + ((count + 1) * row)][c + ((count + 1) * col)] == '0')
            {
                //if next square in this direction is empty and is in field
                field[r + ((count + 1) * row)][c + ((count + 1) * col)] = '*';
                (*move)++;
            }

        count++;
    }
}

void findMine(char field[][8], char player, int *move)
{
    int i, j;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            if (field[i][j] == player)
            {
                int row, col;
                for (row = -1; row <= 1; row++)
                    for (col = -1; col <= 1; col++)
                    { //ckeck pieces in 8 directions
                        if (row == 0 && col == 0)
                            ;
                        else if (checkLimit(i + row, j + col))
                        {
                            findDirection(field, i + row, j + col, row, col, player, move);
                        }
                    }
            }
}
double weightTable(int i, int j)
{
    return weight[i][j];
}

void copy(char temp[][8], char field[][8])
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (field[i][j] == '*')
                temp[i][j] = '0';
            else
                temp[i][j] = field[i][j];
        }
}
int evaluation(char field[][8], char player)
{
    int countMobility1 = 0, score = 0;
    char opposit = getOpposite(player);
    char tempField1[8][8];
    copy(tempField1, field);
    findMine(tempField1, player, &countMobility1); //count possible moves for player
    int countMobility2 = 0;
    char temp_field2[8][8];
    copy(temp_field2, field);
    findMine(temp_field2, opposit, &countMobility2); //count possible moves for opposit
    if (countMobility1 == 0)
        score -= 50 - 64;
    if (countMobility2 == 0)
        score += 50 + 64;
    if (countMobility1 == 0 && countMobility2 == 0)
    {                                                                //if game is ended
        if (countPiece(field, player) >= countPiece(field, opposit)) //if we have more pieces
            score += 9999999999;
    }
    double weightPlayer = 0, weightOpposit = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (field[i][j] == player)
                weightPlayer += weightTable(i, j); //add up squares values
            else if (field[i][j] == opposit)
                weightOpposit += weightTable(i, j);
        }
    score += (weightPlayer - weightOpposit + 10) * 2000;
    return score;
}
int countPiece(char field[][8], char player)
{
    int countPiece = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (field[i][j] == player)
                countPiece++;
    return countPiece;
}
void applyMove(char field[][8], char player, int i, int j)
{
    char opposit = getOpposite(player);

    field[i][j] = player;
    for (int k = -1; k <= 1; k++)
    {
        for (int m = -1; m <= 1; m++)
        {
            if (i == j)
                continue;
            int count = 1;
            int is = 0;
            if (field[i + (count * k)][j + (count * m)] == '0' || field[i + (count * k)][j + (count * m)] == '*')
                continue;

            if (field[i + (count * k)][j + (count * m)] == opposit)
            {
                int count1 = 2;
                while (checkLimit(i + (count1 * k), j + (count1 * m)))
                {
                    if (checkLimit(i + ((count1)*k), j + ((count1)*m)) && field[i + ((count1)*k)][j + ((count1)*m)] == player)
                    {
                        is = 1;
                        break;
                    }
                    count1++;
                }
                if (is)
                    while (checkLimit(i + (count * k), j + (count * m)) && field[i + ((count)*k)][j + ((count)*m)] != player)
                    {
                        field[i + ((count)*k)][j + ((count)*m)] = player;
                        count++;
                    }
            }
        }
    }
}
