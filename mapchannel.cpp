#include "mapchannel.h"

MapChannel::MapChannel(QObject *parent) : QObject(parent)
{

}

void MapChannel::getMousePoint(double lng, double lat)
{
    emit mousePointChanged(lng,lat);
}

void MapChannel::addPoint(double lng, double lat)
{
    emit addPointClicked(lng,lat);
}

void MapChannel::movePoint(int id, double lng, double lat)
{
    emit movePointClicked(id,lng,lat);
}

void MapChannel::transTask(int type, int len)
{
    emit taskCome(type,len);
}

void MapChannel::transPoints(int id, double lng, double lat)
{
    emit pointsCome(id,lng,lat);
}

void MapChannel::updateBoatPos(double lng, double lat, double course)
{
    emit boatPosUpdated(lng,lat,course);
}

void MapChannel::reloadMap()
{
    emit reloadMapClicked();
}

void MapChannel::setOrigin(double lng, double lat)
{
    emit setOriginPoint(lng,lat);
}

void MapChannel::clearWaypoints()
{
    emit clearWaypointsClicked();
}

void MapChannel::clearAll()
{
    emit clearAllClicked();
}

void MapChannel::addFencePoint(double lng, double lat)
{
    emit addFencePointClicked(lng,lat);
}

void MapChannel::addFence()
{
    emit addFenceClicked();
}

void MapChannel::clearFence()
{
    emit clearFenceClicked();
}

void MapChannel::panTo(double lng, double lat)
{
    emit panToClicked(lng,lat);
}

void MapChannel::clearTrack()
{
    emit clearTrackClicked();
}
