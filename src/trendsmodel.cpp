#include "trendsmodel.h"

TrendsModel::TrendsModel(TwitterApi *twitterApi)
{
    this->twitterApi = twitterApi;
    connect(twitterApi, &TwitterApi::getIpInfoSuccessful, this, &TrendsModel::handleGetIpInfoSuccessful);
    connect(twitterApi, &TwitterApi::placesForTrendsSuccessful, this, &TrendsModel::handlePlacesForTrendsSuccessful);
}

int TrendsModel::rowCount(const QModelIndex &) const
{
    return trends.size();
}

QVariant TrendsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }
    if(role == Qt::DisplayRole) {
        return QVariant(trends.value(index.row()));
    }
    return QVariant();
}

void TrendsModel::handleGetIpInfoSuccessful(const QVariantMap &result)
{
    qDebug() << "TrendsModel::handleGetIpInfoSuccessful";
    QString location = result.value("loc").toString();
    QStringList locationList = location.split(",");
    if (locationList.size() == 2) {
        qDebug() << "Getting trends for coordinates lat: " + locationList.value(0) + ", long: " + locationList.value(1);
        twitterApi->placesForTrends(locationList.value(0), locationList.value(1));
    }
}

void TrendsModel::handlePlacesForTrendsSuccessful(const QVariantList &result)
{
    qDebug() << "TrendsModel::handlePlacesForTrendsSuccessful";
    if (!result.isEmpty()) {
        QVariantMap firstResult = result.value(0).toMap();
        QString woeid = firstResult.value("woeid").toString();
        qDebug() << "My place (WOEID): " + woeid;
        twitterApi->trends(woeid);
    }
}
