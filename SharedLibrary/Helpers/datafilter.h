/** Magabe Lab 2019
  */
#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>
#include <QQueue>

typedef QSharedPointer<QByteArray> SharedByteArray;
struct CleanData{
    QString mgbSessionId;
    SharedByteArray cleanData;
};

class DataFilter : public QObject
{
    Q_OBJECT
public:
    explicit DataFilter(QObject *parent = nullptr);
    void appendToBuffer(const QByteArray& dirtyData, const QString mgbSessionId);
    CleanData getCleanData();
    void stopFilter();

signals:

public slots:
private:

    QByteArray *buffer_;
    QMutex mutex_;
    QWaitCondition cleanDataAvailable_;
    QQueue<CleanData> cleanDataQueue_;

    /**
     * @brief has_valid_data
     * @param buffer may or may not contains our complete data
     * (a string of chars that end with \r\n)
     * @param r_index [OUT] index of \r,it is set to -1,if the buffer doesnt contain valid data
     * @param n_index [OUT] index of \n,it is set to -1,if the buffer doesnt contain valid data
     * @param data_size [OUT] length od valid data , this include 2 for \r\n
     * @return true when buffer contain complete data or else returns false
     */
    bool hasValidData(QByteArray *const buffer, int *r_index, int *n_index);

    /**
     * @brief get_valid_data
     * @param buffer
     * @return return a valid pointer if it has valid data or else return null
     */
    QByteArray getValidData();
    bool filterStoped_;
};

#endif // DATAFILTER_H
