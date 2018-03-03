#ifndef CUBE2_H_INCLUDED
#define CUBE2_H_INCLUDED


class cube{
private:
    int Cube[BOUNDARY][BOUNDARY][BOUNDARY]; //��ά�ռ�
    main_vector master_vec;  //����������е��Ŵء�����һ���Ŵ������������������
    int split_direction;  //�и��
    int split_position;   //�и�λ��
    int move_direction;   //�ƶ�����Ĭ��Ϊ0����x����Ϊ�ұ�
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
*Summary:��ʼ�������±�����Ϊ0��index_unsingle��index_single��������һ�����Ŵص�
    vec_lists����ʼ����ά�ռ�Ϊ0����ʾΪ�ա�
*Parameters: no��
*Return: no��
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
*Summary:���ļ����ļ��е���ά��������һ���Ŵ�����master_vec.vec_lists[0]��
    ������Щ��ά���������ά�ռ�Cube�б�ǳ�����
*Parameters: no��
*Return: no��
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
        //ÿ����һ����͸��±߽������ ���min_and_max[i]�б��������µı߽����ꡣ
        coord._update_limit_data();
        master_vec.vec_lists[0].cluster.push_back(coord);
    }
    master_vec.index_single = num_of_node - 1; //Ϊ���һ��λ�õ�������
    //���ó�ʼ���Ŵصı߽����ꡣ
    for(int i=0;i<6;++i){
        master_vec.vec_lists[0].limit_data[i] = min_and_max[i];
    }
    //�����Ŵر�־����ά�����ϡ�
    for(auto coord : master_vec.vec_lists[0].cluster){
        Cube[coord.x][coord.y][coord.z]=cluster_mark;
    }
    //������ȫ�������� ��Ϊ���ս���չ���Ҫ�г�ȫ����Ƭ�� �����Ȱ�vectorŪ�ɸ���
    //�ĳ��������Ż��������Ҫ�е��Ŵذѵ��Ŵطŵ����档��Ϊ�ܹ���Ҫ����
    //num_of_node����һ��ʼ���ļ�������һ�����Ŵأ�����ֻ��Ҫ����num_of_node-1����
    //��vector���������㿪ʼ������i<num_of_node-1��
    for(int i=0;i<num_of_node-1;++i){
        cluster_list tmp_cltr_list;
        master_vec.vec_lists.push_back(tmp_cltr_list);
    }
    //��ʼ������ת����ֱ��ʾ�ռ�ı��תΪ����������תΪ��ǡ�
    for(int i=0;i<=num_of_node;++i){
        mark_change_index.push_back(0); //0�ŵ�Ԫ��ʹ�á�
        index_change_mark.push_back(0);//���һλ���á�
    }
    index_change_mark[0] = 1; //һ��ʼ�±�����0��Ӧ���Ŵر��Ϊ1��

    //��ʼ��Ϊ0����split()�л�Ҫ��������������ɡ�
    split_direction = 0;
    split_position = 0;
    move_direction = 0;
}

/**
*Summary:���Ŵ�����cuted_cluster_list�п����п��Ĳ�����new_cluster_list����
*Parameters:
*   cuted_cluster_list:���е��Ŵ�����
*   new_cluster_list:�������汻�п�����һ��
*Return: no��
**/
void cube::_cut_cluster_list(cluster_list& cuted_cluster_list,cluster_list&
                             new_cluster_list)
{
    auto iter_beg = cuted_cluster_list.cluster.begin(); //��ʼ������
    auto iter_end = cuted_cluster_list.cluster.end();   //ĩβ������
    coord_position tmp_coord_position; //��������
    //�����Ŵ�����cuted_cluster_list
    while(iter_beg!=iter_end){
        tmp_coord_position.__copy_coord_position(*iter_beg);
        //�����Ĭ�ϵ��ƶ�������(x����Ϊ�ұߣ�Z�����ϲ��֣�Y��Ϊǰ����)���д��ڵ���
        //�и��λ�ã��򽫸ĵ���ԭ�����Ŵ�������ɾ�������ƶ�Ȼ������µ��Ŵ������С�
        if(move_direction==0 && iter_beg->return_coordinate(split_direction)>=
           split_position){
            iter_beg = cuted_cluster_list.cluster.erase(iter_beg);
            tmp_coord_position.mov(split_direction,move_direction);
            new_cluster_list.cluster.push_back(tmp_coord_position);
        }
        //�����move_direction==1(x����Ϊ�ұ�),��С�ڵ����и��λ���򽫸ĵ���ԭ����
        //�Ŵ�������ɾ�������ƶ�Ȼ������µ��Ŵ������С�
        else if(move_direction==1 && iter_beg->return_coordinate(split_direction)<=
                (split_position-1)){
            iter_beg = cuted_cluster_list.cluster.erase(iter_beg);
            tmp_coord_position.mov(split_direction,move_direction);
            new_cluster_list.cluster.push_back(tmp_coord_position);
        }else{
            //������һ�����
            ++iter_beg;
        }
    }
}

/**
*Summary:����һ���µ��Ŵ�������뵽master_cluster_list�С�
*Parameters:
*   master_cluster_list:����ÿһ��������Ŵ�����
*   new_cluster_list:�������汻�п�����һ��
*Return: no��
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
*Summary:master_cluster_list���ҳ�����Ŵص�����������iter_max_cluster��
*Parameters:
*   right:���п��ƶ���һ�ߵ�ȫ���Ŵ�����
*   master_cluster_list:���������ҳ� ����Ŵ������ �Ŵ����������
*   iter_max_cluster:��������ָ������Ŵ����������
*Return: no��
**/
void cube::__find_max_cluster(cluster_list &right,list<pair<int,list<coord_position>>>&
                              master_cluster_list,
                    list< pair<int, list<coord_position> > >::iterator &iter_max_cluster)
{
    //��һ�������Ѿ��Ե��Ŵؼ��뵽��master_cluster_list�� ����
    //iter_cur = ++right.cluster.begin()
    auto iter_cur = ++right.cluster.begin(),iter_end = right.cluster.end();
    //����right�е�ÿһ�����ꡣ
    for(;iter_cur!=iter_end;++iter_cur){
        coord_position cur_coord = *iter_cur; //��ǰ��������ꡣ
        auto iter_muster_list_cur = master_cluster_list.begin();
        auto iter_muster_list_end = master_cluster_list.end();
        //save_iter��������͵�ǰ����������������Ŵ������������
        vector<decltype(iter_muster_list_cur)> save_iter;
        //ɨ��master_cluster_list��ÿһ��portion_list
        for(;iter_muster_list_cur!=iter_muster_list_end;++iter_muster_list_cur){
            auto iter_portion_list_cur = (iter_muster_list_cur->second).begin();
            auto iter_portion_list_end = (iter_muster_list_cur->second).end();
            for(;iter_portion_list_cur!=iter_portion_list_end;++iter_portion_list_cur){
                //�������С�ڵ���1����ʾ��ǰ����������͸��Ŵ���������
                if(cur_coord.distance_coord(*iter_portion_list_cur)<=(float)1){
                    //����õĵ�������
                    save_iter.push_back(iter_muster_list_cur);
                    break;//�����ڴ�����Ŵ�����ĺ���������
                }//if
            }//for
        }//for

        //���save_iterΪ�գ�˵����ǰ�����������κ�һ����ǰ���Ŵ�����������
        if(save_iter.empty()){
            //���µĵ���������Ŵ�������뵽master_cluster_list�С�
            __create_portion_list(master_cluster_list,cur_coord);
            continue; //������һ��right�е�����
        }//if
        else{
            //����Щ�뵱ǰ����������ڵ��Ŵ�����ϳ�һ���Ŵ�����
            for(int index_save_iter=save_iter.size()-1;index_save_iter>0;--index_save_iter){
                pair<int,list<coord_position>> tmp_portion_list =
                 std::move(*(save_iter[index_save_iter]));

                save_iter[0]->first += tmp_portion_list.first;  //�������������
                //����ϲ�
                (save_iter[0]->second).splice((save_iter[0]->second).end(),
                                              tmp_portion_list.second);

                // master_cluster_list��ɾ��������(�Ѻϲ�����һ����)
                master_cluster_list.erase(save_iter[index_save_iter]);
                //ɾ��save_iter�е�����(��ʹ��)
                save_iter.erase( --save_iter.end());
            }//for
            (save_iter[0]->second).push_back(cur_coord); //���뵱ǰ���������
            save_iter[0]->first++; //��������1
        }//else
    }//for

    //�ҳ������Ŵ�
    auto iter_muster_list_beg = master_cluster_list.begin();
    auto iter_muster_list_end = master_cluster_list.end();
    int max_cluster = 0;//�������������
    for(;iter_muster_list_beg!=iter_muster_list_end;++iter_muster_list_beg){
        if(max_cluster <= iter_muster_list_beg->first){
            max_cluster = iter_muster_list_beg->first;
            iter_max_cluster = iter_muster_list_beg;
        }
    }
}

/**
*Summary:�������ߵķ�����Ŵذ�Ҫ���ƶ���
*Parameters:
*   left:���п����ƶ���һ�ߵ��Ŵ�����
*   right:���п��ƶ���һ�ߵ��Ŵ�����
*Return: no��
**/
void cube::_adjustment(cluster_list &left,cluster_list &right)
{
    //master_cluster_list:����ÿһ��������Ŵ�����ÿ����һ���ռ������ʱ��
    //������master_cluster_list����������Ŀռ���������е�ĳ�����Ŵ���
    //�����������⼸���Ŵ������ڸĵ��������ģ���ϲ���һ�����ֻ��һ���Ŵ�
    //�������򽫸õ���뵽���Ŵ��������û���Ŵ�������ñ�����������
    //�����򴴽�һ���µ�����(ֻ��һ������)���ŵ�master_cluster_list��ĩβ��
    list< pair<int, list<coord_position> > > master_cluster_list;
    //��һ�����ĵ��ǲ�����κ��Ŵ�����������
    __create_portion_list(master_cluster_list,*right.cluster.begin());

    list< pair<int, list<coord_position> > >::iterator iter_max_cluster;
    //����������е㲢�ҳ������Ŵأ�����Ŵص�������iter_max_cluster��������
    __find_max_cluster(right,master_cluster_list,iter_max_cluster);
    //�����ұߵ�ֻʣ�������Ŵ���
    right.cluster= iter_max_cluster->second;
    auto iter_muster_list_beg = master_cluster_list.begin();
    auto iter_muster_list_end = master_cluster_list.end();
    while(iter_muster_list_beg!=iter_muster_list_end){
        if(iter_muster_list_beg == iter_max_cluster){
            //�����ظ�ָ�� i�ͻᶪʧ
            iter_muster_list_beg = master_cluster_list.erase(iter_muster_list_beg);
        }else{
            //������Ŵص���ʼ������
            auto iter_portion_list_cur = (iter_muster_list_beg->second).begin();
             //������Ŵص�ĩβ������
            auto iter_portion_list_end = (iter_muster_list_beg->second).end();
            for(;iter_portion_list_cur!=iter_portion_list_end;++iter_portion_list_cur){
                if(adjustleft == false){
                    //С�Ŵ�(�ұ�)ȫ���������ƶ���ȥ
                    iter_portion_list_cur->mov(split_direction,(move_direction+1)%2);
                }
                else{
                    //С�Ŵ�(�ұ�С�Ŵ��ƶ�������ˣ�����û�кʹ��Ŵ������ϵ�С�Ŵ�)
                    //ȫ�������������ƶ���ȥ
                    iter_portion_list_cur->mov(split_direction,move_direction);
                }
            }
            //��С�Ŵؽӵ����
            left.cluster.splice(left.cluster.end(),iter_muster_list_beg->second);
            ++iter_muster_list_beg;//������һ��С�Ŵ�
        }
    }
    //�����ұ�
    if( adjustleft==false ){
         adjustleft=true; //���Ϊ�������
         right._set_limit_data(); //�������ñ߽�ֵ
        _adjustment(right,left);//������ߵķ�����Ŵ�
    }
    right._set_limit_data();//��Ϊ�ϱ߻�ݹ� ������һ���������ߵı߽�ֵ
}

/**
*Summary:�Ѵ��ļ���ʾ���Ŵ��г�ȫ����Ƭ
*Parameters: no��
*Return: no��
**/
int cube::split()
{
    over_boundary_time=0; //�����߽������Ϊ0��
    //��ȡ���и��Ŵ�������±�������
    int index_splited = _get_splited_index(master_vec);
    //��������и��
    split_direction = _get_random(0,2);
    //�õ�һ�����и��Ŵ�����ĸ���
    cluster_list splited_cluster_list = master_vec.vec_lists[index_splited];
    //�ĵ��и��ȷ��λ�á�
    split_position = _get_split_position(split_direction,splited_cluster_list.limit_data);
    move_direction = 0; //��ʼ��Ĭ�Ϸ���
    bool flag_finish = false;//��Ǹô��и������
    //������߽����Ϊ0 ���� ���߽���Ϊ1(ֻ��һ�߳��˱߽�)��
    while(flag_finish == false){
        //����һ���µ��Ŵ�����(�����������г������Ŵ�����)��
        cluster_list new_cluster_list;
        //��Ҫ�и����������и�(ֻ���п� ��û�н��е���)��
        _cut_cluster_list(splited_cluster_list,new_cluster_list);
        //�Ա��п������ݽ��е�����
        adjustleft = false;
        _adjustment(splited_cluster_list,new_cluster_list);
        //splited_cluster_list._set_limit_data();
        //new_cluster_list._set_limit_data();
        //����и�����������Ƿ���ײ������������
        int result_handle_collision = handle_collision(new_cluster_list,index_splited);
        //������߽��� ���ҳ��߽�Ĵ���Ϊ1��
        if(result_handle_collision==OVERBOUNDARY && over_boundary_time==1){
            move_direction = 1;  //�ı��п�ʱҪ�ƶ���һ����
            //�������»�û�и�֮ǰ��
            splited_cluster_list = master_vec.vec_lists[index_splited];
            continue;   //�ص�while �������и�һ�� ֻ�Ǹı����п�ʱҪ�ƶ���һ����
        }else if(result_handle_collision==OVERBOUNDARY && over_boundary_time==2){
            //��������� �����Ѿ���������(��ʾ����и�����Ҷ��ǲ��е�)��
            split(); //�ٴ�����������и������Ŵء�
        }
        else{ //û�г��߽� �п�����û����ײ �п�������ײ�� ����֮��û�г��߽硣
            //vector_collision_index�����˱���ײ��������

            int  sz = vector_collision_index.size();
            //��ÿһ������ָ����Ŵ�����
            for(int i=0;i<sz;++i){
                //ÿһ������ײ���Ŵ�������д���
                for(auto coord : master_vec.vec_lists[vector_collision_index[i]].cluster){
                    //�Ȱ�ԭ����ά�ռ�ı�Ǹı��0��
                    Cube[coord.x][coord.y][coord.z] = 0;
                }
                int collision_index = vector_collision_index[i];
                //�ٶ�master_vec�еı�������ݽ����ƶ���
                master_vec.vec_lists[collision_index].move_cluster_list(split_direction,
                                                                        move_direction);
                //�����Լ��ı��д�ء�
                rewrite_cube(master_vec.vec_lists[collision_index],
                             index_change_mark[collision_index]);
            }
            vector_collision_index.clear(); //��ձ���ײ��������
            //�Ա��п����Ŵ�����ԭ������ά�ռ��Ǹ�Ϊ0��
            for(auto coord : master_vec.vec_lists[index_splited].cluster){
                //�ѱ��п���ԭ�����Ŵ���������ά�ռ��еı�־�ǳ�0��
                Cube[coord.x][coord.y][coord.z] = 0;
            }
            //����ת����mark_change_index��index_change_mark
            updata_change_table(splited_cluster_list,new_cluster_list);
            flag_finish = true;//����и�ɹ���
            //��ӡά��vec_lists�������±ꡣ
            cout << "index_unsingle:"<<master_vec.index_unsingle << "|"<<
            "index_single:"<<master_vec.index_single<< endl;
            save_result();  //�����ļ����浱ǰ״����
            //���master_vec.index_single==-1��ʾȫ��������Ƭ�� �˳��и����ݹ顣
            if(master_vec.index_single==-1) return 0;
            else split();
        }
    }

}

/**
*Summary:�и�ɹ������ת����mark_change_index��index_change_mark,���Ҹ���ά�ռ�ռ䡣
*Parameters:
*   splited_cluster_list:���п����ƶ���һ�ߵ��Ŵ�����Ҳ���Ǳ��п�ʣ�µ��Ŵ�����
*   new_cluster_list:���п��ƶ���һ�ߵ��Ŵ�����Ҳ���������ɵ��Ŵ�����
*Return: no��
**/
void cube::updata_change_table(cluster_list &splited_cluster_list,cluster_list
                               &new_cluster_list)
{
    bool flag = false; //����new_cluster_list���Ȳ�Ϊ1ʱ���������(splited_cluster_list
                        //�ĳ���Ϊ1��splited_cluster_list�ĳ��Ȳ�Ϊ1)��
    int flag_double_single = 0;//�г�������Ƭ�ļ�����
    //������п��Ĳ�������һ��ֻʣ�³���Ϊ1���Ŵ�����������Ŵ�������뵽index_single
    //ָ���λ�á�
    if(splited_cluster_list.cluster.size()==1){
        flag_double_single++; //�����Ŵ����������1
        //mark_change_index��cluster_mark����Ϊmaster_vec.index_single��cluster_markΪ
        //�и�ǰ�Ŵ���������ά�ռ�ı��(����������и��Ŵ������б���)��������һ��ʹ��
        //ԭ��ʹ�õı��.
        mark_change_index[cluster_mark] = master_vec.index_single;
        //index_change_mark��master_vec.index_single���Ϊcluster_mark
        index_change_mark[master_vec.index_single] = cluster_mark;
        //��splited_cluster_list���뵽master_vec.vec_lists���±�Ϊ
        //master_vec.index_single�С�
        master_vec.vec_lists[master_vec.index_single--] = splited_cluster_list;
    }
    else{
        //������Ƭ����뵽index_unsingleָ���λ�á�
        flag = true;
        //mark_change_index��cluster_mark����Ϊmaster_vec.index_unsingle��
        mark_change_index[cluster_mark] = master_vec.index_unsingle;
        //index_change_mark�е�master_vec.index_unsingle��Ϊcluster_mark��
        index_change_mark[master_vec.index_unsingle] = cluster_mark;
        //��splited_cluster_list���뵽master_vec.vec_lists���±�Ϊ
        //master_vec.index_unsingle�С�ԭ���ľͱ������ˡ�
        master_vec.vec_lists[master_vec.index_unsingle] = splited_cluster_list;
    }
    //��splited_cluster_listд����ά�ռ䣬���Ϊcluster_mark��
    rewrite_cube(splited_cluster_list,cluster_mark);

    //������п����ƶ�����һ��ֻʣ�³���Ϊ1���Ŵ�����������Ŵ�������뵽
    //index_singleָ���λ�á�
    if(new_cluster_list.cluster.size()==1){
        flag_double_single++;//�����Ŵ����������1
        //��Ϊ���new_cluster_list�����г���������ʹ���µı��new_cluster_mark��
        //������ÿһ�γɹ��и�ͻ��1.���Խ�mark_change_index��++new_cluster_mark
        //��Ϊmaster_vec.index_single��
        mark_change_index[++new_cluster_mark] = master_vec.index_single;
        //index_change_mark��master_vec.index_single��Ϊnew_cluster_mark��
        index_change_mark[master_vec.index_single] = new_cluster_mark;
        //new_cluster_list���뵽master_vec.vec_lists�е�master_vec.index_single�±괦��
        master_vec.vec_lists[master_vec.index_single--] = new_cluster_list;
    }else if(new_cluster_list.cluster.size()!=1 && flag==false){
        //new_cluster_list��Ϊ1����splited_cluster_listΪ1����new_cluster_list���뵽
        //master_vec.index_unsingle�С�
        //mark_change_index��++new_cluster_mark���Ϊmaster_vec.index_unsingle
        mark_change_index[++new_cluster_mark] = master_vec.index_unsingle;
        //index_change_mark��master_vec.index_unsingle���Ϊnew_cluster_mark
        index_change_mark[master_vec.index_unsingle] = new_cluster_mark;
        //new_cluster_list���뵽 master_vec.vec_lists��master_vec.index_unsingle�±괦
        master_vec.vec_lists[master_vec.index_unsingle] = new_cluster_list;
    }else{
        //new_cluster_list��Ϊ1����splited_cluster_list��Ϊ1����new_cluster_list���뵽
        //master_vec.index_unsingle+1����
        mark_change_index[++new_cluster_mark] = ++master_vec.index_unsingle;
        //index_change_mark��master_vec.index_unsingle(�Ѽ�1)���Ϊnew_cluster_mark
        index_change_mark[master_vec.index_unsingle] = new_cluster_mark;
        //new_cluster_list���뵽 master_vec.vec_lists��master_vec.index_unsingle(�Ѽ�1)
        //�±괦
        master_vec.vec_lists[master_vec.index_unsingle] = new_cluster_list;
    }
     //��new_cluster_listд����ά�ռ䣬���Ϊnew_cluster_mark��
    rewrite_cube(new_cluster_list,new_cluster_mark);
    //�������������Ƭ����master_vec.index_unsingle��1����Ϊ�����ֱ�ӽ���index_single
    //��index_single+1��.
    if(flag_double_single==2) --master_vec.index_unsingle;
}

/**
*Summary:�������е�rewrite_cluster��ʾ���Ŵ��Ա��markд����ά�ռ�
*Parameters:
*   rewrite_cluster:��Ҫд����ά�ռ���Ŵ�����
*   mark:д����ά�ռ�ʹ�õı�ǡ�
*Return: no��
**/
void cube::rewrite_cube(cluster_list rewrite_cluster,int mark)
{
    for(auto coord : rewrite_cluster.cluster){
        Cube[coord.x][coord.y][coord.z] = mark;
    }
}

/**
*Summary:���ĳ���Ŵ��Ƿ���������ײ�ˡ�
*Parameters:
*   check_index:����ײ���Ŵص�����
*Return: ��������ײ�˷���true�����򷵻�false��
**/
bool cube::check_cycle(int check_index)
{
    for(auto exist_index:vector_collision_index){
        if(exist_index==check_index) return true;
    }
    return false;
}

/**
*Summary:������ײ
*Parameters:
*   checked_cluster_list:���и�������ƶ����Ŵأ�Ҳ�Ǳ�����Ƿ���ײ�������Ŵص��Ŵ�
*   index_splited:���и��Ŵص�����
*Return: һֱ��ײ���˱߽緵��OVERBOUNDARY���ɹ�����(û���߽�)����OK
**/
int cube::handle_collision(cluster_list checked_cluster_list,int index_splited)
{
    //���checked_cluster_list������û�г��߽�
    int result_check_boundary = check_boundary(checked_cluster_list.limit_data);
    if(result_check_boundary==OVERBOUNDARY){
        //����ײջ��գ���vector_collision_index���
        while(!stack_collision.empty()){
            int index_from_stack_top = stack_collision.top();
            stack_collision.pop();
            //check_same[index_from_stack_top] = 0;
            vector_collision_index.clear();
        }
        over_boundary_time++;//�����߽�Ĵ���(�������α�ʾ�����ƶ�������)
        return OVERBOUNDARY;
    }

    //����ײ���Ŵ���������ά�ռ��еı��
    vector<int> vector_collision_mark={};

    //���ÿһ���������������ǰ���Ƿ��������Ŵ��������򽫱���ײ���Ŵ�����ı�Ǳ���
    //��vector_collision_mark��
    for(auto checked_coord : checked_cluster_list.cluster){
        check_coord_around(checked_coord,vector_collision_mark,index_splited);
    }
    //����ɾ���ظ�Ԫ��
    sort(vector_collision_mark.begin(),vector_collision_mark.end());
    vector_collision_mark.erase(unique(vector_collision_mark.begin(),
                                       vector_collision_mark.end()),
                                    vector_collision_mark.end());
    //�������ϱ�����Ϊvector_collision_mark�е�Ԫ��Ϊ checked_cluster_list����ײ
    //��mark��Ǽ���

    //������ײ���Ŵ�������û�ж����ײ���������뵽stack_collisionջ��
    auto beg = vector_collision_mark.begin();
    auto end = vector_collision_mark.end();
    for(;beg!=end;++beg){
        if(!check_cycle(mark_change_index[*beg])){
            stack_collision.push(mark_change_index[*beg]);
        }
    }
    //ջΪ�գ���������ײ��
    if(stack_collision.empty()){
        return OK;
    }else{
        //������ײ������ջ��Ԫ�� �ݹ鴦����ײ��
        int index_from_stack_top = stack_collision.top();
        stack_collision.pop();
        //vector_collision_index�������汻�ƶ����Ŵ��±�
        vector_collision_index.push_back(index_from_stack_top);
        //ȡ������������master_vec.vec_lists��û�б���������ֹ���߽��˲����ٴ��ƶ���ȥ
        cluster_list collision_cluster_list = master_vec.vec_lists[index_from_stack_top];
        //�ƶ���������Ƿ�����ײ���������Ŵ�
        collision_cluster_list.move_cluster_list(split_direction,move_direction);
        handle_collision(collision_cluster_list,index_from_stack_top);
    }

}

/**
*Summary:����������Χ�Ƿ������������Ŵ�
*Parameters:
*   checked_coord:����������
*   vector_collision_mark:���汻��ײ���Ŵصı��
*   self_index:���������������Ǹ��Ŵص��±�
*Return: no��
**/
void cube::check_coord_around(coord_position checked_coord,vector<int>&
                              vector_collision_mark,int self_index)
{
    vector<int> vector_around_mark; //��������ǰ��
    vector_around_mark.push_back(Cube[checked_coord.x][checked_coord.y][checked_coord.z+1]);
    vector_around_mark.push_back(Cube[checked_coord.x][checked_coord.y][checked_coord.z-1]);
    vector_around_mark.push_back(Cube[checked_coord.x-1][checked_coord.y][checked_coord.z]);
    vector_around_mark.push_back(Cube[checked_coord.x+1][checked_coord.y][checked_coord.z]);
    vector_around_mark.push_back(Cube[checked_coord.x][checked_coord.y+1][checked_coord.z]);
    vector_around_mark.push_back(Cube[checked_coord.x][checked_coord.y-1][checked_coord.z]);
    int len = vector_around_mark.size();
    int self_mark = index_change_mark[self_index];
    for(int i=0;i<len;i++){
        //��Χ��Ϊ0Ҳ��Ϊ�������ڵ��Ŵ� �򽫸ı�Ǽ��뵽vector_collision_mark�С�
        if(vector_around_mark[i]!=0 && vector_around_mark[i]!=self_mark){
            vector_collision_mark.push_back(vector_around_mark[i]);
        }
    }
}

/**
*Summary:����Ƿ񳬳��߽�
*Parameters:
*   limit_data:�������Ŵصı߽�ֵ(Xmin,Xmax...)
*Return: �����߽緵��OVERBOUNDARY�����򷵻�INCUBE��
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
*Summary:���浱ǰ�и��״���������ļ�����
*Parameters: no��
*Return: no��
**/
void cube::save_result()
{
    string file_name;
    stringstream ss;
    ss << file_order; //������ת������ת����string����
    ss >> file_name;
    string file_suffix = ".txt";//�ļ���׺
    file_name += file_suffix; //�õ�txt�ļ���
    ofstream ofst;
    ofst.open(file_name);//�������򴴽��ļ�
    int index_last = num_of_node;
    //д������xyz�Լ��Ŵ�����ά�ռ��еı��
    //д��ǵ��Ŵؼ����
    for(int i=master_vec.index_single+1;i<index_last;++i){
        for(auto coord:master_vec.vec_lists[i].cluster){
            ofst<<coord.x<< " "<<coord.y<< " "<<coord.z<< " "<<index_change_mark[i]<<endl;
        }
    }
    //д�뵥�Ŵؼ����
    for(int i=0;i<=master_vec.index_unsingle;++i){
        for(auto coord:master_vec.vec_lists[i].cluster){
            ofst<<coord.x<<" "<<coord.y<<" "<<coord.z<<" "<<index_change_mark[i]<<endl;
        }
    }
    file_order++; //�ļ�����1
}

#endif // CUBE2_H_INCLUDED
