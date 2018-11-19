#include "uisnakemove.h"
#include "ui_uisnakemove.h"

#define Increasement_a 5
#define Increasement_b 5
#define Increasement_g 5
#define Increasement_w 5
#define Increasement_period 5

UiSnakeMove::UiSnakeMove(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::UiSnakeMove)
{
    ui->setupUi(this);
    flag_connected = 0;//默认舵机未连接
    /*点击start和四个控制键都会使参数显示刷新*/
    connect(this,SIGNAL(move_get_started()),
            this,SLOT(parament_show()));
    connect(this,SIGNAL(parament_changed()),
            this,SLOT(parament_show()));
}

UiSnakeMove::~UiSnakeMove()
{
    delete ui;
}

void UiSnakeMove::on_pushButton_clicked()
{
    int ret;//初始化返回值
    if(mysnake.read_quanity() == 0)
    {
        flag_connected = 0;//连接失败，舵机连接标志为0
        ui->line_showmessage->setText("The number of motors is 0!!");
    }
    else
    {
        ret = mydev.AX_UART_Init();
        if(ret == 1)
        {
            flag_connected = 1;//连接成功
            ui->line_showmessage->setText("OK, let's enjoy it.");
        }
        else
        {
            flag_connected = 0;//连接失败，舵机连接标志为0
            ui->line_showmessage->setText("failed to open.\n");
        }
    }
}

void UiSnakeMove::on_Button_quanityOK_clicked()
{
    mysnake.change_quanity( ui->line_quanity->text().toInt() );
    ui->line_showmessage->setText("舵机数目是："
                                  + ui->line_quanity->text()
                                  + "."
                                  );
}

void UiSnakeMove::on_Button_start_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("请确定您已连接舵机");
    }
    else
    {
        mysnake.mydev_forSnake = &mydev;
        mysnake.moveThread();
        ui->Button_quit->setDisabled(true);
        emit move_get_started();
    }
}

void UiSnakeMove::parament_show()
{
    QString para;
    para = QString::number(mysnake.read_a(),10);
    ui->line_a->setText(para);

    para = QString::number(mysnake.read_b(),10);
    ui->line_b->setText(para);

    para = QString::number(mysnake.read_g(),10);
    ui->line_g->setText(para);

    para = QString::number(mysnake.read_w(),10);
    ui->line_w->setText(para);

    para = QString::number(mysnake.read_p(),10);
    ui->line_period->setText(para);
}

void UiSnakeMove::on_Button_stop_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("小蛇没有在运动.");
    }
    else
    {
        mysnake.stopMoving_signal = 1;//停止运动标志，该标志置1使运动刷新线程终结
        ui->Button_quit->setEnabled(true);
    }
}

void UiSnakeMove::on_Button_fast_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_w( Increasement_w + mysnake.read_w() );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_slow_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_w( mysnake.read_w() - Increasement_w );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_left_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_g(mysnake.read_g() - Increasement_g );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_right_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_g(mysnake.read_g() + Increasement_g );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_a_up_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_a(mysnake.read_a() + Increasement_a );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_a_down_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_a(mysnake.read_a() - Increasement_a );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_b_up_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_b(mysnake.read_b() + Increasement_b );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_b_down_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_b(mysnake.read_b() - Increasement_b );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_period_up_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_p(mysnake.read_p() + Increasement_period );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_period_down_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("没有连接舵机.");
    }
    else
    {
        mysnake.change_p(mysnake.read_p() - Increasement_period );
        emit parament_changed();
    }
}
