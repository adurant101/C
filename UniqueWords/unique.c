#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

//struct node for character array linked list
typedef struct node
{
    char* data;
    struct node *next;
} node;

//node create and node delete
node* node_create(char* s);
void node_delete(node *n);

//creates new node from char* to prepare for add into list
//create memory for char* for node list
node* node_create(char* s)
{
    if (s == NULL)return NULL;
    node* n = (node *) malloc(sizeof (node));
    int len = strlen(s);
    n->data = (char *) malloc(len + 1);
    memset(n->data, '\0', len + 1);
    strncpy(n->data, s, len + 1);
    n->next = NULL;
    return n;
}

//deletes a node* requested
void node_delete(node *n)
{
    if (n->data != NULL)free(n->data);
    n->data = NULL;
    if (n != NULL)free(n);
    n = NULL;
}

//struct occur for head and tail of list
typedef struct occur
{
    node *head;
    node *tail;
} occur;

//initialize occur list
occur* occur_new()
{
    occur* o = (occur *) malloc(sizeof (occur));
    o->head = NULL;
    o->tail = NULL;
    return o;
}

//print occur list
void occur_print(occur*);
//gets count of total number of items in occur list
int getCount(occur*);
//add char* at end of occur list
node* occur_appendNode(occur *, char *);

//get count of total number of nodes in occur list
int getCount(occur* o)
{
    node* nde = o->head;
    if (o->head == NULL)printf("Empty List!\n");
    int i = 0;
    while (nde != NULL)
	{
        i++;
        nde = nde->next;
    }
    return i;
}

//print out all nodes from occur list
void occur_print(occur *o)
{
    node* nde = o->head;
    if (o->head == NULL)printf("Empty List!\n");
    int i = 0;
    while (nde != NULL)
	{
        printf("%s\n", nde->data);
        nde = nde->next;
        i++;
    }
}

//add char* to end of occur list, tail is end of list
node* occur_appendNode(occur *o, char *str)
{
    node *nde = node_create(str);
    if (o->head == NULL)
	{
        o->head = nde;
        o->head->next = NULL;
        o->tail = o->head;
        o->tail->next = NULL;
    }
	else
	{
        o->tail->next = nde;
        o->tail = o->tail->next;
        o->tail->next = NULL;
    }
    return nde;
}

//check if char* is a word, letters of -
int isIden(char *str);
//gets words from a occur list
void getWords(occur*, char*);
//gets lowercase words from a occur list
void getLower(occur*, char[3000]);
//deletes occur list
void delList(occur *o);
//get char* from pos from occur list
char* getFromPos(occur*,int);
//search for char* from position
int searchFromPos(occur*,char*,int);
//get uniques from occur list
void getUniques(occur*,occur*,occur*);
//search for char* in list
int occur_inList(occur *o, char *str);

int main()
{
    //file operations, input output file, variables for use
    char filename[200];
    FILE *fin = NULL, *fout = NULL;
    char str[10000], strOther[10000];
    size_t line_buf_size = 10000;
    int k = 0;
    char line_buf[10000];
    fout = fopen("out.txt", "w");
    while(strcmp(filename,"q"))
    {
        //prompt filename
        printf("Please enter filename, or 'q' to quit\n");
        scanf("%s",filename);
        if(strcmp(filename,"q")==0)
            break;
        fin = fopen(filename, "r");
        //create occur lists
        occur* o = occur_new();
        occur* ol = occur_new();
        occur* uni = occur_new();
        occur* allUni = occur_new();
        if(!fin)
        {
            printf("File not open\n");
        }
        else
        {
            //read file get char from file
            while (fgets(line_buf, line_buf_size, fin))
            {
                memset(str, '\0', 2000);
                k = 0;
                while (k < strlen(line_buf))
                {
                    str[k] = line_buf[k];
                    k++;
                }
                //put file char* into word, and lowercase words functions from list
                strcpy(strOther,str);
                getWords(o,str);
                getLower(ol,strOther);
            }
            //close input for next input prompt
            fclose(fin);
            //gets all uniques, and uniques more than 4 chars, display info
            getUniques(ol,allUni,uni);
            printf("Filename is %s\n", filename);
            fprintf(fout, "Filename is %s\n", filename);
            printf("Total words: %d\n",getCount(o));
            fprintf(fout, "Total words: %d\n",getCount(o));
            printf("Unique words over 4 characters: %d\n",getCount(uni));
            fprintf(fout, "Unique words over 4 characters: %d\n",getCount(uni));
            printf("Unique words total: %d\n\n",getCount(allUni));
            fprintf(fout, "Unique words total: %d\n\n",getCount(allUni));
            //delete lists for reuse
            delList(o);
            delList(ol);
            delList(uni);
            delList(allUni);
        }
    }
	return 0;
}

//checks if char* is a word, only letters or '-'
int isIden(char *str)
{
    for (unsigned int i = 0; i < strlen(str); i++)
	{
        int uppercaseChar = toupper(str[i]);
        if (uppercaseChar < 'A' || uppercaseChar > 'Z')
		{
		    if(uppercaseChar != '-')
                return 0;
        }
    }
    return 1;
}

//deletes all nodes from occur list
void delList(occur *o)
{
    node* nde = o->head;
    if (o->head == NULL)
	{
        if (o != NULL)free(o);
        return;
    }
    while (nde != NULL)
	{
        o->head = nde->next;
        if (nde != NULL)node_delete(nde);
        nde = o->head;
    }
    if (o->head != NULL)printf("Error in delList!\n");
    o->tail = o->head;
    if (o != NULL)free(o);
}

//gets words from char* tokenizing adding each token to occur list
void getWords(occur* o, char* str)
{
    char tmp[400];
    char* p = NULL;
    p = strtok(str," .!?:,'\";");
    while(p!=NULL)
    {
        memset(tmp, '\0', 400);
        for (unsigned int i = 0; i < strlen(p); i++)
		{
            tmp[i] = p[i];
        }
        if(isIden(tmp))
        {
            occur_appendNode(o,tmp);
        }
        p = strtok(NULL," .!?:,'\";");
    }
}

//gets lowercase tokens from char[] tokenizing each token into occur list
void getLower(occur* o, char str[3000])
{
    char tmp[400];
    char* p = NULL;
    p = strtok(str," .!?:,'\";");
    while(p!=NULL)
    {
        memset(tmp, '\0', 400);
        for (unsigned int i = 0; i < strlen(p); i++)
		{
            tmp[i] = p[i];
        }
        for(int i = 0; i < strlen(tmp); i++)
            tmp[i] = tolower(tmp[i]);
        if(isIden(tmp))
        {
            occur_appendNode(o,tmp);
        }
        p = strtok(NULL," .!?:,'\";");
    }
}

//from pos return char* at position from occur list
char* getFromPos(occur* o, int pos)
{
    int i = 0;
    node* nde = o->head;
    while(nde != NULL && i < pos)
    {
        i++;
        nde = nde->next;
    }
    return nde->data;
}

//check true or false if char is in list from a certain pos checking through
//rest of list
int searchFromPos(occur* o,char* str, int pos)
{
    int found = 0, i = 0;
    node *nde = o->head;
    //traverse to pos requested
    while(nde != NULL && i < pos+1)
    {
        i++;
        nde = nde->next;
    }
    //check for matches for inputted char*
    while(nde != NULL)
    {
        if (strncmp(nde->data, str, strlen(str) + 1) == 0) {
            found = 1;
            break;
        }
        nde = nde->next;
    }
    return found;
}

//place unique nodes into two lists, one with all uniques one with only uniques
//that are 4 chars or more
void getUniques(occur* lower, occur* all, occur* more)
{
    node* loNode = lower->head;
    //happens to check if char* is unique in list of tokens
    occur* happens = occur_new();
    int i = 0, len = 0;
    char tok[300];
    while(loNode != NULL)
    {
        //takes token, checks through rest of list to see if token is present,
        //if token is present adds into happens list
        strcpy(tok,loNode->data);
        if(searchFromPos(lower,tok,i))
        {
            occur_appendNode(happens,tok);
        }
        else
        {
            //if token not present checks if token is in happens list(previously
            //found), if not checks length of token to see if more than 4 chars
            //if more than 4 chars adds to occur list with unique words more than
            //4 chars, else adds all unique words into another occur list
            if(! (occur_inList(happens,tok) ) )
            {
                len = strlen(tok);
                if(len>4)
                    occur_appendNode(more,tok);
                occur_appendNode(all,tok);

            }
        }
        i++;
        loNode = loNode->next;
    }
    //delete happens
    delList(happens);
}

//search for char* in occur list, return true or false if found
int occur_inList(occur *o, char *str)
{
    int found = 0;
    node* nde = o->head;
    while (nde != NULL)
	{
        if (strncmp(nde->data, str, strlen(str) + 1) == 0) {
            found = 1;
            break;
        }
        nde = nde->next;
    }
    return found;
}

/*
Output:
Please enter filename, or 'q' to quit
Rats.txt
Filename is Rats.txt
Total words: 290
Unique words over 4 characters: 84
Unique words total: 126

Please enter filename, or 'q' to quit
q*/
