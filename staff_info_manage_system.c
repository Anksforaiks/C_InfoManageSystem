//头文件
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//定义文件名
#define FILE_USER "user_info.dat"
#define FILE_STAFF "staff_info.dat"

//定义别名
#define USER_ADMIN 3
#define USER_MANAGER 2
#define USER_EMPLOYEE 1
#define EDU_SENIORHIGH 1
#define EDU_JUNIORCOLLEGE 2
#define EDU_BACHELOR 3
#define EDU_MASTER 4
#define EDU_DOCTOR 5
#define CURRENT_YEAR 2026

//用户信息结构体
typedef struct USER {
	char user_name[11];
	char password[21];
	int role;
	struct USER *next;
} User;

//日期结构体
typedef struct DATE {
	int year;
	int month;
	int day;
} Date;

//薪资结构体
typedef struct SALARY {
	int year;
	int month;
	float base;
	float bonus;
	float tax;
	float total;
	struct SALARY *next;
} Salary;

//职工信息结构体
typedef struct STAFF {
	char id[11]; //职工号
	char name[51];
	int sex;
	Date birth_date;
	Date hire_date;
	int retirement_year;
	char department[21];
	int edu_level;
	Salary *gain;
	char address[101]; //家庭住址
	char telephone[16];
	char company_email[255];
	char personal_email[255];
	struct STAFF *next;
} Staff;

//不发工资的职工链表
typedef struct STAFF_NO_SALARY {
	char id[11];
	struct STAFF_NO_SALARY *next;
} SNS;

//函数声明
void clear();
char *show_sex(int sex);
char *show_edu(int edu);
User* init_user(User* head);
User* load_user(FILE *f, int *u_len);
Staff* load_staff(FILE *f,int *s_len);
int login(User *head,User **c_user);
int start(User* user_head,User **c_user);
void change_pwd(User* self);
User* find_user(User* head,int target,int len);
void output_user(User* head);
User* add_user(User* head,int *len);
User* delete_user(User* head,int *len);
void change_user(User* head,int len);
User* admin_menu(User* head,int len,User* c_user);
void check_self(Staff* head,User* self);
Staff* change_self(Staff* head,User* self);
void output_staff(Staff* head);
Staff* add_staff(Staff* head,int *len);
Staff* delete_staff(Staff* head,int *len);
void search_staff(Staff* head);
void modify_staff(Staff* head);
void statistics_staff(Staff* head,int len);
void input_salary(Staff* head);
Staff* normal_menu(Staff* head,int len,User* c_user);
void save_user(FILE *f,User* head);
void save_staff(FILE *f,Staff* head);

//主程序
int main(){
	int user_len = 0;
	int staff_len = 0;
	User *cur_user=NULL;
	/***加载***/
	FILE *f_user = fopen(FILE_USER, "r");;
	FILE *f_staff = fopen(FILE_STAFF, "r");
	User *user_head = load_user(f_user,&user_len);
	Staff *staff_head = load_staff(f_staff,&staff_len);
	fclose(f_user);
	fclose(f_staff);
	/**********/
	int cmd = 0;
	while(1){
		cmd = start(user_head,&cur_user);
		if(cmd==0){
			/***保存***/
			f_user = fopen(FILE_USER,"w");
			f_staff = fopen(FILE_STAFF,"w");
			save_user(f_user,user_head);
			save_staff(f_staff,staff_head);
			fclose(f_user);
			fclose(f_staff);
			/**********/
			return 0;
		}
		printf("======职工信息管理系统======\n");
		if(cur_user->role==USER_ADMIN){
			user_head = admin_menu(user_head,user_len,cur_user);
		}else{
			staff_head = normal_menu(staff_head,staff_len,cur_user);
		}
		system("cls");
	}
}

//清空缓冲区剩下的字符
void clear(){
	char c;
	while((c = getchar())!='\n'&&c!=EOF);
}

//返回性别
char *show_sex(int sex){
	switch(sex){
	case 0:
		return "女";
	case 1:
		return "男";
	default:
		return "未知";
	}
}

//返回学历
char *show_edu(int edu){
	switch (edu) {
	case EDU_SENIORHIGH:
		return "高中";
	case EDU_JUNIORCOLLEGE:
		return "大专";
	case EDU_BACHELOR:
		return "本科";
	case EDU_MASTER:
		return "硕士";
	case EDU_DOCTOR:
		return "博士";
	default:
		return "未知";
	}
}

/***********************加载************************/
//初始化管理员用户
User* init_user(User* head){
	head=(User*)malloc(sizeof(User));
	printf("未找到用户文件，正在初始化管理员账号\n");
	strcpy(head->user_name,"admin"),strcpy(head->password,"abc123");
	head->role=3,head->next=NULL;
	printf("账户名：admin\n账号密码：abc123\n");
	return head;
}
//从文件加载数据形成用户链表
User* load_user(FILE *f, int *u_len){
	User *head=NULL,*cur=NULL,*tail=NULL;
	char user_name[11],password[21];
	int role;
	if(f == NULL){
		head = init_user(head);
		*u_len += 1;
	}else{
		while(fscanf(f,"%s %s %d",user_name,password,&role)==3){
			cur=(User*)malloc(sizeof(User));
			strcpy(cur->user_name,user_name);
			strcpy(cur->password,password);
			cur->role=role;
			cur->next=NULL;
			if(head==NULL)
				head=cur;
			else
				tail->next=cur;
			tail=cur;
			(*u_len)++;
		}
	}
	return head;
}
//从文件加载数据形成职工链表
Staff* load_staff(FILE *f,int *s_len){
	Staff *head=NULL,*cur=NULL,*tail=NULL;
	Salary *shead=NULL,*scur=NULL,*stail=NULL;
	char what[10];
	while(fscanf(f,"%s",what)!=EOF){
		if(strcmp(what,"start")==0){
			shead=NULL;
			stail=NULL;
			cur=(Staff*)malloc(sizeof(Staff));
			cur->next=NULL;
			fscanf(f,"%s %s %d %d %d %d %d %d %d %d %s %d %s %s %s %s",
				   cur->id,
				   cur->name,
				   &cur->sex,
				   &cur->birth_date.year,
				   &cur->birth_date.month,
				   &cur->birth_date.day,
				   &cur->hire_date.year,
				   &cur->hire_date.month,
				   &cur->hire_date.day,
				   &cur->retirement_year,
				   cur->department,
				   &cur->edu_level,
				   cur->address,
				   cur->telephone,
				   cur->company_email,
				   cur->personal_email);
		}else if(strcmp(what,"salary")==0){
			scur=(Salary*)malloc(sizeof(Salary));
			scur->next=NULL;
			fscanf(f,"%d %d %f %f %f %f\n",
				   &scur->year,
				   &scur->month,
				   &scur->base,
				   &scur->bonus,
				   &scur->tax,
				   &scur->total);
			if(shead==NULL)
				shead=scur;
			else
				stail->next=scur;
			stail=scur;
		}else if(strcmp(what,"end")==0){
			cur->gain=shead;
			if(head==NULL)
				head=cur;
			else
				tail->next=cur;
			tail=cur;
			(*s_len)++;
		}
	}
	return head;
}
/***********************加载************************/

/***********************登录************************/
//用户登录账号
int login(User *head,User **c_user){
	User *p=head;//用临时指针遍历链表，确保循环输入正常运行
	char name[11];
	char password[21];
	printf("请输入您的用户名（不超过10个字符）:");
	fgets(name, 11, stdin);
	name[strcspn(name, "\n")] = '\0'; // 除去结尾换行符
	printf("请输入您的账号密码（不超过20个字符）:");
	fgets(password, 21, stdin);
	password[strcspn(password, "\n")] = '\0'; // 除去结尾换行符
	for(p=head;p;p = p->next){
		if(strcmp(name, p->user_name)==0 && strcmp(password, p->password) == 0){
			system("cls");
			*c_user = p;
			return 1;
		}
	}
	printf("用户名或密码错误！\n\n");
	clear();
	return 0;
}
//登录菜单
int start(User* user_head,User **c_user){
	int cmd=0;
	while(1){
		printf("======登录账号======\n");
		printf("\n[1]登录账号\n[0]退出系统\n\n请输入(0-1)>");
		scanf("%d",&cmd);
		clear();
		switch (cmd) {
		case 0:
			return cmd;
		case 1:
			if(login(user_head,c_user)==1){
				return (*c_user)->role;
			}
		default:
			printf("未知指令，请重新输入！\n");
		}
		system("cls");
	}
}
/***********************登录************************/

//用户修改密码
void change_pwd(User* self){
	char new_pwd[21];
	printf("请输入您的新账号密码（不超过20个字符）:");
	fgets(new_pwd, 21, stdin);
	new_pwd[strcspn(new_pwd, "\n")] = '\0'; // 除去结尾换行符
	strcpy(self->password,new_pwd);
	printf("您的密码已更改为:%s\n",self->password);
}

/***********************用户管理************************/
//按用户列表序号查找用户
User* find_user(User* head,int target,int len){
	int i=1;
	if(target>0&&target<=len){
		while(head){
			if(i==target){
				return head;
			}
			head=head->next;
			i++;
		}
	}
	return NULL;
}
//查看所有用户的信息
void output_user(User* head){
	int i=0;
	User *p=head;
	system("cls");
	printf("\n序号:%-11s %-21s %-3s\n","用户名","用户密码","权限");
	printf("--------------------------------------------\n");
	while(p!=NULL){
		printf("%4d:%-11s %-21s %-3d\n",++i,p->user_name,p->password,p->role);
		p=p->next;
	}
	printf("--------------------------------------------\n");
}
//增加新用户并授予权限
User* add_user(User* head,int *len){
	User *p=head,*add=NULL;
	char name[11];
	char password[21];
	int role=1;
	system("cls");
	printf("创建新的用户名为（不超过10个字符）:");
	fgets(name, 11, stdin);
	name[strcspn(name, "\n")] = '\0'; // 除去结尾换行符
	while(p->next){
		if(strcmp(p->user_name,name)==0){
			printf("该用户已存在，请修改用户名！\n");
			return head;
		}
		p=p->next;
	}
	printf("创建新的账号密码（不超过20个字符）:");
	fgets(password, 21, stdin);
	password[strcspn(password, "\n")] = '\0'; // 除去结尾换行符
	printf("1->员工,2->经理,3->管理员\n您希望设定该用户的权限为：");
	scanf("%d",&role);
	clear();
	add=(User*)malloc(sizeof(User));
	strcpy(add->user_name,name),strcpy(add->password,password);
	add->role=role,add->next=NULL;
	p->next=add;
	printf("成功添加用户%s\n",add->user_name);
	(*len)++;
	return head;
}
//按用户列表序号删除用户
User* delete_user(User* head,int *len){
	User *pre=head,*p=head;
	int i=1,to_delete=0;
	system("cls");
	printf("请输入您要删除的用户的序号（不超过10个字符）:");
	scanf("%d",&to_delete);
	clear();
	if(to_delete>0&&to_delete<=*len){
		if(to_delete==1){
			printf("该用户账号被系统保留，不能删除！\n");
			return head;
		}else{
			while(i<to_delete){
				pre=p;
				p=p->next;
				i++;
			}
			pre->next=p->next;
			printf("用户%s已删除\n",p->user_name);
			free(p);
			(*len)--;
			return head;
		}
	}else{
		printf("该用户不存在！\n");
		return head;
	}
}
//修改用户的密码和权限
void change_user(User* head,int len){
	int who=0,power=1;
	User* found=NULL;
	system("cls");
	printf("请输入您要修改的用户的序号（不超过10个字符）:");
	scanf("%d",&who);
	clear();
	if((found = find_user(head,who,len))==NULL){
		printf("该用户不存在！");
		return;
	}
	printf("您现在正在操作用户 %s 的密码\n",found->user_name);
	change_pwd(found);
	if(who==1){
		printf("该用户权限被系统锁定，禁止修改！\n");
		return;
	}
	if(found==NULL){
		printf("该用户不存在！\n");
		return;
	}
	printf("该用户%s目前的权限为%d\n1->员工,2->经理,3->管理员\n您希望修改该用户的权限为：",found->user_name,found->role);
	scanf("%d",&power);
	if(power>0&&power<4){
		found->role=power;
		printf("授权成功\n");
	}else{
		printf("授予未知权限无效\n");
	}
}
//管理员用户的功能菜单
User* admin_menu(User* head,int len,User* c_user){
	int cmd=0;
	while(1){
		printf("系统目前共有%d名用户\n",len);
		printf("\n[1]查看用户列表\n[2]增加用户\n[3]删除用户\n[4]修改用户\n[5]修改账号密码\n[0]回到登录界面\n\n%s>>>",c_user->user_name);
		scanf("%d",&cmd);
		clear();
		switch (cmd) {
		case 0:
			return head;
		case 1:
			output_user(head);
			break;
		case 2:
			head = add_user(head,&len);
			break;
		case 3:
			head = delete_user(head,&len);
			break;
		case 4:
			change_user(head,len);
			break;
		case 5:
			change_pwd(c_user);
			return head;
		default:
			printf("未知指令，请重新输入！\n");
		}
	}
	return head;
}
/***********************用户管理************************/

/***********************职工管理************************/
//职工查看自己的信息
void check_self(Staff* head,User* self){
	Staff *p=head;
	system("cls");
	while(p){
		if(strcmp(p->id,self->user_name)==0){
			printf("----------------------------\n");
			printf("工号：%s\n",p->id);
			printf("姓名：%s\n",p->name);
			printf("性别：%s",show_sex(p->sex));
			printf("\n出生时间：%d-%d-%d\n",
				   p->birth_date.year,
				   p->birth_date.month,
				   p->birth_date.day);
			printf("学历：%s",show_edu(p->edu_level));
			printf("\n入职时间：%d-%d-%d\n",
				   p->hire_date.year,
				   p->hire_date.month,
				   p->hire_date.day);
			printf("退休时间：%d年\n",p->retirement_year);
			printf("所属部门：%s\n",p->department);
			printf("联系电话：%s\n",p->telephone);
			printf("个人邮箱：%s\n",p->personal_email);
			printf("工作邮箱：%s\n",p->company_email);
			printf("家庭住址：%s\n",p->address);
			printf("----------------------------\n\n");
			return;
		}
		p=p->next;
	}
	printf("您不在员工列表中！\n\n");
}
//职工修改本人部分的信息（家庭住址，联系电话，个人邮箱）
Staff* change_self(Staff* head,User* self){
	Staff* p=head;
	system("cls");
	while(p){
		if(strcmp(p->id,self->user_name) == 0){ // 找到当前登录员工的档案
			printf("请输入新的家庭住址: "); 
			fgets(p->address, 101, stdin); 
			p->address[strcspn(p->address, "\n")] = '\0'; // 去除换行符
			printf("请输入新的联系电话: "); 
			fgets(p->telephone, 16, stdin); 
			p->telephone[strcspn(p->telephone, "\n")] = '\0'; // 去除换行符
			printf("请输入新的个人邮箱: "); 
			fgets(p->personal_email, 255, stdin); 
			p->personal_email[strcspn(p->personal_email, "\n")] = '\0'; // 去除换行符
			printf("\n个人信息修改成功！\n\n");
			return head;
		}
		p = p->next;
	}
	printf("未找到您的职工信息，无法修改！\n\n");
	return head;
}
//职工查看自己所有工资账单
void check_salary(Staff* head,User* self){
	Staff *p=head;
	system("cls");
	while(p){
		if(strcmp(p->id,self->user_name)==0){
			printf("%-7s %-10s %-10s %-10s %-10s","年-月","基本工资","业绩分成","税收","实发工资");
			printf("\n-----------------------------------------------------\n");
			Salary *s=p->gain;
			if(s==NULL){
				printf("暂无工资记录\n");
				return;
			}
			while(s){
				printf("%d-%d：%-10.2f %-10.2f %-10.2f %-10.2f\n",
					   s->year,
					   s->month,
					   s->base,
					   s->base,
					   s->tax,
					   s->total);
				s=s->next;
			}
			printf("-----------------------------------------------------\n\n");
			return;
		}
		p=p->next;
	}
	printf("您不在员工列表中！\n\n");
}
//查看所有职工简略列表，根据输入显示详细信息
void output_staff(Staff* head) {
	Staff* p = head;
	char search_id[11]; // 用于接收用户输入的查找工号
	int found;
	system("cls");
	printf("%-12s %-15s %-6s %-15s %-16s\n", "工号", "姓名", "性别", "部门", "联系电话");
	printf("---------------------------------------------------------------------------\n");
	while (p != NULL) {
		printf("%-12s %-15s %-6s %-15s %-16s\n",
			   p->id,
			   p->name,
			   show_sex(p->sex),
			   p->department,
			   p->telephone);
		p = p->next;
	}
	printf("---------------------------------------------------------------------------\n");
	while (1) {
		printf("\n请输入您希望查看详细信息的员工工号 (输入 0 退出)：");
		if (scanf("%10s", search_id) != 1) break; 
		if (strcmp(search_id, "0") == 0) {
			return;
		}
		found = 0;
		for (p = head; p != NULL; p = p->next) {
			if (strcmp(p->id, search_id) == 0) {
				found = 1;
				printf("\n========== 员工详细信息 ==========\n");
				printf("工号：%s\n", p->id);
				printf("姓名：%s\n", p->name);
				printf("性别：%s\n",show_sex(p->sex));
				printf("\n出生日期：%d年%d月%d日 (约 %d 岁)\n",
					   p->birth_date.year, 
					   p->birth_date.month, 
					   p->birth_date.day, 
					   CURRENT_YEAR-p->birth_date.year);
				printf("入职日期：%d年%d月%d日\n", 
					   p->hire_date.year, 
					   p->hire_date.month, 
					   p->hire_date.day);
				printf("预计退休年份：%d年\n", p->retirement_year);
				printf("所属部门：%s\n", p->department);
				printf("学历等级：%s\n",show_edu(p->edu_level));
				printf("\n家庭住址：%s\n", p->address);
				printf("联系电话：%s\n", p->telephone);
				printf("公司邮箱：%s\n", p->company_email);
				printf("个人邮箱：%s\n", p->personal_email);
				printf("\n---------- 薪资信息 ----------\n");
				if (p->gain!= NULL) {
					Salary *s=p->gain;
					while(s){
						printf("薪资月份：%d年%d月\n", s->year, s->month);
						printf("基本工资：%.2f\n", s->base);
						printf("奖金：%.2f\n", s->bonus);
						printf("扣税：%.2f\n", s->tax);
						printf("实发总计：%.2f\n\n", s->total);
						s=s->next;
					}
				} else {
					printf("暂无该员工的薪资记录。\n");
				}
				printf("================================\n");
				break;
			}
		}
		if (!found) {
			printf("未找到工号为 [%s] 的员工，请检查输入！\n", search_id);
		}
	}
}
//插入一个新职工并录入信息
Staff* add_staff(Staff* head, int *len) {
	Staff* new_node = NULL;
	Staff* p = head;
	char temp_id[11];
	int pos, f;
	system("cls");
	while (1) {
		printf("请输入插入位置(1-%d, 0退出): ", *len + 1);
		if (scanf("%d", &pos) != 1) {
			// 如果输入非数字，防止死循环
			clear();
			continue;
		}
		if (pos == 0) {
			break;
		}
		// 检查位置合法性 (允许插在尾部，即 len+1)
		if (pos < 1 || pos > *len + 1) {
			printf("位置非法！请输入 1 到 %d 之间的数字。\n", *len + 1);
			continue;
		}
		// --- 开始录入数据 ---
		f = 1; // 标记是否允许插入
		// 1. 输入 ID 并查重
		printf("请输入职工工号: ");
		scanf("%s", temp_id); // ID通常无空格，用%s较安全，或者用fgets
		clear(); // 清除换行符
		for (p = head; p != NULL; p = p->next) {
			if (strcmp(temp_id, p->id) == 0) {
				printf("该职工工号 [%s] 已存在！\n", temp_id);
				f = 0;
				break;
			}
		}
		if (!f) continue; // 如果ID重复，重新循环
		// 2. 分配内存
		new_node = (Staff*)malloc(sizeof(Staff));
		if (new_node == NULL) {
			printf("内存分配失败！\n");
			return head;
		}
		// 3. 填充基本数据
		strcpy(new_node->id, temp_id);
		printf("请输入职工姓名: ");
		fgets(new_node->name, 51, stdin);
		new_node->name[strcspn(new_node->name,"\n")] = '\0';
		printf("请输入职工性别(1为男, 0为女): ");
		scanf("%d", &new_node->sex);
		clear();
		// 结构体中没有 age，改为输入出生日期
		printf("请输入出生日期 (年 月 日): ");
		scanf("%d %d %d", &new_node->birth_date.year, &new_node->birth_date.month, &new_node->birth_date.day);
		clear();
		printf("请输入入职日期 (年 月 日): ");
		scanf("%d %d %d", &new_node->hire_date.year, &new_node->hire_date.month, &new_node->hire_date.day);
		clear();
		printf("请输入所属部门：");
		fgets(new_node->department, 21, stdin);
		new_node->department[strcspn(new_node->department,"\n")]='\0';
		printf("请输入预计退休年份：");
		scanf("%d",&new_node->retirement_year);
		printf("请输入学历等级(数字): ");
		scanf("%d", &new_node->edu_level);
		clear();
		new_node->gain = NULL;
		printf("请输入家庭住址: ");
		fgets(new_node->address, 101, stdin);
		new_node->address[strcspn(new_node->address,"\n")]='\0';
		printf("请输入联系电话: ");
		fgets(new_node->telephone, 16, stdin);
		new_node->telephone[strcspn(new_node->telephone,"\n")]='\0';
		printf("请输入个人邮箱: ");
		fgets(new_node->personal_email, 255, stdin);
		new_node->personal_email[strcspn(new_node->personal_email,"\n")]='\0';
		printf("请输入公司邮箱: ");
		fgets(new_node->company_email, 255, stdin);
		new_node->company_email[strcspn(new_node->company_email,"\n")]='\0';
		// 5. 执行链表插入逻辑
		new_node->next = NULL;
		if (pos == 1) {
			// 头插法
			new_node->next = head;
			head = new_node;
		} else {
			// 寻找第 pos-1 个节点
			p = head;
			for (int i = 1; i < pos - 1; i++) {
				p = p->next;
			}
			// 插入到 p 之后
			new_node->next = p->next;
			p->next = new_node;
		}
		printf("成功插入职工！\n");
		(*len)++;
	}
	return head;
}
//按工号删除职工
Staff* delete_staff(Staff* head, int *len) {
	char del_id[11];
	Staff *p = head,*pre = NULL;
	Salary *s=NULL; // 用于记录当前节点的前一个节点
	int found = 0;
	system("cls");
	printf("请输入您要删除的员工工号：");
	if (scanf("%10s", del_id) != 1) return head;
	// 遍历链表查找对应工号的员工
	while (p != NULL) {
		if (strcmp(p->id, del_id) == 0) {
			found = 1;
			break;
		}
		pre = p;      // 记录前驱节点
		p = p->next;  // 继续向后查找
	}
	if (!found) {
		printf("未找到工号为 [%s] 的员工，删除失败！\n", del_id);
	} else {
		// 找到目标节点，开始执行删除逻辑
		if (pre == NULL) {
			// 情况1：要删除的是链表的头节点
			head = p->next;
		} else {
			// 情况2：要删除的是中间或尾部的节点
			pre->next = p->next;
		}
		
		// 释放该员工薪资结构体的内存（防止内存泄漏）
		if (p->gain != NULL) {
			while (s != NULL) {
				Salary *temp = s;
				s = s->next;
				free(temp); // 逐个释放薪资节点
			}
		}
		// 释放该员工节点本身的内存
		free(p);
		
		(*len)--; // 总人数减一
		printf("工号为 [%s] 的员工已成功删除！\n", del_id);
	}
	return head;
}
//查询职工信息的小菜单，可按工号，部门，学历查询
void search_staff(Staff* head) {
	int choice;
	char target_str[51]; // 用于存储工号或部门的字符串输入
	int target_int;      // 用于存储学历等级的输入
	int found;
	system("cls");
	if (head == NULL) {
		printf("\n当前没有任何员工记录，请先录入信息！\n");
		return;
	}
	while (1) {
		// 打印查询子菜单
		printf("========== 职工信息查询 ==========\n");
		printf("[1]按工号查询\n");
		printf("[2]按部门查询\n");
		printf("[3]按学历查询\n");
		printf("[0]返回主菜单\n");
		printf("\n请输入您的选择 (0-3): ");
		if (scanf("%d", &choice) != 1) {
			while (getchar() != '\n'); // 清除非法输入
			printf("输入无效，请输入0-3之间的数字！\n");
			continue;
		}
		if (choice == 0) break; // 退出子菜单
		if (choice < 1 || choice > 3) {
			printf("输入错误，请重新选择！\n");
			continue;
		}
		// 根据选择提示用户输入查询条件
		if (choice == 1) {
			printf("请输入要查询的工号: ");
			scanf("%10s", target_str);
		} else if (choice == 2) {
			printf("请输入要查询的部门名称: ");
			scanf("%20s", target_str);
		} else if (choice == 3) {
			printf("请输入要查询的学历等级 (1-高中, 2-大专, 3-本科, 4-硕士, 5-博士): ");
			scanf("%d", &target_int);
		}
		// 打印表头
		printf("\n%-10s %-10s %-15s %-8s %-12s\n", "工号", "姓名", "部门", "学历", "联系电话");
		printf("------------------------------------------------------------------\n");
		found = 0;
		Staff* p = head;
		
		// 遍历链表进行查询
		while (p != NULL) {
			int is_match = 0;
			if (choice == 1 && strcmp(p->id, target_str) == 0) {
				is_match = 1;
			} else if (choice == 2 && strcmp(p->department, target_str) == 0) {
				is_match = 1;
			} else if (choice == 3 && p->edu_level == target_int) {
				is_match = 1;
			}
			if (is_match) {
				// 打印匹配到的员工信息
				printf("%-10s %-10s %-15s %-8s %-11s\n", 
					   p->id, 
					   p->name, 
					   p->department, 
					   show_edu(p->edu_level), // 打印时调用辅助函数将数字转为文字
					   p->telephone);
				found = 1;
				// 如果是按工号查询，找到唯一结果后可以直接跳出循环
				if (choice == 1) break; 
			}
			p = p->next;
		}
		if (!found) {
			printf("未找到符合条件的员工记录！\n");
		} else {
			printf("------------------------------------------------------------------\n");
		}
	}
}
//按工号修改对应职工的信息
void modify_staff(Staff* head) {
	Staff *p = head;
	char search_id[11];
	system("cls");
	if (head == NULL) {
		printf("\n当前没有任何员工记录，请先录入信息！\n");
		return;
	}
	printf("请输入要修改的员工工号：");
	scanf("%10s", search_id);
	while (p) {
		if(strcmp(p->id,search_id)==0){
			while(1){
				int choice=0;
				printf("\n---职工[%s]的信息如下---\n",p->id);
				printf("1.工号：%s\n",p->id);
				printf("2.姓名：%s\n",p->name);
				printf("3.性别：%s\n",show_sex(p->sex));
				printf("4.出生日期：%d年%d月%d日 (约 %d 岁)\n",
					   p->birth_date.year, 
					   p->birth_date.month, 
					   p->birth_date.day, 
					   CURRENT_YEAR-p->birth_date.year);
				printf("5.入职日期：%d年%d月%d日\n",p->hire_date.year,p->hire_date.month,p->hire_date.day);
				printf("6.所属部门：%s\n",p->department);
				printf("7.预计退休年份：%d\n",p->retirement_year);
				printf("8.学历等级：%s\n",show_edu(p->edu_level));
				printf("9.联系电话：%s\n",p->telephone);
				printf("10.家庭住址：%s\n",p->address);
				printf("11.公司邮箱：%s\n",p->company_email);
				printf("12.个人邮箱：%s\n",p->personal_email);
				printf("-------------------\n");
				printf("\n请输入要修改的选项 (0退出,1-12修改): ");
				if (scanf("%d", &choice) != 1) {
					while (getchar() != '\n'); // 清除非法输入
					printf("输入无效，请输入1-12之间的数字！\n");
					continue;
				}
				if (choice == 0){
					printf("退出修改菜单\n");
					break; // 退出子菜单
				}
				if (choice < 1 || choice > 12) {
					printf("输入错误，请重新选择！\n");
					continue;
				}
				if(choice==1){
					printf("请输入新的工号: ");
					scanf("%10s", p->id);
					clear();
				}else if(choice==2){
					printf("请输入新的姓名: ");
					fgets(p->name, 51, stdin);
					p->name[strcspn(p->name,"\n")] = '\0';
				}else if(choice==3){
					printf("请输入新的性别(1为男, 0为女): ");
					scanf("%d", &p->sex);
				}else if(choice==4){
					printf("请输入新的出生日期 (年 月 日): ");
					scanf("%d %d %d", &p->birth_date.year, &p->birth_date.month, &p->birth_date.day);
					clear();
				}else if(choice==5){
					printf("请输入新的入职日期 (年 月 日): ");
					scanf("%d %d %d", &p->hire_date.year, &p->hire_date.month, &p->hire_date.day);
					clear();
				}else if(choice==6){
					printf("请输入新的所属部门：");
					fgets(p->department, 21, stdin);
					p->department[strcspn(p->department,"\n")]='\0';
				}else if(choice==7){
					printf("请输入新的预计退休年份：");
					scanf("%d",&p->retirement_year);
				}else if(choice==8){
					printf("请输入新的学历等级(数字): ");
					scanf("%d", &p->edu_level);
					clear();
				}else if(choice==9){
					printf("请输入新的联系电话: ");
					fgets(p->telephone, 16, stdin);
					p->telephone[strcspn(p->telephone,"\n")]='\0';
				}else if(choice==10){
					printf("请输入新的家庭住址: ");
					fgets(p->address, 101, stdin);
					p->address[strcspn(p->address,"\n")]='\0';
				}else if(choice==11){
					printf("请输入新的公司邮箱: ");
					fgets(p->company_email, 255, stdin);
					p->company_email[strcspn(p->company_email,"\n")]='\0';
				}else if(choice==12){
					printf("请输入新的个人邮箱: ");
					fgets(p->personal_email, 255, stdin);
					p->personal_email[strcspn(p->personal_email,"\n")]='\0';
				}
			}
		}
		p=p->next;
	}
	printf("未找到工号为 [%s] 的员工，请检查输入！\n",search_id);
}
//统计职工的相关信息
/*（职工总数，男女数，工龄>3年及<3年的人数，
已经退休的人数，学历高于等于本科的人数，
性别及学历信息错误的人数，平均工资）*/
void statistics_staff(Staff* head,int len) {
	Staff *p = head;
	int male_num = 0, female_num = 0, above_bachelor=0;
	int sex_unknown = 0, edu_unknown = 0;
	int freshman_num = 0, senior_num = 0, retired_num = 0;
	float total_salary = 0;
	system("cls");
	if(p==NULL){
		printf("当前没有任何员工记录，请先录入信息！\n");
		return;
	}
	while(p){
		if(p->sex==1){
			male_num++;
		}else if(p->sex==0){
			female_num++;
		}else{
			sex_unknown++;
		}
		if(p->edu_level>=3){
			above_bachelor++;
		}
		if(p->edu_level>5 || p->edu_level<1){
			edu_unknown++;
		}
		if(CURRENT_YEAR>=p->retirement_year){
			retired_num++;
		}
		if(CURRENT_YEAR-p->hire_date.year<3){
			freshman_num++;
		}else if(CURRENT_YEAR-p->hire_date.year>=3){
			senior_num++;
		}
		total_salary+=p->gain->total;
		p=p->next;
	}
	printf("\n职工统计结果如下：\n");
	printf("--------------------------\n");
	printf("总职工数：%d\n",len);
	printf("男性人数：%d\n",male_num);
	printf("女性人数：%d\n",female_num);
	printf("性别未知人数：%d\n",sex_unknown);
	printf("工龄小于3年的员工数人数：%d\n",freshman_num);
	printf("工龄大于等于3年的员工数人数：%d\n",senior_num);
	printf("退休员工数：%d\n",retired_num);
	printf("学历未知人数：%d\n",edu_unknown);
	printf("学历高于本科的人数：%d\n",above_bachelor);
	printf("本月平均工资：%.2f\n",total_salary/len);
	printf("--------------------------\n");
}
//为职工录入本月薪资
void input_salary(Staff* head){
	SNS *h=NULL,*c=NULL,*t=NULL;
	char id[11];
	Staff *p=head;
	int current_year,current_month,f=1,is_all=1;
	system("cls");
	if(p==NULL){
		printf("当前没有任何员工记录，请先录入信息！\n");
		return;
	}
	printf("是否要普发工资(1表示是，0表示否)：");
	scanf("%d",&is_all);
	if(is_all==0){
		while(1){
			printf("输入您不想发工资的员工工号（0退出）：");
			scanf("%s",id);
			if(strcmp(id, "0")==0){
				break;
			}else{
				c=(SNS*)malloc(sizeof(SNS));
				strcpy(c->id, id);
				c->next=NULL;
				if(h==NULL)
					h=c;
				else
					t->next=c;
				t=c;
			}
		}
	}
	printf("请输入当前年份：");
	scanf("%d",&current_year);
	printf("请输入当前月份：");
	scanf("%d",&current_month);
	while(p){
		f=1;
		Salary *s=p->gain;
		while(s){
			if(s->year==current_year && s->month==current_month){
				printf("员工[%s]的%d年%d月份薪资已存在，请勿重复录入！\n",p->id,current_year,current_month);
				f=0;
				break;
			}
			s=s->next;
		}
		if(is_all==0){
			for(c=h;c;c=c->next){
				if(strcmp(p->id, c->id)==0){
					f=0;
					break;
				}
			}
		}
		if(f){
			Salary *cur=(Salary*)malloc(sizeof(Salary));
			cur->year=current_year;
			cur->month=current_month;
			printf("请输入员工[%s]的%d年%d月份的薪资：\n",p->id,current_year,current_month);
			printf("请输入基本工资：");
			scanf("%f",&cur->base);
			printf("请输入奖金：");
			scanf("%f",&cur->bonus);
			printf("请输入税金：");
			scanf("%f",&cur->tax);
			cur->total=cur->base+cur->bonus-cur->tax;
			cur->next=p->gain;
			p->gain=cur;
			printf("录入成功！\n");
		}
		p=p->next;
	}
	c=h;
	while(c){
		t=c;
		c=c->next;
		free(t);
	}
	printf("本月薪资录入完成！\n");
}
//普通用户的菜单（经理和职工）
Staff* normal_menu(Staff* head,int len,User* c_user){
	int cmd=0;
	if(c_user->role == USER_MANAGER){
		while(1){//经理菜单
			printf("\n[1]查看职工列表\n[2]录入职工\n[3]删除职工\n[4]查询职工\n[5]修改职工\n[6]录入薪资\n[7]统计职工\n[8]修改账号密码\n[0]回到登录界面\n\n请输入(0-8)：");
			scanf("%d",&cmd);
			clear();
			switch (cmd) {
			case 0:
				return head;
			case 1:
				output_staff(head);
				break;
			case 2:
				head = add_staff(head,&len);
				break;
			case 3:
				head = delete_staff(head,&len);
				break;
			case 4:
				search_staff(head);
				break;
			case 5:
				modify_staff(head);
				break;
			case 6:
				input_salary(head);
				break;
			case 7:
				statistics_staff(head,len);
				break;
			case 8:
				change_pwd(c_user);
				return head;
			default:
				printf("未知指令，请重新输入！\n");
			}
		}
	}else if(c_user->role==USER_EMPLOYEE){
		while(1){//员工菜单
			printf("[1]查看我的信息\n[2]修改我的信息\n[3]查看工资账单\n[4]修改账号密码\n[0]回到登录界面\n\n请输入(0-4)：");
			scanf("%d",&cmd);
			clear();
			switch (cmd) {
			case 0:
				return head;
			case 1:
				check_self(head,c_user);
				break;
			case 2:
				head = change_self(head,c_user);
				break;
			case 3:
				check_salary(head,c_user);
				break;
			case 4:
				change_pwd(c_user);
				return head;
			default:
				printf("未知指令，请重新输入！\n");
			}
		}
	}
	return head;
}
/***********************职工管理************************/

/***********************保存************************/
//保存用户链表到文件
void save_user(FILE *f,User* head){
	while(head){
		fprintf(f,"%s %s %d\n",
				head->user_name,
				head->password,
				head->role);
		head=head->next;
	}
	printf("\n用户数据保存完毕\n");
}
//保存职工链表到文件
void save_staff(FILE *f,Staff* head){
	Staff *p=head;
	while(p){
		fprintf(f,"start %s %s %d %d %d %d %d %d %d %d %s %d %s %s %s %s\n",
				p->id,
				p->name,
				p->sex,
				p->birth_date.year,
				p->birth_date.month,
				p->birth_date.day,
				p->hire_date.year,
				p->hire_date.month,
				p->hire_date.day,
				p->retirement_year,
				p->department,
				p->edu_level,
				p->address,
				p->telephone,
				p->company_email,
				p->personal_email);
		Salary *s=p->gain;
		while(s){
			fprintf(f,"salary %d %d %.2f %.2f %.2f %.2f\n",
					s->year,
					s->month,
					s->base,
					s->bonus,
					s->tax,
					s->total);
			s=s->next;
		}
		fprintf(f,"end\n");
		p=p->next;
	}
	printf("职工数据保存完毕\n");
}
/***********************保存************************/
