#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonParseError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    mapWidgetInit();

    timer0 = new QTimer(this);
    connect(this->timer0,&QTimer::timeout,this,&MainWindow::updateTrack);
    this -> ui -> pushButton_start -> setEnabled(false);
    this -> ui -> comboBox_times_vel -> setEnabled(false);

    help = new Help();
}

MainWindow::~MainWindow()
{
    delete help;
    channel -> deregisterObject(mapChannel);
    delete ui;
}

void MainWindow::mapWidgetInit()
{
    channel = new QWebChannel(this);
    mapChannel = new MapChannel(this);
    channel -> registerObject("passId",mapChannel);
    this -> ui -> widget_map -> page() -> setWebChannel(channel);
    this -> ui -> widget_map -> load(QUrl("file:///./onlinemap/map1.html"));
    connect(mapChannel,&MapChannel::reloadMapClicked,this,&MainWindow::reloadMap);

    connect(mapChannel,&MapChannel::pointsCome,[](int index, double lng, double lat){
        qDebug()<<index<<QString::number(lng,'f',6)<<QString::number(lat,'f',6);
    });
}

bool MainWindow::readfromfile()
{

    QString path = QFileDialog::getOpenFileName(this,"Open","./","TXT(*.txt *.csv)");
    if(path.isEmpty() == false)
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return false;
        }

        lines.clear();
        LineIndex = 0;
        this -> mapChannel -> clearTrack();

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            lines.append(line);
        }
        file.close();
        LineLength = lines.length();

        QString line = lines.at(0);
        QStringList subs = line.split(",");
        this -> ui -> label_time_value -> setText(subs.at(timeCol));
        this -> ui -> label_lng_value -> setText(subs.at(lngCol));
        this -> ui -> label_lat_value -> setText(subs.at(latCol));
        this -> ui -> label_yaw_value -> setText(subs.at(yawCol));
        mapChannel -> panTo(subs.at(lngCol).toDouble(),subs.at(latCol).toDouble());

        return true;
    }
    else {
        return false;
    }
}


void MainWindow::updateTrack()
{
    if(LineIndex < LineLength)
    {
        QString line = lines.at(LineIndex);
        line  = line.trimmed();
        QStringList subs = line.split(",");

        double lng = subs.at(lngCol).toDouble();
        double lat = subs.at(latCol).toDouble();
        double yaw = subs.at(yawCol).toDouble();
        mapChannel->updateBoatPos(lng,lat,yaw);
        this -> ui -> label_time_value -> setText(subs.at(timeCol));
        this -> ui -> label_lng_value -> setText(subs.at(lngCol));
        this -> ui -> label_lat_value -> setText(subs.at(latCol));
        this -> ui -> label_yaw_value -> setText(subs.at(yawCol));

        LineIndex++;
    }
    else
    {
        this -> timer0 -> stop();
        this -> ui -> pushButton_start -> setText("开始回放");
        this -> ui -> pushButton_start -> setEnabled(false);
        this -> ui -> comboBox_times_vel -> setEnabled(false);
    }
}

void MainWindow::on_pushButton_open_file_clicked()
{
    timeCol = this -> ui -> spinBox_time_col -> value();
    lngCol = this -> ui -> spinBox_lng_col -> value();
    latCol = this -> ui -> spinBox_lat_col -> value();
    yawCol = this -> ui -> spinBox_yaw_col -> value();

    if(readfromfile())
    {
        QMessageBox::information(this,"成功","文件打开成功,请检查实时数据是否正确!");
        this -> ui -> pushButton_start -> setEnabled(true);
        this -> ui -> comboBox_times_vel -> setEnabled(true);
    }
    else {
        //this->close();
        QMessageBox::critical(this,"错误","文件打开失败，请重试!");
    }
}

void MainWindow::on_pushButton_start_clicked()  //开始
{
    if(this -> ui -> pushButton_start -> text() == "开始回放")
    {
        switch(this -> ui -> comboBox_times_vel  -> currentIndex())
        {
        case 0:
            this -> TimersVel = 1;
            break;
        case 1:
            this -> TimersVel = 1.25;
            break;
        case 2:
            this -> TimersVel = 1.5;
            break;
        case 3:
            this -> TimersVel = 2;
            break;
        case 4:
            this -> TimersVel = 3;
            break;
        case 5:
            this -> TimersVel = 5;
            break;
        case 6:
            this -> TimersVel = 10;
            break;
        case 7:
            this -> TimersVel = 15;
            break;
        default:
            break;
        }
        this -> timer0 -> start(int(1000/TimersVel));
        this -> ui -> pushButton_start -> setText("停止回放");
    }
    else {
        this -> timer0 -> stop();
        this -> ui -> pushButton_start -> setText("开始回放");
    }
}

void MainWindow::on_comboBox_times_vel_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        this -> TimersVel = 1;
        break;
    case 1:
        this -> TimersVel = 1.25;
        break;
    case 2:
        this -> TimersVel = 1.5;
        break;
    case 3:
        this -> TimersVel = 2;
        break;
    case 4:
        this -> TimersVel = 3;
        break;
    case 5:
        this -> TimersVel = 5;
        break;
    case 6:
        this -> TimersVel = 10;
        break;
    case 7:
        this -> TimersVel = 15;
        break;
    default:
        break;
    }
    if(this -> ui -> pushButton_start -> text() == "停止回放")
        this -> timer0 ->start(int(1000/TimersVel));
}

void MainWindow::on_spinBox_time_col_valueChanged(int arg1)
{
    timeCol = arg1;
    QString line = lines.at(0);
    QStringList subs = line.split(",");
    this -> ui -> label_time_value -> setText(subs.at(timeCol));
}

void MainWindow::on_spinBox_lng_col_valueChanged(int arg1)
{
    lngCol = arg1;
    QString line = lines.at(0);
    QStringList subs = line.split(",");
    this -> ui -> label_lng_value -> setText(subs.at(lngCol));
    mapChannel -> panTo(subs.at(lngCol).toDouble(),subs.at(latCol).toDouble());
}

void MainWindow::on_spinBox_lat_col_valueChanged(int arg1)
{
    latCol = arg1;
    QString line = lines.at(0);
    QStringList subs = line.split(",");
    this -> ui -> label_lat_value -> setText(subs.at(latCol));
    mapChannel -> panTo(subs.at(lngCol).toDouble(),subs.at(latCol).toDouble());
}

void MainWindow::on_spinBox_yaw_col_valueChanged(int arg1)
{
    yawCol = arg1;
    QString line = lines.at(0);
    QStringList subs = line.split(",");
    this -> ui -> label_yaw_value -> setText(subs.at(yawCol));
}

void MainWindow::reloadMap()
{
    this -> ui -> widget_map -> load(QUrl("file:///./onlinemap/map.html"));
}

void MainWindow::on_pushButton_open_desired_track_file_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Open Json Task",QDir::currentPath()+"/task","JSON File(*.json)");
    QFile jsonFile(path);
    if(!jsonFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this,"提示","Json文件打开失败，请重试");
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonFile.readAll(),&jsonError));
    jsonFile.close();
    if(jsonError.error != QJsonParseError::NoError)
    {
        QMessageBox::information(this,"提示",QString("Json解析失败，错误代码：%1，%2").arg(jsonError.error).arg(jsonError.errorString()));
        return;
    }

    QJsonObject rootObj = jsonDoc.object();


    mapChannel->clearWaypoints();
    int pointsNum=0;
    if(rootObj.contains("pointsNum"))
    {
        QJsonValue pointsNumValue = rootObj.value("pointsNum");
        if(pointsNumValue.isDouble())
            pointsNum = pointsNumValue.toInt();
        else{
            QMessageBox::information(this,"提示","Json中pointsNum类型错误！");
            return;
        }
    }else{
        QMessageBox::information(this,"提示","Json中无pointsNum！");
        return;
    }
    if(rootObj.contains("waypoints"))
    {
        QVector<double> lng(pointsNum);
        QVector<double> lat(pointsNum);
        QJsonValue waypointsValue = rootObj.value("waypoints");
        if(waypointsValue.isObject())
        {
            QJsonObject waypointsObject = waypointsValue.toObject();
            for (int i=0; i<pointsNum; i++)
            {
                QString waypointIndex = QString::number(i);
                if(waypointsObject.contains(waypointIndex))
                {
                    QJsonObject waypointObject = waypointsObject.value(waypointIndex).toObject();
                    lng[i] = waypointObject.value("lng").toDouble();
                    lat[i] =  waypointObject.value("lat").toDouble();
                    qDebug()<<waypointIndex<< lng[i]<< lat[i];
                }else{
                    QMessageBox::information(this,"提示","Json中pointsNum和waypoints内的航点个数不同！");
                    return;
                }
            }
            for(int i=0; i<pointsNum; i++)
                mapChannel->addPoint(lng[i],lat[i]);
        }else{
            QMessageBox::information(this,"提示","Json中waypoints不是JsonObject！");
            return;
        }
    }else{
        QMessageBox::information(this,"提示","Json中无waypoints！");
        return;
    }
}
void MainWindow::on_pushButton_help_clicked()
{
    help->show();
}
