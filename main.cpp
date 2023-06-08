#include <cstring>
#include <iostream>


#define MAX_USERS 100 // 最大用户数
#define MAX_NAME_LEN 20 // 学生名最大长度
#define MAX_ID_LEN 20 // 用户名最大长度
#define MAX_PASS_LEN 20 // 密码最大长度
#define MAX_STUDENTS 500//最大学生数

// 学生结构体
typedef struct {
    char id[20];
    char name[MAX_NAME_LEN];
    char major[20];
    int c_language;
    int computational_physics;
    int university_physics;
    int atomic_physics;
    int optics;
} Student;

// 用户结构体
typedef struct {
    char name[MAX_ID_LEN];
    char password[MAX_PASS_LEN];
    int role; // 用户角色：0-管理员，1-教师，2-学生
} User;

void select_student_grades(Student *students, int count);

void select_student_grades_for_teacher(Student *students, int count);

// 验证用户名和密码是否正确
int login(User *users, int count, const char *name, const char *password) {
    for (int i = 0; i < count; i++) {
        User *user = &users[i];
        if (strcmp(user->name, name) == 0 && strcmp(user->password, password) == 0) {
            return user->role;
        }
    }
    return -1;
}

// 保存用户信息到文件
int save_users(User *users, int count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("保存文件失败");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        User *user = &users[i];
        fprintf(fp, "%s,%s,%d\n", user->name, user->password, user->role);
    }

    fclose(fp);
    return 0;
}
// 保存学生信息到文件
int save_students(Student *students, int count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("保存文件失败");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (i == 0){
            fprintf(fp,"    学号             姓名             专业        C语言  计算物理学  大学物理  原子物理  光学\n");
        }else{
                fprintf(fp, "%s         %s \t \t \t%s          %d      %d      %d      %d      %d\n",
                    students[i].id,
                    students[i].name,
                    students[i].major,
                    students[i].c_language,
                    students[i].computational_physics,
                    students[i].university_physics,
                    students[i].atomic_physics,
                    students[i].optics);
        }

    }

    fclose(fp);
    return 0;
}
// 从文件中读取用户信息
int read_users(User *users, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("读取文件失败");
        return -1;
    }

    int count = 0;
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        // 解析行，获取用户名、密码和角色
        char *name = strtok(line, ",");
        char *password = strtok(NULL, ",");
        char *role_str = strtok(NULL, ",");
        if (!name || !password || !role_str) {
            printf("文件格式错误\n");
            fclose(fp);
            return -1;
        }

        // 添加到用户列表
        User *user = &users[count++];
        strcpy(user->name, name);
        strcpy(user->password, password);
        user->role = atoi(role_str);
    }

    fclose(fp);
    return count;
}
// 读取学生信息表格
int read_students(Student *students, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 0;
    }
    int count = 0;
    char line[500];
    while (count < MAX_STUDENTS && fgets(line, sizeof(line), fp) != NULL) {
        // 解析每行数据，将学生信息保存到结构体中
        sscanf(line, "%s\t\t%s\t\t\t%s\t\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               students[count].id,
               students[count].name,
               students[count].major,
               &students[count].c_language,
               &students[count].computational_physics,
               &students[count].university_physics,
               &students[count].atomic_physics,
               &students[count].optics);
        count++;
    }
    fclose(fp);
    return count;
}
//管理员教师信息界面
void show_admin_teacher_menu(User *users, int count){
    printf("欢迎进入教师管理界面\n");
    while (1) {
        printf("请选择操作：\n");
        printf("1. 添加教师\n");
        printf("2. 删除教师\n");
        printf("3. 修改教师信息\n");
        printf("4. 返回管理员界面\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // 添加教师
                if (count >= MAX_USERS) {
                    printf("用户数量已达上限\n");
                    break;
                }
                printf("请输入教师用户名：");
                char name[MAX_ID_LEN];
                scanf("%s", name);

                printf("请输入教师密码：");
                char password[MAX_PASS_LEN];
                scanf("%s", password);

                users[count].role = 1;

                strcpy(users[count].name, name);
                strcpy(users[count].password, password);
                count++;

                printf("添加教师成功\n");
                save_users(users, count, "..\\id.txt");
                break;
            case 2:
                // 删除教师
            {
                printf("请输入要删除的教师用户名：");
                char name2[MAX_ID_LEN];
                scanf("%s", name2);
                int found = 0;
                for (int i = 0; i < count; i++) {
                    User *user = &users[i];
                    if (strcmp(user->name, name2) == 0 && user->role == 1) {
                        // 找到了要删除的教师用户
                        found = 1;
                        // 将后面的用户依次向前移动一个位置
                        for (int j = i; j < count - 1; j++) {
                            users[j] = users[j + 1];
                        }
                        count--;
                        printf("删除教师成功\n");
                        save_users(users, count, "..\\id.txt");
                        break;
                    }
                }
                if (!found) {
                    printf("没有找到要删除的教师用户\n");
                }
                break;
            }
            case 3:
                // 修改教师信息
            {
                printf("请输入要修改的教师用户名：");
                char name3[MAX_ID_LEN];
                scanf("%s", name3);
                int found2 = 0;
                for (int i = 0; i < count; i++) {
                    User *user = &users[i];
                    if (strcmp(user->name, name3) == 0 && user->role == 1) {
                        // 找到了要修改的教师用户
                        found2 = 1;
                        printf("请输入新的教师密码：");
                        char password2[MAX_PASS_LEN];
                        scanf("%s", password2);
                        strcpy(user->password, password2);
                        printf("修改教师信息成功\n");
                        save_users(users, count, "..\\id.txt");
                        break;
                    }
                }
                if (!found2) {
                    printf("没有找到要修改的教师用户\n");
                }
            }
                break;
            case 4:
                printf("返回管理员界面\n");
                return;
            default:
                printf("无效的选择\n");
                break;
        }
    }
}

//管理员学生信息界面
void show_admin_student_menu(User *users, int count) {
    printf("欢迎进入学生管理界面\n");
    while (1) {
        printf("请选择操作：\n");
        printf("1. 添加学生\n");
        printf("2. 删除学生\n");
        printf("3. 修改学生信息\n");
        printf("4. 返回管理员界面\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // 添加学生
                if (count >= MAX_USERS) {
                    printf("用户数量已达上限\n");
                    break;
                }
                printf("请输入学生用户名：");
                char name[MAX_ID_LEN];
                scanf("%s", name);

                printf("请输入学生密码：");
                char password[MAX_PASS_LEN];
                scanf("%s", password);

                users[count].role = 2;

                strcpy(users[count].name, name);
                strcpy(users[count].password, password);
                count++;

                printf("添加学生成功\n");
                save_users(users, count, "..\\id.txt");
                break;
            case 2:
                // 删除学生
            {
                printf("请输入要删除的学生用户名：");
                char name2[MAX_ID_LEN];
                scanf("%s", name2);
                int found = 0;
                for (int i = 0; i < count; i++) {
                    User *user = &users[i];
                    if (strcmp(user->name, name2) == 0 && user->role == 2) {
                        // 找到了要删除的学生用户
                        found = 1;
                        // 将后面的用户依次向前移动一个位置
                        for (int j = i; j < count - 1; j++) {
                            users[j] = users[j + 1];
                        }
                        count--;
                        printf("删除学生成功\n");
                        save_users(users, count, "..\\id.txt");
                        break;
                    }
                }
                if (!found) {
                    printf("没有找到要删除的学生用户\n");
                }
                break;
            }
            case 3:
                // 修改学生信息
            {
                printf("请输入要修改的学生用户名：");
                char name3[MAX_ID_LEN];
                scanf("%s", name3);
                int found2 = 0;
                for (int i = 0; i < count; i++) {
                    User *user = &users[i];
                    if (strcmp(user->name, name3) == 0 && user->role == 2) {
                        // 找到了要修改的学生用户
                        found2 = 1;
                        printf("请输入新的学生密码：");
                        char password2[MAX_PASS_LEN];
                        scanf("%s", password2);
                        strcpy(user->password, password2);
                        printf("修改学生信息成功\n");
                        save_users(users, count, "..\\id.txt");
                        break;
                    }
                }
                if (!found2) {
                    printf("没有找到要修改的学生用户\n");
                }
            }
                break;
            case 4:
                printf("返回管理员界面\n");
                return;
            default:
                printf("无效的选择\n");
                break;
        }
    }

}

// 显示管理员界面
void show_admin_menu(User *users, int count) {
    printf("管理员界面\n");
    // TODO: 实现管理员操作
    printf("欢迎进入管理员界面\n");
    while (true) {
        printf("请选择操作：\n");
        printf("1. 教师管理\n");
        printf("2. 学生管理\n");
        printf("3. 退出\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // TODO: 实现教师管理
                printf("进入教师管理界面\n");
                show_admin_teacher_menu(users,count);
                break;
            case 2:
                // TODO: 实现学生管理
                printf("进入学生管理界面\n");
                show_admin_student_menu(users,count);
                break;
            case 3:
                printf("退出管理员界面\n");
                return;
            default:
                printf("无效的选择\n");
                break;
        }
    }
}

// 显示教师界面
void show_teacher_menu(Student *students, int count) {
    printf("教师界面\n");
    // TODO: 实现教师操作
    printf("欢迎进入教师界面\n");
    while (true) {
        printf("请选择操作：\n");
        printf("1. 增加学生信息\n");
        printf("2. 删除学生信息\n");
        printf("3. 修改学生信息\n");
        printf("4. 查询学生信息\n");
        printf("5. 返回\n");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                // 增加学生成绩
//                刚开始改
                if (count >= MAX_STUDENTS) {
                    printf("学生信息数量已达上限\n");
                    break;
                }
                printf("请输入学生学号：");
                char id[20];
                scanf("%s", id);
                printf("请输入学生姓名：");
                char name[MAX_NAME_LEN];
                scanf("%s", name);
                printf("请输入学生专业：");
                char major[20];
                scanf("%s", major);
                int c_lauguage;
                printf("请输入学生C语言成绩：");
                scanf("%d", &c_lauguage);
                int computational_physics;
                printf("请输入学生计算物理学成绩：");
                scanf("%d", &computational_physics);
                int university_physics;
                printf("请输入学生大学物理成绩：");
                scanf("%d",&university_physics);
                int atimic_physics;
                printf("请输入学生原子物理成绩：");
                scanf("%d", &atimic_physics);
                int optics;
                printf("请输入学生光学成绩：");
                scanf("%d", &optics);


                strcpy(students[count].id, id);
                strcpy(students[count].name, name);
                strcpy(students[count].major, major);
                students[count].c_language = c_lauguage;
                students[count].computational_physics = computational_physics;
                students[count].university_physics = university_physics;
                students[count].atomic_physics = atimic_physics;
                students[count].optics = optics;

                count++;
                save_students(students, count, "..\\学生成绩单.txt");
                printf("增加学生成绩成功\n");
                break;
            case 2: {// 删除学生成绩
                printf("请输入要删除的学生学号：");
                char student_id[20];
                scanf("%s", student_id);
                int found = 0;
                for (int i = 0; i < count; i++) {
                    if (strcmp(students[i].id, student_id) == 0) {
                        found = 1;
                        // 将后面的学生成绩依次向前移动一个位置
                        for (int j = i; j < count - 1; j++) {
                            students[j] = students[j + 1];
                        }
                        count--;
                        printf("删除学生成绩成功\n");
                        save_students(students, count, "..\\学生成绩单.txt");
                        break;
                    }
                }
                if (!found) {
                    printf("没有找到要删除的学生成绩\n");
                }
            }
                break;
            case 3:
                // 修改学生成绩
            {
                printf("请输入要修改的学生学号：");
                char student_id[20];
                scanf("%s", student_id);
                int found2 = 0;
                for (int i = 1; i < count; i++) {
                    if (strcmp(students[i].id, student_id) == 0) {
                        found2 = 1;
                        printf("请输入修改后的学生信息\n");
                        printf("请输入学生学号：");
                        char id[20];
                        scanf("%s", id);
                        printf("请输入学生姓名：");
                        char name[MAX_NAME_LEN];
                        scanf("%s", name);
                        printf("请输入学生专业：");
                        char major[20];
                        scanf("%s", major);
                        int c_lauguage;
                        printf("请输入学生C语言成绩：");
                        scanf("%d", &c_lauguage);
                        int computational_physics;
                        printf("请输入学生计算物理学成绩：");
                        scanf("%d", &computational_physics);
                        int university_physics;
                        printf("请输入学生大学物理成绩：");
                        scanf("%d",&university_physics);
                        int atimic_physics;
                        printf("请输入学生原子物理成绩：");
                        scanf("%d", &atimic_physics);
                        int optics;
                        printf("请输入学生光学成绩：");
                        scanf("%d", &optics);
                        strcpy(students[i].id, id);
                        strcpy(students[i].name, name);
                        strcpy(students[i].major, major);
                        students[i].c_language = c_lauguage;
                        students[i].computational_physics = computational_physics;
                        students[i].university_physics = university_physics;
                        students[i].atomic_physics = atimic_physics;
                        students[i].optics = optics;
                        printf("修改学生成绩成功\n");
                        save_students(students, count, "..\\学生成绩单.txt");
                        break;
                    }
                }
                if (found2 == 0) {
                    printf("没有找到要修改的学生成绩\n");
                }
            }
                break;
            case 4:
                // 查询学生成绩
                select_student_grades_for_teacher(students, count);
                break;
            case 5:
                printf("返回\n");
                return;
            default:
                printf("无效的选择\n");
                break;
        }
    }
}

void select_student_grades_for_teacher(Student *students, int count) {
    printf("请输入您想要查询的学生成绩");
    char id[20];
    scanf("%s",id);
    int found3 = 0;
    for (int i = 1; i < count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            found3 = 1;
            printf("学生学号：%s\n",students[i].id);
            printf("学生姓名：%s\n",students[i].name);
            printf("学生专业：%s\n",students[i].major);
            printf("C语言成绩：%d\n",students[i].c_language);
            printf("计算物理学成绩：%d\n",students[i].computational_physics);
            printf("大学物理成绩：%d\n",students[i].university_physics);
            printf("原子物理成绩：%d\n",students[i].atomic_physics);
            printf("光学成绩：%d\n",students[i].optics);
            break;
        }
    }
    if (found3 == 0) {
        printf("没有找到学号为%s的学生成绩\n", id);
    }
}

void select_student_grades(Student *students, int count, char* number) {
    int found3 = 0;
    for (int i = 1; i < count; i++) {
        if (strcmp(students[i].id, number) == 0) {
            found3 = 1;
            printf("学生学号：%s\n",students[i].id);
            printf("学生姓名：%s\n",students[i].name);
            printf("学生专业：%s\n",students[i].major);
            printf("C语言成绩：%d\n",students[i].c_language);
            printf("计算物理学成绩：%d\n",students[i].computational_physics);
            printf("大学物理成绩：%d\n",students[i].university_physics);
            printf("原子物理成绩：%d\n",students[i].atomic_physics);
            printf("光学成绩：%d\n",students[i].optics);
            break;
        }
    }
    if (found3 == 0) {
        printf("没有找到学号为%s的学生成绩\n", number);
    }
}
void select_and_student_grades(Student *students, int count, char* number) {
    int found4 = 0;
    for (int i = 1; i < count; i++) {
        if (strcmp(students[i].id, number) == 0) {
            found4 = 1;
            int sumGrades = students[i].c_language + students[i].computational_physics +
                            students[i].university_physics + students[i].atomic_physics + students[i].optics;
            printf("姓名：%s的总成绩为:%d\n",students[i].name,sumGrades);
            break;
        }
    }
    if (found4 == 0) {
        printf("没有找到学号为%s的学生成绩\n",number);
    }
}
// 显示学生界面
void show_student_menu(Student *students, int count, char* number) {
    printf("学生界面\n");
    printf("欢迎进入学生界面\n");
    while(true){
        printf("请选择操作：\n");
        printf("1. 查询单科成绩\n");
        printf("2. 查询总成绩\n");
        printf("3. 退出\n");
        int choice;
        scanf("%d", &choice);
        switch (choice){
            case 1:
                select_student_grades(students, count, number);
                break;
            case 2:
                select_and_student_grades(students, count, number);
                break;
            case 3:
                return;
            default:
                printf("您的输入有误");
        }
    }
}



// 主函数
int main() {
    Student students[MAX_STUDENTS];
    User users[MAX_USERS];
    int count_students = read_students(students, "..\\学生成绩单.txt");
    int count_user = read_users(users, "..\\id.txt");
    if (count_user == -1) {
        printf("读取用户信息失败\n");
        return 1;
    }

    char number[MAX_ID_LEN], password[MAX_PASS_LEN];
    printf("请输入用户名：");
    scanf("%s", number);
    printf("请输入密码：");
    scanf("%s", password);

    int role = login(users, count_user, number, password);
    if (role == -1) {
        printf("用户名或密码错误\n");
        return 1;
    }

    switch (role) {
        case 0:
            show_admin_menu(users, count_user);
            break;
        case 1:
            show_teacher_menu(students, count_students);
            break;
        case 2:
            show_student_menu(students,count_students,number);
            break;
        default:
            printf("未知角色捏\n");
            break;
    }

    return 0;
}