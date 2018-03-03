#ifndef RANDOM2_H_INCLUDED
#define RANDOM2_H_INCLUDED


/**
*Summary:在区间[lft_data,rht_data]获取一个随机数。
*Parameters:
*    lft_data：随机数的最小值。
*    rht_data：随机数的最大值。
*Return: no。
**/
int _get_random(size_t lft_data,size_t rht_data)
{
    int tmp;
    random_pointer %= 1000;   //指回随机数组的前面
    tmp = random_vector[random_pointer++];       //取出一个随机数
    //得到[lft_data,rht_data]之间的一个随机数
    tmp = tmp%((rht_data-lft_data)+1)+lft_data;
    return tmp;
}

/**
*Summary:创建一个随机数数组，里边生成长度为random_vector_len个随机数。
*Parameters:
*    min_val：随机数的最小值。
*    max_val：随机数的最大值。
*Return: no。
**/
void creat_random_vector(int min_val,int max_val)
{
    default_random_engine e(random_seed);  //随机数引擎
    uniform_int_distribution<int> u(min_val,max_val); //均匀分布
    for(int i=0;i<random_vector_len;++i){
        random_vector.push_back(u(e));  //生成random_vector_len个随机数
    }
}

/**
*Summary:在一个给定区间(参数limit_data给出)中生成切割位置，传进的
    切割方向有可能会被改变。
*Parameters:
*    split_dire：切割方向。
*    limit_data：要被切割的团簇的边界值。
*Return: 返回一个合适的切割位置。
**/
int _get_split_position(int &split_dire,vector<int> limit_data)
{
    int split_count = 3;    //计数 三次不行就退出。
    //因为第一次,第二次都有可能切割不成功，用两个变量保存前两次切割方向
    //用于保存第一次切割方向和第二次切割方向
    int tmp_split_dire_first,tmp_split_dire_second;
    tmp_split_dire_first = split_dire;  //保存第一次切割方向
    while(split_count){
        //如果该方向不能切割break出switch语句，在switch外修改方向，
        //如果该方向可以切割，在给定的范围(团簇xyz最大最小值)内生成一个随机切割位置。
        switch(split_dire){
            case 0:   //垂直Z轴切割,即在X轴上切割。
                if(limit_data[1]-limit_data[0]==0 ) break;//该方向不能切割
                else return _get_random(limit_data[0]+1,limit_data[1]);
            case 1:   //垂直Y轴切割,即在Z轴上切割。
                if(limit_data[5]-limit_data[4]==0) break; //该方向不能切割
                else return _get_random(limit_data[4]+1,limit_data[5]);
            case 2:   //垂直X轴切割,即在Y轴上切割。
                if(limit_data[3]-limit_data[2]==0) break; //该方向不能切割
                else return _get_random(limit_data[2]+1,limit_data[3]);
        }
        //split_count==3表示第一次切割不成功,在随机切割另外两个方向
        if(split_count==3){
            int choose_other_direction = _get_random(0,1);
            //得到的切割方向不能切割则50%几率选择另外两个方向。因为split_dire只
            //能是0,1,2所以对每一个值来说加1或加2再取余3就可以转换另外两个方向
            //。起始为0,(0+1)%3=1,(0+2)%3=2.原来的切割方向就从。起始为0,(0+1)%3=1
            //在X轴上切割转成在Z轴上切割和Y轴上切割。起始为1,(1+1)%3=2,(1+2)%3=0.
            //同理上面这样就可以转为另外两个方向。起始为2,(2+1)%3=0,(2+2)%3=1.同理。
            split_dire = (choose_other_direction==0)? (split_dire+1)%3 : (split_dire+2)%3;
            tmp_split_dire_second = split_dire; //保存第二次切割方向。
        }
        if(split_count==2){
            //两个方向都不行 选择第三个方向
            //0+1==1所以第三次为2。
            if(tmp_split_dire_first+tmp_split_dire_second==1) split_dire = 2;
            //1+2==3所以第三次为0。
            else if(tmp_split_dire_first+tmp_split_dire_second==3)split_dire = 0;
            else split_dire = 1;  //除了上面就是1了。
        }
        --split_count;
    }
}

/**
*Summary:生成一个要切割的团簇的索引。
*Parameters:
*    master_vec：全部的团簇都在里面。
*Return: 返回一个合适的切割位置。
**/
int _get_splited_index(main_vector & master_vec)
{
    int index_last = master_vec.index_unsingle; //可切割团簇的末尾索引。
    int index_splited= _get_random(0,index_last); //生成可以切割索引。
    //cluster_mark为全局变量，为该被切割团簇在三维数组中的标号。
    cluster_mark = index_change_mark[index_splited];

    //取出该被切割团簇 的 团簇链表与要末尾链表交换。
    //因为有可能被切割的团簇只有两个结点，所以切出来两个都是单个结点团簇，
    //都会被加到master_vec.vec_lists的后面，所以被切割的位置就会空出来算法
    //就不能继续执行，所以必须得交换。
    cluster_list temp_cluster_list = master_vec.vec_lists[index_splited];
    master_vec.vec_lists[index_splited] = master_vec.vec_lists[index_last];
    master_vec.vec_lists[index_last] = temp_cluster_list;

    //维护的转换表也要交换。
    int tmp = index_change_mark[index_splited];
    index_change_mark[index_splited] = index_change_mark[index_last];
    index_change_mark[index_last] = tmp;

    tmp = mark_change_index[index_change_mark[index_splited]];
    mark_change_index[index_change_mark[index_splited]] =
     mark_change_index[index_change_mark[index_last]];

    mark_change_index[index_change_mark[index_last]] = tmp;
    //因为生成的随机被切割团簇链表已经 长度不为1的末尾的团簇链表交换了。
    //所以返回切割的索引为可切割团簇的末尾索引。但是本质随机切割的，只不
    //过把要切割的团簇链表放到了可切割团簇链表的最后而已。
    return index_last;
}


#endif // RANDOM2_H_INCLUDED
