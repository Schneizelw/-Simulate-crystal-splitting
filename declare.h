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


#define OK -2           //检测碰撞中用于返回表示成功
#define OVERBOUNDARY -3   //检测超出边界返回表示超出边界
#define INCUBE -4;         //表示在三维结构中没有超出边界
#define BOUNDARY 100   //三维空间的长宽高
#define INITIALIZE_BOUNDARY_DATA -1  //初始limit_data边界的初始值
using namespace std;


/*cube.h中*/
vector<int> vector_collision_index;//用于保存被碰撞过的团簇链表的索引
stack<int> stack_collision; //被碰撞团簇的索引栈
int new_cluster_mark = 1;    //当且最新的团簇的标记
int over_boundary_time=0;    //超出边界的次数
bool adjustleft = false;   //cube.h文件中adjustment函数 用于判断第一次还是第二次调用
int num_of_node = 0;    //文件中结点的数量
static int file_order = 10001; //保存结果的文件起始名字
string filename = "file.txt"; //要打开的文件名


/*用于random.h文件中*/
vector<int> random_vector;    //随机数数组
static int random_pointer=0;  //指向random_vector随机数下标
static int random_seed = 798591;    //随机数种子
static int random_vector_len = 1000;  //随机数数组长度


/*random.h文件 和 cube.h中都有出现*/
//设置团簇链表的属性(Xmax Xmin等)使用的vector,只在初始化的时候使用
static vector<int> min_and_max{BOUNDARY,0,BOUNDARY,0,BOUNDARY,0};
vector<int> mark_change_index;  //三维空间中的标记转化为vector_lists索引的转换表
vector<int> index_change_mark; //vector_lists索引的转换为三维空间中的标记的转换表
int cluster_mark = 1;    //保存被切割团簇在三维结构中的标记


#endif // DECLARE2_H_INCLUDED
