#ifndef CLUSTERLIST2_H_INCLUDED
#define CLUSTERLIST2_H_INCLUDED


class coord_position{
public:
    int x=0; //x���ꡣ
    int y=0; //y���ꡣ
    int z=0; //z���ꡣ
public:
    /**
    *Summary:����һ��ȫ�ֱ���min_and_max�ı�߽�ֵ��(Xmin,Xmax,Ymin,Ymax,Zmin,Zmax)��
    *Parameters: no��
    *Return: no��
    **/
    void _update_limit_data()
    {
        //����ԭ�����Ŵر߽�ֵ��
        if(x<min_and_max[0])min_and_max[0]=x;
        if(x>min_and_max[1])min_and_max[1]=x;

        if(y<min_and_max[2])min_and_max[2]=y;
        if(y>min_and_max[3])min_and_max[3]=y;

        if(z<min_and_max[4])min_and_max[4]=z;
        if(z>min_and_max[5])min_and_max[5]=z;
    }

    /**
    *Summary:�õ��и���Ӧ�����ꡣ
    *Parameters:
    *    split_dire:�и��,0��ʾ��X���У�1��ʾ��Y���У�2��ʾZ���С�
    *Return: �����и�򷵻�x,y,z����㡣
    **/
    int return_coordinate(int split_dire){
        if(split_dire==0) return x;
        else if(split_dire==1) return z;
        else return y;
    }

    /**
    *Summary:�����и����ƶ������ƶ��õ㡣
    *Parameters:
    *    split_dire:�и��,0��ʾ��X���У�1��ʾ��Y���У�2��ʾZ����x��
    *    mov_dire:�ƶ�����0��ʾĬ�Ϸ���(x����Ϊ���)��
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
    *Summary:�жϲ�������������͵������Ƿ���ͬ��
    *Parameters:
    *    coord:Ҫ����Ƿ���ȵĵ㡣
    *Return: ������������ͬ����true�����򷵻�false��
    **/
    bool equal_coord(coord_position coord){
        if(x==coord.x && y==coord.y && z==coord.z)
            return true;
        else
            return false;
    }

    /**
    *Summary:�����������֮��ľ��롣
    *Parameters:
    *    coord:���꣬���ں͵�ǰ��������������㡣
    *Return: �������������ľ��� ��������Ϊfloat��
    **/
    float distance_coord(coord_position coord)
    {
        return sqrt((x-coord.x)*(x-coord.x) +(y-coord.y)*(y-coord.y)+
                    (z-coord.z)*(z-coord.z));
    }

    /**
    *Summary:����������������ǰ���ꡣ
    *Parameters:
    *    coord:���꣬����Դ��
    *Return: no��
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
   vector<int> limit_data;          //�ֱ�Ϊ(Xmin,Xmax,Ymin,Ymax,Zmin,Zmax)��
   list<coord_position> cluster;    //���Ŵذ���������㡣
public:
    /**
    *Summary:���캯������ʼ�Ŵصı߽�ֵΪ-1��
    *Parameters: no��
    *Return: no��
    **/
    cluster_list(){
         for(int i=0;i<6;++i)
            limit_data.push_back(INITIALIZE_BOUNDARY_DATA);
    }
    /**
    *Summary:�õ���ǰ�Ŵصı߽�ֵ(Xmin,Xmax...)��
    *Parameters: no��
    *Return: no��
    **/
    void _set_limit_data()
    {
        //����һ���Ŵ����� �õ����Ŵ���������ԡ�
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
    *Summary:�����и����ƶ����ƶ���ǰ�Ŵأ��߽�ֵҲ��ͬʱ�趨��
    *Parameters:
    *    split_dire:�и��0��ʾ��X���У�1��ʾ��Y���У�2��ʾZ����x��
    *    mov_dire:�ƶ�����0��ʾĬ�Ϸ���(x����Ϊ���)��
    *Return: no��
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
    vector<cluster_list> vec_lists;  //����������е��Ŵء�
    //ָ���ǵ������Ŵص�����������[0,index_unsingle]Ϊ�ǵ�������Ŵء�
    int index_unsingle;
    //ָ���ǵ������Ŵص�����������(index_single,number_of_node]Ϊ��������Ŵء�
    int index_single;
};


#endif // CLUSTERLIST2_H_INCLUDED
