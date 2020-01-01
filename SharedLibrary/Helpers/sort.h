#ifndef SORT_H
#define SORT_H

#include <QObject>

class Sort : public QObject
{
    Q_OBJECT
public:
    enum class Order{
        DESC = 0,
        ASC = 1
    };

    Q_ENUM(Order)

    explicit Sort(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SORT_H
