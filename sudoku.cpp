/*
 *  @file   sudoku.cpp
 *  @brief  解数独模块
 *  Copyright (c) 2017
 */
#include "sudoku.h"

Sudoku::Sudoku():num_candidate_(0)
{
    InitNum();
}

Sudoku::~Sudoku()
{
    ;
}

/**
 *  @brief 计算主函数，有限状态自动机
 *  @param 无
 *  @retval 有解1 or 无解0
 */
bool Sudoku::Calculate()
{
    ClearWithLastValue();

    int min_pos = -1;
    int value = 0;// 0 表示所有格无候选数

    while(state_solution_ != 1)
    {
        qDebug() << "Position" << min_pos << endl;
        if(state_solution_ == 0)
        {
            if(value == 0 || stack_pos_.isEmpty())
            {
                state_solution_ = 0;
                return false;
            }


            min_pos = stack_pos_.last();

            if(num_candidate_[min_pos] <= 0)
            {
                stack_pos_.pop();
                num_[min_pos] = stack_num_.pop();
                min_pos = stack_pos_.last();
            }


            //num_[min_pos] = stack_num_.pop();
            //num_remainder_++;

            value = GetNextChoice();

            num_[min_pos] = value;

            ClearWithLastValue();

        }
        else if(state_solution_ == -1)
        {
            min_pos = FindMinCell();
            qDebug() << "Minimum Cell Position" << min_pos << endl;
            GetAllChoice(min_pos);
            value = GetNextChoice();

            if(value == 0)
            {
                state_solution_ = 1;
                return true;
            }

            stack_pos_.push(min_pos);
            stack_num_.push(num_[min_pos]);
            num_[min_pos] = value;

            ClearWithLastValue();
        }
    }

}

/**
 *  @brief 获取下一个候选数
 *  @param 无
 *  @retval 返回下一个候选数值
 */
int Sudoku::GetNextChoice()
{
    if(stack_candidate_.isEmpty())
    {
        return 0;
    }

    return stack_candidate_.pop();
}

/**
 *  @brief 获取该单元格所有候选数
 *  @param 单元格坐标
 *  @retval 无
 */
void Sudoku::GetAllChoice(int pos)
{
    for(int i = 0; i < 9;i++)
    {
        if((num_[i] & num_[pos]) == num_[i])
        {
            stack_candidate_.push(map_label_to_num_.find(num_[i]).value());
        }
    }
}


/**
 *  @brief 寻找最少候选数的单元格
 *  @param 无
 *  @retval 单元格坐标
 */
int Sudoku::FindMinCell()
{
    int min_count = GetOneCount(num_[0]);
    int min_pos = 0;
    int tmp_count;
    for(int i = 1;i < 81;i++)
    {
        if(num_[i] < 0)
        {
            num_candidate_[i] = 0;
            continue;

        }
        //qDebug() << "tmp_count_num:" << num_[i] << endl;
        tmp_count = GetOneCount(num_[i]);

        qDebug() << "num_pos:" << i << "count:" << tmp_count << endl;

        num_candidate_[i] = tmp_count;

        if(tmp_count < min_count)
        {
            min_pos = i;
            min_count = tmp_count;
        }

        if(min_count == 2)
        {
            return min_pos;
        }
    }

    return min_pos;
}

bool Sudoku::ClearWithLastValue()
{
    int flag_last_value = 0;
    for(int i = 0; i < 81;i++)
    {
        if(GetOneCount(num_[i]) == 1)
        {
            SetNum(i, map_label_to_num_[num_[i]]);
            flag_last_value = 1;
        }
    }

    if(flag_last_value == 1)
        return false;
    else
        return true;
}

/**
 *  @brief 从外部获取数独
 *  @param tmp_num整个数组
 *  @retval 无
 */
void Sudoku::GetNum(QVector<int> tmp_num)
{
    for(int i = 0;i < 81;i ++)
    {
        if(tmp_num[i] > 0 && tmp_num[i] < 10)
        {
            SetNum(i, tmp_num[i]);

        }
        //has been inited.
//        else
//            num_[i] = num_label_[9];
    }
}

/**
 *  @brief 初始化数组为二进制候选数数组,并初始化二进制候选列-数映射表
 *  @param 无
 *  @retval 无
 */
void Sudoku::InitNum()
{
    //num_remainder_ = 81;

    num_label_.resize(10);
    num_.resize(81);
    num_candidate_.resize(81);
    num_available_.resize(81);

    state_solution_ = -1;


    num_label_[0] = 1;
    for(int i = 1;i < 9; i++)
    {
        num_label_[i] = 2 * num_label_[i-1];
    }
    num_label_[9] = 511;

    for(int i = 0; i < 81; i++)
    {
        num_[i] = num_label_[9];
    }

    for(int i = 0;i < 9; i++)
    {
        map_label_to_num_[num_label_[i]] = i+1;
    }
}

/**
 *  @brief 删除可选数
 *  @param 坐标pos， 移除数removed_num
 *  @retval bool
 */
bool Sudoku::RemovePossibleNum(int pos, int removed_num)
{
    //Remove the table with the removed_num.
    if(num_[pos] > 0)
    {
        num_[pos] = num_[pos] & ~num_label_[removed_num];
        return true;
    }
    return false;
}

/**
 *  @brief 设置唯一数,并筛选
 *  @param 坐标pos, 唯一确定数ensured_num
 *  @retval bool，返回true表示已确定唯一数，返回false表示仍有多个候选数
 */
bool Sudoku::SetNum(int pos, int ensured_num)
{
    if(GetOneCount(num_[pos]) > 1)
    {
        return false;
    }

    num_[pos] = -ensured_num;
    //num_remainder_--;

    LastValue(pos, ensured_num);

    return true;
}

/**
 *  @brief 唯一余数法
 *  @param row, col, ensured_num
 *  @retval 无
 */
void Sudoku::LastValue(int pos, int ensured_num)
{
    int row = pos/9;
    int col = pos%9;

    for(int i = 0; i < 9; i++)
    {
        RemovePossibleNum(row * 9 + col + i, ensured_num);
        RemovePossibleNum((row + i) * 9 + col, ensured_num);
    }

    for(int i = row/3*3;i < 3; i++)
    {
        for(int j = col/3*3;j < 3; j++)
        {
            RemovePossibleNum(i*9+j, ensured_num);
        }
    }
}

/**
 *  @brief 获得候选数数量,当唯一确定时，候选数数量为-1，代表已填充，当候选数量为0时表示填充有问题，无可行解
 *  @param tmp_num，候选数的二进制列表
 *  @retval 候选数数量
 */
int Sudoku::GetOneCount(int tmp_num)
{
    if(tmp_num < 0)
        return -1;

    int count = 0;
    while(tmp_num)
    {
        qDebug() << tmp_num <<endl;
        tmp_num &= tmp_num-1;
        qDebug() << "answer:" << tmp_num << endl;
        count++;
    }

    return count;
}

QVector<int> Sudoku::GetSudokuValue()
{
    return this->num_;
}
