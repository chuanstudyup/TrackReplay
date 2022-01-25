#ifndef MAPCHANNEL_H
#define MAPCHANNEL_H

#include <QObject>

class MapChannel : public QObject
{
    Q_OBJECT
public:
    explicit MapChannel(QObject *parent = nullptr);

    /*MainWindow 调用*/
    void addPoint(double lng, double lat);
    void movePoint(int id, double lng, double lat);
    void updateBoatPos(double lng, double lat, double course);
    void setOrigin(double lng, double lat);
    void addFencePoint(double,double);
    void addFence();
    void clearFence();
    void clearWaypoints();
    void clearTrack();
    void panTo(double,double);

signals:

    /*MapChannel -> MainWindow*/
    void mousePointChanged(double, double);
    void reloadMapClicked();
    void taskCome(int,int);
    void pointsCome(int,double,double);

    /*MapChannel -> html*/
    void addPointClicked(double,double);
    void movePointClicked(int,double,double);
    void setOriginPoint(double,double);
    void boatPosUpdated(double,double,double);
    void addFencePointClicked(double,double);
    void addFenceClicked();
    void clearFenceClicked();
    void clearWaypointsClicked();
    void clearAllClicked();
    void clearTrackClicked();
    void panToClicked(double,double);

public slots:
    /*html调用*/
    void getMousePoint(double lng, double lat);
    void reloadMap();
    void transTask(int type, int len);
    void transPoints(int id, double lng, double lat);

    /*MainWindow 调用*/
    void clearAll();
};

#endif // MAPCHANNEL_H
