#ifndef RANDOM2_H_INCLUDED
#define RANDOM2_H_INCLUDED


/**
*Summary:������[lft_data,rht_data]��ȡһ���������
*Parameters:
*    lft_data�����������Сֵ��
*    rht_data������������ֵ��
*Return: no��
**/
int _get_random(size_t lft_data,size_t rht_data)
{
    int tmp;
    random_pointer %= 1000;   //ָ����������ǰ��
    tmp = random_vector[random_pointer++];       //ȡ��һ�������
    //�õ�[lft_data,rht_data]֮���һ�������
    tmp = tmp%((rht_data-lft_data)+1)+lft_data;
    return tmp;
}

/**
*Summary:����һ����������飬������ɳ���Ϊrandom_vector_len���������
*Parameters:
*    min_val�����������Сֵ��
*    max_val������������ֵ��
*Return: no��
**/
void creat_random_vector(int min_val,int max_val)
{
    default_random_engine e(random_seed);  //���������
    uniform_int_distribution<int> u(min_val,max_val); //���ȷֲ�
    for(int i=0;i<random_vector_len;++i){
        random_vector.push_back(u(e));  //����random_vector_len�������
    }
}

/**
*Summary:��һ����������(����limit_data����)�������и�λ�ã�������
    �и���п��ܻᱻ�ı䡣
*Parameters:
*    split_dire���и��
*    limit_data��Ҫ���и���Ŵصı߽�ֵ��
*Return: ����һ�����ʵ��и�λ�á�
**/
int _get_split_position(int &split_dire,vector<int> limit_data)
{
    int split_count = 3;    //���� ���β��о��˳���
    //��Ϊ��һ��,�ڶ��ζ��п����и�ɹ�����������������ǰ�����и��
    //���ڱ����һ���и��͵ڶ����и��
    int tmp_split_dire_first,tmp_split_dire_second;
    tmp_split_dire_first = split_dire;  //�����һ���и��
    while(split_count){
        //����÷������и�break��switch��䣬��switch���޸ķ���
        //����÷�������и�ڸ����ķ�Χ(�Ŵ�xyz�����Сֵ)������һ������и�λ�á�
        switch(split_dire){
            case 0:   //��ֱZ���и�,����X�����и
                if(limit_data[1]-limit_data[0]==0 ) break;//�÷������и�
                else return _get_random(limit_data[0]+1,limit_data[1]);
            case 1:   //��ֱY���и�,����Z�����и
                if(limit_data[5]-limit_data[4]==0) break; //�÷������и�
                else return _get_random(limit_data[4]+1,limit_data[5]);
            case 2:   //��ֱX���и�,����Y�����и
                if(limit_data[3]-limit_data[2]==0) break; //�÷������и�
                else return _get_random(limit_data[2]+1,limit_data[3]);
        }
        //split_count==3��ʾ��һ���и�ɹ�,������и�������������
        if(split_count==3){
            int choose_other_direction = _get_random(0,1);
            //�õ����и�����и���50%����ѡ����������������Ϊsplit_direֻ
            //����0,1,2���Զ�ÿһ��ֵ��˵��1���2��ȡ��3�Ϳ���ת��������������
            //����ʼΪ0,(0+1)%3=1,(0+2)%3=2.ԭ�����и��ʹӡ���ʼΪ0,(0+1)%3=1
            //��X�����и�ת����Z�����и��Y�����и��ʼΪ1,(1+1)%3=2,(1+2)%3=0.
            //ͬ�����������Ϳ���תΪ��������������ʼΪ2,(2+1)%3=0,(2+2)%3=1.ͬ��
            split_dire = (choose_other_direction==0)? (split_dire+1)%3 : (split_dire+2)%3;
            tmp_split_dire_second = split_dire; //����ڶ����и��
        }
        if(split_count==2){
            //�������򶼲��� ѡ�����������
            //0+1==1���Ե�����Ϊ2��
            if(tmp_split_dire_first+tmp_split_dire_second==1) split_dire = 2;
            //1+2==3���Ե�����Ϊ0��
            else if(tmp_split_dire_first+tmp_split_dire_second==3)split_dire = 0;
            else split_dire = 1;  //�����������1�ˡ�
        }
        --split_count;
    }
}

/**
*Summary:����һ��Ҫ�и���Ŵص�������
*Parameters:
*    master_vec��ȫ�����Ŵض������档
*Return: ����һ�����ʵ��и�λ�á�
**/
int _get_splited_index(main_vector & master_vec)
{
    int index_last = master_vec.index_unsingle; //���и��Ŵص�ĩβ������
    int index_splited= _get_random(0,index_last); //���ɿ����и�������
    //cluster_markΪȫ�ֱ�����Ϊ�ñ��и��Ŵ�����ά�����еı�š�
    cluster_mark = index_change_mark[index_splited];

    //ȡ���ñ��и��Ŵ� �� �Ŵ�������Ҫĩβ��������
    //��Ϊ�п��ܱ��и���Ŵ�ֻ��������㣬�����г����������ǵ�������Ŵأ�
    //���ᱻ�ӵ�master_vec.vec_lists�ĺ��棬���Ա��и��λ�þͻ�ճ����㷨
    //�Ͳ��ܼ���ִ�У����Ա���ý�����
    cluster_list temp_cluster_list = master_vec.vec_lists[index_splited];
    master_vec.vec_lists[index_splited] = master_vec.vec_lists[index_last];
    master_vec.vec_lists[index_last] = temp_cluster_list;

    //ά����ת����ҲҪ������
    int tmp = index_change_mark[index_splited];
    index_change_mark[index_splited] = index_change_mark[index_last];
    index_change_mark[index_last] = tmp;

    tmp = mark_change_index[index_change_mark[index_splited]];
    mark_change_index[index_change_mark[index_splited]] =
     mark_change_index[index_change_mark[index_last]];

    mark_change_index[index_change_mark[index_last]] = tmp;
    //��Ϊ���ɵ�������и��Ŵ������Ѿ� ���Ȳ�Ϊ1��ĩβ���Ŵ��������ˡ�
    //���Է����и������Ϊ���и��Ŵص�ĩβ���������Ǳ�������и�ģ�ֻ��
    //����Ҫ�и���Ŵ�����ŵ��˿��и��Ŵ�����������ѡ�
    return index_last;
}


#endif // RANDOM2_H_INCLUDED
