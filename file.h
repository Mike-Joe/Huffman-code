#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int char_count = 0;
int max_heap_index;
int input_file_char_num = 0;
int output_file_char_num = 0;

typedef struct Node
{
    char c;
    int freq;
    struct Node*left ;
    struct Node*right ;
}Node;

Node * heap;

typedef struct ascii_table_node
{
    int freq;
    char code[100];
}ascii_table_node;

ascii_table_node * ascii_table;

void initialize_table_nodes();
void create_ascii_table();
void insert_into_table(char c);
void read_from_file(char file[]);
void initialize_array_nodes();
void create_array();
void insert_chars_into_array();
void exchange(int index1 , int index2);
int parent_index(int index);
int left_index(int index);
int right_index(int index);
void min_heapify(int index);
void build_min_heap();
Node * create_node();
Node * extract_min();
void assign(int index , Node*new_node);
void fix_heap(int index);
void insert_into_heap(int index , Node*new_node);
Node * huffman_tree();
int is_leaf(Node * node);
void get_char_codes(Node * root);
void print_output(char file[]);
char * string_parsing(char * str,int start , int end);
void compressed_file(char file[]);
void compress_file(char file[]);

void compress_file(char file[])
{
  clock_t execution_time;
  execution_time = clock();
  create_ascii_table();
  read_from_file(file);
  create_array();
  insert_chars_into_array();
  build_min_heap();
  get_char_codes(huffman_tree());
  print_output(file);
  compressed_file(file);
  execution_time = clock() - execution_time;
  printf("execution time = %f", ((double)execution_time)/CLOCKS_PER_SEC );


}



void initialize_table_nodes()
{
   int i;
   for(i = 0;i<256;i++)
    ascii_table[i].freq = 0;
}

void create_ascii_table()
{
    ascii_table = (ascii_table_node*) malloc(256 * sizeof(ascii_table_node));
    initialize_table_nodes();
}


void insert_into_table(char c)
{
    if(ascii_table[(int)c].freq == 0)
     char_count ++;

    ascii_table[(int)c].freq ++;
}


void read_from_file(char file[])
{
    char c;
    FILE * fptr = fopen(file,"r");
    if(fptr == NULL)
     {
       printf("error opening read file");
       return;
     }

    while( ( c = fgetc(fptr) )!= EOF)
    {
       insert_into_table(c);
       input_file_char_num ++;
    }
    fclose(fptr);
    max_heap_index = char_count -1;
}


void initialize_array_nodes()
{
    int i;
    for(i = 0;i <= max_heap_index;i++)
     {
       heap[i].left = heap[i].right = NULL;
     }
}


void create_array()
{
   heap = (Node*) malloc(char_count * sizeof(Node));
   initialize_array_nodes();
}


void insert_chars_into_array()
{
    int i,array_index=0;
    for(i = 0;i<256;i++)
     {
       if (ascii_table[i].freq != 0 )
        {
           heap[array_index].c = (char)i;
           heap[array_index].freq = ascii_table[i].freq;
           array_index ++;
        }
     }
}


void exchange(int index1 , int index2)
{
    int temp_c = heap[index1].c;
    int temp_freq = heap[index1].freq;
    Node* temp_left_node_ptr = heap[index1].left;
    Node* temp_right_node_ptr = heap[index1].right;

    heap[index1].c = heap[index2].c;
    heap[index1].freq = heap[index2].freq;
    heap[index1].left = heap[index2].left;
    heap[index1].right = heap[index2].right;

    heap[index2].c = temp_c;
    heap[index2].freq = temp_freq;
    heap[index2].left = temp_left_node_ptr;
    heap[index2].right = temp_right_node_ptr;

}


int parent_index(int index) { return (index - 1)/2;  }

int left_index(int index) { return 2*index+1;  }

int right_index(int index) { return 2*index+2; }


void min_heapify(int index)
{

  if( left_index(index) <= max_heap_index)
  {
    int smallest_index;
    if( heap[left_index(index)].freq < heap[index].freq )
    smallest_index = left_index(index);
    else
    smallest_index = index;

    if( right_index(index) <= max_heap_index && heap [right_index(index)].freq < heap[smallest_index].freq )
    smallest_index = right_index(index);

    if( smallest_index != index)
    {
     exchange( index , smallest_index);
     min_heapify(smallest_index);
    }

  }
  else return;
}


void build_min_heap()
{
    int i;
    for( i = parent_index(max_heap_index) ; i>=0 ; i--)
     {
       min_heapify (i);
     }
}


Node * create_node()  {   return (Node*) malloc(sizeof(Node));     }


Node * extract_min()
{  Node * min = create_node();
   min ->c = heap[0].c;
   min ->freq = heap[0].freq;
   min ->left = heap[0].left;
   min ->right = heap[0].right;
   exchange(0,max_heap_index);
   max_heap_index --;
   min_heapify(0);
   return min;
}


void assign(int index , Node*new_node)
{
   heap[index].freq = new_node ->freq;
   heap[index].left = new_node ->left;
   heap[index].right = new_node ->right;
}


void fix_heap(int index)
{
    if (index != 0)
    {
      if (heap[index].freq < heap[parent_index(index)].freq)
      {
          exchange(index , parent_index(index));
          fix_heap(parent_index(index));
      }
    }
}


void insert_into_heap(int index , Node*new_node)
{
    assign(index , new_node);
    fix_heap(index);
}


Node * huffman_tree()
{
  int i;
  for(i=0 ; i<char_count-1 ; i++)
  {
      Node * new_node = create_node();
      new_node ->left = extract_min();
      new_node ->right = extract_min();
      new_node ->freq = new_node ->left ->freq + new_node ->right ->freq;
      insert_into_heap(++max_heap_index, new_node);
  }
  return extract_min();
}


int is_leaf(Node * node)
{
  if (node ->left == NULL && node ->right == NULL)
    return 1;
  else
    return 0;
}


void get_char_codes(Node * root)
{
  static char code[100] = "\0";
  if (!is_leaf(root))
  {
    strcat(code,"0");
    get_char_codes(root ->left);

    code[strlen(code)-1] = '1';
    get_char_codes(root->right);

    code[strlen(code)-1] = '\0';
  }
  else
    strcpy(ascii_table[(int)root ->c].code , code);
}


void print_output(char file[])
{
   char fname[1000];
   strcpy(fname, file);
    int i;
    FILE * fptr = fopen(strcat(fname, ".huff"),"w+");
    fprintf(fptr,"%d\n%d\n",input_file_char_num,char_count);
    for(i = 255;i>=0;i--)
     {
       if (ascii_table[i].freq != 0 )
        {
            printf("char = %c frequency = %d code = %s\n",(char)i ,ascii_table[i].freq , ascii_table[i].code);
            fprintf(fptr,"%d ",i);
            fputs(ascii_table[i].code,fptr);
            fputc('\n',fptr);
        }
     }

     fclose(fptr);
}

char * string_parsing(char * str,int start , int end)
{
   // printf("%d %d\n",end ,start);
    char * parsed_str = (char*)malloc((end - start + 2)*sizeof(char));
    int i=0;

    for(i=0 ; start<=end ; start++)
    {
      parsed_str[i] = str[start];
      i++;
    }
    parsed_str[i] = '\0';

    return parsed_str;
}


void compressed_file(char file[])
{
  char fname[1000];
  strcpy(fname, file);

  int length;
  char c ;
  char buffer_str[50] = "\0";
///  int buffer_counter = 0;
///  char * buffer_str = (char*)malloc(sizeof(char));
  *buffer_str = '\0';
  char * temp_str;
  FILE * fptr_read = fopen(file,"r");
  FILE * fptr_write = fopen(strcat(fname, ".huff"),"a");
  if(fptr_read == NULL)
   {
    printf("error opening read file");
    return;
   }
  if(fptr_write == NULL)
   {
    printf("error opening write file");
    return;
   }
  while( ( c = fgetc(fptr_read) )!= EOF)
   {
///    buffer_counter = buffer_counter + strlen(ascii_table[(int)c].code) ;
//    printf("bufct = %d\n",buffer_counter);
///    buffer_str = (char*)realloc(buffer_str,buffer_counter*sizeof(char));
//    printf("realloc\n");
    printf("c = %c\n",c);

    strcat(buffer_str , ascii_table[(int)c].code);
    printf("buffer str before = %s\n",buffer_str);
    while(strlen(buffer_str) >= 8)
    {

      temp_str = string_parsing(buffer_str , 0,7);

///      buffer_counter = buffer_counter - 8;
//      printf("temp str = %s , char in file = %c\n",temp_str,(char)strtol(temp_str,NULL,2));
      fputc((char)strtol(temp_str,NULL,2),fptr_write);
      output_file_char_num ++;
///     buffer_str = string_parsing( buffer_str   , 8 , strlen(buffer_str)-1  ) ;
      strcpy(buffer_str, string_parsing( buffer_str   , 8 , strlen(buffer_str)-1  ) );
//      printf("buffer str after = %s\n",buffer_str);
    }

   }
   length = strlen(buffer_str);
   if(length >0)
   {
       printf("here\n");
//       printf("buffer before = %s\n",buffer_str);
       while(length < 8)
       {
           strcat(buffer_str ,"0");
           length++;
       }
//       printf("buffer after = %s\n",buffer_str);
       fputc((char)strtol(buffer_str,NULL,2),fptr_write);
   }
    fclose(fptr_read);
    fclose(fptr_write);
   printf("compression ratio = %f\n",(float)output_file_char_num/input_file_char_num);
}
