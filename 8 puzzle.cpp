/*
   ~~~ BISMILLAH HIR RAHMANIR RAHIM ~~~
   Project Name: 8 puzzle solver
        Subject: Artificial Intelligence
      Algorithm: A* search
  Author's Name: Abdullah Al Masud Tushar
   ~~~     Never Lose Hope     ~~~
*/


#include<windows.h>
#include<bits/stdc++.h>
#define PS system("pause")
#define cs system("cls")
#define siter(n,T1)  for(set<T1>::iterator it=n.begin();it!=n.end();it++)
#define miter(n,T1,T2)  for(map<T1,T2>::iterator it=n.begin();it!=n.end();it++)
#define pb push_back
#define ppb pop_back
#define F first
#define S second
#define sqr(x) (( (x)* (x))%MOD)
#define btz(a)   __builtin_ctz(a)
#define Mems(p,n) memset(p, n, sizeof(p))
#define BOUNDARY(i,j,Row,Col) ((i >= 0 && i < Row) && (j >= 0 && j < Col))
#define pf printf

using namespace std;

int dx[]= {1,0,-1,0};
int dy[]= {0,1,0,-1};                        //4 Direction



template<class T>
double dif(T &a,T& b)
{
    double ans=(a.F-b.F)*(a.F-b.F)+(a.S-b.S)*(a.S-b.S);
    return sqrt(ans);
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~ Code Starting Point ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// These functions are used for color and other designs which are collected from net

COORD xy = {0,0};
void gotoxy (int x, int y)    // sets the cursor position
{
    xy.X = x;
    xy.Y = y; // X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}
void SetColorAndBackground(int ForgC, int BackC) // Sets color for foreground and background
{
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
    return;
}

void ClearConsoleToColors(int ForgC, int BackC)  // clears console color
{
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SetConsoleTextAttribute(hStdOut, wColor);
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
        SetConsoleCursorPosition(hStdOut, coord);
    }
    return;
}

void SetColor(int ForgC)  // sets selected color with color code send as parameter
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}


// ||||||||||||||||||||||||||  MAIN FUNCTIONS OF PROGRAM ||||||||||||||||||||||||||||||||||||||||||||||||

struct st  // this structure includes a maze as a state,  path of the final solution,total cost, total depth and co-ordinate of empty grid
{
    int maze[4][4];
    int f;
    int g;
    int zx;
    int zy;
    vector<int> path;
    bool operator<(const st& op)const
    {
        return f>op.f||(f==op.f&&g>op.g);
    }
} ans,res;

string dir[]= {"UP","LEFT","DOWN","RIGHT"};  // direction array indicator

double s_time,e_time;
int tot;

void init() // this function initializes final target state of the puzzle that is a sorted number from 1-8, zero stands for empty grid
{
    int cnt=1;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            ans.maze[i][j]=cnt++;
    ans.maze[2][2]=0;
}

bool cmp(st a, st b)  // compares two different states whether matches or not
{
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(a.maze[i][j]!=b.maze[i][j])
                return false;
    return true;
}

bool issolv(st tmp)  // ensures whether a given puzzle is solvable or not
{
    int flag=0;

    for(int i=0; i<9; i++)
    {
        int x1=i/3;
        int y1=i%3;
        for(int j=0; j<i; j++)
        {
            int x2=j/3;
            int y2=j%3;
            flag+=(tmp.maze[x1][y1]<tmp.maze[x2][y2]&&tmp.maze[x1][y1]);
        }
    }
    return (flag%2==0);
}

int heuristic(st a) // calculates heuristic value of a state
{
    int sum=0;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(a.maze[i][j])
                sum+=abs(i-(a.maze[i][j]-1)/3)+abs(j-(a.maze[i][j]-1)%3);
    return sum;
}

long long Hash(st tmp) // calculates hash value of a state
{
    long long sum=0;

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            sum=(sum*10+tmp.maze[i][j]);
    return sum;
}

void prnt(st tmp) // prints state of a puzzle grid
{
    for(int i=0; i<3; i++)
    {
        gotoxy(30,i+9);
        for(int j=0; j<3; j++)
        {
            if(tmp.maze[i][j]!=0)
                cout<<tmp.maze[i][j]<<' ';
            else
                cout<<" "<<' ';
        }
        cout<<endl;
    }
}

int A_star(st tmp) // A* search algorithm
{
    st u,v,nx;
    priority_queue<st> q;
    map<long long ,bool> mark;
    long long hs=Hash(tmp);
    mark[hs]=true;
    tmp.f=tmp.g=tot=0;

    for(int i=0; i<9; i++)
        if(!tmp.maze[i/3][i%3])
        {
            tmp.zx=i/3;
            tmp.zy=i%3;
            break;
        }

    q.push(tmp);

    while(!q.empty())
    {
        u=q.top();
        q.pop();
        tot++;
        if(cmp(ans,u))
        {
            res=u;
            return u.g;
        }

        for(int i=0; i<4; i++)
        {
            v=u;
            v.zx=u.zx+dx[i];
            v.zy=u.zy+dy[i];
            if(BOUNDARY(v.zx,v.zy,3,3))
            {
                swap(v.maze[v.zx][v.zy],v.maze[u.zx][u.zy]);
                hs=Hash(v);
                if(!mark[hs])
                {
                    mark[hs]=true;
                    v.f=v.g+heuristic(v);
                    v.g=u.g+1;
                    v.path.pb(i);
                    q.push(v);
                }
            }
        }
    }
}


void path_print() // prints path of the move from initial state to target state
{
    int ln=res.path.size();
    for(int i=0; i<ln; i++)
        cout<<dir[res.path[i]]<<',';
    pf("\n");
    res.path.clear();
}


void simulation(st tmp)  // this function shows the simulation of the program step by step
{
    st in=tmp;
    int ln=res.path.size();
    prnt(tmp);
    int nx,ny,px,py;

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(tmp.maze[i][j]==0)
                px=i,py=j;

    for(int i=0; i<ln; i++)
    {
        cs;
        SetColor(10);
        if(res.path[i]==1)
        {
            nx=px;
            ny=py+1;
            gotoxy(22,10);
            pf("LEFT\n");
            swap(tmp.maze[nx][ny],tmp.maze[px][py]);
        }
        else if(res.path[i]==3)
        {
            nx=px;
            ny=py-1;
            gotoxy(40,10);
            pf("RIGHT\n");
            swap(tmp.maze[nx][ny],tmp.maze[px][py]);
        }
        else if(res.path[i]==0)
        {
            nx=px+1;
            ny=py;
            gotoxy(32,7);
            pf("UP\n");
            swap(tmp.maze[nx][ny],tmp.maze[px][py]);
        }
        else if(res.path[i]==2)
        {
            gotoxy(30,13);
            pf("DOWN\n");
            nx=px-1;
            ny=py;
            swap(tmp.maze[nx][ny],tmp.maze[px][py]);
        }
        SetColor(14);
        prnt(tmp);
        Sleep(500);

        px=nx;
        py=ny;
    }

    SetColor(13);
    pf("\n\t\t   YeHeee!!! PUZZLE SOLVED :D :D\n\n\t\t  PRESS ANY KEY TO SEE THE RESULT");
    getchar();
    cs;
    SetColor(14);
    pf("\n\n\n\n\n\n\t\t     INITIAL STATE OF PUZZLE \n");
    SetColor(11);
    prnt(in);
}

int main()
{

    init();
    st tmp;
    int xd;
    char ch;

    while(true)
    {
        cs;
        gotoxy(30,10);
        SetColorAndBackground(9,14);
        pf("1.GIVE PUZZLE INPUT\n");
        gotoxy(30,12);
        SetColorAndBackground(12,14);
        pf("2.EXIT\n");
        SetColorAndBackground(15,0);

        scanf("%d",&xd);
        if(xd==1)
        {
            cs;
            SetColor(14);

            pf("\n\n\n_________________________________________________________\n");
            pf("GIVE YOUR INITIAL PUZZLE BELLOW:- (EX: 5 2 3 0 1 8 6 4 7)\n");

            set<int> xm;

            for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                {
                    scanf("%d",&tmp.maze[i][j]);
                    if(tmp.maze[i][j]>8||tmp.maze[i][j]<0)
                    {
                        pf("Don't give any invalid input -_-\n");
                        j--;
                        continue;
                    }
                    xm.insert(tmp.maze[i][j]);
                }

            if(xm.size()!=9)
            {
                cs;
                SetColor(12);
                pf("\n\n\n\n\n\t SRY THE GIVEN INPUT PUZZLE IS INVALID.TRY AGAIN.");
                SetColor(11);
                pf("\n\n\t\t      PRESS ANY KEY\n");
                getchar();
                continue;
            }
            cs;
            SetColor(14);
            pf("\n\n\n\n\n\t\t\t   YOUR GIVEN PUZZLE \n");
            prnt(tmp);
            pf("\n\n\t\t         PRESS ANY KEY TO START\n");
            pf("\n");
            getchar();
            getchar();
            if(cmp(tmp,ans))
            {
                SetColor(13);
                pf("\t          THE GIVEN PUZZLE IS ALREADY SOLVED\n");
            }
            else if(issolv(tmp))
            {
                s_time=clock();
                int anc=A_star(tmp);
                e_time=clock();
                simulation(tmp);
                SetColor(14);
                pf("\nMINIMUM STEPS TO THE GOAL STATE : %d\n",anc);
                pf("\nELLAPS TIME OF CALCULATION      : %.4f\n",(e_time-s_time)/1000.0);
                pf("\nTOTAL VISITED STATES            : %d\n",tot);
                pf("\nSTEPS ARE GIVEN BELOW:- \n");
                SetColor(10);
                path_print();
            }
            else
            {
                SetColor(13);
                pf("\t         THIS GIVEN PUZZLE IS IMPOSSIBLE TO SOLVE\n");
            }
            SetColor(12);
            pf("\n\n\n\t~~~~~~~~~~~~~~~~~~~~ PRESS ANY KEY ~~~~~~~~~~~~~~~~~~~\n");
            getchar();
        }
        else
            return 0;
    }
    return 0;
}
