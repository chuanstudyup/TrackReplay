#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


#include <QMainWindow>
#include <QFile>
#include <QList>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QUrl>
#include <QWebEnginePage>
#include <QWebChannel>
#include <QTimer>
#include <QFileDialog>
#include <QTime>
#include "mapchannel.h"
#include "help.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool readfromfile();

signals:
    void update(int);

private slots:
    void updateTrack();
    void reloadMap();

private slots:

    void on_pushButton_open_file_clicked();

    void on_pushButton_start_clicked();

    void on_comboBox_times_vel_currentIndexChanged(int index);

    void on_spinBox_time_col_valueChanged(int arg1);

    void on_spinBox_lng_col_valueChanged(int arg1);

    void on_spinBox_lat_col_valueChanged(int arg1);

    void on_spinBox_yaw_col_valueChanged(int arg1);

    void on_pushButton_open_desired_track_file_clicked();

    void on_pushButton_help_clicked();

private:
    Ui::MainWindow *ui;


    void mapWidgetInit();
    QWebChannel *channel;
    MapChannel *mapChannel;

    QList<QString> lines;
    QTimer* timer0;
    int LineIndex;
    int LineLength;
    int timeCol;
    int lngCol;
    int latCol;
    int yawCol;

    double TimersVel;

    Help *help;
};

#endif // MAINWINDOW_H
