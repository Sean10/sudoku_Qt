/*
 *  @file   sudoku.h
 *  @brief  摘要
 *  Copyright (c) 2017
 */
#ifndef SUDOKU_H
#define SUDOKU_H

#include <QVector>
#include <QStack>
#include <QMap>
#include <QDebug>


class Sudoku
{
public:
    Sudoku();
    ~Sudoku();
    void GetNum(QVector<int> tmp_num);
    bool Calculate();
    QVector<int> GetSudokuValue();

private:
    QVector <int> num_; //Sudoku 9*9 81 num
    QVector <int> num_label_; //ten binary situation
    QVector <int> num_available_;
    QVector <int> num_candidate_;
    QStack <int> stack_num_;
    QStack <int> stack_pos_;
    QStack <int> stack_candidate_;
    QMap <int, int> map_label_to_num_;
    int state_solution_; // 0:no solution,1:solved, -1: haven't been solved and maybe can be solved
    int num_remainder_; // 0 shows that has been solved.

    void InitNum();
    bool RemovePossibleNum(int pos, int removed_num);
    bool SetNum(int pos, int ensured_num);
    void LastValue(int pos, int ensured_num);
    int FindMinCell();
    int GetOneCount(int tmp_num);
    bool ClearWithLastValue();
    void GetAllChoice(int pos);
    int GetNextChoice();
};

#endif // SUDOKU_H
