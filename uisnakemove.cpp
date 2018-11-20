#include "uisnakemove.h"
#include "ui_uisnakemove.h"

#define Increasement_a 5
#define Increasement_b 5
#define Increasement_g 5
#define Increasement_w 5
#define Increasement_period 5

#define PERIOD_MAXIMUM  60
#define PERIOD_MINIMUM  20
#define A_MAXIMUM       60
#define A_MINIMUM       20
#define B_MAXIMUM       60
#define B_MINIMUM       20
#define GAMA_RIGHT      30
#define GAMA_LEFT       -30

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
    ui->line_showmessage->setText("The number of motors is :"
                                  + ui->line_quanity->text()
                                  + "."
                                  );
}

void UiSnakeMove::on_Button_start_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("Please make sure that it is connected.");
    }
    else
    {
        mysnake.mydev_forSnake = &mydev;
        mysnake.moveThread();
        ui->Button_quit->setDisabled(true);
        emit move_get_started();
    }
}

void UiSnakeMove::on_Button_stop_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It isn't moving.");
    }
    else
    {
        mysnake.stopMoving_signal = 1;//停止运动标志，该标志置1使运动刷新线程终结
        ui->Button_quit->setEnabled(true);
    }
}

void UiSnakeMove::on_Button_a_up_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        ui->Button_a_down->setEnabled(true);
        mysnake.change_a(mysnake.read_a() + Increasement_a );
        emit parament_changed();
        if(mysnake.read_a() >= A_MAXIMUM)
        {
             ui->Button_a_up->setDisabled(true);
        }
        else
        {
            ui->Button_a_up->setEnabled(true);
        }
    }
}

void UiSnakeMove::on_Button_a_down_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        ui->Button_a_up->setEnabled(true);
        mysnake.change_a(mysnake.read_a() - Increasement_a );
        emit parament_changed();
        if(mysnake.read_a() <= A_MINIMUM)
        {
             ui->Button_a_down->setDisabled(true);
        }
        else
        {
            ui->Button_a_down->setEnabled(true);
        }
    }
}

void UiSnakeMove::on_Button_b_up_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        ui->Button_b_down->setEnabled(true);
        mysnake.change_b(mysnake.read_b() + Increasement_b );
        emit parament_changed();
        if(mysnake.read_b() >= B_MAXIMUM)
        {
             ui->Button_b_up->setDisabled(true);
        }
        else
        {
            ui->Button_b_up->setEnabled(true);
        }
    }
}

void UiSnakeMove::on_Button_b_down_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        ui->Button_b_up->setEnabled(true);
        mysnake.change_b(mysnake.read_b() - Increasement_b );
        emit parament_changed();
        if(mysnake.read_b() <= B_MINIMUM)
        {
             ui->Button_b_down->setDisabled(true);
        }
        else
        {
            ui->Button_b_down->setEnabled(true);
        }
    }
}

void UiSnakeMove::on_Button_period_up_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        ui->Button_period_down->setEnabled(true);
        mysnake.change_p(mysnake.read_p() + Increasement_period );
        emit parament_changed();
        if(mysnake.read_p() > PERIOD_MAXIMUM)
        {
             ui->Button_period_up->setDisabled(true);
        }
        else
        {
            ui->Button_period_up->setEnabled(true);
        }
    }
}

void UiSnakeMove::on_Button_period_down_clicked()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        ui->Button_period_up->setEnabled(true);
        mysnake.change_p(mysnake.read_p() - Increasement_period );
        emit parament_changed();
        if(mysnake.read_p() < PERIOD_MINIMUM)
        {
             ui->Button_period_down->setDisabled(true);
        }
        else
        {
            ui->Button_period_down->setEnabled(true);
        }
    }
}

void UiSnakeMove::on_Button_right_pressed()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        mysnake.change_g( GAMA_RIGHT );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_right_released()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        mysnake.change_g( 0 );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_left_pressed()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        mysnake.change_g( GAMA_LEFT );
        emit parament_changed();
    }
}

void UiSnakeMove::on_Button_left_released()
{
    if(flag_connected == 0)
    {
        ui->line_showmessage->setText("It's disconnected.");
    }
    else
    {
        mysnake.change_g( 0 );
        emit parament_changed();
    }
}
