#ifndef DECLARE2_H_INCLUDED
#define DECLARE2_H_INCLUDED

#include<iostream>
#include<list>
#include<vector>
#include<string>
#include<ctime>
#include<memory>
#include<random>
#include<utility>
#include<fstream>
#include<sstream>
#include<cmath>
#include<stack>
#include<algorithm>


#define OK -2           //�����ײ�����ڷ��ر�ʾ�ɹ�
#define OVERBOUNDARY -3   //��ⳬ���߽緵�ر�ʾ�����߽�
#define INCUBE -4;         //��ʾ����ά�ṹ��û�г����߽�
#define BOUNDARY 100   //��ά�ռ�ĳ����
#define INITIALIZE_BOUNDARY_DATA -1  //��ʼlimit_data�߽�ĳ�ʼֵ
using namespace std;


/*cube.h��*/
vector<int> vector_collision_index;//���ڱ��汻��ײ�����Ŵ����������
stack<int> stack_collision; //����ײ�Ŵص�����ջ
int new_cluster_mark = 1;    //�������µ��Ŵصı��
int over_boundary_time=0;    //�����߽�Ĵ���
bool adjustleft = false;   //cube.h�ļ���adjustment���� �����жϵ�һ�λ��ǵڶ��ε���
int num_of_node = 0;    //�ļ��н�������
static int file_order = 10001; //���������ļ���ʼ����
string filename = "file.txt"; //Ҫ�򿪵��ļ���


/*����random.h�ļ���*/
vector<int> random_vector;    //���������
static int random_pointer=0;  //ָ��random_vector������±�
static int random_seed = 798591;    //���������
static int random_vector_len = 1000;  //��������鳤��


/*random.h�ļ� �� cube.h�ж��г���*/
//�����Ŵ����������(Xmax Xmin��)ʹ�õ�vector,ֻ�ڳ�ʼ����ʱ��ʹ��
static vector<int> min_and_max{BOUNDARY,0,BOUNDARY,0,BOUNDARY,0};
vector<int> mark_change_index;  //��ά�ռ��еı��ת��Ϊvector_lists������ת����
vector<int> index_change_mark; //vector_lists������ת��Ϊ��ά�ռ��еı�ǵ�ת����
int cluster_mark = 1;    //���汻�и��Ŵ�����ά�ṹ�еı��


#endif // DECLARE2_H_INCLUDED
