#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>

typedef struct node
{
    int data;
    char alpha;
    struct node* next;
    struct tree* subtree;
}node;

typedef struct tree
{
    int data;
    int alpha;
    struct tree* left;
    struct tree* right;
}tree;

typedef struct binarystore{
    unsigned char* bits;
    char x;
    struct binarystore *next;
}binarystore;

struct compressedData
{
    unsigned int a;
};


unsigned char* binarycodes[127];
node *head;
binarystore *head1;
tree *root;
// char msg[10000000]="";
void insert(int data,char index){
    node *newnode=(struct node*)malloc(sizeof(struct node));
    newnode->data=data;
    newnode->alpha=index;
    newnode->next=NULL;
    newnode->subtree=NULL;
    if(head==NULL || head->data>newnode->data){
        newnode->next=head;
        head=newnode;
    }else{
        node *ite;
        ite=head;
        while (ite->next!=NULL && ite->next->data<=newnode->data)
        {
            ite=ite->next;
        }
        newnode->next=ite->next;
        ite->next=newnode;
        
    }
}
void storebinarycode(char *str,char alpha){
    binarystore *newnode=(struct binarystore*)malloc(sizeof(binarystore));
    // char tmpstr[16] = "";
    // strcpy(tmpstr, str);
    // printf("in storebinarycode %s\n", str);
    // newnode->bits=atoi(str);
    binarycodes[alpha] = (char *)malloc(sizeof(str));
    strcpy(binarycodes[alpha], str);

    newnode->bits = (char *)malloc(sizeof(str));
    strcpy(newnode->bits, str);
    // newnode->bits = str;
    newnode->x=alpha;
    newnode->next=NULL;
    if(head1==NULL){
        head1=newnode;
    }else{
        newnode->next=head1;
        head1=newnode;
    }
}
void tracker(tree *root,char str[]){
    if(root->left==NULL && root->right==NULL){
        strcat(str, "\0");
        storebinarycode(str,root->alpha);
        // Dispbinarycodes();
        // printf("%s ",str);
        // printf("%c\n", root->alpha);
        // str="";
        return;
    }
    char tmpstr[100] = "";
    strcpy( tmpstr, str);
    // printf("at tracker %s %s\n", tmpstr, str);
    if(root->left!=NULL){
        strcat(tmpstr,"0");
        tracker(root->left,tmpstr);
    }
    // strcpy(str, tmpstr);
    strcpy( tmpstr, str);
    // printf("at tracker %s %s\n", tmpstr, str);
    if(root->right!=NULL){
        strcat(tmpstr,"1");
        tracker(root->right,tmpstr);
    }
    
}
void push(tree* tnode){
    node *newnode=(struct node*)malloc(sizeof(struct node));
    newnode->data=tnode->data;
    newnode->subtree=tnode;
    newnode->next=NULL;

    if(head==NULL || head->data>newnode->data){
        newnode->next=head;
        head=newnode;
    }else{
        node *ite;
        ite=head;
        while (ite->next!=NULL && ite->next->data<newnode->data)
        {
            ite=ite->next;
        }
        newnode->next=ite->next;
        ite->next=newnode;
    }
}
void display(tree *parent){
    if(parent==NULL){
        return;
    }
    display(parent->left);

    printf("Data are : %d \n",parent->data);
    
    display(parent->right);
}

node* pop(){
    node *temp=head;
    if(temp!=NULL){
        head=head->next;
        return temp;
        
    }
    return NULL;
}

void Disp(){
    node *print;
    print=head;
    while (print!=NULL)
    {
        printf("the elements are %c:%d\n",print->alpha,print->data);
        print =  print->next;
    }
}

void Dispbinarycodes(){
    binarystore *temp=head1;
    // FILE *fptr = fopen("sample_compressed.txt", "w");
    // printf("%s",temp->bits);
    while (temp!=NULL)
    {
        printf("The Element are %c:%s\n",temp->x,temp->bits);
        // char *toWrite = "";
        // strcat(toWrite, temp->x);
        // strcat(toWrite, " ");
        // strcat(toWrite, temp->bits);
        // strcat(toWrite, "\n");
        // fwrite(toWrite, sizeof(char), strlen(toWrite), fptr);
        // printf("The Element are %c: %s\n", temp->x, binarycodes[temp->x]);
        temp=temp->next;
    }
    // fclose(fptr);
}
tree* treenodegenerator(node* temp){
    tree* newnode=(struct tree*)malloc(sizeof(struct tree));
    if(temp->subtree==NULL){
        newnode->data=temp->data;
        newnode->alpha=temp->alpha;
        newnode->left=NULL;
        newnode->right=NULL;
    }
    else{
        return temp->subtree;
    }
    return newnode;
}
void treeinsert(){
    // printf("Entered");
    tree* newnode=(struct tree*)malloc(sizeof(struct tree));
    newnode->left=treenodegenerator(pop());
    newnode->right=treenodegenerator(pop());
    newnode->data=newnode->left->data + newnode->right->data;
    root=newnode;
    push(newnode);
    // printf("exited");
}

void printleafnodes(tree *root){
    if(root->left==NULL && root->right==NULL){
        // printf("%c",root->alpha);
        return;
    }
    printleafnodes(root->left);
    printleafnodes(root->right);
}

void decoding(tree *root1,char text[], char msg[]){
    tree *temp=root1;
    int k=0;
    for(int i=0;i<strlen(text);i++){
        if(text[i]=='0'){
            temp=temp->left;
        }
        else{
            temp=temp->right;
        }
        if(temp->left==NULL && temp->right==NULL){
            msg[k++]=temp->alpha;
            temp=root1;
        }
    }
    msg[k] = '\0';
    printf("The decoded text is : %s\n",msg);
}
void encoding(char text[],char msg[],int n){
    for(int i=0;i<n;i++){
        // printf("%s",binarycodes[msg[i]]);
        strcat(text,binarycodes[msg[i]]);
    }
    printf("The Encoded Text is : %s\n",text);
}

long long int getFileSize(FILE *fp){
    fseek(fp, 0L, SEEK_END);

    return ftell(fp);
}


void slice( char *input, int start, int end, char *result) {
        int i = 0;
        result[0] = '\0';
        // printf("\nslice %s %d %d %s\n", input, start, end, result);
        // end = (strlen(input)<end)? strlen(input): end;
        for( i = start; i<end; i++){
                // printf("\nslice %c %d %d %s\n", input[i], i, i-start, result);
                result[i-start] = input[i];
                // printf("res %s\n", result);
                if( i == strlen(input)){
                        result[i-start+1] = '\0';
                        break;
                }
        }
        result[i-start] = '\0';
}

void getTwoPower(int *ret, int size){
        // int ret[size];
        int i = 0;
        ret[0] = 1;
        for(i=1; i<size; i++){
                ret[i] = ret[i-1]*2;
        }
        // return ret;
}

int getDecimal(char* input){
        int len = strlen(input);
        int size = sizeof(int)*8;
        int twoPower[size+2];
        getTwoPower(twoPower, size+1);
        int dec = 0;

        // printf("len %d\n", len);

        for(int i=len-1; i >= 0; i--){
                // printf("dec = %c %d %d\n", input[i], dec, twoPower[len-i-1]);
                dec += (input[i]-'0') * twoPower[len-i-1];
        }

        return dec;
}

void addStrings( char *dest, char *source){
        int i = 0;
        int j = strlen(dest);
        // printf("dest = %d\n", j);
        for( i = 0; i<strlen(source); i++){
                dest[j++] = source[i];
        }
        dest[j] = '\0';
}

int convertEncoded(char *encode, int* converted) {
        int size = sizeof(int)*8;
        // int converted = 0;
        char inp[size+1];
        int j = 0;

        int block = 0;
        // printf("inp len %d\n", strlen(encode));

        while( block <= strlen(encode) ) {
                inp[0] = '\0';
                // printf("convert before = %s\n", inp);
                slice(encode, block, block+size-2, inp);
                // printf("convert = %s\n", inp);
                char toConvert[size+1];
                // if( inp[0] == '0' ){
                        toConvert[0] = '1';
                        toConvert[1] = '\0';
                        addStrings(toConvert, inp);
                // }
                // else{
                //         toConvert[0] = '0';
                //         toConvert[1] = '\0';
                //         addStrings(toConvert, inp);
                //         // strncat( toConvert, inp, strlen(inp));
                // }
                // printf("toConvert = %s\n", toConvert);
                converted[j++] = getDecimal(toConvert);
                // write to file
                // printf("converted = %d\n",converted[j-1]);
                block+=size-2;
        }
        return j;
}

void getBinary(int val,char bin[]){
        // char bin[33];
        // bin[0] = '\0';
        while(val > 0){
                char tp[33];
                if( val%2 == 1){
                        tp[0] = '1';
                }
                else{
                        tp[0] = '0';
                }
                tp[1] = '\0';
                // printf("%s %s\n", tp, bin);
                addStrings(tp, bin);
                strcpy(bin, tp);
                val/=2;
        }
        // printf("bin = %s\n", bin);
}

void getDecoded(int* converted, int sizeArray, char bin[]){
        // printf("array size = %d\n", sizeArray);
        int i = 0;
        for(i = 0; i < sizeArray; i++ ){
                int val = converted[i];
                // printf("val = %d\n", val);
                // bin[i] = malloc(sizeof(char)*33);
                char tempbin[33] = "\0";
                getBinary(val, tempbin);
                // printf("bin = %s\n", tempbin);
                int len = strlen(tempbin);
                // if( len == 31 ){
                char toAdd[33];
                toAdd[0] = '\0';
                slice(tempbin, 1, len, toAdd);
                addStrings(bin, toAdd);
                // }
                // else{
                //         addStrings(bin, tempbin);
                // }
        }
}
void decimalToBinary(int num,char bin[]) {   
   // Stores binary representation of number.
   int binaryNum[32]; // Assuming 32 bit integer.
   int i=0;
   
   for ( ;num > 0; ){
      binaryNum[i++] = num % 2;
      num /= 2;
   }
   int k=0;
   
   // Printing array in reverse order.
   for (int j = i-2; j >= 0; j--){
        if(binaryNum[j]==0){
            bin[k]='0';
        }else{
            bin[k]='1';
        }
        k++;
   }
}

int main(){
    int arr[127];
    FILE *out;
    for(int i=0;i<127;i++){
        arr[i]=0;
    }
    int n,data,count=0;
    FILE *fp1=fopen("sample.txt","r");
    int filesize = getFileSize(fp1);
    char msg[filesize+1];
    // fseek(fdes, 0L, SEEK_SET);
    printf("Text read %d\n", filesize);
    char buff[filesize];
    int fdes = open("sample.txt", O_RDONLY, 0);
    size_t noofchar=read(fdes,buff,sizeof(buff));
    // printf("Text read %s\n",buff);
    close(fdes);
    printf("%d",noofchar);
    for(int i=0;i<strlen(buff);i++){
        arr[buff[i]]++;
    }
    char str[16]="";
    
    char text[100*filesize];
    while (1)
    {
        printf("\nEnter the choice \n1:Insert\n2:display\n3:tree construct\n4:display tree\n5:Tracker\n6:Display binary codes\n7:Print leaf nodes\n8:Encoding\n9:Decoding\n10:exit\n");
        // scanf("%d",&n);
        for(int i=1;i<=10;i++){
            n=i;
            switch(n){
                case 1:
                    for(int i=0;i<127;i++){
                        if(arr[i]>0){
                            insert(arr[i],i);
                            count++;
                        }
                    }
                    break;
                case 2:Disp();
                    break;
                case 3:
                    for(int i=0;i<count-1;i++){
                        treeinsert();
                    }
                    break;
                case 4:display(root);
                    break;
                case 5:tracker(root,str);
                    break;
                case 6:Dispbinarycodes();
                    break;
                case 7:printleafnodes(root);
                break;
                case 8:
                    encoding(text,buff,noofchar);
                    int encoded[100000];
                    int len = convertEncoded(text, encoded);
                    FILE *fdes2=fopen("sample-compressed.txt","w");
                    // putw(strlen(text), fdes2);
                    // fwrite( "\n", sizeof(char), 1, fdes2);
                    // printf("%d",encoded[0]);
                    for(int i = 0; i < len; i++){
                        putw(encoded[i], fdes2);
                        // fwrite( " ", sizeof(char), 1, fdes2);
                    }
                    fclose(fdes2);
                    char decoded[1000000];
                    getDecoded(encoded, len, decoded);
                    printf("\nDecoded from file = %s\n", decoded);
                    // if( strcmp(text, decoded) == 0 ){
                    //     printf("Match\n");
                    // }
                    // FILE *fdes2=fopen("sample-compressed.bin","wb");
                    // if( !fdes2 ){
                    //     printf("error opening file");
                    //     return -1;
                    // }
                    // printf("Size of compressedData is %lu bytes\n", sizeof(short));
                    // for(int i=0;i<d;i++){
                    //     struct compressedData rec;
                    //     int *wr = NULL;
                    //     int tp = 0;
                    //     if(text[i]=='0'){
                    //         tp = 0;
                    //         // wr=&tp;
                    //     }else{
                    //         tp = 1;
                    //         // wr = &tp;
                    //     }
                    //     // fwrite(&tp, 1, 1, fdes2);
                    // }
                    // fopen(&out,"sample_compressed.bin","wb");
                    // size_t elements_written=fwrite(text,sizeof(bool),d,out);
                    // printf("%d == %d\n",elements_written,d);
                    // fclose(out);
                    // char *des;
                    // des=(struct array*)malloc(sizeof(struct array)*strlen(text));
                    // for(int i=0;i<strlen(text);i++){
                    //     des[i]=text[i];
                    // }
                    // int e=strlen(des);
                    // printf(" Des : %s  :  %d\n",des,sizeof(des));
                    // for(int i=0;i<d;i++){
                    //     printf(" %d ",des[i]);
                    // }
                    // write(fdes2,text,100000);
                    // fclose(fdes2);
                    break;
                case 9:
                    // char text[1000000];
                    // FILE *fdes3=fopen("sample-compressed.txt","r");
                    // int i=getw(fdes3);
                    // while (i!=EOF)
                    // {
                    //     char bin[32];
                    //     decimalToBinary(i,bin);
                    //     strcat(text,bin);
                    //     i=getw(fdes3);
                    // }
                    decoding(root,text, msg);
                    break;
                case 10:exit(0);
                default:exit(0);
            }
        }
    }
}