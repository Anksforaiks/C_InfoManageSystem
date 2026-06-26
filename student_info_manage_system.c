#include<stdio.h>
#include<stdlib.h>
#define FN "student.dat"

typedef struct STUDENT{
	int num;
	char name[20];
	int score;
	struct STUDENT *next;
} STU;

void append_new_info(STU **head){// solution直接传头指针地址并修改头指针
	printf("请输入要添加的学生信息(学号 姓名 成绩)\n");
	STU *add=(STU*)malloc(sizeof(STU)),*cur=*head;
	add->next=NULL;
	scanf("%d %s %d",&add->num,add->name,&add->score);
	if(*head==NULL){//处理头指针为空
		*head=add;
		printf("学生信息添加成功:)\n\n");
	}else{
		while(cur->next){
			if(cur->num==add->num){
				printf("该学生信息已存在，添加失败:(\n\n");
				break;
			}
			cur=cur->next;
		}
		if(cur->next==NULL){
			cur->next=add;
			printf("学生信息添加成功:)\n\n");
		}
	}
}

void output_info(STU *head){
	while(head){
		printf("%d %s %d\n",head->num,head->name,head->score);
		head=head->next;
	}
	putchar(10);
}

void delete_info(STU **head){
	printf("请输入要删除的学生学号\n");
	int toDel;
	STU *pre=NULL,*cur=*head;
	scanf("%d",&toDel);
	if((*head)->num==toDel){
		*head=cur->next;
		free(cur);
		printf("学号为%d的学生信息已成功删除:)\n\n",toDel);
	}else{
		while(cur){
			if(cur->num==toDel){
				pre->next=cur->next;
				free(cur);
				printf("学号为%d的学生信息已成功删除:)\n\n",toDel);
				break;
			}
			pre=cur;
			cur=cur->next;
		}
		if(cur==NULL)
			printf("不存在这个学生，删除失败:(\n\n");
	}
}

void change_score(STU *head){
	printf("请输入要修改的学生学号及成绩修改目标值(学号 成绩)\n");
	int cnum,cscore;
	scanf("%d %d",&cnum,&cscore);
	while(head){
		if(head->num==cnum){
			head->score=cscore;
			printf("已修改学生%d的成绩为%d了:)\n\n",head->num,head->score);
			break;
		}
		head=head->next;
	}
	if(head==NULL)
		printf("不存在这个学生，修改失败:(\n\n");
}

void show_info(STU *head){
	printf("请输入分数最低值(成绩大于该值的学生信息将被展示)\n");
	int min;
	scanf("%d",&min);
	system("cls");
	while(head){
		if(head->score>=min)
			printf("%d %s %d\n",head->num,head->name,head->score);
		head=head->next;
	}
	putchar(10);
}

void save_data(STU *head){
	FILE *f=fopen(FN,"w");
	if(f==NULL){
		printf("无法打开文件:(\n\n");
		exit(0);
	}else{
		printf("已成功打开文件:)\n");
		while(head){
			fprintf(f,"%d %s %d\n",head->num,head->name,head->score);
			head=head->next;
		}
		printf("已成功保存信息:)\n");
	}
	if(fclose(f)==-1)
		printf("无法关闭文件:(\n\n");
	else
		printf("已成功关闭文件:)\n\n");
}

STU *load_data(){
	int i;
	FILE *f=fopen(FN,"r");
	STU *head=NULL,*cur=(STU*)malloc(sizeof(STU)),*tail=NULL;
	if(f==NULL){
		printf("无法打开文件:(\n\n");
		exit(0);
	}else{
		printf("已成功打开文件:)\n");
		i=fscanf(f,"%d %s %d",&cur->num,cur->name,&cur->score);
		while(i>=0){
			if(head==NULL)
				head=cur;
			else
				tail->next=cur;
			tail=cur;
			cur=(STU*)malloc(sizeof(STU));
			i=fscanf(f,"%d %s %d",&cur->num,cur->name,&cur->score);
		}
		if(tail) tail->next=NULL;
		printf("已成功读取信息:)\n");
	}
	if(fclose(f)==-1)
		printf("无法关闭文件:(\n\n");
	else
		printf("已成功关闭文件:)\n\n");
	return head;
}

void exit_system(STU *head){
	printf("正在保存数据文件......\n");
	save_data(head);
	printf("数据文件自动保存成功，位于同目录%s文件下\n",FN);
	printf("正在释放链表空间......\n");
	STU *tmp=NULL;
	while(head){
		tmp=head;
		head=head->next;
		free(tmp);
	}
	printf("空间释放成功，正常退出程序:)\n");
}

int main(){
	int option=0;
	STU *head=NULL;
	printf("——————《学生成绩管理系统》——————\n");
	printf("\nver:0.9  Created by Anks on 2026.3.15\n");
	while(1){
		printf("-------------------------------------\n输入对应数字执行相应操作\n\n");
		printf("[1]添加学生信息\n[2]显示所有学生\n[3]按学号删除学生\n[4]按学号修改学生成绩\n[5]按分数筛选显示\n[6]保存数据到文件\n[7]从文件读取数据\n[8]退出系统\n\n");
		printf(">>>");
		scanf("%d",&option);
		system("cls");
		switch(option){
			case 1:
				append_new_info(&head);
				break;
			case 2:
				output_info(head);
				break;
			case 3:
				delete_info(&head);
				break;
			case 4:
				change_score(head);
				break;
			case 5:
				show_info(head);
				break;
			case 6:
				save_data(head);
				break;
			case 7:
				head=load_data();
				break;
			case 8:
				exit_system(head);
				break;
			default:
				printf("%d？数字范围是1-8哦，你是故意还是有意的？\n\n",option);	
		}
		if(option==8) break;
	}
	return 0;
}
