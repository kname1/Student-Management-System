#include <cstring>
#include <iostream>


#define MAX_USERS 100 // ����û���
#define MAX_NAME_LEN 20 // ѧ������󳤶�
#define MAX_ID_LEN 20 // �û�����󳤶�
#define MAX_PASS_LEN 20 // ������󳤶�
#define MAX_STUDENTS 500//���ѧ����

// ѧ���ṹ��
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

// �û��ṹ��
typedef struct {
    char name[MAX_ID_LEN];
    char password[MAX_PASS_LEN];
    int role; // �û���ɫ��0-����Ա��1-��ʦ��2-ѧ��
} User;

void select_student_grades(Student *students, int count);

void select_student_grades_for_teacher(Student *students, int count);

// ��֤�û����������Ƿ���ȷ
int login(User *users, int count, const char *name, const char *password) {
    for (int i = 0; i < count; i++) {
        User *user = &users[i];
        if (strcmp(user->name, name) == 0 && strcmp(user->password, password) == 0) {
            return user->role;
        }
    }
    return -1;
}

// �����û���Ϣ���ļ�
int save_users(User *users, int count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("�����ļ�ʧ��");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        User *user = &users[i];
        fprintf(fp, "%s,%s,%d\n", user->name, user->password, user->role);
    }

    fclose(fp);
    return 0;
}
// ����ѧ����Ϣ���ļ�
int save_students(Student *students, int count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("�����ļ�ʧ��");
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (i == 0){
            fprintf(fp,"    ѧ��             ����             רҵ        C����  ��������ѧ  ��ѧ����  ԭ������  ��ѧ\n");
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
// ���ļ��ж�ȡ�û���Ϣ
int read_users(User *users, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("��ȡ�ļ�ʧ��");
        return -1;
    }

    int count = 0;
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        // �����У���ȡ�û���������ͽ�ɫ
        char *name = strtok(line, ",");
        char *password = strtok(NULL, ",");
        char *role_str = strtok(NULL, ",");
        if (!name || !password || !role_str) {
            printf("�ļ���ʽ����\n");
            fclose(fp);
            return -1;
        }

        // ��ӵ��û��б�
        User *user = &users[count++];
        strcpy(user->name, name);
        strcpy(user->password, password);
        user->role = atoi(role_str);
    }

    fclose(fp);
    return count;
}
// ��ȡѧ����Ϣ���
int read_students(Student *students, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 0;
    }
    int count = 0;
    char line[500];
    while (count < MAX_STUDENTS && fgets(line, sizeof(line), fp) != NULL) {
        // ����ÿ�����ݣ���ѧ����Ϣ���浽�ṹ����
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
//����Ա��ʦ��Ϣ����
void show_admin_teacher_menu(User *users, int count){
    printf("��ӭ�����ʦ�������\n");
    while (1) {
        printf("��ѡ�������\n");
        printf("1. ��ӽ�ʦ\n");
        printf("2. ɾ����ʦ\n");
        printf("3. �޸Ľ�ʦ��Ϣ\n");
        printf("4. ���ع���Ա����\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // ��ӽ�ʦ
                if (count >= MAX_USERS) {
                    printf("�û������Ѵ�����\n");
                    break;
                }
                printf("�������ʦ�û�����");
                char name[MAX_ID_LEN];
                scanf("%s", name);

                printf("�������ʦ���룺");
                char password[MAX_PASS_LEN];
                scanf("%s", password);

                users[count].role = 1;

                strcpy(users[count].name, name);
                strcpy(users[count].password, password);
                count++;

                printf("��ӽ�ʦ�ɹ�\n");
                save_users(users, count, "..\\id.txt");
                break;
            case 2:
                // ɾ����ʦ
            {
                printf("������Ҫɾ���Ľ�ʦ�û�����");
                char name2[MAX_ID_LEN];
                scanf("%s", name2);
                int found = 0;
                for (int i = 0; i < count; i++) {
                    User *user = &users[i];
                    if (strcmp(user->name, name2) == 0 && user->role == 1) {
                        // �ҵ���Ҫɾ���Ľ�ʦ�û�
                        found = 1;
                        // ��������û�������ǰ�ƶ�һ��λ��
                        for (int j = i; j < count - 1; j++) {
                            users[j] = users[j + 1];
                        }
                        count--;
                        printf("ɾ����ʦ�ɹ�\n");
                        save_users(users, count, "..\\id.txt");
                        break;
                    }
                }
                if (!found) {
                    printf("û���ҵ�Ҫɾ���Ľ�ʦ�û�\n");
                }
                break;
            }
            case 3:
                // �޸Ľ�ʦ��Ϣ
            {
                printf("������Ҫ�޸ĵĽ�ʦ�û�����");
                char name3[MAX_ID_LEN];
                scanf("%s", name3);
                int found2 = 0;
                for (int i = 0; i < count; i++) {
                    User *user = &users[i];
                    if (strcmp(user->name, name3) == 0 && user->role == 1) {
                        // �ҵ���Ҫ�޸ĵĽ�ʦ�û�
                        found2 = 1;
                        printf("�������µĽ�ʦ���룺");
                        char password2[MAX_PASS_LEN];
                        scanf("%s", password2);
                        strcpy(user->password, password2);
                        printf("�޸Ľ�ʦ��Ϣ�ɹ�\n");
                        save_users(users, count, "..\\id.txt");
                        break;
                    }
                }
                if (!found2) {
                    printf("û���ҵ�Ҫ�޸ĵĽ�ʦ�û�\n");
                }
            }
                break;
            case 4:
                printf("���ع���Ա����\n");
                return;
            default:
                printf("��Ч��ѡ��\n");
                break;
        }
    }
}

//����Աѧ����Ϣ����
void show_admin_student_menu(User *users, int count) {
    printf("��ӭ����ѧ���������\n");
    while (1) {
        printf("��ѡ�������\n");
        printf("1. ���ѧ��\n");
        printf("2. ɾ��ѧ��\n");
        printf("3. �޸�ѧ����Ϣ\n");
        printf("4. ���ع���Ա����\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // ���ѧ��
                if (count >= MAX_USERS) {
                    printf("�û������Ѵ�����\n");
                    break;
                }
                printf("������ѧ���û�����");
                char name[MAX_ID_LEN];
                scanf("%s", name);

                printf("������ѧ�����룺");
                char password[MAX_PASS_LEN];
                scanf("%s", password);

                users[count].role = 2;

                strcpy(users[count].name, name);
                strcpy(users[count].password, password);
                count++;

                printf("���ѧ���ɹ�\n");
                save_users(users, count, "..\\id.txt");
                break;
            case 2:
                // ɾ��ѧ��
            {
                printf("������Ҫɾ����ѧ���û�����");
                char name2[MAX_ID_LEN];
                scanf("%s", name2);
                int found = 0;
                for (int i = 0; i < count; i++) {
                    User *user = &users[i];
                    if (strcmp(user->name, name2) == 0 && user->role == 2) {
                        // �ҵ���Ҫɾ����ѧ���û�
                        found = 1;
                        // ��������û�������ǰ�ƶ�һ��λ��
                        for (int j = i; j < count - 1; j++) {
                            users[j] = users[j + 1];
                        }
                        count--;
                        printf("ɾ��ѧ���ɹ�\n");
                        save_users(users, count, "..\\id.txt");
                        break;
                    }
                }
                if (!found) {
                    printf("û���ҵ�Ҫɾ����ѧ���û�\n");
                }
                break;
            }
            case 3:
                // �޸�ѧ����Ϣ
            {
                printf("������Ҫ�޸ĵ�ѧ���û�����");
                char name3[MAX_ID_LEN];
                scanf("%s", name3);
                int found2 = 0;
                for (int i = 0; i < count; i++) {
                    User *user = &users[i];
                    if (strcmp(user->name, name3) == 0 && user->role == 2) {
                        // �ҵ���Ҫ�޸ĵ�ѧ���û�
                        found2 = 1;
                        printf("�������µ�ѧ�����룺");
                        char password2[MAX_PASS_LEN];
                        scanf("%s", password2);
                        strcpy(user->password, password2);
                        printf("�޸�ѧ����Ϣ�ɹ�\n");
                        save_users(users, count, "..\\id.txt");
                        break;
                    }
                }
                if (!found2) {
                    printf("û���ҵ�Ҫ�޸ĵ�ѧ���û�\n");
                }
            }
                break;
            case 4:
                printf("���ع���Ա����\n");
                return;
            default:
                printf("��Ч��ѡ��\n");
                break;
        }
    }

}

// ��ʾ����Ա����
void show_admin_menu(User *users, int count) {
    printf("����Ա����\n");
    // TODO: ʵ�ֹ���Ա����
    printf("��ӭ�������Ա����\n");
    while (true) {
        printf("��ѡ�������\n");
        printf("1. ��ʦ����\n");
        printf("2. ѧ������\n");
        printf("3. �˳�\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // TODO: ʵ�ֽ�ʦ����
                printf("�����ʦ�������\n");
                show_admin_teacher_menu(users,count);
                break;
            case 2:
                // TODO: ʵ��ѧ������
                printf("����ѧ���������\n");
                show_admin_student_menu(users,count);
                break;
            case 3:
                printf("�˳�����Ա����\n");
                return;
            default:
                printf("��Ч��ѡ��\n");
                break;
        }
    }
}

// ��ʾ��ʦ����
void show_teacher_menu(Student *students, int count) {
    printf("��ʦ����\n");
    // TODO: ʵ�ֽ�ʦ����
    printf("��ӭ�����ʦ����\n");
    while (true) {
        printf("��ѡ�������\n");
        printf("1. ����ѧ����Ϣ\n");
        printf("2. ɾ��ѧ����Ϣ\n");
        printf("3. �޸�ѧ����Ϣ\n");
        printf("4. ��ѯѧ����Ϣ\n");
        printf("5. ����\n");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                // ����ѧ���ɼ�
//                �տ�ʼ��
                if (count >= MAX_STUDENTS) {
                    printf("ѧ����Ϣ�����Ѵ�����\n");
                    break;
                }
                printf("������ѧ��ѧ�ţ�");
                char id[20];
                scanf("%s", id);
                printf("������ѧ��������");
                char name[MAX_NAME_LEN];
                scanf("%s", name);
                printf("������ѧ��רҵ��");
                char major[20];
                scanf("%s", major);
                int c_lauguage;
                printf("������ѧ��C���Գɼ���");
                scanf("%d", &c_lauguage);
                int computational_physics;
                printf("������ѧ����������ѧ�ɼ���");
                scanf("%d", &computational_physics);
                int university_physics;
                printf("������ѧ����ѧ����ɼ���");
                scanf("%d",&university_physics);
                int atimic_physics;
                printf("������ѧ��ԭ������ɼ���");
                scanf("%d", &atimic_physics);
                int optics;
                printf("������ѧ����ѧ�ɼ���");
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
                save_students(students, count, "..\\ѧ���ɼ���.txt");
                printf("����ѧ���ɼ��ɹ�\n");
                break;
            case 2: {// ɾ��ѧ���ɼ�
                printf("������Ҫɾ����ѧ��ѧ�ţ�");
                char student_id[20];
                scanf("%s", student_id);
                int found = 0;
                for (int i = 0; i < count; i++) {
                    if (strcmp(students[i].id, student_id) == 0) {
                        found = 1;
                        // �������ѧ���ɼ�������ǰ�ƶ�һ��λ��
                        for (int j = i; j < count - 1; j++) {
                            students[j] = students[j + 1];
                        }
                        count--;
                        printf("ɾ��ѧ���ɼ��ɹ�\n");
                        save_students(students, count, "..\\ѧ���ɼ���.txt");
                        break;
                    }
                }
                if (!found) {
                    printf("û���ҵ�Ҫɾ����ѧ���ɼ�\n");
                }
            }
                break;
            case 3:
                // �޸�ѧ���ɼ�
            {
                printf("������Ҫ�޸ĵ�ѧ��ѧ�ţ�");
                char student_id[20];
                scanf("%s", student_id);
                int found2 = 0;
                for (int i = 1; i < count; i++) {
                    if (strcmp(students[i].id, student_id) == 0) {
                        found2 = 1;
                        printf("�������޸ĺ��ѧ����Ϣ\n");
                        printf("������ѧ��ѧ�ţ�");
                        char id[20];
                        scanf("%s", id);
                        printf("������ѧ��������");
                        char name[MAX_NAME_LEN];
                        scanf("%s", name);
                        printf("������ѧ��רҵ��");
                        char major[20];
                        scanf("%s", major);
                        int c_lauguage;
                        printf("������ѧ��C���Գɼ���");
                        scanf("%d", &c_lauguage);
                        int computational_physics;
                        printf("������ѧ����������ѧ�ɼ���");
                        scanf("%d", &computational_physics);
                        int university_physics;
                        printf("������ѧ����ѧ����ɼ���");
                        scanf("%d",&university_physics);
                        int atimic_physics;
                        printf("������ѧ��ԭ������ɼ���");
                        scanf("%d", &atimic_physics);
                        int optics;
                        printf("������ѧ����ѧ�ɼ���");
                        scanf("%d", &optics);
                        strcpy(students[i].id, id);
                        strcpy(students[i].name, name);
                        strcpy(students[i].major, major);
                        students[i].c_language = c_lauguage;
                        students[i].computational_physics = computational_physics;
                        students[i].university_physics = university_physics;
                        students[i].atomic_physics = atimic_physics;
                        students[i].optics = optics;
                        printf("�޸�ѧ���ɼ��ɹ�\n");
                        save_students(students, count, "..\\ѧ���ɼ���.txt");
                        break;
                    }
                }
                if (found2 == 0) {
                    printf("û���ҵ�Ҫ�޸ĵ�ѧ���ɼ�\n");
                }
            }
                break;
            case 4:
                // ��ѯѧ���ɼ�
                select_student_grades_for_teacher(students, count);
                break;
            case 5:
                printf("����\n");
                return;
            default:
                printf("��Ч��ѡ��\n");
                break;
        }
    }
}

void select_student_grades_for_teacher(Student *students, int count) {
    printf("����������Ҫ��ѯ��ѧ���ɼ�");
    char id[20];
    scanf("%s",id);
    int found3 = 0;
    for (int i = 1; i < count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            found3 = 1;
            printf("ѧ��ѧ�ţ�%s\n",students[i].id);
            printf("ѧ��������%s\n",students[i].name);
            printf("ѧ��רҵ��%s\n",students[i].major);
            printf("C���Գɼ���%d\n",students[i].c_language);
            printf("��������ѧ�ɼ���%d\n",students[i].computational_physics);
            printf("��ѧ����ɼ���%d\n",students[i].university_physics);
            printf("ԭ������ɼ���%d\n",students[i].atomic_physics);
            printf("��ѧ�ɼ���%d\n",students[i].optics);
            break;
        }
    }
    if (found3 == 0) {
        printf("û���ҵ�ѧ��Ϊ%s��ѧ���ɼ�\n", id);
    }
}

void select_student_grades(Student *students, int count, char* number) {
    int found3 = 0;
    for (int i = 1; i < count; i++) {
        if (strcmp(students[i].id, number) == 0) {
            found3 = 1;
            printf("ѧ��ѧ�ţ�%s\n",students[i].id);
            printf("ѧ��������%s\n",students[i].name);
            printf("ѧ��רҵ��%s\n",students[i].major);
            printf("C���Գɼ���%d\n",students[i].c_language);
            printf("��������ѧ�ɼ���%d\n",students[i].computational_physics);
            printf("��ѧ����ɼ���%d\n",students[i].university_physics);
            printf("ԭ������ɼ���%d\n",students[i].atomic_physics);
            printf("��ѧ�ɼ���%d\n",students[i].optics);
            break;
        }
    }
    if (found3 == 0) {
        printf("û���ҵ�ѧ��Ϊ%s��ѧ���ɼ�\n", number);
    }
}
void select_and_student_grades(Student *students, int count, char* number) {
    int found4 = 0;
    for (int i = 1; i < count; i++) {
        if (strcmp(students[i].id, number) == 0) {
            found4 = 1;
            int sumGrades = students[i].c_language + students[i].computational_physics +
                            students[i].university_physics + students[i].atomic_physics + students[i].optics;
            printf("������%s���ܳɼ�Ϊ:%d\n",students[i].name,sumGrades);
            break;
        }
    }
    if (found4 == 0) {
        printf("û���ҵ�ѧ��Ϊ%s��ѧ���ɼ�\n",number);
    }
}
// ��ʾѧ������
void show_student_menu(Student *students, int count, char* number) {
    printf("ѧ������\n");
    printf("��ӭ����ѧ������\n");
    while(true){
        printf("��ѡ�������\n");
        printf("1. ��ѯ���Ƴɼ�\n");
        printf("2. ��ѯ�ܳɼ�\n");
        printf("3. �˳�\n");
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
                printf("������������");
        }
    }
}



// ������
int main() {
    Student students[MAX_STUDENTS];
    User users[MAX_USERS];
    int count_students = read_students(students, "..\\ѧ���ɼ���.txt");
    int count_user = read_users(users, "..\\id.txt");
    if (count_user == -1) {
        printf("��ȡ�û���Ϣʧ��\n");
        return 1;
    }

    char number[MAX_ID_LEN], password[MAX_PASS_LEN];
    printf("�������û�����");
    scanf("%s", number);
    printf("���������룺");
    scanf("%s", password);

    int role = login(users, count_user, number, password);
    if (role == -1) {
        printf("�û������������\n");
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
            printf("δ֪��ɫ��\n");
            break;
    }

    return 0;
}