﻿#ifndef AGVTASK_H
#define AGVTASK_H

#include <QObject>
#include <QDateTime>

enum{
    AGV_TASK_STATUS_UNEXIST = -3,//不存在
    AGV_TASK_STATUS_UNEXCUTE=-2,//未执行
    AGV_TASK_STATUS_EXCUTING=-1,//正在执行
    AGV_TASK_STATUS_DONE=0,//完成
    AGV_TASK_STATUS_FAIL=1,//失败
    AGV_TASK_STATSU_CANCEL=2//取消
};

enum{
    AGV_TASK_WAIT_TYPE_TIME = 0,    // 达到后等待时间
    AGV_TASK_WAIT_TYPE_SIGNAL = 1,  //到达后等待信号
    AGV_TASK_WAIT_TYPE_NOWAIT = 2   //到达就可以了，就是不等待
};

enum{
    AGV_TASK_NODE_STATUS_UNDO = 0,
    AGV_TASK_NODE_STATUS_DOING = 1,
    AGV_TASK_NODE_STATUS_DONE = 2,
};

struct TaskNode{
    int id = 0;
    int status = 0;  //0未执行  1正在执行  2执行完成了
    int queueNumber = 0; //在这个任务中的序列号
    int aimStation = 0;//要去的位置()如果是0，说明没有节点任务
    int waitType = AGV_TASK_WAIT_TYPE_TIME;//到达该位置后的等待方式
    int waitTime = 30;//到达该位置后的等待时间(秒)
    QDateTime arriveTime;//到达时间
    QDateTime leaveTime;//离开时间
};

class AgvTask : public QObject
{
    Q_OBJECT
public:
    explicit AgvTask(QObject *parent = nullptr);

    //getter
    int id(){return m_id;}
    QDateTime produceTime(){return m_produceTime;}
    QDateTime doneTime(){return m_doneTime;}
    QDateTime doTime(){return m_doTime;}
    int excuteCar(){return m_excuteCar;}
    int status(){return m_status;}

    //setter
    void setId( int newId){m_id=newId;emit idChanged(newId);}
    void setProduceTime( QDateTime newProduceTime){m_produceTime=newProduceTime;emit produceTimeChanged(newProduceTime);}
    void setDoneTime( QDateTime newDoneTime){m_doneTime=newDoneTime;emit doneTimeChanged(newDoneTime);}
    void setDoTime( QDateTime newDoTime){m_doTime=newDoTime;emit doTimeChanged(newDoTime);}
    void setExcuteCar( int newExcuteCar){m_excuteCar=newExcuteCar;emit excuteCarChanged(newExcuteCar);}
    void setStatus(int newStatus){m_status=newStatus;emit statusChanged(newStatus);}
signals:
    //change signal
    void idChanged(int newid);
    void produceTimeChanged(QDateTime newproduceTime);
    void doneTimeChanged(QDateTime newdoneTime);
    void doTimeChanged(QDateTime newdoTime);
    void excuteCarChanged(int newexcuteCar);
    void statusChanged(int newstatus);

public slots:

private:
    int m_id;
    QDateTime m_produceTime;
    QDateTime m_doneTime;
    QDateTime m_doTime;
    int m_excuteCar;
    int m_status;

public:
    //这两个是可以公有访问的.不过一般仅限于task中访问
    QVector<TaskNode *> taskNodes;


    bool isDone();

    TaskNode *nextTodoNode();

    TaskNode *lastDoneNode();

    TaskNode *currentDoingNode();
    //OK 完成了

    //为了给node进行排序，
    int nodeAmount;
};

#endif // AGVTASK_H
