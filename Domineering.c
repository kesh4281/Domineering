#include<stdio.h>
#include<stdlib.h>
#define HORIZONTAL 1
#define VERTICAL 2
#define BOARD_SIZE 13
#define PIECE_PLACED 1
#define PIECE_NOT_PLACED 0
int minimax(int depth,int move,int scores);
int board[BOARD_SIZE][BOARD_SIZE];
int eye[BOARD_SIZE+2][BOARD_SIZE+2],global_max=0,no_of_moves=0,lock=1;
int our_domino,oppo_domino,Diff_Moves[BOARD_SIZE][BOARD_SIZE];
void Possible_Half_Moves_Func(int our_domino);
int Possible_Half_Moves[13][13],against_oppo_move;
int our_book_pos[BOARD_SIZE][BOARD_SIZE],oppo_book_pos[BOARD_SIZE][BOARD_SIZE],pieces_placed_distinct[BOARD_SIZE][BOARD_SIZE],TERMINAL;
void Possible_Moves();
int FindBestMove(int );
int Save_Possible_Move[BOARD_SIZE][BOARD_SIZE];
void main()
{
    int Possible_Bomb_Location[BOARD_SIZE][BOARD_SIZE];
    int our_book_pos_count=0,oppo_book_pos_count=0,ty;;
    int best_move=0;
    int completely_filled_bomb_pos[85];
    int my_team_safe_bomb_pos[85],oppo_team_safe_bomb_pos[85];
    int oppo_bomb=1,my_bomb=1;
    int oppo_bomb_pos_i,oppo_bomb_pos_j;
    int x,i,j,bomb,current_turn,c=-1,a,b;
    for(i=0;i<BOARD_SIZE+2;i++)
    {
        eye[0][i]=PIECE_PLACED;
        eye[i][0]=PIECE_PLACED;
        eye[14][i]=PIECE_PLACED;
        eye[i][14]=PIECE_PLACED;
    }

        scanf("%d",&our_domino);
        if(our_domino==HORIZONTAL)
            TERMINAL=18;
        else
            TERMINAL=20;
        oppo_domino=((our_domino!=HORIZONTAL)?HORIZONTAL:VERTICAL);
        int y=501,z=-999,q=2;
        for(i=0;i<13;i++)
            for(j=0;j<13;j++)
            {
                scanf("%d",&x);
                if(x==1)
                    {
                        current_turn=board[i][j];
                        if(Diff_Moves[i][j]==0)
                        {
                            Diff_Moves[i][j]=Diff_Moves[i][j+1]=y++;
                            no_of_moves++;
                        }
                        board[i][j]=HORIZONTAL;
                        eye[i+1][j+1]=PIECE_PLACED;

                    }
                else if(x==2)
                    {
                        if(Diff_Moves[i][j]==0)
                        {
                            Diff_Moves[i][j]=Diff_Moves[i+1][j]=z++;
                            no_of_moves++;
                        }
                        current_turn=board[i][j];
                        board[i][j]=VERTICAL;
                        eye[i+1][j+1]=PIECE_PLACED;

                    }
                else if(x==0)
                    {
                        Diff_Moves[i][j]=q++;
                        board[i][j]=0;
                        eye[i+1][j+1]=0;
                    }
            }
        scanf("%d",&bomb);
        if(bomb==1)
        {
            best_move=Max_Bomb_Gain(oppo_domino);
            if(best_move>=0)
            {
                printf("2 %d %d",best_move/100,best_move%100);
                exit(1);
            }
        }
        int Final_max;
        FindBestMove(oppo_domino);
        Final_max=global_max;
        if(global_max==20)
        {
            lock=0;
            FindBestMove(our_domino);
            lock=1;
            for(i=0;i<13;i++)
            {
                for(j=0;j<13;j++)
                {
                if(Save_Possible_Move[i][j]==1)
                {

                    if(our_domino==HORIZONTAL)
                    {
                        board[i][j]=board[i][j+1]=eye[i+1][j+1]=eye[i+1][j+2]=1;
                    }
                    if(our_domino==VERTICAL)
                    {
                        board[i][j]=board[i+1][j]=eye[i+1][j+1]=eye[i+2][j+1]=1;
                    }
                    FindBestMove(oppo_domino);
                    if(global_max==10)
                    {
                        printf("1 %d %d",i,j);
                        exit(1);
                    }
                    else
                    {
                        if(our_domino==HORIZONTAL)
                            board[i][j]=board[i][j+1]=eye[i+1][j+1]=eye[i+1][j+2]=0;
                        if(our_domino==VERTICAL)
                            board[i][j]=board[i+1][j]=eye[i+1][j+1]=eye[i+2][j+1]=0;
                    }
                }
            }
            }
        }

        best_move=FindBestMove(our_domino);
        if(best_move<0)
        {
            Possible_Half_Moves_Func(our_domino);
            best_move=Best_Half_Move(our_domino);
            if(best_move>=0)
            {
                printf("1 %d %d",best_move/100,best_move%100);
                exit(1);
            }
        }
        if(best_move<0)
        {
            for(i=0;i<13;i++)
            for(j=0;j<13;j++)
            {
                if(our_domino==1)
                    if(j<=11&&board[i][j]==0&&board[i][j+1]==0)
                    {
                        printf("1 %d %d",i,j);
                        exit(1);
                    }
                if(our_domino==2)
                    if(i<=11&&board[i][j]==0&&board[i+1][j]==0)
                    {
                        printf("1 %d %d",i,j);
                        exit(1);
                    }
            }
        }
        printf("1 %d %d",best_move/100-1,best_move%100-1);
}

int minimax(int depth,int whose_chance,int scores)
{
    int new_move_location=-1000;
    int i,j,max_score=-100;;
    if(depth==1)
    return scores;


    for(i=0;i<13;i++)
    for(j=0;j<13;j++)
    if(lock==0)
    Save_Possible_Move[i][j]=0;

    if(whose_chance==HORIZONTAL)
    for(i=0;i<15;i++)
    for(j=1;j<14;j++)
    {
        {
            scores=0;
            if(j>0&&j<14&&i<=12&&eye[i][j]!=0&&eye[i][j+1]!=0&&eye[i+1][j]==0&&eye[i+1][j+1]==0&&eye[i+2][j]==0&&eye[i+2][j+1]==0)
                {
                    scores+=10;
                    if(i<=10&&eye[i+3][j]==0&&eye[i+3][j+1]==0&&eye[i+4][j]!=0&&eye[i+4][j+1]!=0)
                    {
                        scores+=10;
                    }
                }
                if(scores!=0&&max_score<scores)
                {
                    global_max=scores;
                    max_score=scores;
                    new_move_location=(i+2)*100+j;
                }
                if(scores>0&&lock==0)
                    Save_Possible_Move[i+1][j-1]=1;

                scores=0;
            if(j>0&&j<14&&i>=2&&eye[i][j]!=0&&eye[i][j+1]!=0&&eye[i-1][j]==0&&eye[i-1][j+1]==0&&eye[i-2][j]==0&&eye[i-2][j+1]==0)
                {
                    scores+=10;
                    if(i>=4&&eye[i-3][j]==0&&eye[i-3][j+1]==0&&eye[i-4][j]!=0&&eye[i-4][j+1]!=0)
                    {
                        scores+=10;
                    }
                }
                if(scores!=0&&max_score<scores)
                {
                    global_max=scores;
                    max_score=scores;
                    new_move_location=(i-2)*100+j;
                }
                if(scores>0&&lock==0)
                    Save_Possible_Move[i-3][j-1]=1;
        }
    }

    for(i=0;i<13;i++)
    for(j=0;j<13;j++)
    if(lock==0)
    Save_Possible_Move[i][j]=0;

    if(whose_chance==VERTICAL)
    for(i=1;i<14;i++)
    for(j=0;j<15;j++)
    {
        {
            scores=0;
            if(i>=1&&i<=13&&j>=0&&j<=12&&eye[i][j]!=0&&eye[i+1][j]!=0&&eye[i][j+1]==0&&eye[i+1][j+1]==0&&eye[i][j+2]==0&&eye[i+1][j+2]==0)
                {
                    scores+=10;
                    if(j<=10&&eye[i][j+3]==0&&eye[i+1][j+3]==0&&eye[i][j+4]!=0&&eye[i+1][j+4]!=0)
                    {
                        scores+=10;
                    }
                }

                if(scores!=0&&max_score<scores)
                {
                    global_max=scores;
                    max_score=scores;
                    new_move_location=(i)*100+(j+2);
                }
                if(scores>0&&lock==0)
                   {
                       Save_Possible_Move[i-1][j+1]=1;
                   }

            scores=0;
            if(i>=1&&i<=13&&j>=2&&j<=14&&eye[i][j]!=0&&eye[i+1][j]!=0&&eye[i][j-1]==0&&eye[i+1][j-1]==0&&eye[i][j-2]==0&&eye[i+1][j-2]==0)
                {
                    scores+=10;
                    if(j>=10&&eye[i][j-3]==0&&eye[i+1][j-3]==0&&eye[i][j-4]!=0&&eye[i+1][j-4]!=0)
                    {
                        scores+=10;
                    }
                }
                if(scores!=0&&max_score<scores)
                {
                    global_max=scores;
                    max_score=scores;
                    new_move_location=(i)*100+j-2;
                }
                if(scores>0&&lock==0)
                    {
                        Save_Possible_Move[i-1][j-3]=1;
                    }

        }
    }

    if(whose_chance==HORIZONTAL)
    {
        minimax(depth+1,VERTICAL,scores);
    }
    else if(whose_chance==VERTICAL)
    {
        minimax(depth+1,HORIZONTAL,scores);
    }
    return new_move_location;
}

int FindBestMove(int our_domino)
{
    int new_move_location=-1000,max,current_move_score,i,j;
    return new_move_location=minimax(0,our_domino,0);
}

int Max_Bomb_Gain(int oppo_domino)
{
    int check=1,score=0,i,j,m,n,max_score=0,new_move_location=-100,mul=1;
    if(oppo_domino==VERTICAL)
    mul=-1;
    for(i=0;i<=8;i++)
    for(j=0;j<=8;j++)
    {
        score=0;
        check=1;
        for(m=i;m<5+i;m++)
        for(n=j;n<5+j;n++)
        {
            if(Diff_Moves[m][n]*mul>=500)
                score++;
            if((Diff_Moves[m][n]*mul)<=-500)
                score--;
        }
        for(m=0;m<5;m++)
        {
            if(i>0&&Diff_Moves[i][m+j]==Diff_Moves[i-1][m+j])
            check=0;
            if((i+4)<12&&Diff_Moves[i+4][m+j]==Diff_Moves[i+5][m+j])
            check=0;
            if(j>0&&Diff_Moves[m+i][j]==Diff_Moves[m+i][j-1])
            check=0;
            if((j+4)<12&&Diff_Moves[m+i][j+4]==Diff_Moves[m+i][j+5])
            check=0;
            if(check==0)
                break;
        }
        if(no_of_moves<=TERMINAL&&score==12&&check==1)
        {
            new_move_location=100*i+j;
            return new_move_location;
        }
        if(no_of_moves>TERMINAL&&score>=8&&check==1)
        {
            new_move_location=100*i+j;
            return new_move_location;
        }
    }
    return new_move_location;
}
void Possible_Half_Moves_Func(int our_domino)
{
    int i,j;
    if(our_domino==HORIZONTAL)
    for(i=2;i<15;i++)
    for(j=0;j<15;j++)
    {
        if(((eye[i-1][j]!=0&&eye[i-1][j+1]==0)||(eye[i-1][j]==0&&eye[i-1][j+1]!=0))&&eye[i][j]==0&&eye[i][j+1]==0)
        {
            Possible_Half_Moves[i-1][j-1]=1;
        }
        if(((eye[i+1][j]==0&&eye[i+1][j+1]!=0)||(eye[i+1][j]!=0&&eye[i+1][j+1]==0))&&eye[i][j]==0&&eye[i][j+1]==0)
        {
            Possible_Half_Moves[i-1][j-1]=1;
        }
    }
    if(our_domino==VERTICAL)
    for(i=0;i<15;i++)
    for(j=0;j<15;j++)
    {
        if(((eye[i][j-1]!=0&&eye[i+1][j-1]==0)||(eye[i][j-1]==0&&eye[i+1][j-1]!=0))&&eye[i][j]==0&&eye[i+1][j]==0)
        Possible_Half_Moves[i-1][j-1]=1;
        if(((eye[i][j+1]!=0&&eye[i+1][j+1]==0)||(eye[i][j+1]==0&&eye[i+1][j+1]!=0))&&eye[i][j]==0&&eye[i+1][j]==0)
        Possible_Half_Moves[i-1][j-1]=1;
    }

}
int Best_Half_Move(int our_domino)
{
    int i,j,m,n,counter1=0,counter2=0,scores=0,resup=0,resdown=0,max_score=-100,new_move_location=-1000,check=0,odd,even,isvertical=0,temp;
    if(our_domino==VERTICAL)
    {
        our_domino=1;
        isvertical=1;
        for(i=0;i<13;i++)
        for(j=0;j!=i;j++)
        {
            temp=board[i][j];
            board[i][j]=board[j][i];
            board[j][i]=temp;
        }
        for(i=0;i<15;i++)
        for(j=0;j!=i;j++)
        {
            temp=eye[i][j];
            eye[i][j]=eye[j][i];
            eye[j][i]=temp;
        }
    }
    if(our_domino==HORIZONTAL)
    {
        for(i=0;i<13;i++)
        for(j=0;j<14;j++)
        {
            scores=0,resup=0,resdown=0,check=0;
            if(Possible_Half_Moves[i][j]==1)
            {
                    m=i+1;
                    n=j;
                    while(eye[m][n]==0)
                    {
                        odd++;
                        n--;
                    }
                    m=i+1;
                    n=j+2;
                    while(eye[m][n]==0)
                    {
                        even++;
                        n++;
                    }
                    if(odd%2==1&&even%2==0)
                    scores-=10;
                    m=i,n=j+1;
                    while(eye[m][n]==0)
                    {
                        if(Possible_Half_Moves[i-2][j]==1||Possible_Half_Moves[i-2][j-1]==1&&check==0)
                        {
                            scores+=6;
                            resup++;
                            check=1;
                        }
                        m--;
                        counter1++;
                    }
                    check=0;
                    m=i+2,n=j+1;
                    while(eye[m][n]==0)
                    {
                        if(Possible_Half_Moves[i+2][j]==1||Possible_Half_Moves[i+2][j-1]==1&&check==0)
                        {
                            scores+=6;
                            resdown++;
                            check=1;
                        }
                        m++;
                        counter2++;

                    }
                    if(counter1%2==1||counter2%2==1)
                    scores+=8;
                    if(max_score<scores)
                    {
                        max_score=scores;
                        new_move_location=i*100+j;
                    }
                    check=0;
                    counter1=0;
                    counter2=0;
                    m=i,n=j+2;
                    while(eye[m][n]==0)
                    {
                        if((Possible_Half_Moves[i-2][j+1]==1||Possible_Half_Moves[i-2][j]==1)&&resup==0&&check==0)
                        {
                            resup++;
                            scores+=6;
                            check=1;
                        }
                        m--;
                        counter1++;
                    }
                    check=0;
                    m=i+2,n=j+2;
                    while(eye[m][n]==0)
                    {
                        if((Possible_Half_Moves[i+3][j+1]==1||Possible_Half_Moves[i+3][j]==1)&&resdown==0&&check==0)
                        {
                            resdown++;
                            scores+=6;
                            check=1;
                        }
                        m++;
                        counter2++;
                    }
                    if(counter1%2==1||counter2%2==1)
                    scores+=8;
                    if(max_score<scores)
                    {
                        max_score=scores;
                        new_move_location=i*100+j;
                    }
                    counter1=0;
                    counter2=0;
            }
        }
    }
    if(isvertical==1)
    {
        our_domino=2;
        for(i=0;i<13;i++)
        for(j=0;j!=i;j++)
        {
            temp=board[i][j];
            board[i][j]=board[j][i];
            board[j][i]=temp;
        }
        for(i=0;i<15;i++)
        for(j=0;j!=i;j++)
        {
            temp=eye[i][j];
            eye[i][j]=eye[j][i];
            eye[j][i]=temp;
        }
    }
    return new_move_location;
}



