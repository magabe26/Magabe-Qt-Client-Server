/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef MAGABEICONPROVIDER_H
#define MAGABEICONPROVIDER_H

#include <QQuickImageProvider>
#include "QtAwesome.h"
#include <QDebug>
#include"icons.h"
#include <QMetaEnum>


class MagabeIconProvider : public QQuickImageProvider
{

public:
  MagabeIconProvider();
  QIcon getIcon(const QString& name, const QVariantMap& options);
  QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
  QString getIconName(QString id);
  QString getColor(QString id);
  //    QString getOptions(QString id);
  int getCodePoint(QString id);
  ~MagabeIconProvider();
signals:

public slots:
private:
  QtAwesome* awesome;

public:
};
#endif // MAGABEICONPROVIDER_H
