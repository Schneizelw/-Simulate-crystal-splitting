#ifndef CUBE2_H_INCLUDED
#define CUBE2_H_INCLUDED


class cube{
private:
    int Cube[BOUNDARY][BOUNDARY][BOUNDARY]; //三维空间
    main_vector master_vec;  //用来存放所有的团簇。包含一个团簇链表数组和两个索引
    int split_direction;  //切割方向
    int split_position;   //切割位置
    int move_direction;   //移动方向，默认为0，在x轴下为右边
public:
    void save_result();
    cube();
    void initialize(string filename);
    void _cut_cluster_list(cluster_list& cuted_cluster_list,cluster_list& new_cluster_list);
    void __create_portion_list(list<pair<int,list<coord_position>>> &master_cluster_list,
                       coord_position coord);
    void _adjustment(cluster_list &left,cluster_list &right);
    int split();
    void updata_change_table(cluster_list &splited_cluster_list,cluster_list &new_cluster_list);
    void rewrite_cube(cluster_list rewrite_cluster,int mark);
    bool check_cycle(int check_index);
    int handle_collision(cluster_list checked_cluster_list,int index_splited);
    void check_coord_around(coord_position checked_coord,vector<int>& vector_collision_mark,
                          int self_index);
    int check_boundary(vector<int> &limit_data);
    void __find_max_cluster(cluster_list &right,list<pair<int,list<coord_position>>>&
                            master_cluster_list,
                            list< pair<int, list<coord_position> > >::iterator &iter_max_cluster);

};

/**
*Summary:初始化两个下标索引为0，index_unsingle，index_single。并存入一个空团簇到
    vec_lists。初始化三维空间为0，表示为空。
*Parameters: no。
*Return: no。
**/
cube::cube()
{
    cluster_list tmp_cltr_list;
    master_vec.vec_lists.push_back(tmp_cltr_list);
    master_vec.index_unsingle = 0;
    master_vec.index_single = 0;
    for(size_t i=0;i<BOUNDARY;++i)
        for(size_t j=0;j<BOUNDARY;++j)
            for(size_t k=0;k<BOUNDARY;++k)
                Cube[i][j][k] = 0;

}

/**
*Summary:打开文件将文件中的三维坐标生成一个团簇链表master_vec.vec_lists[0]。
    并将这些三维坐标点在三维空间Cube中标记出来。
*Parameters: no。
*Return: no。
**/
void cube::initialize(string filename)
{
    ifstream fstrm(filename);
    string line;

    while(getline(fstrm,line)){
        ++num_of_node;
        istringstream ss(line);
        coord_position coord;
        ss >> coord.x;
        ss >> coord.y;
        ss >> coord.z;
        //每加入一个点就更新边界的坐标 最后min_and_max[i]中保存了最新的边界坐标。
        coord._update_limit_data();
        master_vec.vec_lists[0].cluster.push_back(coord);
    }
    master_vec.index_single = num_of_node - 1; //为最后一个位置的索引。
    //设置初始的团簇的边界坐标。
    for(int i=0;i<6;++i){
        master_vec.vec_lists[0].limit_data[i] = min_and_max[i];
    }
    //将该团簇标志在三维数组上。
    for(auto coord : master_vec.vec_lists[0].cluster){
        Cube[coord.x][coord.y][coord.z]=cluster_mark;
    }
    //先生成全部的链表 因为最终结果终归是要切成全部碎片的 所以先把vector弄成该有
    //的长度用于优化生成随机要切的团簇把单团簇放到后面。因为总共需要生成
    //num_of_node个，一开始从文件读入了一个大团簇，所以只需要生成num_of_node-1个，
    //又vector的索引从零开始，所以i<num_of_node-1。
    for(int i=0;i<num_of_node-1;++i){
        cluster_list tmp_cltr_list;
        master_vec.vec_lists.push_back(tmp_cltr_list);
    }
    //初始化两个转化表分别表示空间的标记转为索引，索引转为标记。
    for(int i=0;i<=num_of_node;++i){
        mark_change_index.push_back(0); //0号单元不使用。
        index_change_mark.push_back(0);//最后一位不用。
    }
    index_change_mark[0] = 1; //一开始下标索引0对应的团簇标号为1。

    //初始化为0，在split()中还要调用随机函数生成。
    split_direction = 0;
    split_position = 0;
    move_direction = 0;
}

/**
*Summary:将团簇链表cuted_cluster_list切开，切开的部分用new_cluster_list保存
*Parameters:
*   cuted_cluster_list:被切的团簇链表
*   new_cluster_list:用来保存被切开的另一半
*Return: no。
**/
void cube::_cut_cluster_list(cluster_list& cuted_cluster_list,cluster_list&
                             new_cluster_list)
{
    auto iter_beg = cuted_cluster_list.cluster.begin(); //起始迭代器
    auto iter_end = cuted_cluster_list.cluster.end();   //末尾迭代器
    coord_position tmp_coord_position; //用来保存
    //遍历团簇链表cuted_cluster_list
    while(iter_beg!=iter_end){
        tmp_coord_position.__copy_coord_position(*iter_beg);
        //如果在默认的移动方向上(x轴上为右边，Z轴上上部分，Y轴为前部分)，切大于等于
        //切割的位置，则将改点在原来的团簇链表中删除，并移动然后加入新的团簇链表中。
        if(move_direction==0 && iter_beg->return_coordinate(split_direction)>=
           split_position){
            iter_beg = cuted_cluster_list.cluster.erase(iter_beg);
            tmp_coord_position.mov(split_direction,move_direction);
            new_cluster_list.cluster.push_back(tmp_coord_position);
        }
        //如果在move_direction==1(x轴上为右边),切小于等于切割的位置则将改点在原来的
        //团簇链表中删除，并移动然后加入新的团簇链表中。
        else if(move_direction==1 && iter_beg->return_coordinate(split_direction)<=
                (split_position-1)){
            iter_beg = cuted_cluster_list.cluster.erase(iter_beg);
            tmp_coord_position.mov(split_direction,move_direction);
            new_cluster_list.cluster.push_back(tmp_coord_position);
        }else{
            //处理下一个结点
            ++iter_beg;
        }
    }
}

/**
*Summary:创建一个新的团簇链表加入到master_cluster_list中。
*Parameters:
*   master_cluster_list:处理每一个点的总团簇链表。
*   new_cluster_list:用来保存被切开的另一半
*Return: no。
**/
void cube::__create_portion_list(list<pair<int,list<coord_position>>> &
                                 master_cluster_list,
                           coord_position coord)
{
    pair<int,list<coord_position>> portion_list;
    portion_list.first = 1;
    portion_list.second.push_back(coord);
    master_cluster_list.push_back(portion_list);
}

/**
*Summary:master_cluster_list中找出组大团簇的索引保存在iter_max_cluster。
*Parameters:
*   right:被切开移动的一边的全部团簇链表
*   master_cluster_list:用来处理找出 最大团簇链表的 团簇链表的链表。
*   iter_max_cluster:用来保存指向最大团簇链表的索引
*Return: no。
**/
void cube::__find_max_cluster(cluster_list &right,list<pair<int,list<coord_position>>>&
                              master_cluster_list,
                    list< pair<int, list<coord_position> > >::iterator &iter_max_cluster)
{
    //第一个坐标已经以单团簇加入到了master_cluster_list中 所以
    //iter_cur = ++right.cluster.begin()
    auto iter_cur = ++right.cluster.begin(),iter_end = right.cluster.end();
    //遍历right中的每一个坐标。
    for(;iter_cur!=iter_end;++iter_cur){
        coord_position cur_coord = *iter_cur; //当前处理的坐标。
        auto iter_muster_list_cur = master_cluster_list.begin();
        auto iter_muster_list_end = master_cluster_list.end();
        //save_iter用来保存和当前处理的坐标相连的团簇链表的索引。
        vector<decltype(iter_muster_list_cur)> save_iter;
        //扫描master_cluster_list的每一个portion_list
        for(;iter_muster_list_cur!=iter_muster_list_end;++iter_muster_list_cur){
            auto iter_portion_list_cur = (iter_muster_list_cur->second).begin();
            auto iter_portion_list_end = (iter_muster_list_cur->second).end();
            for(;iter_portion_list_cur!=iter_portion_list_end;++iter_portion_list_cur){
                //如果距离小于等于1，表示当前被检测的坐标和该团簇链表相邻
                if(cur_coord.distance_coord(*iter_portion_list_cur)<=(float)1){
                    //保存该的迭代器。
                    save_iter.push_back(iter_muster_list_cur);
                    break;//不用在处理改团簇链表的后续坐标了
                }//if
            }//for
        }//for

        //如果save_iter为空，说明当前处理坐标与任何一个当前的团簇链表都不相邻
        if(save_iter.empty()){
            //以新的单个坐标的团簇链表加入到master_cluster_list中。
            __create_portion_list(master_cluster_list,cur_coord);
            continue; //处理下一个right中的坐标
        }//if
        else{
            //将这些与当前检测坐标相邻的团簇链表合成一个团簇链表
            for(int index_save_iter=save_iter.size()-1;index_save_iter>0;--index_save_iter){
                pair<int,list<coord_position>> tmp_portion_list =
                 std::move(*(save_iter[index_save_iter]));

                save_iter[0]->first += tmp_portion_list.first;  //坐标点数加起来
                //链表合并
                (save_iter[0]->second).splice((save_iter[0]->second).end(),
                                              tmp_portion_list.second);

                // master_cluster_list中删除该链表(已合并到第一个中)
                master_cluster_list.erase(save_iter[index_save_iter]);
                //删除save_iter中的索引(已使用)
                save_iter.erase( --save_iter.end());
            }//for
            (save_iter[0]->second).push_back(cur_coord); //加入当前处理的坐标
            save_iter[0]->first++; //坐标数加1
        }//else
    }//for

    //找出最大的团簇
    auto iter_muster_list_beg = master_cluster_list.begin();
    auto iter_muster_list_end = master_cluster_list.end();
    int max_cluster = 0;//保存最大坐标数
    for(;iter_muster_list_beg!=iter_muster_list_end;++iter_muster_list_beg){
        if(max_cluster <= iter_muster_list_beg->first){
            max_cluster = iter_muster_list_beg->first;
            iter_max_cluster = iter_muster_list_beg;
        }
    }
}

/**
*Summary:调整两边的非最大团簇按要求移动。
*Parameters:
*   left:被切开不移动的一边的团簇链表。
*   right:被切开移动的一边的团簇链表。
*Return: no。
**/
void cube::_adjustment(cluster_list &left,cluster_list &right)
{
    //master_cluster_list:处理每一个点的总团簇链表。每处理一个空间坐标的时候
    //都会检测master_cluster_list，如果被检测的空间坐标和其中的某几个团簇链
    //表相连，则这几个团簇链表于改点是相连的，则合并到一起，如果只和一个团簇
    //链表集合则将该点加入到该团簇链表，如果没有团簇链表与该被检测的坐标相
    //连，则创建一个新的链表(只有一个坐标)，放到master_cluster_list的末尾。
    list< pair<int, list<coord_position> > > master_cluster_list;
    //第一个检测的点是不会和任何团簇链表相连的
    __create_portion_list(master_cluster_list,*right.cluster.begin());

    list< pair<int, list<coord_position> > >::iterator iter_max_cluster;
    //检测后面的所有点并找出最大的团簇，最大团簇的索引用iter_max_cluster保存下来
    __find_max_cluster(right,master_cluster_list,iter_max_cluster);
    //留在右边的只剩下最大的团簇了
    right.cluster= iter_max_cluster->second;
    auto iter_muster_list_beg = master_cluster_list.begin();
    auto iter_muster_list_end = master_cluster_list.end();
    while(iter_muster_list_beg!=iter_muster_list_end){
        if(iter_muster_list_beg == iter_max_cluster){
            //不返回该指针 i就会丢失
            iter_muster_list_beg = master_cluster_list.erase(iter_muster_list_beg);
        }else{
            //非最大团簇的起始迭代器
            auto iter_portion_list_cur = (iter_muster_list_beg->second).begin();
             //非最大团簇的末尾迭代器
            auto iter_portion_list_end = (iter_muster_list_beg->second).end();
            for(;iter_portion_list_cur!=iter_portion_list_end;++iter_portion_list_cur){
                if(adjustleft == false){
                    //小团簇(右边)全部反方向移动回去
                    iter_portion_list_cur->mov(split_direction,(move_direction+1)%2);
                }
                else{
                    //小团簇(右边小团簇移动到左边了，还是没有和打团簇连接上的小团簇)
                    //全部沿着正方向移动回去
                    iter_portion_list_cur->mov(split_direction,move_direction);
                }
            }
            //将小团簇接到左边
            left.cluster.splice(left.cluster.end(),iter_muster_list_beg->second);
            ++iter_muster_list_beg;//处理下一个小团簇
        }
    }
    //处理右边
    if( adjustleft==false ){
         adjustleft=true; //标记为处理左边
         right._set_limit_data(); //重新设置边界值
        _adjustment(right,left);//处理左边的非最大团簇
    }
    right._set_limit_data();//因为上边会递归 所以这一句会调整两边的边界值
}

/**
*Summary:把打开文件表示的团簇切成全部碎片
*Parameters: no。
*Return: no。
**/
int cube::split()
{
    over_boundary_time=0; //超出边界的数量为0。
    //获取被切割团簇链表的下标索引。
    int index_splited = _get_splited_index(master_vec);
    //随机生成切割方向。
    split_direction = _get_random(0,2);
    //得到一个被切割团簇链表的副本
    cluster_list splited_cluster_list = master_vec.vec_lists[index_splited];
    //的到切割的确定位置。
    split_position = _get_split_position(split_direction,splited_cluster_list.limit_data);
    move_direction = 0; //初始化默认方向。
    bool flag_finish = false;//标记该次切割结束。
    //如果出边界次数为0 或者 出边界数为1(只有一边除了边界)。
    while(flag_finish == false){
        //创建一个新的团簇链表(用来接受新切出来的团簇链表)。
        cluster_list new_cluster_list;
        //对要切割的链表进行切割(只是切开 还没有进行调整)。
        _cut_cluster_list(splited_cluster_list,new_cluster_list);
        //对被切开的两份进行调整。
        adjustleft = false;
        _adjustment(splited_cluster_list,new_cluster_list);
        //splited_cluster_list._set_limit_data();
        //new_cluster_list._set_limit_data();
        //检测切割出来的链表是否碰撞了其他的链表。
        int result_handle_collision = handle_collision(new_cluster_list,index_splited);
        //如果出边界了 而且出边界的次数为1。
        if(result_handle_collision==OVERBOUNDARY && over_boundary_time==1){
            move_direction = 1;  //改变切开时要移动的一方。
            //副本更新回没切割之前。
            splited_cluster_list = master_vec.vec_lists[index_splited];
            continue;   //回到while 重新再切割一次 只是改变了切开时要移动的一方。
        }else if(result_handle_collision==OVERBOUNDARY && over_boundary_time==2){
            //如果出边了 而且已经出了两次(表示这个切割方向左右都是不行的)。
            split(); //再次生成随机数切割其他团簇。
        }
        else{ //没有出边界 有可能是没有碰撞 有可能是碰撞了 处理之后没有出边界。
            //vector_collision_index保存了被碰撞的索引。

            int  sz = vector_collision_index.size();
            //对每一个索引指向的团簇链表。
            for(int i=0;i<sz;++i){
                //每一个被碰撞的团簇链表进行处理。
                for(auto coord : master_vec.vec_lists[vector_collision_index[i]].cluster){
                    //先把原来三维空间的标记改变成0。
                    Cube[coord.x][coord.y][coord.z] = 0;
                }
                int collision_index = vector_collision_index[i];
                //再对master_vec中的保存的数据进行移动。
                master_vec.vec_lists[collision_index].move_cluster_list(split_direction,
                                                                        move_direction);
                //再以自己的标记写回。
                rewrite_cube(master_vec.vec_lists[collision_index],
                             index_change_mark[collision_index]);
            }
            vector_collision_index.clear(); //清空被碰撞的索引。
            //对被切开的团簇链表原来的三维空间标记改为0。
            for(auto coord : master_vec.vec_lists[index_splited].cluster){
                //把被切开的原来的团簇链表在三维空间中的标志记成0。
                Cube[coord.x][coord.y][coord.z] = 0;
            }
            //更新转化表mark_change_index，index_change_mark
            updata_change_table(splited_cluster_list,new_cluster_list);
            flag_finish = true;//标记切割成功。
            //打印维护vec_lists的两个下标。
            cout << "index_unsingle:"<<master_vec.index_unsingle << "|"<<
            "index_single:"<<master_vec.index_single<< endl;
            save_result();  //生成文件保存当前状况。
            //如果master_vec.index_single==-1表示全部都是碎片了 退出切割，否则递归。
            if(master_vec.index_single==-1) return 0;
            else split();
        }
    }

}

/**
*Summary:切割成功后更新转换表mark_change_index和index_change_mark,并且更三维空间空间。
*Parameters:
*   splited_cluster_list:被切开不移动的一边的团簇链表，也就是被切开剩下的团簇链表。
*   new_cluster_list:被切开移动的一边的团簇链表，也就是新生成的团簇链表。
*Return: no。
**/
void cube::updata_change_table(cluster_list &splited_cluster_list,cluster_list
                               &new_cluster_list)
{
    bool flag = false; //区分new_cluster_list长度不为1时的两种情况(splited_cluster_list
                        //的长度为1，splited_cluster_list的长度不为1)。
    int flag_double_single = 0;//切出来是碎片的计数器
    //如果被切开的不动的那一边只剩下长度为1的团簇则将这个单个团簇链表加入到index_single
    //指向的位置。
    if(splited_cluster_list.cluster.size()==1){
        flag_double_single++; //单个团簇链表计数加1
        //mark_change_index的cluster_mark处记为master_vec.index_single，cluster_mark为
        //切割前团簇链表在三维空间的标记(在生成随机切割团簇链表中保存)，不动的一边使用
        //原来使用的标记.
        mark_change_index[cluster_mark] = master_vec.index_single;
        //index_change_mark的master_vec.index_single则记为cluster_mark
        index_change_mark[master_vec.index_single] = cluster_mark;
        //将splited_cluster_list接入到master_vec.vec_lists的下标为
        //master_vec.index_single中。
        master_vec.vec_lists[master_vec.index_single--] = splited_cluster_list;
    }
    else{
        //不是碎片则加入到index_unsingle指向的位置。
        flag = true;
        //mark_change_index的cluster_mark处记为master_vec.index_unsingle。
        mark_change_index[cluster_mark] = master_vec.index_unsingle;
        //index_change_mark中的master_vec.index_unsingle记为cluster_mark。
        index_change_mark[master_vec.index_unsingle] = cluster_mark;
        //将splited_cluster_list接入到master_vec.vec_lists的下标为
        //master_vec.index_unsingle中。原来的就被覆盖了。
        master_vec.vec_lists[master_vec.index_unsingle] = splited_cluster_list;
    }
    //将splited_cluster_list写会三维空间，标记为cluster_mark。
    rewrite_cube(splited_cluster_list,cluster_mark);

    //如果被切开的移动的那一边只剩下长度为1的团簇则将这个单个团簇链表加入到
    //index_single指向的位置。
    if(new_cluster_list.cluster.size()==1){
        flag_double_single++;//单个团簇链表计数加1
        //因为这个new_cluster_list是新切出来的所以使用新的标记new_cluster_mark。
        //这个标记每一次成功切割就会加1.所以将mark_change_index的++new_cluster_mark
        //记为master_vec.index_single。
        mark_change_index[++new_cluster_mark] = master_vec.index_single;
        //index_change_mark的master_vec.index_single记为new_cluster_mark。
        index_change_mark[master_vec.index_single] = new_cluster_mark;
        //new_cluster_list接入到master_vec.vec_lists中的master_vec.index_single下标处。
        master_vec.vec_lists[master_vec.index_single--] = new_cluster_list;
    }else if(new_cluster_list.cluster.size()!=1 && flag==false){
        //new_cluster_list不为1，但splited_cluster_list为1，将new_cluster_list接入到
        //master_vec.index_unsingle中。
        //mark_change_index的++new_cluster_mark标记为master_vec.index_unsingle
        mark_change_index[++new_cluster_mark] = master_vec.index_unsingle;
        //index_change_mark的master_vec.index_unsingle标记为new_cluster_mark
        index_change_mark[master_vec.index_unsingle] = new_cluster_mark;
        //new_cluster_list接入到 master_vec.vec_lists的master_vec.index_unsingle下标处
        master_vec.vec_lists[master_vec.index_unsingle] = new_cluster_list;
    }else{
        //new_cluster_list不为1，且splited_cluster_list不为1，将new_cluster_list接入到
        //master_vec.index_unsingle+1处。
        mark_change_index[++new_cluster_mark] = ++master_vec.index_unsingle;
        //index_change_mark的master_vec.index_unsingle(已加1)标记为new_cluster_mark
        index_change_mark[master_vec.index_unsingle] = new_cluster_mark;
        //new_cluster_list接入到 master_vec.vec_lists的master_vec.index_unsingle(已加1)
        //下标处
        master_vec.vec_lists[master_vec.index_unsingle] = new_cluster_list;
    }
     //将new_cluster_list写会三维空间，标记为new_cluster_mark。
    rewrite_cube(new_cluster_list,new_cluster_mark);
    //如果两个都是碎片，则master_vec.index_unsingle减1，因为两个分别加进了index_single
    //和index_single+1中.
    if(flag_double_single==2) --master_vec.index_unsingle;
}

/**
*Summary:将参数中的rewrite_cluster表示的团簇以标记mark写会三维空间
*Parameters:
*   rewrite_cluster:需要写会三维空间的团簇链表
*   mark:写会三维空间使用的标记。
*Return: no。
**/
void cube::rewrite_cube(cluster_list rewrite_cluster,int mark)
{
    for(auto coord : rewrite_cluster.cluster){
        Cube[coord.x][coord.y][coord.z] = mark;
    }
}

/**
*Summary:检查某个团簇是否被连续的碰撞了。
*Parameters:
*   check_index:被碰撞的团簇的索引
*Return: 被连续碰撞了返回true，否则返回false。
**/
bool cube::check_cycle(int check_index)
{
    for(auto exist_index:vector_collision_index){
        if(exist_index==check_index) return true;
    }
    return false;
}

/**
*Summary:处理碰撞
*Parameters:
*   checked_cluster_list:被切割出来的移动的团簇，也是被检测是否碰撞了其他团簇的团簇
*   index_splited:被切割团簇的索引
*Return: 一直碰撞除了边界返回OVERBOUNDARY，成功处理(没出边界)返回OK
**/
int cube::handle_collision(cluster_list checked_cluster_list,int index_splited)
{
    //检查checked_cluster_list本身有没有出边界
    int result_check_boundary = check_boundary(checked_cluster_list.limit_data);
    if(result_check_boundary==OVERBOUNDARY){
        //将碰撞栈清空，将vector_collision_index清空
        while(!stack_collision.empty()){
            int index_from_stack_top = stack_collision.top();
            stack_collision.pop();
            //check_same[index_from_stack_top] = 0;
            vector_collision_index.clear();
        }
        over_boundary_time++;//超出边界的次数(超出两次表示两边移动都不行)
        return OVERBOUNDARY;
    }

    //被碰撞的团簇链表在三维空间中的标记
    vector<int> vector_collision_mark={};

    //检查每一个坐标的上下左右前后是否有其他团簇链表，有则将被碰撞的团簇链表的标记保存
    //在vector_collision_mark中
    for(auto checked_coord : checked_cluster_list.cluster){
        check_coord_around(checked_coord,vector_collision_mark,index_splited);
    }
    //排序删除重复元素
    sort(vector_collision_mark.begin(),vector_collision_mark.end());
    vector_collision_mark.erase(unique(vector_collision_mark.begin(),
                                       vector_collision_mark.end()),
                                    vector_collision_mark.end());
    //到这里上边意义为vector_collision_mark中的元素为 checked_cluster_list所碰撞
    //的mark标记集合

    //将被碰撞的团簇链表且没有多次碰撞的索引加入到stack_collision栈中
    auto beg = vector_collision_mark.begin();
    auto end = vector_collision_mark.end();
    for(;beg!=end;++beg){
        if(!check_cycle(mark_change_index[*beg])){
            stack_collision.push(mark_change_index[*beg]);
        }
    }
    //栈为空，表明无碰撞。
    if(stack_collision.empty()){
        return OK;
    }else{
        //存在碰撞，弹出栈顶元素 递归处理碰撞。
        int index_from_stack_top = stack_collision.top();
        stack_collision.pop();
        //vector_collision_index用来保存被移动的团簇下标
        vector_collision_index.push_back(index_from_stack_top);
        //取出副本，本身master_vec.vec_lists并没有被动过。防止出边界了不用再次移动回去
        cluster_list collision_cluster_list = master_vec.vec_lists[index_from_stack_top];
        //移动副本检测是否又碰撞了其他的团簇
        collision_cluster_list.move_cluster_list(split_direction,move_direction);
        handle_collision(collision_cluster_list,index_from_stack_top);
    }

}

/**
*Summary:检查坐标的周围是否碰到的其他团簇
*Parameters:
*   checked_coord:被检测的坐标
*   vector_collision_mark:保存被碰撞的团簇的标记
*   self_index:被检测的坐标属于那个团簇的下标
*Return: no。
**/
void cube::check_coord_around(coord_position checked_coord,vector<int>&
                              vector_collision_mark,int self_index)
{
    vector<int> vector_around_mark; //上下左右前后
    vector_around_mark.push_back(Cube[checked_coord.x][checked_coord.y][checked_coord.z+1]);
    vector_around_mark.push_back(Cube[checked_coord.x][checked_coord.y][checked_coord.z-1]);
    vector_around_mark.push_back(Cube[checked_coord.x-1][checked_coord.y][checked_coord.z]);
    vector_around_mark.push_back(Cube[checked_coord.x+1][checked_coord.y][checked_coord.z]);
    vector_around_mark.push_back(Cube[checked_coord.x][checked_coord.y+1][checked_coord.z]);
    vector_around_mark.push_back(Cube[checked_coord.x][checked_coord.y-1][checked_coord.z]);
    int len = vector_around_mark.size();
    int self_mark = index_change_mark[self_index];
    for(int i=0;i<len;i++){
        //周围不为0也不为本身属于的团簇 则将改标记加入到vector_collision_mark中。
        if(vector_around_mark[i]!=0 && vector_around_mark[i]!=self_mark){
            vector_collision_mark.push_back(vector_around_mark[i]);
        }
    }
}

/**
*Summary:检查是否超出边界
*Parameters:
*   limit_data:被检测的团簇的边界值(Xmin,Xmax...)
*Return: 超出边界返回OVERBOUNDARY，否则返回INCUBE。
**/
int cube::check_boundary(vector<int> &limit_data)
{

    if(limit_data[1]>99 || limit_data[5]>99 ||
       limit_data[3]>99 || limit_data[0]<0  ||
       limit_data[4]<0  || limit_data[2]<0)
        return OVERBOUNDARY;
    else
        return INCUBE;
}

/**
*Summary:保存当前切割的状况，生成文件保存
*Parameters: no。
*Return: no。
**/
void cube::save_result()
{
    string file_name;
    stringstream ss;
    ss << file_order; //将整形转成流再转换成string类型
    ss >> file_name;
    string file_suffix = ".txt";//文件后缀
    file_name += file_suffix; //得到txt文件名
    ofstream ofst;
    ofst.open(file_name);//不存在则创建文件
    int index_last = num_of_node;
    //写入坐标xyz以及团簇在三维空间中的标记
    //写入非单团簇及标记
    for(int i=master_vec.index_single+1;i<index_last;++i){
        for(auto coord:master_vec.vec_lists[i].cluster){
            ofst<<coord.x<< " "<<coord.y<< " "<<coord.z<< " "<<index_change_mark[i]<<endl;
        }
    }
    //写入单团簇及标记
    for(int i=0;i<=master_vec.index_unsingle;++i){
        for(auto coord:master_vec.vec_lists[i].cluster){
            ofst<<coord.x<<" "<<coord.y<<" "<<coord.z<<" "<<index_change_mark[i]<<endl;
        }
    }
    file_order++; //文件名加1
}

#endif // CUBE2_H_INCLUDED
