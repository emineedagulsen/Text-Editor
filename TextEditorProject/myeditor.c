#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node 
{
	char statement[40]; // max. 40 characters
	int next; // points to the textbuffer[] index of the next statement 
} ;
struct node textbuffer[25]; // max. 25 lines
int free_head; // head node that points to the first free line in textbuffer[]
int inuse_head; // head node that points to the first used line in textbuffer[]

//edit
void edit( char *filename){//filename is a string pointer
	 
	 
	FILE *file;
	file = fopen(filename, "r");
	
    if (file==NULL){//file does not exists
        printw("Error!!! File does not exists!!! \n");
		inuse_head=-1;
		free_head=0;
		textbuffer[0].next=free_head+1;
		for(int i=1;i<25;i++){
			textbuffer[i].next=textbuffer[i-1].next+1;
		}
		textbuffer[24].next=-1;
	}
	
	else {//file exist 
		
		int i,j;
		inuse_head=-1;//daha bi şeyi point etmiyor
		char c;
		c=fgetc(file);
		fclose(file);
		if(c==EOF){//empty file
			
			free_head=0;//textbuffer[0] dan itibaren boş olduğu için
			textbuffer[0].next=free_head+1;
			for(int i=1;i<25;i++){
				textbuffer[i].next=textbuffer[i-1].next+1;
				
			}
			textbuffer[24].next=-1;//son satır bir şeyi point etmez
		}
		else{//not empty file
		file = fopen(filename, "r");
	
		for(j=0;fgets(textbuffer[j].statement,40,file)!=NULL;j++){//satır satır dosyayı okuyor file ın sonuna gelince dur
			inuse_head=0;
			textbuffer[j].next=j+1;
		}
		if(inuse_head!=-1){//not empty file
			free_head=textbuffer[j-1].next;//free_head son satırın altını point edicek
			textbuffer[j-1].next=-1;//son satır başka line point edemez
			for(i=free_head;i<25;i++){
				textbuffer[i].next=i+1;
			}
			textbuffer[24].next=-1;
		
			int k=0;
			do{
			printw("%s",textbuffer[k].statement);
			k++;
			}while(textbuffer[k].next!=-1);
			printw("%s",textbuffer[k].statement);
		}
		fclose(file);
		}
	
	}
	
}


//insert
void insert(int line,char *stat){
	int i,j,k,m,prev;
	if(line>25){//line 25ten fazlaysa error verir
			printw("ERROR!!TEXT BUFFER HAS 25 LINES!!");
		}
	if(inuse_head==-1){// textbuffer boşsa
		
		for(j=0;(j<40);j++)//koyulacak text karakter karakter okunur
				textbuffer[0].statement[j]=stat[j];// ilk satıra konulur
		free_head++;//free head 1 artırılır
		inuse_head=0;//inuse head ilk satırı point eder
		textbuffer[0].next=-1;
	
	}
	else{
		if(line==free_head){//eklemek istediğim line free_heade eşitse
			for(j=0;j<40;j++)
				textbuffer[line].statement[j]=stat[j];//hangi lineda ise ona koyarım 
				prev=inuse_head;//geçici olarak
				int count=line-1;//line'ın 1 eksiğine kadar sayar
				while(count>0){//kaçıncı linea ulaşcaksam o kadar ilerlemesi gerekir
					prev=textbuffer[prev].next; //eklenenin bir üstündekinin linki
					count--;
				}
				textbuffer[free_head].next=textbuffer[prev].next;	//	textbuffer[free_head]'e text eklendi ve son dolu satır olduğundan nexti textbuffer[prev].nexti göstermeli=-1
				textbuffer[prev].next=free_head;
				free_head++; 		
		}
		
		//line free_heade eşit değilse 		
		else {
			if(line==0){//liste doluyken başına ekleme
			
				for(j=0;j<40;j++)
					textbuffer[free_head].statement[j]=stat[j];//free listin ilk satırına texti koydum
				//linkleri güncelle
				textbuffer[free_head].next=inuse_head;
				inuse_head=free_head;
				free_head++;
				printw("\n");
				
				
				}
			else{//insert middle
				for(j=0;j<40;j++)
					textbuffer[free_head].statement[j]=stat[j];//free listin ilk satırına texti koydum
		
				prev=inuse_head;
				int count=line-1;
				while(count>0){
					prev=textbuffer[prev].next;
					count--;
				}
				textbuffer[free_head].next=textbuffer[prev].next;				
				textbuffer[prev].next=free_head;
				free_head++; 
			
			}
		}	
		
	}					
	clear();
	refresh();
	move(0,0);
	k=inuse_head;
	do{
		printw("%s",textbuffer[k].statement);
		k=textbuffer[k].next;
	}while(k!=-1);
	refresh();
	
}

//Delete
void delete (int line) {
	
	int k;
	if ((textbuffer[line].next == -1)&&(textbuffer[inuse_head].next == -1)){
		textbuffer[inuse_head].next = free_head;						              // ilk hâlde free_head = bir alt satır
        free_head = inuse_head;                                           // ilk hâlde free_head = line (index + 1)
		inuse_head=-1;
		clear();
		refresh();
	}
	
    else if (textbuffer[line].next == -1){// son satırı silerken
        // İLK DURUM
        textbuffer[line].next = free_head;						              // ilk hâlde free_head = bir alt satır
        free_head = line;                                           // ilk hâlde free_head = line (index + 1)
		textbuffer[line-1].next = -1;		// artık bir öncekinin next’i, boşu (free_head) gösteriyor
       clear();
		refresh();
		move(0,0);
		k=inuse_head;
		do{
			printw("%s",textbuffer[k].statement);
			k=textbuffer[k].next;
		}while(textbuffer[k].next!=-1);
		
		printw("%s",textbuffer[k].statement );
		refresh();
	}  
    else	// textbuffer[line-1].next != -1
    {
	      if (line != 0)		 															  // ORTADAN SİLERKEN
	      {
				textbuffer[line-1].next = textbuffer[line].next;
		        textbuffer[line].next = free_head;					            // ilk hâlde free_head = satırın kendisi
		        free_head = line;					            // yeni free_head = eski bir öncekinin bir alt satırı
		        		// artık bir öncekinin next’i, yeni şimdikini gösteriyor
	      }
     
	      else  //(line == 1)								                      // BAŞTAN SİLERKEN
	      {
				textbuffer[0].next = free_head;   // ilk hâlde free_head = en son satırın bir altı
				free_head=0;
		        inuse_head ++;			  // yeni inuse_head = ilk satır (kendisi)
	      }
			clear();
			refresh();
			move(0,0);
			k=inuse_head;
			do{
				//printw("%s %d\n",textbuffer[k].statement,textbuffer[k].next );
				printw("%s",textbuffer[k].statement);
				k=textbuffer[k].next;
			}while(textbuffer[k].next!=-1);
			
			printw("%s",textbuffer[k].statement );
			refresh();

	}
}

//save
void save(char *filename){
    FILE *fp;
    fp = fopen(filename ,"w");
    if(fp == NULL){
            perror("fopen failed\n");
            exit(-1);
    }
    int index = inuse_head;
    while((index >= 0) && (index <25)){
    
        fprintf(fp, "%s", textbuffer[index].statement);
        
        if(textbuffer[index].next != -1){
            index = textbuffer[index].next;
        }
        else if(textbuffer[index].next == -1){
            break;
        }
    }
    
    fclose(fp);
}
int main(){

	char filename[20];
	char ch;
	printf("enter a file name:\n");
	scanf("%s",filename);
	initscr();
	edit(filename);
	
	int y=0,x=0,k;
	noecho();
	move(y,x);
	refresh();
	while( ch != 'X'){
		ch=getch();//take input from user
		
		if(ch=='w'){//up
			move(--y,x);
			refresh();
		}
		else if(ch=='z'){//down
			move(++y,x);
			refresh();
		}
		else if(ch=='I'){//insert
		
			int a=0;
			char stat[40];
			scanw("%s", &stat);
			while(stat[a]!='\0'){
				 a++;
			}
			stat[a]='\n';
			stat[a+1]='\0';
			insert(y,stat);
			
			move(++y,x);
			refresh();
		}
		else if(ch=='D'){//delete
			delete(y);
			refresh();
		}
		else if(ch=='S'){//save
			save(filename);
			refresh();
		}
	
	}
	endwin();
	return 0;
}