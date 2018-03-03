#ifndef CLUSTERLIST2_H_INCLUDED
#define CLUSTERLIST2_H_INCLUDED


class coord_position{
public:
    int x=0; //x坐标。
    int y=0; //y坐标。
    int z=0; //z坐标。
public:
    /**
    *Summary:更新一个全局变量min_and_max改变边界值即(Xmin,Xmax,Ymin,Ymax,Zmin,Zmax)。
    *Parameters: no。
    *Return: no。
    **/
    void _update_limit_data()
    {
        //更新原来的团簇边界值。
        if(x<min_and_max[0])min_and_max[0]=x;
        if(x>min_and_max[1])min_and_max[1]=x;

        if(y<min_and_max[2])min_and_max[2]=y;
        if(y>min_and_max[3])min_and_max[3]=y;

        if(z<min_and_max[4])min_and_max[4]=z;
        if(z>min_and_max[5])min_and_max[5]=z;
    }

    /**
    *Summary:得到切割方向对应的坐标。
    *Parameters:
    *    split_dire:切割方向,0表示沿X轴切，1表示沿Y轴切，2表示Z轴切。
    *Return: 根据切割方向返回x,y,z坐标点。
    **/
    int return_coordinate(int split_dire){
        if(split_dire==0) return x;
        else if(split_dire==1) return z;
        else return y;
    }

    /**
    *Summary:给定切割方向和移动方向移动该点。
    *Parameters:
    *    split_dire:切割方向,0表示沿X轴切，1表示沿Y轴切，2表示Z轴切x。
    *    mov_dire:移动方向，0表示默认方向(x轴下为左边)。
    *Return: no
    **/
    void mov(int split_dire,int mov_dire){
        if(split_dire==0)
            x = mov_dire==0? x+1:x-1;
        else if(split_dire==1)
            z = mov_dire==0? z+1:z-1;
        else
            y = mov_dire==0? y+1:y-1;
    }

    /**
    *Summary:判断参数传进的坐标和当坐标是否相同。
    *Parameters:
    *    coord:要检测是否相等的点。
    *Return: 如果两坐标点相同返回true，否则返回false。
    **/
    bool equal_coord(coord_position coord){
        if(x==coord.x && y==coord.y && z==coord.z)
            return true;
        else
            return false;
    }

    /**
    *Summary:求两个坐标点之间的距离。
    *Parameters:
    *    coord:坐标，用于和当前坐标做求距离运算。
    *Return: 返回两个坐标点的距离 返回类型为float。
    **/
    float distance_coord(coord_position coord)
    {
        return sqrt((x-coord.x)*(x-coord.x) +(y-coord.y)*(y-coord.y)+
                    (z-coord.z)*(z-coord.z));
    }

    /**
    *Summary:拷贝参数坐标点给当前坐标。
    *Parameters:
    *    coord:坐标，拷贝源。
    *Return: no。
    **/
    void __copy_coord_position(coord_position& coord)
    {
        x = coord.x;
        y = coord.y;
        z = coord.z;
    }
};

class cluster_list{
public:
   vector<int> limit_data;          //分别为(Xmin,Xmax,Ymin,Ymax,Zmin,Zmax)。
   list<coord_position> cluster;    //该团簇包括的坐标点。
public:
    /**
    *Summary:构造函数，初始团簇的边界值为-1。
    *Parameters: no。
    *Return: no。
    **/
    cluster_list(){
         for(int i=0;i<6;++i)
            limit_data.push_back(INITIALIZE_BOUNDARY_DATA);
    }
    /**
    *Summary:得到当前团簇的边界值(Xmin,Xmax...)。
    *Parameters: no。
    *Return: no。
    **/
    void _set_limit_data()
    {
        //传入一个团簇链表 得到该团簇链表的属性。
        vector<int> temp{BOUNDARY,0,BOUNDARY,0,BOUNDARY,0};
        min_and_max = temp;
        for(auto coord : cluster){
            coord._update_limit_data();
        }
        for(size_t i=0;i<6;++i){
            limit_data[i] = min_and_max[i];
        }
    }
    /**
    *Summary:给定切割方向和移动，移动当前团簇，边界值也会同时设定。
    *Parameters:
    *    split_dire:切割方向，0表示沿X轴切，1表示沿Y轴切，2表示Z轴切x。
    *    mov_dire:移动方向，0表示默认方向(x轴下为左边)。
    *Return: no。
    **/
    void move_cluster_list(int split_dire,int mov_dire)
    {
        auto iter_beg = cluster.begin(),iter_end = cluster.end();
        for(;iter_beg!=iter_end;++iter_beg){
            iter_beg->mov(split_dire,mov_dire);
        }
        _set_limit_data();
    }
};

class main_vector{
public:
    vector<cluster_list> vec_lists;  //用来存放所有的团簇。
    //指向不是单结点的团簇的索引，区间[0,index_unsingle]为非单个结点团簇。
    int index_unsingle;
    //指向是单结点的团簇的索引，区间(index_single,number_of_node]为单个结点团簇。
    int index_single;
};


#endif // CLUSTERLIST2_H_INCLUDED
