//BT20CSE055 Joel

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define size 50
typedef enum bool{False, True} Bool;
//Node for book_db
struct Book_node{
    int book_id;
    char title[size];
    char author[size];
    char subject[size];
    int copies_issued;
    int copies_available;
    int height;
    struct Book_node *lchild;
    struct Book_node *rchild;
};
//Node for request_queue
struct request_node{
    char name[size];
    char title[size];
    struct request_node *next;
};
struct request_q{
    struct request_node *front;
    struct request_node *rear;
};
//Node for borrower_list
struct borrow_node{
    char name[size];
    char title[size];
    char issue_date[9];
    char return_date[9]; //if book is not returned date is 00/00/00 
    struct borrow_node *next;
};
//Node to sort books of a particular subject
struct Node{
    int id;
    char title[size];
    char author[size];
    char subject[size];
    int copies_issued;
    int copies_available;
    struct Node *next;
};
//Node to sort borrower's list
struct Node2{
    char name[size];
    int count;
    char title[size];
    char issue_date[9];
    char return_date[9];
    struct Node2 *next;
};

//Merge sort to sort books of a subject
struct Node* MergeSortedList(struct Node* lst1, struct Node* lst2) 
{  
	struct Node* result = NULL; 
	if (lst1 == NULL){ 
		return (lst2);
    } 
	else if (lst2 == NULL){ 
		return (lst1); 
    }
	if (lst1->copies_available <= lst2->copies_available) { 
		result = lst1; 
		result->next = MergeSortedList(lst1->next, lst2); 
	} 
	else { 
		result = lst2; 
		result->next = MergeSortedList(lst1, lst2->next); 
	} 
	return result; 
} 
void SplitList(struct Node* head, struct Node** front, struct Node** back) 
{ 
	struct Node* p; 
	struct Node* q; 
	q = head; 
	p = head->next; 
	while (p != NULL) { 
		p = p->next; 
		if (p != NULL) { 
			q = q->next; 
			p = p->next; 
		} 
	} 
	*front = head; 
	*back = q->next; 
	q->next = NULL; 
} 
void MergeSort(struct Node** book_db) 
{ 
	struct Node* head = *book_db; 
	struct Node* p; 
	struct Node* q; 
	if ((head == NULL) || (head->next == NULL)) { 
		return; 
	} 
	SplitList(head, &p, &q); 
	MergeSort(&p); 
	MergeSort(&q); 
	*book_db = MergeSortedList(p, q); 
} 

//Merge sort to sort borrowers
struct Node2* MergeSortedList_node(struct Node2* lst1, struct Node2* lst2) 
{ 
	struct Node2* result = NULL; 
	if (lst1 == NULL){ 
		return (lst2);
    } 
	else if (lst2 == NULL){ 
		return (lst1); 
    }
	if (lst1->count >= lst2->count) { 
		result = lst1; 
		result->next = MergeSortedList_node(lst1->next, lst2); 
	} 
	else { 
		result = lst2; 
		result->next = MergeSortedList_node(lst1, lst2->next); 
	} 
	return result; 
} 
void SplitList_node(struct Node2* head, struct Node2** front, struct Node2** back) 
{ 
	struct Node2* p; 
	struct Node2* q; 
	q = head; 
	p = head->next; 
	while (p != NULL) { 
		p = p->next; 
		if (p != NULL) { 
			q = q->next; 
			p = p->next; 
		} 
	} 
	*front = head; 
	*back = q->next; 
	q->next = NULL; 
} 
void MergeSort_node(struct Node2** borrowers) 
{ 
	struct Node2* head = *borrowers; 
	struct Node2* p; 
	struct Node2* q; 
	if ((head == NULL) || (head->next == NULL)) { 
		return; 
	} 
	SplitList_node(head, &p, &q); 
	MergeSort_node(&p); 
	MergeSort_node(&q); 
	*borrowers= MergeSortedList_node(p, q); 
} 

//Function to check if book exists and to make sure books of same subject and title are not written by different authors
Bool check_book(struct Book_node *book_db,char title[],char author[],char subject[],int available){
    struct Book_node *p=book_db;
    Bool retval=True;
    if(p==NULL){
        return retval;
    }
    if(strcmp(p->title,title)==0 && strcmp(p->subject,subject)==0){
        retval=False;
        if(strcmp(p->author,author)!=0){
            printf("Cannot add book\n");
        }
        else{
            printf("Book already exists, Adding the extra copies\n");
            p->copies_available+=available;
        }
        return retval;
    }
    if(retval){
        retval=check_book(book_db->lchild,title,author,subject,available);
    }
    if(retval){
        retval=check_book(book_db->rchild,title,author,subject,available);
    }
    return retval;
}

//Functions for insertion into book_db
int Node_height(struct Book_node *p){
    if(p==NULL){
        return 0;
    }
    if(Node_height(p->lchild)>Node_height(p->rchild)){
        return 1+Node_height(p->lchild);
    }
    else{
        return 1+Node_height(p->rchild);
    }
}

int balance_factor(struct Book_node *p){
    if(p==NULL){
        return 0;
    }
    return Node_height(p->lchild)-Node_height(p->rchild);
}

struct Book_node *left_rotate(struct Book_node *p){
    struct Book_node *q=p->rchild;
    struct Book_node *r=q->lchild;
    q->lchild=p;
    p->rchild=r;
    p->height=Node_height(p);
    q->height=Node_height(q);
    return q;
}

struct Book_node *right_rotate(struct Book_node *p){
    struct Book_node *q=p->lchild;
    struct Book_node *r=q->rchild;
    q->rchild=p;
    p->lchild=r;
    p->height=Node_height(p);
    q->height=Node_height(q);
    return q;
}

//Function to add new book to book_db
struct Book_node *add_book(struct Book_node *root, int id,char title[],char author[],char subject[], int available,int issued){
    if(root==NULL){
        struct Book_node *t=(struct Book_node *)malloc(sizeof(struct Book_node));
        t->book_id=id;
        strcpy(t->author,author);
        strcpy(t->subject,subject);
        strcpy(t->title,title);
        t->copies_available=available;
        t->copies_issued=issued; 
        t->height=1;
        t->lchild=NULL;
        t->rchild=NULL;
        return t;
    }
    if(root->book_id<id){
        root->rchild=add_book(root->rchild,id, title, author, subject, available,issued);
    }
    else if(root->book_id>id){
        root->lchild=add_book(root->lchild,id, title, author, subject, available,issued);
    }
    else {
        if(strcmp(root->title,title)!=0){
            printf("Another book with the same id already exists\n");
        }
        else{
            root->copies_available+=1;
        }
        return root;
    }
    root->height=Node_height(root);
    int b=balance_factor(root);
    if(b==2){
        if(balance_factor(root->lchild)==1){
            root=right_rotate(root);
        }
        else if(balance_factor(root->lchild)==-1){
            root->lchild=left_rotate(root->lchild);
            root=right_rotate(root);
        }
    }
    else if(b==-2){
        if(balance_factor(root->rchild)==-1){
            root=left_rotate(root);
        }
        else if(balance_factor(root->rchild)==1){
            root->rchild=right_rotate(root->rchild);
            root=left_rotate(root);
        }
    }
    return root;  
}

//Function to take input from text file containing information about Books
void read_book_file(struct Book_node **book_db){
    FILE *ptr=fopen("book.txt","r");
    char line[100];
    int line_count=0;
    while(fgets(line,100,ptr)){
        int i=0;
        int num=0;
        int id;
        char name[size];
        char author[size];
        char subject[size];
        int available;
        int issued;
        while(num<5 && line[i]=='0'){
            num++;
            i++;
        }
        num=5-num;
        if(num<=1){
            id=line[i]-'0';
            i++;
        }
        else{
            int sum=0;
            while(num>0){
                sum=sum+(pow(10,num-1))*(line[i]-'0');
                i++;
                num--;
            }
            id=sum;
        }
        i++;
        int j=0;
        while(line[i]!='|'){
            name[j]=line[i];
            i++;
            j++;
        }
        name[j]='\0';
        i++;
        j=0;
        while(line[i]!='|'){
            author[j]=line[i];
            j++;
            i++;
        }
        author[j]='\0';
        i++;
        j=0;
        while(line[i]!='|'){
            subject[j]=line[i];
            j++;
            i++;
        }
        subject[j]='\0';
        i++;
        int n=0;
        j=i;
        while(line[i]!='|'){
            n++;
            i++;
        }
        if(n==1){
            available=line[i-1]-'0';
        }
        else{
            int sum=0;
            while(n>0){
                sum=sum+(pow(10,n-1))*(line[j]-'0');
                j++;
                n--;
            }
            available=sum;
        }
        i++;
        n=0;
        j=i;
        while(line[i]!='\n'){
            n++;
            i++;
        }
        if(n==1){
            issued=line[i-1]-'0';
        }
        else{
            int sum=0;
            while(n>0){
                sum=sum+(pow(10,n-1))*(line[j]-'0');
                j++;
                n--;
            }
            issued=sum;
        }
        *book_db=add_book(*book_db,id,name,author,subject,available,issued);
    }
}

//Function to initialize request_queue
void initialize(struct request_q *request_queue){
    request_queue->front=NULL;
    request_queue->rear=NULL;
}

//Function to return book node containing book with given title
struct Book_node * is_present(struct Book_node *p,char title[]){
    if(p==NULL){
        return NULL;
    }
    if(strcmp(p->title,title)==0){
        return p;
    }
    else{
        struct Book_node *q=is_present(p->lchild,title);
        if(q!=NULL){
            return q;
        }
        struct Book_node *r=is_present(p->rchild,title);
        if(r!=NULL){
            return r;
        }
        else{
            return NULL;
        }
    }   
}
//Function to add a new request
void insert_request (struct request_q *request_queue,char name[],char title[], struct Book_node *book_db, struct borrow_node *borrower_list){
    struct Book_node *p=is_present(book_db,title);
    if(p==NULL || p->copies_available==0){
        printf("Book not available\n");
    }
    else{
        struct borrow_node *q=borrower_list;
        struct request_node *r=request_queue->front;
        int count=0;
        while(q!=NULL && count<3){
            if(strcmp(q->name,name)==0 && strcmp(q->return_date,"00/00/00")==0){
                count++;
            }
            q=q->next;
        }
        while(r!=NULL && count<3){
            if(strcmp(r->name,name)==0){
                count++;
            }
            r=r->next;
        }
        if(count==3){
            printf("Cannot issue more than 3 books to same person\n");
        }
        
        else{
            struct request_node *t=(struct request_node *)malloc(sizeof(struct request_node));
            if(request_queue->front==NULL){
                strcpy(t->name,name);
                strcpy(t->title,title);
                request_queue->front=t;
                request_queue->rear=t;
                t->next=NULL;
            }
            else{
                (request_queue->rear)->next=t;
                strcpy(t->name,name);
                strcpy(t->title,title);
                t->next=NULL;
                (request_queue->rear)=t;
            }
        }
    }
}

//Function to read from text file storing the requests
void read_request_file(struct request_q *request_queue,struct Book_node* book_db,struct borrow_node *borrower_list){
    FILE *ptr2=fopen("requests.txt","r");
    char line[100];
    int line_count=0;
    while(fgets(line,100,ptr2)){
        int i=0;
        char name[size];
        char title[size];
        while(line[i]!='|'){
            name[i]=line[i];
            i++;
        }
        name[i]='\0';
        i++;
        int j=0;
        while(line[i]!='\n'){
            title[j]=line[i];
            i++;
            j++;
        }
        title[j]='\0';
        insert_request(request_queue,name,title,book_db,borrower_list);
    }
}

//Function to check if difference between return/current date and issue date is greater than 15
Bool check_date(struct borrow_node *b, char current[8]);

//Function to insert a borrow node into the borrower's list
void insert_borrow(struct borrow_node **borrower_list,char name[], char title[],char issue[],char ret[]){
    struct borrow_node *t=(struct borrow_node *)malloc(sizeof(struct borrow_node));
    strcpy(t->name,name);
    strcpy(t->title,title);
    strcpy(t->issue_date,issue);
    strcpy(t->return_date,ret);
    struct borrow_node *p=*borrower_list;
    if(p==NULL){
        *borrower_list=t;
        t->next=NULL;
    }
    else{
        while(p->next!=NULL){
            p=p->next;
        }
        p->next=t;
        t->next=NULL;
    }
}

//Function to read from text file containing information of borrowers
void read_borrow_file(struct borrow_node **borrower_list){
    FILE *ptr3=fopen("borrowers.txt","r");
    char line[100];
    int line_count=0;
    while(fgets(line,100,ptr3)){
        int i=0;
        char name[size];
        char title[size];
        char issue[9];
        char return_date[9];
        while(line[i]!='|'){
            name[i]=line[i];
            i++;
        }
        name[i]='\0';
        i++;
        int j=0;
        while(line[i]!='|'){
            title[j]=line[i];
            i++;
            j++;
        }
        title[j]='\0';
        i++;
        j=0;
        while(line[i]!='|'){
            issue[j]=line[i];
            i++;
            j++;
        }
        issue[j]='\0';
        i++;
        j=0;
        while(line[i]!='\n'){
            return_date[j]=line[i];
            i++;
            j++;
        }
        return_date[j]='\0';
        insert_borrow(borrower_list,name,title,issue,return_date);
    }
}

//Function to fulfil request of first person in request queue. Checks necessary conditions before issuing book
void fulfil_request(struct request_q *request_queue, struct borrow_node **borrower_list, char issue[], struct Book_node *book_db ){
    if(request_queue->front==NULL){
        printf("Request queue is empty\n");
        printf("\n");
    }
    else{
        struct Book_node *q=is_present(book_db,(request_queue->front)->title);
        if(q->copies_available==0){
            printf("Cannot issue, book currently unavailable. Request removed.\n");
            printf("\n");
        }
        else{
            Bool check=True;
            struct borrow_node *a=*borrower_list;
            while(a!=NULL){
                if(strcmp(a->name,(request_queue->front)->name)==0){
                    if(strcmp(a->return_date,"00/00/00")==0){
                        if(!check_date(a,issue)){
                            check=False;
                        }
                    }
                    else{
                        if(!check_date(a,a->return_date)){
                            check=False;
                        }
                    }
                }
                a=a->next;
            }
            if(check){
                q->copies_issued+=1;
                q->copies_available-=1;
                insert_borrow(borrower_list,(request_queue->front)->name,(request_queue->front)->title,issue,"00/00/00");
                struct request_node *q=(request_queue->front)->next;
                struct request_node *p=request_queue->front;
                char name[size];
                strcpy(name,(request_queue->front)->name);
                while(q!=NULL && q!=request_queue->rear){
                    if(strcmp(q->name,name)==0){
                        p->next=q->next;
                        (request_queue->rear)->next=q;
                        request_queue->rear=q;
                        q->next=NULL;
                    }
                    p=p->next;
                    q=p->next;
                }
            }
            else{
                printf("Cannot issue book as student is a defaulter. Request removed\n");
                printf("\n");
            }
        }
        struct request_node *r=request_queue->front;
        request_queue->front=r->next;
        if(request_queue->front==NULL){
            request_queue->rear=NULL;
        }
        free(r);
    }
}

//Function to return a book
void return_book(struct Book_node *book_db,struct borrow_node *borrower_list,char name[],char title[],char date[]){
    struct borrow_node *p=borrower_list;
    Bool found=False;
    while(p!=NULL && !found){
        if(strcmp(p->name,name)==0 && strcmp(p->title,title)==0){
            found=True;
            strcpy(p->return_date,date);
        }
        p=p->next;
    }
    if(p!=NULL){
        struct Book_node *q=is_present(book_db,title);
        q->copies_available+=1;
        q->copies_issued-=1;
    }
}

//Function to find no of days a student has kept book
int calculate_days(struct borrow_node *b,char current[]){
    int days;
    if(current[4]==b->issue_date[4] && current[3]==b->issue_date[3]){
        days=(current[0]-'0')*10+(current[1]-'0')-((b->issue_date[0]-'0')*10+(b->issue_date[1]-'0'));
    }
    else{
        if(b->issue_date[3]=='0' && (b->issue_date[4]=='1' || b->issue_date[4]=='3' || b->issue_date[4]=='5' || b->issue_date[4]=='7' || b->issue_date[4]=='8')){
            days=31-((b->issue_date[0]-'0')*10 + b->issue_date[1]-'0')+(current[0]-'0')*10+(current[1]-'0');
        }
        else if(b->issue_date[3]=='0' && (b->issue_date[4]=='4' || b->issue_date[4]=='6' || b->issue_date[4]=='9')){
            days=30-((b->issue_date[0]-'0')*10 + b->issue_date[1]-'0')+(current[0]-'0')*10+(current[1]-'0');
        }
        else if(b->issue_date[3]=='0' && b->issue_date[4]=='2'){
            days=28-((b->issue_date[0]-'0')*10 + b->issue_date[1]-'0')+(current[0]-'0')*10+(current[1]-'0');
        }
        else if(b->issue_date[3]=='1'){
            if(b->issue_date[4]=='0'){
                days=31-((b->issue_date[0]-'0')*10 + b->issue_date[1]-'0')+(current[0]-'0')*10+(current[1]-'0');
            }
            else{
                days=30-((b->issue_date[0]-'0')*10 + b->issue_date[1]-'0')+(current[0]-'0')*10+(current[1]-'0');
            }
        }
    }
    return days;
}

//Function to check if student is a defaulter
Bool check_date(struct borrow_node *b, char current[9]){
    Bool retval=True;
    int days;
    int month=(current[3]-'0')*10+(current[4]-'0')-((b->issue_date[3]-'0')*10+(b->issue_date[4]-'0'));
    if(month>=2){
        retval=False;
    }
    else{
        days=calculate_days(b,current);
        if(days>15){
            retval=False;
        }
    }    
    return retval;
}

//Extra function for Q3)
void find_next_date(char current[],char second[]){
    int date=(current[0]-'0')*10+(current[1]-'0');
    if(current[3]=='0' && (current[4]=='1' || current[4]=='3' || current[4]=='5' || current[4]=='7' || current[4]=='8')){
        if(date==31){
            second[0]='0';
            second[1]='1';
            second[3]=current[3];
            second[4]=current[4]+1;
        }
        else{
            int date2=date+1;
            second[0]=(date2/10)+'0';
            second[1]=(date2%10)+'0';
            second[3]=current[3];
            second[4]=current[4];
        }
    }
    else if(current[3]=='0' && (current[4]=='4' || current[4]=='6' || current[4]=='9')){
        if(date==30){
            second[0]='0';
            second[1]='1';
            if(current[4]=='9'){
                second[3]='1';
                second[4]='0';
            }
            else{
                second[3]=current[3];
                second[4]=current[4]+1;
            }
        }
        else{
            int date2=date+1;
            second[0]=(date2/10)+'0';
            second[1]=(date2%10)+'0';
            second[3]=current[3];
            second[4]=current[4];
        }
    }
    else if(current[3]=='0' && current[4]=='2'){
        if(date==28){
            second[0]='0';
            second[1]='1';
            second[3]='0';
            second[4]='3';
        }
        else{
            int date2=date+1;
            second[0]=(date2/10)+'0';
            second[1]=(date2%10)+'0';
            second[3]=current[3];
            second[4]=current[4];
        }
    }
    else if(current[3]=='1'){
        if((current[4]=='0' && date==31) || (current[4]=='1' && date==30)){
            second[0]='0';
            second[1]='1';
            second[3]='1';
            second[4]=current[4]+1;
        }
        else{
            int date2=date+1;
            second[0]=(date2/10)+'0';
            second[1]=(date2%10)+'0';
            second[3]=current[3];
            second[4]=current[4];
        }
    }
}
int total_books(struct Book_node *book_db){
    if(book_db==NULL){
        return 0;
    }
    return book_db->copies_available+total_books(book_db->lchild)+total_books(book_db->rchild);
}
//Q3)Function to find the no. of books that can be given in three days assuming students will return books 15 days after date of issue
void print_books_three_days(struct Book_node *book_db, char current[],struct borrow_node *borrower_list){
    int n=0;
    struct Book_node *p=book_db;
    n=total_books(book_db);
    struct borrow_node *q=borrower_list;
    char second[9];
    char third[9];
    third[2]=third[5]=second[2]=second[5]='/';
    third[6]=second[6]=current[6];
    third[7]=second[7]=current[7];
    third[8]=second[8]=current[8];
    find_next_date(current,second);
    find_next_date(second,third);
    while(q!=NULL){
        if(strcmp(q->return_date,"00/00/00")==0){
            if(calculate_days(q,current)==15 || calculate_days(q,second)==15 || calculate_days(q,third)==15){
                n++;
            }
        }
        q=q->next;
    }
    printf("No of books available are %d\n",n);

}

//Q1)Function to display students who will get books
void display(struct request_q *request_queue, struct borrow_node *borrower_list, struct Book_node *book_db,char current_date[]){
    struct request_node *p=request_queue->front;
    while(p!=NULL){
        struct borrow_node *q=borrower_list;
        Bool check=True;
        while(q!=NULL && check){
            if(strcmp(p->name,q->name)==0){
                if(strcmp(q->return_date,"00/00/00")==0){
                    if(!check_date(q,current_date)){
                        check=False;
                    }
                }
                else{
                    if(!check_date(q,q->return_date)){
                        check=False;
                    }   
                }      
            }
            q=q->next;
        }
        if(check){
            struct Book_node *r=is_present(book_db,p->title);
            if(r->copies_available==0){
                check =False;
            }
            if(check){
                printf("%s\n",p->name);
            }
        }
        p=p->next;
    }
}

//Q2)Function to find the book in most demand
struct Book_node* most_popular(struct Book_node *book_db,struct request_q *request_queue,struct Book_node *q){
    if(book_db==NULL){
        return q;
    }
    int n1=0;int n2=0;
    struct request_node *p=request_queue->front;
    while(p!=NULL){
        if(strcmp(p->title,book_db->title)==0){
            n1++;
        }
        if(strcmp(p->title,q->title)==0){
            n2++;
        }
        p=p->next;
    }
    if(book_db->copies_issued+n1>q->copies_issued+n2){
        q=book_db;
    }
    struct Book_node *r=most_popular(book_db->lchild,request_queue,q);
    struct Book_node *s=most_popular(book_db->rchild,request_queue,r);
    return s;
    
}

//Q6)Function to display titles of books whose copies are available
void display_requested_available(struct request_q *request_queue,struct Book_node *book_db){
    if(book_db==NULL){
        return;
    }
    display_requested_available(request_queue,book_db->lchild);
    struct request_node *p=request_queue->front;
    int count=0;
    while(p!=NULL){
        if(strcmp(p->title,book_db->title)==0){
            count++;
        }
        p=p->next;
    }
    if(count!=0 && book_db->copies_available>=count){
        printf("%s\n",book_db->title);
    }
    display_requested_available(request_queue,book_db->rchild);
}


//Q7)Function to display books which have not yet been issued
void display_not_issued(struct Book_node *book_db){
    if(book_db==NULL){
        return ;
    }
    display_not_issued(book_db->lchild);
    if(book_db->copies_issued==0){
        printf("%s\n",book_db->title);
    }
    display_not_issued(book_db->rchild);
}

//Function to sort books of a subject
void insert(struct Book_node *p,struct Node **books){
    struct Node *t=(struct Node *)malloc(sizeof(struct Node));
    strcpy(t->author,p->author);
    strcpy(t->subject,p->subject);
    strcpy(t->title,p->title);
    t->id=p->book_id;
    t->copies_available=p->copies_available;
    t->copies_issued=p->copies_issued;
    t->next=NULL;
    if(*books==NULL){
        *books=t;
    }
    else{
        struct Node *q=*books;
        while(q->next!=NULL){
            q=q->next;
        }
        q->next=t;
    }
    //printf("%s\n",t->title);
}
void find_books(struct Book_node *book_db,char subject[],struct Node **books){
    if(book_db==NULL){
        return;
    }
    find_books(book_db->lchild,subject,books);
    if(strcmp(book_db->subject,subject)==0){
        //printf("%s\n",book_db->title);
        insert(book_db,books);
    }
    find_books(book_db->rchild,subject,books);
}
//Q5)Function to sort books of a subject
void sort_subject(struct Book_node *book_db,char subject[]){
    struct Node *books=NULL;
    find_books(book_db,subject,&books);
    MergeSort(&books);
    struct Node *p=books;
    struct Node *q=NULL;
    while(p!=NULL){
        q=p;
        printf("%s by %s\n",p->title,p->author);
        p=p->next;
        free(q);
    }
}

//Q4)Function to sort borrower's list
void sort_borrowers(struct borrow_node *borrowers_list){
    struct borrow_node *p=borrowers_list;
    struct Node2 *r=NULL;
    while(p!=NULL){
        struct Node2 *a;
        if(r==NULL){
            r=(struct Node2 *)malloc(sizeof(struct Node2));
            strcpy(r->name,p->name);
            strcpy(r->title,p->title);
            strcpy(r->issue_date,p->issue_date);
            strcpy(r->return_date,p->return_date);
            r->next=NULL;
            a=r;
        }
        else{
            struct Node2 *t=(struct Node2 *)malloc(sizeof(struct Node2));
            struct Node2 *q=r;
            struct Node2 *s=NULL;
            strcpy(t->name,p->name);
            strcpy(t->title,p->title);
            strcpy(t->issue_date,p->issue_date);
            strcpy(t->return_date,p->return_date);
            while(q!=NULL){
                s=q;
                q=q->next;
            }
            s->next=t;
            t->next=NULL;
            a=t;
            
        }
        struct borrow_node *b=borrowers_list;
        int c=0;
        while(b!=NULL){
            if(strcmp(a->name,b->name)==0){
                c++;
            }
            b=b->next;
        }
        a->count=c;
        struct Node2 *u=r;
        while(u!=NULL){
            if(strcmp(u->name,a->name)==0){
                u->count=c;
            }
            u=u->next;
        }
        p=p->next;
    }
    MergeSort_node(&r);
    struct Node2 *y=r;
    struct Node2 *z=NULL;
    struct borrow_node *x=borrowers_list;
    while(y!=NULL){
        strcpy(x->name,y->name);
        strcpy(x->title,y->title);
        strcpy(x->issue_date,y->issue_date);
        strcpy(x->return_date,y->return_date);
        z=y;
        y=y->next;
        free(z);
        x=x->next;
    }
}

//Function to display book list
void display_book(struct Book_node *root){
    if(root==NULL){
        return;
    }
    else{
        display_book(root->lchild);
        printf("%s\n",root->title);
        display_book(root->rchild);
    }
}

//Function to display request queue
void display_requests(struct request_q *request_queue){
    struct request_node *p=request_queue->front;
    while(p!=NULL){
        printf("%s : %s\n",p->name,p->title);
        p=p->next;
    }
}

//Function to display borrowers list
void display_borrow(struct borrow_node *borrower_list){
    struct borrow_node *p=borrower_list;
    while(p!=NULL){
        printf("%s:%s from %s to %s\n",p->name,p->title,p->issue_date,p->return_date);
        p=p->next;
    }
}

//Function to write back into books file
void rewrite(struct Book_node *book_db, FILE *ptr){
    if(book_db==NULL){
        return;
    }
    rewrite(book_db->lchild,ptr);
    char line[100];
    int i=0;
    int j=0;
    int a=book_db->book_id;
    int b[5];
    while(a>0){
        b[j]=(char)((a%10)+'0');
        j++;
        a=a/10;
    }
    while(j<5){
        b[j]='0';
        j++;
    }
    j--;
    while(j>=0){
        line[i]=b[j];
        j--;
        i++;
    }
    line[i]='|';
    j=0;
    i++;
    while(book_db->title[j]!='\0'){
        line[i]=book_db->title[j];
        i++;
        j++;
    }
    line[i]='|';
    i++;
    j=0;
    while(book_db->author[j]!='\0'){
        line[i]=book_db->author[j];
        i++;
        j++;
    }
    line[i]='|';
    i++;
    j=0;
    while(book_db->subject[j]!='\0'){
        line[i]=book_db->subject[j];
        i++;
        j++;
    }
    line[i]='|';
    i++;
    j=0;
    int n=book_db->copies_available;
    char num[20];
    while(n>0){
        num[j]=(char)((n%10)+'0');
        j++;
        n=n/10;
    }
    j--;
    while(j>=0){
        line[i]=num[j];
        i++;
        j--;
    }
    line[i]='|';
    i++;
    j=0;
    int m=book_db->copies_issued;
    char z[20];
    if(m==0){
        z[0]='0';
        j++;
    }
    while(m>0){
        z[j]=(char)((m%10)+'0');
        j++;
        m=m/10;
    }
    j--;
    while(j>=0){
        line[i]=z[j];
        i++;
        j--;
    }
    line[i]='\n';
    line[i+1]='\0';
    fprintf(ptr,"%s",line);
    rewrite(book_db->rchild,ptr);
        
}
//Function to free book_db
void free_book_db(struct Book_node *book_db){
    if(book_db==NULL){
        return;
    }
    free_book_db(book_db->lchild);
    free_book_db(book_db->rchild);
    free(book_db);
}
//Function to write back into request file and free request queue
void rewrite_request(struct request_q *request_queue){
    FILE *fptr=fopen("requests.txt","w");
    struct request_node *p=request_queue->front;
    struct request_node *q=NULL;
    while(p!=NULL){
        char line[100];
        int i=0;
        int j=0;
        while(p->name[j]!='\0'){
            line[i]=p->name[j];
            i++;
            j++;
        }
        line[i]='|';
        i++;
        j=0;
        while(p->title[j]!='\0'){
            line[i]=p->title[j];
            i++;
            j++;
        }
        line[i]='\n';
        line[i+1]='\0';
        fprintf(fptr,"%s",line);
        q=p;
        p=p->next;
        free(q);
    }
    free(request_queue);
}

//Function to write back into borrowers file and free borrowers list
void rewrite_borrow(struct borrow_node *borrower_list){
    FILE* ptr2=fopen("borrowers.txt","w");
    struct borrow_node *p=borrower_list;
    struct borrow_node *q=NULL;
    while(p!=NULL){
        char line[200];
        int i=0;
        int j=0;
        while(p->name[j]!='\0'){
            line[i]=p->name[j];
            i++;
            j++;
        }
        line[i]='|';
        i++;
        j=0;
        while(p->title[j]!='\0'){
            line[i]=p->title[j];
            i++;
            j++;
        }
        line[i]='|';
        i++;
        j=0;
        while(p->issue_date[j]!='\0'){
            line[i]=p->issue_date[j];
            i++;
            j++;
        }
        line[i]='|';
        i++;
        j=0;
        while(p->return_date[j]!='\0'){
            line[i]=p->return_date[j];
            i++;
            j++;
        }
        line[i]='\n';
        line[i+1]='\0';
        fprintf(ptr2,"%s",line);
        q=p;
        p=p->next;
        free(q);
    }
}

//8)Function to print books in range
void print_in_range(struct Book_node *book_db,int from_id,int to_id){
    if(book_db==NULL){
        return;
    }
    if(book_db->book_id>=from_id && book_db->book_id<=to_id){
        print_in_range(book_db->lchild,from_id,to_id);
        printf("%s\n",book_db->title);
        print_in_range(book_db->rchild,from_id,to_id);
    }
    if(book_db->book_id>to_id){
        print_in_range(book_db->lchild,from_id,to_id);
    }
    if(book_db->book_id<from_id){
        print_in_range(book_db->rchild,from_id,to_id);
    }
}
int main(){
    printf("---------BOOK MANAGEMENT SYSTEM---------\n");
    printf("Options: \n");
    printf("1.Enter a new book   2.Enter a new request   3.Fulfil a request  4.Return book\n");
    printf("5.Students who will get books  6.Book in most demand  7.No of books in three days  8.Sort borrowers list\n");
    printf("9.Sort books of a subject    10.Requested books whose copies are available    11.Books not issued\n");
    printf("12.Display books in range\n");
    printf("13.Exit\n");
    struct Book_node *book_db=NULL; 
    read_book_file(&book_db);
    struct request_q *request_queue=(struct request_q *)malloc(sizeof(struct request_q));
    initialize(request_queue);
    struct borrow_node *borrower_list=NULL;
    read_borrow_file(&borrower_list);
    read_request_file(request_queue,book_db,borrower_list);
    int run=1;
    while(run!=0){
        int n;
        printf("Enter your choice: ");
        scanf("%d",&n);
        printf("\n");
        char title[size];char name[size];char current[9];char subject[size];char temp;
        switch(n){
            case 1:;
                int id;char author[size];int available;
                printf("Enter book id: ");
                scanf("%d",&id);
                printf("Enter title: ");
                scanf("%c",&temp);
                scanf("%[^\n]",title);
                printf("Enter author: ");
                scanf("%c",&temp);
                scanf("%[^\n]",author);
                printf("Enter subject: ");
                scanf("%c",&temp);
                scanf("%[^\n]",subject);
                printf("Enter no. of available copies: ");
                scanf("%d",&available);
                printf("\n");
                book_db=add_book(book_db,id,title,author,subject,available,0);
                printf("\n");
                display_book(book_db);
                printf("\n");
                break;
            case 2:
                printf("Enter name: ");
                scanf("%c",&temp);
                scanf("%[^\n]",name);
                printf("Enter title of book: ");
                scanf("%c",&temp);
                scanf("%[^\n]",title);
                printf("\n");
                insert_request(request_queue,name,title,book_db,borrower_list);
                display_requests(request_queue);
                printf("\n");
                break;
            case 3:;
                char issue[8];
                printf("Enter the date of issue dd/mm/yy: ");
                scanf("%s",issue);
                printf("\n");
                fulfil_request(request_queue,&borrower_list,issue,book_db);
                printf("Request: \n");
                display_requests(request_queue);
                printf("\n");
                printf("Borrowers: \n");
                display_borrow(borrower_list);
                printf("\n");
                break;
            case 4:
                printf("Enter name: ");
                scanf("%c",&temp);
                scanf("%[^\n]",name);
                printf("Enter title: ");
                scanf("%c",&temp);
                scanf("%[^\n]",title);
                printf("Enter the current date dd/mm/yy: ");
                scanf("%s",current);
                printf("\n");
                return_book(book_db,borrower_list,name,title,current);
                display_borrow(borrower_list);
                printf("\n");
                break;
            case 5:;
                printf("Enter the current date dd/mm/yy: ");
                scanf("%s",current);
                printf("\n");
                display(request_queue,borrower_list,book_db,current);
                printf("\n");
                break;
            case 6:;
                struct Book_node *p=most_popular(book_db,request_queue,book_db);
                printf("%s\n",p->title);
                printf("\n");
                break;
            case 7:
                printf("Enter current date dd/mm/yy: ");
                scanf("%s",current);
                printf("\n");
                print_books_three_days(book_db,current,borrower_list);
                break;
            case 8:
                sort_borrowers(borrower_list);
                display_borrow(borrower_list);
                printf("\n");
                break;
            case 9:;
                printf("Enter subject: ");
                scanf("%c",&temp);
                scanf("%[^\n]",subject);
                sort_subject(book_db,subject);
                printf("\n");
                break;
            case 10:
                display_requested_available(request_queue,book_db);
                printf("\n");
                break;
            case 11:
                display_not_issued(book_db);
                printf("\n");
                break;
            case 12:;
                int from,to;
                printf("Enter from id and to id: ");
                scanf("%d %d",&from,&to);
                print_in_range(book_db,from,to);
                printf("\n");
                break;
            case 13:
                run=0;
                FILE *ptr=fopen("book.txt","w");
                rewrite(book_db,ptr);
                free_book_db(book_db);
                rewrite_request(request_queue);
                rewrite_borrow(borrower_list);
                break;
        }
    }
    return 0;
}