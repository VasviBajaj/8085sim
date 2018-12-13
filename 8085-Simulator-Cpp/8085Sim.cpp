#include<iostream>
#include<math.h>
#include <stdio.h>
#include<iomanip>
#include<graphics.h>
#include <fstream>
#include <stdlib.h>
#include<conio.h>
#include <string.h>
#define S 0
#define Z 1
#define FL1 2
#define AC 3
#define FL2 4
#define P 5
#define FL3 6
#define CY 7
using namespace std;
char opcode2[5]="0000";
char org[4]={'\0'};
char db[10][4]={'\0'};
char opcode[7];
static int c_l=0;
char comp_prog[100][100]={'\0'},curr_prog[100]={'\0'};
int prog_length=0,prog_length_run=0;
void draw_tables(int ls=815, int re=1315);
void fill_table_data();
void prog_disp();
void di();
void dib();
void read_code();
void nextprog();
int hexto(char a);
char last_reg(char t[100]);
void word_dect(char ch[]);
int opcode_dect(char ch[]);
void add_reg(char t[100],char t2[100]);
int todec(char c[100]);
char tochar(int a);
char * tohex(int a);
int bintodec(char s[9]);
void parity(char str[3]);
struct regis
{
    char addr[4];
    char data[4];
};
struct regis A,B,C,D,E,H,L;
static int dbl=0;
void ini()
{
    strcpy(A.addr,"00");
    strcpy(B.addr,"00");
    strcpy(C.addr,"00");
    strcpy(D.addr,"00");
    strcpy(E.addr,"00");
    strcpy(H.addr,"00");
    strcpy(L.addr,"00");
    strcpy(A.data,"00");
    strcpy(B.data,"00");
    strcpy(C.data,"00");
    strcpy(D.data,"00");
    strcpy(E.data,"00");
    strcpy(H.data,"00");
    strcpy(L.data,"00");
}
char M[3]="00";
int FLAGRG[8]={0};
/////////////////////////////////////////////////////////////////////////
/************************ORG/DB SETTING*********************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
void db_set(char c[])
{
    char temp[100];
    int i=0,j=0;
    for(i=4;c[i]!='\0';i++)
    {
        if(c[i]=='H')
            i++;
        else
            {
                //printf("db=%c\n",c[i]);
                db[j][0]=c[i];
                db[j][1]=c[i+1];
                db[j][2]='\0';
                j++;
                i++;
            }
    }
    dbl=j-1;
   // cout<<dbl<<endl<<j;
}
int org_dect(char ch[])
{
    word_dect(ch);
    if(strcmp(opcode,"#ORG")==0)
        return 1;
    else if(strcmp(opcode,"#DB")==0)
        return 2;
    else
        return 3;
}
void read_code2(char filea[100])
{
    ifstream fina;
    char d[100];
    fina.open(filea,ios::in);
    do
    {
        fina.getline(d,30);
        int y=org_dect(d);
     //   cout<<y;
        if(y==1)
        {

            int i=0;
            for(i=0;i<4;i++)
                org[i]=d[5+i];
            org[4]='\0';
        }
        if(y==2)
        {
            db_set(d);
        }
    }while(!fina.eof());
    org[4]='\0';
    fina.close();
}
void set_org(char filea[100])
{
    read_code2(filea);
}
/////////////////////////////////////////////////////////////////////////
/************************ORG/DB SETTING*********************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/************************LOAD FUNCTIONS*********************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
void LDA(char im[4])
{
    int x,y,z;
    x=todec(org);
    y=todec(im);
    z=y-x;
    strcpy(A.data,db[z]);
}
void LHLD(char im[4])
{
    int x,y,z;
    x=todec(im);
    y=todec(org);
    z=x-y;
    strcpy(L.data,db[z]);
    strcpy(H.data,db[z+1]);
}
void LXI(char rp,char im[4])
{
    if(rp=='B')
    {
        B.data[0]=im[0];
        B.data[1]=im[1];
        B.data[2]='\0';
        C.data[0]=im[2];
        C.data[1]=im[3];
        C.data[2]='\0';
    }
    if(rp=='D')
    {
        D.data[0]=im[0];
        D.data[1]=im[1];
        D.data[2]='\0';
        E.data[0]=im[2];
        E.data[1]=im[3];
        E.data[2]='\0';
        int x,y,z;
    }
    if(rp=='H')
    {
        H.data[0]=im[0];
        H.data[1]=im[1];
        H.data[2]='\0';
        L.data[0]=im[2];
        L.data[1]=im[3];
        L.data[2]='\0';
        int x,y,z;
        x=todec(im);
        y=todec(org);
        //printf("ORG=%s\n",org);
        z=x-y;
        //printf("z=%d %s\n",z,db[0]);
        strcpy(M,db[z]);
    }
}
/////////////////////////////////////////////////////////////////////////
/************************LOAD FUNCTIONS*********************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/************************INCREMENT/DECREMENT FUNCTIONS******************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
void INX(char xt)
{
    if(xt=='B')
    {
        int c;
        c=todec(C.data);
        if(c==255)
        {
            c=0;
            int b=todec(B.data);
            b++;
            strcpy(B.data,tohex(b));
        }
        else
        {
            c++;
        }
        strcpy(C.data,tohex(c));
        //printf("%s",C.addr);
        return;
    }
    if(xt=='D')
    {
        int e;
        e=todec(E.data);
        if(e==255)
        {
            e=0;
            int d=todec(D.data);
            d++;
            strcpy(D.data,tohex(d));
        }
        else
        {
            e++;
        }
        strcpy(E.data,tohex(e));
        return;
    }
    if(xt=='H')
    {
        int l;
        l=todec(L.data);
        if(l==255)
        {
            l=0;
            int h=todec(H.data);
            h++;
            strcpy(H.data,tohex(h));
        }
        else
        {
            l++;
        }
        strcpy(L.data,tohex(l));
        //printf("%s %s\n",H.addr,L.addr);
        char s[4];
        /*s[0]=H.addr[0];
        s[1]=H.addr[1];
        s[2]=L.addr[0];
        s[3]=L.addr[1];
        s[4]='\0';*/
        strcpy(s,H.data);
        strcat(s,L.data);
        //printf("s=%s",s);
        int x,y,z;
        x=todec(s);
        //printf("org=%s",org);
        y=todec(org);
        z=x-y;
        //printf("x=%d y=%d z=%d",x,y,z);
        strcpy(M,db[z]);
        return;
    }
}
void DCX(char xt)
{
    if(xt=='B')
    {
        int c;
        c=todec(C.data);
        if(c==0)
        {
            c=255;
            int b=todec(B.data);
            b--;
            strcpy(B.data,tohex(b));
        }
        c--;
        strcpy(C.data,tohex(c));
        //printf("%s",C.addr);
        return;
    }
    if(xt=='D')
    {
        int e;
        e=todec(E.data);
        if(e==0)
        {
            e=255;
            int d=todec(D.data);
            d--;
            strcpy(D.data,tohex(d));
        }
        e--;
        strcpy(E.data,tohex(e));
        return;
    }
    if(xt=='H')
    {
        int l;
        l=todec(L.data);
        if(l==0)
        {
            l=255;
            int h=todec(H.data);
            h--;
            strcpy(H.data,tohex(h));
        }
        l--;
        strcpy(L.data,tohex(l));
        //printf("%s %s\n",H.addr,L.addr);
        char s[4];
        /*s[0]=H.addr[0];
        s[1]=H.addr[1];
        s[2]=L.addr[0];
        s[3]=L.addr[1];
        s[4]='\0';*/
        strcpy(s,H.data);
        strcat(s,L.data);
        //printf("s=%s",s);
        int x,y,z;
        x=todec(s);
        //printf("org=%s",org);
        y=todec(org);
        z=x-y;
        //printf("x=%d y=%d z=%d",x,y,z);
        strcpy(M,db[z]);
        return;
    }
}
void INR(char x)
{
    int y;
    if(x=='A')
    {
        y=todec(A.data);
        if(A.data[1]=='F')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        y++;
        if(y==256)
        {
            y=0;
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        if(y>128)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        strcpy(A.data,tohex(y));
        parity(A.data);
        return;
    }
    if(x=='B')
    {
        y=todec(B.data);
        if(B.data[1]=='F')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        y++;
        if(y==256)
        {
            y=0;
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        if(y>128)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        strcpy(B.data,tohex(y));
        parity(B.data);
        return;
    }
    if(x=='C')
    {
        y=todec(C.data);
        if(C.data[1]=='F')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        y++;
        if(y==256)
        {
            y=0;
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        if(y>128)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        strcpy(C.data,tohex(y));
        parity(C.data);
        return;
    }
    if(x=='D')
    {
        y=todec(D.data);
        if(D.data[1]=='F')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        y++;
        if(y==256)
        {
            y=0;
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        if(y>128)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        strcpy(D.data,tohex(y));
        parity(D.data);
        return;
    }
    if(x=='E')
    {
        y=todec(E.data);
        if(E.data[1]=='F')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        y++;
        if(y==256)
        {
            y=0;
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        if(y>128)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        strcpy(E.data,tohex(y));
        parity(E.data);
        return;
    }
    if(x=='H')
    {
        y=todec(H.data);
        if(H.data[1]=='F')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        y++;
        if(y==256)
        {
            y=0;
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        if(y>128)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        strcpy(H.data,tohex(y));
        parity(H.data);
        return;
    }
    if(x=='L')
    {
        y=todec(L.data);
        if(L.data[1]=='F')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        y++;
        if(y==256)
        {
            y=0;
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        if(y>128)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        strcpy(L.data,tohex(y));
        parity(L.data);
        return;
    }
    if(x=='M')
    {
        y=todec(M);
        if(M[1]=='F')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        y++;
        if(y==256)
        {
            y=0;
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        if(y>128)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        strcpy(M,tohex(y));
        int a1=todec(org);
        char s[5];
        strcpy(s,H.data);
        strcat(s,L.data);
        int a2=todec(s);
        strcpy(db[a2-a1],M);
        parity(M);
        return;
    }
}
void DCR(char x)
{
    int y;
    if(x=='A')
    {
        y=todec(A.data);
        if(A.data[1]=='0')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        if(y==0)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        y--;
        if(y==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        strcpy(A.data,tohex(y));
        parity(A.data);
        return;
    }
    if(x=='B')
    {
        y=todec(B.data);
        if(B.data[1]=='0')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        if(y==0)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        y--;
        if(y==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        strcpy(B.data,tohex(y));
        parity(B.data);
        return;
    }
    if(x=='C')
    {
        y=todec(C.data);
        if(C.data[1]=='0')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        if(y==0)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        y--;
        if(y==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        strcpy(C.data,tohex(y));
        parity(C.data);
        return;
    }
    if(x=='D')
    {
        y=todec(D.data);
        if(D.data[1]=='0')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        if(y==0)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        y--;
        if(y==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        strcpy(D.data,tohex(y));
        parity(D.data);
        return;
    }
    if(x=='E')
    {
        y=todec(E.data);
        if(E.data[1]=='0')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        if(y==0)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        y--;
        if(y==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        strcpy(E.data,tohex(y));
        parity(E.data);
        return;
    }
    if(x=='H')
    {
        y=todec(H.data);
        if(H.data[1]=='0')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        if(y==0)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        y--;
        if(y==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        strcpy(H.data,tohex(y));
        parity(H.data);
        return;
    }
    if(x=='L')
    {
        y=todec(L.data);
        if(L.data[1]=='0')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        if(y==0)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        y--;
        if(y==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        strcpy(L.data,tohex(y));
        parity(L.data);
        return;
    }
    if(x=='M')
    {
        y=todec(M);
        if(M[1]=='0')
            FLAGRG[3]=1;
        else
            FLAGRG[3]=0;
        if(y==0)
        {
            FLAGRG[0]=1;
        }
        else
        {
            FLAGRG[0]=0;
        }
        y--;
        if(y==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        strcpy(M,tohex(y));
        int a1=todec(org);
        char s[5];
        strcpy(s,H.data);
        strcat(s,L.data);
        int a2=todec(s);
        strcpy(db[a2-a1],M);
        parity(M);
        return;
    }
}
/////////////////////////////////////////////////////////////////////////
/************************INCREMENT/DECREMENT FUNCTIONS******************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/************************MOVE FUNCTIONS*********************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
void MVI(char ele[5])
{
    if(ele[0]=='A')
    {
        A.data[0]=ele[2];
        A.data[1]=ele[3];
        A.data[2]='\0';
    }
    if(ele[0]=='B')
    {
        B.data[0]=ele[2];
        B.data[1]=ele[3];
        B.data[2]='\0';
    }
    if(ele[0]=='C')
    {
        C.data[0]=ele[2];
        C.data[1]=ele[3];
        C.data[2]='\0';
    }
    if(ele[0]=='D')
    {
        D.data[0]=ele[2];
        D.data[1]=ele[3];
        D.data[2]='\0';
    }
    if(ele[0]=='E')
    {
        E.data[0]=ele[2];
        E.data[1]=ele[3];
        E.data[2]='\0';
    }
    if(ele[0]=='H')
    {
        H.data[0]=ele[2];
        H.data[1]=ele[3];
        H.data[2]='\0';
    }
    if(ele[0]=='L')
    {
        L.data[0]=ele[2];
        L.data[1]=ele[3];
        L.data[2]='\0';
    }
    if(ele[0]=='M')
    {
        M[0]=ele[2];
        M[1]=ele[3];
        M[2]='\0';
        int a1=todec(org);
        char s[4];
        strcpy(s,H.data);
        strcat(s,L.data);
        int a2=todec(s);
        strcpy(db[a2-a1],M);
    }
    /*int x=todec(org);
    char s[5];
    strcpy(s,H.data);
    strcat(s,L.data);
    int y=todec(s);
    cout<<y-x<<endl;
    if((y-x)>=dbl)
        strcpy(M,"00");
    else
        strcpy(M,db[y-x]);*/
}
void MOV(char ele[4])
{
    if(ele[0]=='A')
    {
        if(ele[2]=='B')
        {
            A=B;
            return;
        }
        if(ele[2]=='C')
        {
            A=C;
            return;
        }
        if(ele[2]=='D')
        {
            A=D;
            return;
        }
        if(ele[2]=='E')
        {
            A=E;
            return;
        }
        if(ele[2]=='H')
        {
            A=H;
            return;
        }
        if(ele[2]=='L')
        {
            A=L;
            return;
        }
        if(ele[2]=='M')
        {
            strcpy(A.data,M);
            return;
        }
    }
    if(ele[0]=='B')
    {
        if(ele[2]=='A')
        {
            B=A;
            return;
        }
        if(ele[2]=='C')
        {
            B=C;
            return;
        }
        if(ele[2]=='D')
        {
            B=D;
            return;
        }
        if(ele[2]=='E')
        {
            B=E;
            return;
        }
        if(ele[2]=='H')
        {
            B=H;
            return;
        }
        if(ele[2]=='L')
        {
            B=L;
            return;
        }
        if(ele[2]=='M')
        {
            strcpy(B.data,M);
            return;
        }
    }
    if(ele[0]=='C')
    {
        if(ele[2]=='A')
        {
            C=A;
            return;
        }
        if(ele[2]=='B')
        {
            C=B;
            return;
        }
        if(ele[2]=='D')
        {
            C=D;
            return;
        }
        if(ele[2]=='E')
        {
            C=E;
            return;
        }
        if(ele[2]=='H')
        {
            C=H;
            return;
        }
        if(ele[2]=='L')
        {
            C=L;
            return;
        }
        if(ele[2]=='M')
        {
            strcpy(C.data,M);
            return;
        }
    }
    if(ele[0]=='D')
    {
        if(ele[2]=='A')
        {
            D=A;
            return;
        }
        if(ele[2]=='B')
        {
            D=B;
            return;
        }
        if(ele[2]=='C')
        {
            D=C;
            return;
        }
        if(ele[2]=='E')
        {
            D=E;
            return;
        }
        if(ele[2]=='H')
        {
            D=H;
            return;
        }
        if(ele[2]=='L')
        {
            D=L;
            return;
        }
        if(ele[2]=='M')
        {
            strcpy(D.data,M);
            return;
        }
    }
    if(ele[0]=='E')
    {
        if(ele[2]=='A')
        {
            E=A;
            return;
        }
        if(ele[2]=='B')
        {
            E=B;
            return;
        }
        if(ele[2]=='C')
        {
            E=C;
            return;
        }
        if(ele[2]=='D')
        {
            E=D;
            return;
        }
        if(ele[2]=='H')
        {
            E=H;
            return;
        }
        if(ele[2]=='L')
        {
            E=L;
            return;
        }
        if(ele[2]=='M')
        {
            strcpy(E.data,M);
            return;
        }
    }
    if(ele[0]=='H')
    {
        if(ele[2]=='A')
        {
            H=A;
            return;
        }
        if(ele[2]=='B')
        {
            H=B;
            return;
        }
        if(ele[2]=='C')
        {
            H=C;
            return;
        }
        if(ele[2]=='D')
        {
            H=D;
            return;
        }
        if(ele[2]=='E')
        {
            H=E;
            return;
        }
        if(ele[2]=='L')
        {
            H=L;
            return;
        }
        if(ele[2]=='M')
        {
            strcpy(H.data,M);
            return;
        }
    }
    if(ele[0]=='L')
    {
        if(ele[2]=='A')
        {
            L=A;
            return;
        }
        if(ele[2]=='B')
        {
            L=B;
            return;
        }
        if(ele[2]=='C')
        {
            L=C;
            return;
        }
        if(ele[2]=='D')
        {
            L=D;
            return;
        }
        if(ele[2]=='E')
        {
            L=E;
            return;
        }
        if(ele[2]=='H')
        {
            L=H;
            return;
        }
        if(ele[2]=='M')
        {
            strcpy(L.data,M);
            return;
        }
    }
    if(ele[0]=='M')
    {
        if(ele[2]=='A')
        {
            strcpy(M,A.data);
        }
        if(ele[2]=='B')
        {
            strcpy(M,B.data);
        }
        if(ele[2]=='C')
        {
            strcpy(M,C.data);
        }
        if(ele[2]=='D')
        {
            strcpy(M,D.data);
        }
        if(ele[2]=='E')
        {
            strcpy(M,E.data);
        }
        if(ele[2]=='H')
        {
            strcpy(M,H.data);
        }
        if(ele[2]=='L')
        {
            strcpy(M,L.data);
        }
        int a1=todec(org);
        char s[5];
        strcpy(s,H.data);
        strcat(s,L.data);
        int a2=todec(s);
        strcpy(db[a2-a1],M);
    }
}
/////////////////////////////////////////////////////////////////////////
/************************MOVE FUNCTIONS*********************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/*******************ARITHEMATIC FUNCTIONS*******************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
void ADD(char x)
{
    int a;
    a=todec(A.data);
    if(x=='B')
    {
        int b;
        b=todec(B.data);
        a=a+b;
    }
    if(x=='C')
    {
        int c;
        c=todec(C.data);
        a=a+c;
    }
    if(x=='D')
    {
        int d;
        d=todec(D.data);
        a=a+d;
    }
    if(x=='E')
    {
        int e;
        e=todec(E.data);
        a=a+e;
    }
    if(x=='H')
    {
        int h;
        h=todec(H.data);
        a=a+h;
    }
    if(x=='L')
    {
        int l;
        l=todec(L.data);
        a=a+l;
    }
    if(x=='M')
    {
        int m;
        m=todec(M);
        a=a+m;
    }
    if(a>128)
    {
        FLAGRG[0]=1;
    }
    else
    {
        FLAGRG[0]=0;
    }
    if(a>255)
    {
        FLAGRG[7]=1;
        FLAGRG[1]=0;
        a=a-256;
        strcpy(A.data,tohex(a));
        parity(A.data);
        return;
    }
    else
    {
        FLAGRG[7]=0;
        strcpy(A.data,tohex(a));
        if(a==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        parity(A.data);
        return;
    }
}
void DAD(char xt)
{
    int h=todec(H.data);
    int l=todec(L.data);
    int x,y;
    if(xt=='B')
    {
        x=todec(B.data);
        y=todec(C.data);
    }
    if(xt=='D')
    {
        x=todec(D.data);
        y=todec(E.data);
    }
    if(xt=='H')
    {
        x=todec(H.data);
        y=todec(L.data);
    }
    l=l+y;
    if(l>255)
    {
        l=l-256;
        h=h+1;
    }
    h=h+x;
    if(h>255)
    {
        h=h-256;
        FLAGRG[7]=1;
    }
    else
    {
        FLAGRG[7]=0;
    }
    strcpy(H.data,tohex(h));
    strcpy(L.data,tohex(l));
    return;
}
void SUB(char x)
{
    int a;
    a=todec(A.data);
    if(x=='B')
    {
        int b;
        b=todec(B.data);
        a=a-b;
    }
    if(x=='C')
    {
        int c;
        c=todec(C.data);
        a=a-c;
    }
    if(x=='D')
    {
        int d;
        d=todec(D.data);
        a=a-d;
    }
    if(x=='E')
    {
        int e;
        e=todec(E.data);
        a=a-e;
    }
    if(x=='H')
    {
        int h;
        h=todec(H.data);
        a=a-h;
    }
    if(x=='L')
    {
        int l;
        l=todec(L.data);
        a=a-l;
    }
    if(x=='M')
    {
        int m;
        m=todec(M);
        a=a-m;
    }
    if(a<0)
    {
        FLAGRG[7]=1;
        FLAGRG[1]=0;
        FLAGRG[0]=1;
        a=a+256;
        strcpy(A.data,tohex(a));
        parity(A.data);
        return;
    }
    else
    {
        FLAGRG[7]=0;
        strcpy(A.data,tohex(a));
        if(a==0)
        {
            FLAGRG[1]=1;
        }
        else
        {
            FLAGRG[1]=0;
        }
        FLAGRG[0]=0;
        parity(A.data);
        return;
    }
}
void CMP(char x)
{
    int a,y;
    a=todec(A.data);
    if(x=='A')
    {
        y=todec(A.data);
    }
    if(x=='B')
    {
        y=todec(B.data);
    }
    if(x=='C')
    {
        y=todec(C.data);
    }
    if(x=='D')
    {
        y=todec(D.data);
    }
    if(x=='E')
    {
        y=todec(E.data);
    }
    if(x=='H')
    {
        y=todec(H.data);
    }
    if(x=='L')
    {
        y=todec(L.data);
    }
    if(x=='M')
    {
        y=todec(M);
    }
    if(a<y)
    {
        FLAGRG[7]=1;
        FLAGRG[1]=0;
        FLAGRG[0]=1;
    }
    else if(a==y)
    {
        FLAGRG[7]=0;
        FLAGRG[1]=1;
        FLAGRG[0]=0;
    }
    else
    {
        FLAGRG[7]=0;
        FLAGRG[1]=0;
        FLAGRG[0]=0;
    }
    parity(A.data);
}
/////////////////////////////////////////////////////////////////////////
/*******************ARITHEMATIC FUNCTIONS*******************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/***********************************************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
void drawln(char ch)
{
for (int i=0; i<=79; i++)
cout<<ch;
}

void display()
{
    static int i=0;
    draw_tables();
    fill_table_data();
    rectangle(650,200,750,250);
    if(i==0)
        outtextxy(665,215,"COMPILE");
    else
        outtextxy(665,215,"NEXT");
    rectangle(650,300,750,350);
    outtextxy(665,315,"EXIT");
    prog_disp();
    i=1;
}
/////////////////////////////////////////////////////////////////////////
/*********************COMPLEMENT FUNCTION********************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
void CMA()
{
    int j=7;
    int i,a;
    a=todec(A.data);
    char s[9];
    itoa(a,s,2);
    int s4=strlen(s);
    char s1[9]="00000000";
    for(int i=(s4-1);i>=0;i--)
    {
     //   cout<<s2[i];
        if(s[i]=='1')
            s1[j]='1';
        j--;
    }
    for(i=0;s1[i]!='\0';i++)
    {
        if(s1[i]=='0')
        {
            s1[i]='1';
        }
        else
        {
            s1[i]='0';
        }
    }
    a=bintodec(s1);
    strcpy(A.data,tohex(a));
}
/////////////////////////////////////////////////////////////////////////
/*********************COMPLEMENT FUNCTION********************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/**********************JUMP FUNCTION************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
int JMP(char temp[100])
{
    char check[100];
    int i=0;
    while(i<prog_length_run)
    {
        int j=0;
        for(j=0;(comp_prog[i][j])!=' ';j++)
            check[j]=comp_prog[i][j];
        check[j-1]='\0';
        if(strcmpi(temp,check)==0)
            return i;
        i++;
    }
}
/////////////////////////////////////////////////////////////////////////
/**********************JUMP FUNCTION************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/***********************************************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
int main()
{
int mx,my;
ini();
read_code();
int gdriver = DETECT, gmode;
initwindow(1350,700,"8085 Simutalor-RASP",5,5);
display();
while (1)
{
WE:    while (!ismouseclick(WM_LBUTTONDOWN));

    getmouseclick(WM_LBUTTONDOWN, mx, my);
    if((mx>649)&&(mx<751)&&(my>199)&&(my<251))
        {
            outtextxy(200,200,"Next line is to be implemented!");
            nextprog();
        }
    else if((mx>649)&&(mx<751)&&(my>299)&&(my<351))
        {
            closegraph();
            system("cls");
            cout<<setw(40)<<"Exit";
            return 0;
            //break;
        }
    else
        {outtextxy(200,200,"You pressed outside the box");display();goto WE;}
    //display();
    cleardevice();
    int y=opcode_dect(curr_prog);
    if(y==1)
        ADD(last_reg(curr_prog));
    else if(y==2)
        {
            char temp2[10];
            add_reg(curr_prog,temp2);
            char temp[6];
            temp[0]=temp2[2];
            temp[1]=temp2[3];
            temp[2]=temp2[4];
            temp[3]=temp2[5];
            temp[4]='\0';
           // cout<<temp;
            LXI(temp2[0],temp);
        }
    else if(y==3)
        {
            char temp[10];
            add_reg(curr_prog,temp);
            MOV(temp);
        }
    else if(y==4)
        {
            //cout<<"inx working";
            INX(last_reg(curr_prog));
        }
    else if(y==5)
            {
                char temp2[10];
                add_reg(curr_prog,temp2);
                c_l=JMP(temp2);
            }
    else if(y==6)           //JC
            {
                if(FLAGRG[CY]==1)
                {
                    char temp2[10];
                    add_reg(curr_prog,temp2);
                    c_l=JMP(temp2);
                }
            }
    else if(y==7)           //JNC
            {
                if(FLAGRG[CY]==0)
                {
                    char temp2[10];
                    add_reg(curr_prog,temp2);
                    c_l=JMP(temp2);
                }
            }
    else if(y==8)       //JP
            {
                if(FLAGRG[S]==0)
                {
                    char temp2[10];
                    add_reg(curr_prog,temp2);
                    c_l=JMP(temp2);
                }
            }
    else if(y==9)       //JM
            {
                if(FLAGRG[S]==1)
                {
                    char temp2[10];
                    add_reg(curr_prog,temp2);
                    c_l=JMP(temp2);
                }
            }
    else if(y==10)      //JPE
            {
                if(FLAGRG[P]==1)
                {
                    char temp2[10];
                    add_reg(curr_prog,temp2);
                    c_l=JMP(temp2);
                }
            }
    else if(y==11)      //JPO
            {
                if(FLAGRG[P]==0)
                {
                    char temp2[10];
                    add_reg(curr_prog,temp2);
                    c_l=JMP(temp2);
                }
            }
    else if(y==12)      //JZ
            {
                if(FLAGRG[Z]==1)
                {
                    char temp2[10];
                    add_reg(curr_prog,temp2);
                    c_l=JMP(temp2);
                }
            }
    else if(y==13)       //JNZ
            {
                if(FLAGRG[Z]==0)
                {
                    char temp2[10];
                    add_reg(curr_prog,temp2);
                    c_l=JMP(temp2);
                }
            }
    else if(y==14)
        {
            char t[100];
            add_reg(curr_prog,t);
            LDA(t);
        }
    else if(y==15)
        {
            char t[100];
            add_reg(curr_prog,t);
            LHLD(t);
        }
    else if(y==16)
        {
            CMA();
        }
    else if(y==17)
        {
            char t[100];
            add_reg(curr_prog,t);
            MVI(t);
        }
    else if(y==18)
        {
            SUB(last_reg(curr_prog));
        }
    else if(y==19)
        {
            INR(last_reg(curr_prog));
        }
    else if(y==20)
        {
            DCR(last_reg(curr_prog));
        }
    else if(y==21)
        {
            DAD(last_reg(curr_prog));
        }
    else if(y==22)
        {
            DCX(last_reg(curr_prog));
        }
    else if(y==23)
        {
            CMP(last_reg(curr_prog));
        }
    else
        outtextxy(200,300,"none");
    display();

}
//getch();
return 0;
}
/////////////////////////////////////////////////////////////////////////
/***********************************************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
char last_reg(char t[100])
{
    int i=0;
    while(t[i]!='\0')
        i++;
    return (t[i-1]);
}
void add_reg(char t[100],char t2[100])
{
    int i=0,k=0;
    for(i=0;t[i]!='\0';i++);
    for(int j=i-1;t[j]!=' ';j--)
        t2[k++]=t[j];
    t2[k]='\0';
    strrev(t2);
}
void word_dect(char ch[])
{
    int i,j;
    for(i=0;((ch[i]!=' ')&&(ch[i]!='\0'));i++)
        opcode[i]=ch[i];
    opcode[i]='\0';
    if(opcode[i-1]==':')
    {
        char opcode99[6];
        int k=0;
        for(int j=i+1;ch[j]!=' ';j++)
            opcode99[k++]=ch[j];
        opcode99[k]='\0';
        strcpy(opcode,opcode99);
    }
}
int opcode_dect(char ch[])
{
    word_dect(ch);
    //cout<<opcode<<endl;
    if(strcmp(opcode,"ADD")==0)
        return 1;
    else if(strcmp(opcode,"LXI")==0)
        return 2;
    else if(strcmp(opcode,"MOV")==0)
        return 3;
    else if(strcmp(opcode,"INX")==0)
        return 4;
    else if(strcmp(opcode,"JMP")==0)
        return 5;
    else if(strcmp(opcode,"JC")==0)
        return 6;
    else if(strcmp(opcode,"JNC")==0)
        return 7;
    else if(strcmp(opcode,"JP")==0)
        return 8;
    else if(strcmp(opcode,"JM")==0)
        return 9;
    else if(strcmp(opcode,"JPE")==0)
        return 10;
    else if(strcmp(opcode,"JPO")==0)
        return 11;
    else if(strcmp(opcode,"JZ")==0)
        return 12;
    else if(strcmp(opcode,"JNZ")==0)
        return 13;
    else if(strcmp(opcode,"LDA")==0)
        return 14;
    else if(strcmp(opcode,"LHLD")==0)
        return 15;
    else if(strcmp(opcode,"CMA")==0)
        return 16;
    else if(strcmp(opcode,"MVI")==0)
        return 17;
    else if(strcmp(opcode,"SUB")==0)
        return 18;
    else if(strcmp(opcode,"INR")==0)
        return 19;
    else if(strcmp(opcode,"DCR")==0)
        return 20;
    else if(strcmp(opcode,"DAD")==0)
        return 21;
    else if(strcmp(opcode,"DCX")==0)
        return 22;
    else if(strcmp(opcode,"CMP")==0)
        return 23;
    else
        return 72;
}
/////////////////////////////////////////////////////////////////////////
/***********************************************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
void nextprog()
{
    if(c_l<prog_length_run)
    {
        strcpy(curr_prog,comp_prog[c_l]);
        c_l++;
    }
    //cout<<c_l<<endl;
}
void di()
{
        drawln('=');
        cout<<"\n\n\n\n"<<setw(50)<<"8085 Simulator\n\n\n\n";
        drawln('=');
        cout<<endl;
}
void read_code()
{
    ifstream fin;
    char a[100];
    int flag=0;
    di();
    do
    {

        if(flag==0)
            cout<<setw(45)<<"Enter File Name: ";
        cin>>a;
        char b[]=".rasp";
        strcat(a,b);
        fin.open(a,ios::in);
        if(!fin)
        {
            system("cls");
            di();
            cout<<setw(45)<<"File Not Found!"<<endl<<setw(45)<<"Please Enter File Name Again: ";
            flag=1;
        }
        else
        {
            cout<<"\n\n\n"<<setw(40)<<"Loading";
            for(int i=0;i<6;i++)
            {
                cout<<".";
                delay(300);
            }
        }
    }while(!fin);
    set_org(a);
    int flag1=0;
    do
    {
        fin.getline(comp_prog[prog_length],30);
        if(flag1==0)
        {
            //cout<<comp_prog[prog_length];
            prog_length_run++;
            if(strcmp(comp_prog[prog_length],"HLT")==0)
                flag1=1;
        }
        prog_length++;
    }while(!fin.eof());
   // cout<<prog_length<<endl<<prog_length_run;
   // getch();
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************Sometimes you must,***************************************************/
/*******************HURT in order to KNOW,************************************************/
///////////////////////////////////////////////////////////////////////////////////////////
void prog_disp()
{
    rectangle(10,10,600,450);//for whole program display
    line(15,15,15,50);
    line(15,15,90,15);
    line(90,15,100,50);
    rectangle(15,50,595,445);
    outtextxy(20,20,"Program:");
    for(int i=0;i<prog_length;i++)
        outtextxy(25,60+(20*i),comp_prog[i]);
    rectangle(10,480,600,580);//for current line
    line(15,485,15,520);
    line(15,485,120,485);
    line(120,485,130,520);
    outtextxy(20,490,"Current Line:");
    rectangle(15,520,595,575);
    outtextxy(25,530,curr_prog);
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************FAIL in order to GROW,************************************************/
/*******************LOSE in order to GAIN,************************************************/
///////////////////////////////////////////////////////////////////////////////////////////
void draw_tables(int ls,int re)
{

    //int ls=815,re=1315;
    rectangle(ls-15,10,re+15,650);
    rectangle(ls,25,re,65);
    rectangle(ls,65,re,95);
    rectangle(ls,95,re,125);
    rectangle(ls,125,re,155);
    rectangle(ls,155,re,185);
    rectangle(ls,185,re,215);
    rectangle(ls,215,re,245);
    rectangle(ls,245,re,275);
    rectangle(ls,275,re,305);
    rectangle(ls,305,re,315);
    rectangle(ls,430,re,470);
    rectangle(ls,430,re,635);
    line(1065,470,1065,635);
                            //125 units name
    line(940,25,940,305); //125 units for value
    line(1065,25,1065,305); //125 units for first 4 digits
    line(1096,25,1096,305);
    line(1128,25,1128,305);
    line(1159,25,1159,305);
    line(1190,25,1190,305); //125 units for last 4 didgits
    line(1222,25,1222,305);
    line(1254,25,1254,305);
    line(1284,25,1284,305);
    /* above table creation of reg table*/
    /* below table creation of flag table*/
    rectangle(ls,335,re,375);
    rectangle(ls,375,re,405);
    line(940,335,940,405); //125 units for value
    line(1065,335,1065,405); //125 units for first 4 digits
    line(1096,335,1096,405);
    line(1128,335,1128,405);
    line(1159,335,1159,405);
    line(1190,335,1190,405); //125 units for last 4 didgits
    line(1222,335,1222,405);
    line(1254,335,1254,405);
    line(1284,335,1284,405);
}
////////////////////////////////////////////////////////////////////////////////////////////
/*******************Because life's greatest lessons****************************************/
/*******************are learned through PAIN,**********************************************/
///////////////////////////////////////////////////////////////////////////////////////////
void dib(char temp[],int yaxis)
{
    int x=todec(temp);
    int j=7;
    char s2[8];
    itoa(x,s2,2);
    int s4=strlen(s2);
    char disp[]="00000000";
    for(int i=(s4-1);i>=0;i--)
    {
     //   cout<<s2[i];
        if(s2[i]=='1')
            disp[j]='1';
        j--;
    }
    char t[2];t[0]=disp[0];t[1]='\0';
    outtextxy(1075,yaxis,t);
    t[0]=disp[1];t[1]='\0';
    outtextxy(1105,yaxis,t);
    t[0]=disp[2];t[1]='\0';
    outtextxy(1135,yaxis,t);
    t[0]=disp[3];t[1]='\0';
    outtextxy(1170,yaxis,t);
    t[0]=disp[4];t[1]='\0';
    outtextxy(1200,yaxis,t);
    t[0]=disp[5];t[1]='\0';
    outtextxy(1233,yaxis,t);
    t[0]=disp[6];t[1]='\0';
    outtextxy(1265,yaxis,t);
    t[0]=disp[7];t[1]='\0';
    outtextxy(1295,yaxis,t);
}
void fill_table_data()
{
    char temp[4];
    settextstyle(3,HORIZ_DIR,1);
    outtextxy(830,35,"Register");
    outtextxy(960,35,"Value");
    outtextxy(1075,35,"7");
    outtextxy(1105,35,"6");
    outtextxy(1135,35,"5");
    outtextxy(1170,35,"4");
    outtextxy(1200,35,"3");
    outtextxy(1233,35,"2");
    outtextxy(1265,35,"1");
    outtextxy(1295,35,"0");
    outtextxy(830,345,"Register");
    outtextxy(960,345,"Value");
    outtextxy(820,70,"Accumulator");
    outtextxy(960,70,A.data);
    dib(A.data,70);
    outtextxy(820,100,"Register B");
    outtextxy(960,100,B.data);
    dib(B.data,100);
    outtextxy(820,130,"Register C");
    outtextxy(960,130,C.data);
    dib(C.data,130);
    outtextxy(820,160,"Register D");
    outtextxy(960,160,D.data);
    dib(D.data,160);
    outtextxy(820,190,"Register E");
    outtextxy(960,190,E.data);
    dib(E.data,190);
    outtextxy(820,220,"Register H");
    outtextxy(960,220,H.data);
    dib(H.addr,220);
    outtextxy(820,250,"Register L");
    outtextxy(960,250,L.data);
    dib(L.addr,250);
    outtextxy(820,280,"Memory(M)");
    outtextxy(960,280,M);
    dib(M,280);
    outtextxy(1075,345,"S");
    itoa(FLAGRG[S],temp,2);
    outtextxy(1075,380,temp);
    outtextxy(1105,345,"Z");
    itoa(FLAGRG[Z],temp,2);
    outtextxy(1105,380,temp);
    outtextxy(1135,345,"*");
    itoa(FLAGRG[FL1],temp,2);
    outtextxy(1135,380,temp);
    outtextxy(1165,345,"AC");
    itoa(FLAGRG[AC],temp,2);
    outtextxy(1165,380,temp);
    outtextxy(1200,345,"*");
    itoa(FLAGRG[FL2],temp,2);
    outtextxy(1200,380,temp);
    outtextxy(1233,345,"P");
    itoa(FLAGRG[P],temp,2);
    outtextxy(1233,380,temp);
    outtextxy(1265,345,"*");
    itoa(FLAGRG[FL3],temp,2);
    outtextxy(1265,380,temp);
    outtextxy(1290,345,"CY");
    itoa(FLAGRG[CY],temp,2);
    outtextxy(1290,380,temp);
    outtextxy(820,380,"Flag Register");
    int val=0;
    for(int i=7;i>0;i--)
       {
           val=val+(FLAGRG[i]*(pow(2,7-i)));
           //cout<<"j="<<7-i<<val;
        }
    //cout<<"\nk="<<val<<endl;
    char te[10]={'\0'};
    itoa(val,te,16);
    outtextxy(970,380,te);
    for(int i=0;i<dbl;i++)
        outtextxy(835,(480+(i*20)),db[i]);
}
/////////////////////////////////////////////////////////////////////////
/***********************************************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////

int hexto(char a)
{
    switch(a)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'A':
        return 10;
    case 'B':
        return 11;
    case 'C':
        return 12;
    case 'D':
        return 13;
    case 'E':
        return 14;
    case 'F':
        return 15;
    }
}
/////////////////////////////////////////////////////////////////////////
/***********************************************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////

int todec(char c[100])
{
    int i,l=strlen(c);
    int d,s=0;
    for(i=l-1;i>=0;i--)
    {
        d=hexto(c[i]);
        s=s+d*pow(16,(l-1-i));
    }
    return(s);
}
/////////////////////////////////////////////////////////////////////////
/***********************************************************************/
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////

char tochar(int a)
{
    char c=(a+48);
    return(c);
}
char * tohex(int a)
{
    char s[5];
    char t;
    int i=0;
    while(a>0)
    {
        int r=a%16;
        a=a/16;
        if(r==15)
        {
            t='F';
        }
        else if(r==14)
        {
            t='E';
        }
        else if(r==13)
        {
            t='D';
        }
        else if(r==12)
        {
            t='C';
        }
        else if(r==11)
        {
            t='B';
        }
        else if(r==10)
        {
            t='A';
        }
        else
        {
            t=tochar(r);
        }
        s[i]=t;
        i++;
    }
    s[i]='\0';
    strrev(s);
   // cout<<s;
    return (s);
}
int bintodec(char s[8])
{
    int i,j=0,sum=0;
    for(i=strlen(s)-1;i>=0;i--)
    {
        if(s[i]=='1')
            sum=sum+pow(2,j);
        j++;
    }
    return sum;
}
void parity(char str[3])
{
    int y,i,c=0;
    y=todec(str);
    char s[8];
    itoa(y,s,2);
    //printf("s=%s",s);
    for(i=0;s[i]!='\0';i++)
    {
        if(s[i]==1)
            c++;
    }
    if(c%2==0)
    {
        FLAGRG[5]=1;
    }
    else
    {
        FLAGRG[5]=0;
    }
    //printf("%d",FLAGRG[5]);
}
