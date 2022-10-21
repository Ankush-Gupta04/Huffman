#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
struct heap{
    int freq;
    char element;
    struct heap *left;
    struct heap *right;
};
struct minheap{
    int size;
    int capacity;
    struct heap** array;
};
struct minheap* createminheap(int data){
    struct minheap* minheap = (struct minheap*)malloc(sizeof(struct minheap));
    minheap->size = 0;
    minheap->capacity = data;
    minheap->array = (struct heap**)malloc(minheap->capacity * sizeof(struct heap*));
    return minheap;
}
void swap(struct heap** a, struct heap** b){
    struct heap* temp = *a;
    *a = *b;
    *b = temp;
} 
struct heap* newnode(char data, int freq){
    struct heap* temp = (struct heap*)malloc(sizeof(struct heap));
    temp->left = temp->right = 0;
    temp->element = data;
    temp->freq = freq;
    return temp;
}
void minheapify(struct minheap* minheap, int idx){
    int smallest = idx;
    int left = 2*idx + 1;
    int right = 2*idx + 2;
    if (left < minheap->size && minheap->array[left]->freq < minheap->array[smallest]->freq){
        smallest = left;
    }
    if (right < minheap->size && minheap->array[right]->freq < minheap->array[smallest]->freq){
        smallest = right;
    }
    if (smallest != idx){
        swap(&minheap->array[smallest], &minheap->array[idx]);
        minheapify(minheap,smallest);
    }
}
void buildminheap(struct minheap* minheap){
    int n = minheap->size -1;
    int i;
    for (i = (n-1)/2 ; i>= 0; --i){
        minheapify(minheap,i);
    }
}
int isSizeOne(struct minheap* minheap){
    return (minheap->size == 1);
}
struct heap* extractmin(struct minheap* minheap){
    struct heap* temp = minheap->array[0];
    minheap->array[0] = minheap->array[minheap->size -1];
    --minheap->size;
    minheapify(minheap,0);
    return temp;
}
void insertminheap(struct minheap* minheap, struct heap* top){
    int i = minheap->size;
    ++minheap->size;
    while (i && top->freq < minheap->array[(i-1)/2]->freq){
        minheap->array[i] = minheap->array[(i-1)/2];
        i = (i-1)/2;
    }
    minheap->array[i] = top ;
}
struct heap* buildtree(char arr[], int freq[], int n){
    struct heap *left, *right, *top;
    struct minheap* minheap = createminheap(n);
    for (int i = 0; i< n; ++i){
        minheap->array[i] = newnode(arr[i], freq[i]);
    }
    minheap->size = n;
    buildminheap(minheap);
    while(!isSizeOne(minheap)){
        left = extractmin(minheap);
        right = extractmin(minheap);
        top = newnode('$',left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertminheap(minheap,top);
    }
    return extractmin(minheap);
}
void printArr(char arr[],int top, char final2[][20],int j){
	char chr[10]; 
    int i;
    for (i = 0; i<top; i++){
        printf("%d ",arr[i]);
		if (arr[i] == 0){
			chr[i] = '0';}
		else{
			chr[i] = '1';
		}
	}
	chr[i] = '\0';
	strcpy(final2[j], chr);
    printf("\n");
}
int isLeaf(struct heap* root){
    return !(root->left) && !(root->right);
}
void printCodes(struct heap* root,char arr[],int top,char final1[],char final2[][20]){
	static int i = 0;
    if (root->left){
        arr[top] = 0;
        printCodes(root->left,arr,top+1,final1,final2);
    }
    if (root->right){
        arr[top] = 1;
        printCodes(root->right,arr,top+1,final1,final2);
    }
    if (isLeaf(root)){
        printf("%c -> ",root->element);
		final1[i] = root->element;
        printArr(arr,top,final2,i);
		i++;
    }
}
struct heap* huffmancodes(char arr[],int freq[], int n,char final1[], char final2[][20]){
    struct heap *root;
    root = buildtree(arr,freq,n);
    int  top = 0;
    printCodes(root,arr,top,final1,final2);
	return root;
}
void converttobin(int ba, char str[]){
    int i = 0;
    char f[8];
    while(ba != 0){
        int rem;
        rem = ba % 2;
        if (rem){
            f[i] = '1';
        }
        else{
            f[i] = '0';
        }
        i++;
        ba = ba/2;
    }
    int k;
    f[i] = '\0';
    k = 8- i;
    int index = 0;
    while(k){
        str[index] = '0';
        k = k-1;
        index++;
    }
    i--;
    while(index < 7){
        str[index] = f[i-k];
        k++;
        index++;
    }
}
int main(){
	char input[10];
	char output[10];
	printf("Enter the name of the input file ");
	scanf("%s",input);
	printf("Enter the name of the output file ");
	scanf("%s",output);
	int what;
	printf("Enter mode (0 for compression, 1 for decompression) ");
	scanf("%d",&what);
	if (!what){
        FILE* x;
        char ch;
        char ele[30];
	    int freq[30];
		x = fopen(input,"r");
		int i = 0;
		while (!feof(x)){
			ch = fgetc(x);
			int flag = 1;
			for (int j = 0; j<i; j++){
				if (ele[j] == ch){
					freq[j] += 1;
					flag = 0;
					break;
				}
			}
			if (flag){
				ele[i] = ch;
				freq[i] = 1;
				i++;
			}
		}
		i--;
		// for (int j = 0; j<i; j++){
		// 	printf("%c %d\n",ele[j],freq[j]);
		// }
		fclose(x);
		char final1[20];
        char final2[20][20];
        for (int j = 0; j < 20; j++) {
            final2[j][0] = '\0';
        }
		struct heap* root;
		root = huffmancodes(ele,freq,i,final1,final2);
		// for (int j = 0; j < i ; j++ ){
        //         // printf("%d ",j);
		// 		printf("%c %s \n",final1[j],final2[j]);
		// 	}
        // printf("b ");
        FILE* mn;
        // printf("b");
		mn = fopen(input,"r");
        // printf(" b");
		FILE* z;
		z = fopen("temp.txt","w");
        printf("Generating ");
        printf("%s\n",output);
		while(!feof(mn)){
			ch = fgetc(mn);
			for (int j = 0; j < i; j++){
                if (ch == final1[j]){
                    fprintf(z,"%s",final2[j]);
                    break;
                }
            }
		}
        fclose(mn);
        fclose(z);
        FILE* bjp;
        FILE* cng;
        bjp = fopen("temp.txt","r");
        cng = fopen(output,"w");
        int bains = 0;
        int counter;
        counter = 0;
        while(!feof(bjp)){
            if (counter < 7){
                ch = fgetc(bjp);
                // printf("%c-",ch);
                if (ch == '1'){
                    bains += 1*pow(2,6 - counter);
                    // printf("%d ",bains);
                    counter ++;
                }
                else if (ch == '0'){
                    bains += 0;
                    counter ++;
                    // printf("%d ",bains);
                }
                else{
                    break;
                }
            }
            else{
                counter = 0;
                char temp;
                // printf("%d ",bains);
                temp = (char)bains;
                bains = 0;
                fprintf(cng,"%c",temp);
            }
        }
        fclose(bjp);
        fclose(cng);
        bjp = fopen(output,"r");
        char proxy[3200];
        int counti = 0;
        while(!feof(bjp)){
            char c;
            c = fgetc(x);
            proxy[counti] = c;
            counti++;
        }
        fclose(bjp);
        bjp = fopen(output,"w");
        for (int j = 0; j<i; j++){
            fprintf(bjp,"%c",final1[j]);
            fprintf(bjp,"%s"," ");
            fprintf(bjp,"%s",final2[j]);
            fprintf(bjp,"%s","\n");
        }
        for (counti = 0; counti < 3200; counti++){
            fprintf(bjp,"%c",proxy[counti]);
        }
        fclose(bjp);
        }

	
    else{
    printf("Genrating ");
    printf("%s",output);
        int jeet = 0;
    int counter = 1;
    FILE* x;
    x = fopen(output,"r");
    FILE* y;
    y = fopen("temp2.txt","w");
    char c;
    while(!feof(x)){
        c = fgetc(x);
        if (counter <7){
            if (c == '\n'){
                counter++;
                continue;
            }
            // printf("%c",c);
        }
        if (counter >= 7){
        // printf("%d\n",c);
        int p;
        p = (int)c;
        int i = 0;
        char ram[8];
        ram[7] = '\0';
        while (p>= 1){
            // printf("%d ",p);
            int b;
            b = p%2;
            if (b){
                ram[6-i] = '1';
            }
            else{
                ram[6-i] = '0';
            }
            i++;
            p = p/2;
        }
        while (i<=6){
            ram[6-i] = '0';
            i++;
        }
        // printf("%s\n",ram);
        fprintf(y,"%s",ram);
        }
    }
    fclose(x);
    fclose(y);
    char final[30];
    char final2[30][30];
    final[0] = 'a';
    final[1] = 'c';
    final[2] = 'b';
    final[3] = 'f';
    final[4] = 'e';
    final[6] = 'd';
    strcpy(final2[0], "0\0");
    strcpy(final2[1] , "100\0");
    strcpy(final2[2] , "101\0");
    strcpy(final2[3] , "1100\0");
    strcpy(final2[4] , "1101\0");
    strcpy(final2[5] , "111\0");
    int i = 6;
    x = fopen("in.txt","r");
    y = fopen("res.txt","w");
    unsigned char cl;
    int z = 0;
    char ramm[8];
    int flag = 0;
    char local;
    while(!feof(x)){
        cl = fgetc(x);
        int cht;
        cht = (int)cl;
        if (cht < 130){
        fprintf(y,"%c",cl);}
    }
    fclose(x);
    fclose(y);
    }
    return 0;
}