#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

FILE * fr;
FILE * fw;
int numofchar;

typedef struct node
{
    int character;
    struct node * left;
    struct node * right;
}node;

node* root;
/*
void rootinit()
{
    root->left=NULL;
    root->right=NULL;
}
*/
node* newnode()
{
    node *n=malloc(sizeof(node));
    n->left=NULL;
    n->right=NULL;
    return n;

}

node* adddtotree(node *n,bool isright)
{
    if(!isright)
    {
        puts("left");
        if(n->left==NULL)
            {
                n->left=newnode();
                return n->left;
            }
            else
            {
                return n->left;
            }
    }
    else
    {
        puts("right");
        if(n->right==NULL)
            {
                n->right=newnode();
                return n->right;
            }
            else
            {
                return n->right;
            }
    }
}

node* traversetree(node *n,bool isright)
{
    if(!isright)
    {
                return n->left;
    }
    else if(isright)
    {
                return n->right;
    }
    else
        puts("error");
}
/*Dumb what if he started with zero meaning left branch
bool goright(int *n )
{
    int temp= *n%10;
    *n/=10;
    return temp;
}
*/

bool getnextbit(FILE * f)
{
    static int i=0;
    static int character;
    if(i%8==0)
    {
        i=0;
        character= fgetc(f);
    }
    bool temp =character & ( 128 >> i);
    i++;
    return temp;
    //return character & ( 128 >> i++);
}

void filesinit(char* f_name)
{
    int i,j,diffchar;
    char code[64];
    char numread[4];
    char character;
    bool bit;
    node* n;
    root=newnode();
    fr=fopen(f_name,"r");
    fw=fopen("decompressed.txt","w");
    fscanf(fr,"%d %d ",&numofchar,&diffchar);
    /*numofchar=fgetc(fr);
    fgetc(fr);
    diffchar=fgetc(fr);
    fgetc(fr);
    */
    printf("%d\n%d\n",numofchar,diffchar);

    for (i=0;i<diffchar;i++)
    {
        //character=fgetc(fr);
        fscanf(fr,"%s %s\n",numread,code);
        character = atoi(numread);
        printf("%c %s\n",character,code);
        //code[strlen(code)]='\0'�
        n=root;
        j=0;
        while (code[j]!='\0')
        {
            if(code[j]=='0')
                n=adddtotree(n,false);
            else if(code[j]=='1')
                n=adddtotree(n,true);
            else
                puts("error");
            j++;
        }
        printf("%c\n",character);
        n->character=character;
        printf("%c\n",n->character);
    }
    puts("header read");
    n=root;
    for (i=0;i<numofchar;i++)
    {
        do
        {
            bit=getnextbit(fr);
            n=traversetree(n,bit);
            if(n->right==NULL&&n->left==NULL)
            {
                //fprintf(fw,"%c",n->character);
                fputc(n->character,fw);
                printf("%c",n->character);
                n=root;
            }

        }while(n!=root);
    }
}
