/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#include "magabeiconprovider.h"

MagabeIconProvider::MagabeIconProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap),
    awesome(new QtAwesome())
{
    awesome->initFontAwesome(); //initilize codepoints and names

}

/**
 * @brief MagabeIconProvider::getIcon
 * @param name
 * @param options
 * @return
 */
QIcon MagabeIconProvider::getIcon(const QString &name, const QVariantMap &options)
{
    return awesome->icon( name, options );
}

/**
 * @brief MagabeIconProvider::getIconName
 * @param  id parameter is iconname:fa::squareo*color:#ff0000*options:optn1=value,opt2=value
 * @return
 */
QString MagabeIconProvider::getIconName(QString id)
{
    int position =   id.indexOf(QStringLiteral("*"),0,Qt::CaseSensitive);
    //        int secondposition = id.indexOf("#",position+1,Qt::CaseSensitive);
    QString name = id.left(position).remove(QStringLiteral("iconname:"),Qt::CaseInsensitive);
    //        QString width = id.mid(position+1,secondposition-(position+1)).remove("width:",Qt::CaseInsensitive);
    //        QString height  =  id.mid(secondposition+1).remove("height:",Qt::CaseInsensitive);
    //        qDebug() << name  << width << height ;
    return name;
}

/**
 * @brief MagabeIconProvider::getWidth
 * @param  id parameter is iconname:fa::squareo*color:#ff0000*options:optn1=value,opt2=value
 * @return
 */
QString MagabeIconProvider::getColor(QString id)
{
    int position =   id.indexOf(QStringLiteral("*"),0,Qt::CaseSensitive);
    int secondposition = id.indexOf(QStringLiteral("*"),position+1,Qt::CaseSensitive);
    QString colorName = id.mid(position+1,secondposition-(position+1)).remove(QStringLiteral("color:"),Qt::CaseInsensitive);
    return colorName;
}


/**
 * @brief MagabeIconProvider::getHeight
 * @param example id parameter is iconname:fa::squareo*color:#ff0000*options:optn1=value,opt2=value
 * @return
 */
//QString MagabeIconProvider::getOptions(QString id)
//{
//    int position =   id.indexOf("*",0,Qt::CaseSensitive);
//    int secondposition = id.indexOf("*",position+1,Qt::CaseSensitive);
//    QString options  =  id.mid(secondposition+1).remove("options:",Qt::CaseInsensitive);
//    return options;
//}

int MagabeIconProvider::getCodePoint(QString id)
{
    return awesome->getCodePoint(id.remove(QStringLiteral("fa::")));
}


/**
 * @brief MagabeIconProvider::~MagabeIconProvider
 */
MagabeIconProvider::~MagabeIconProvider()
{
    if(awesome) delete awesome;
}


/**
 * @brief MagabeIconProvider::requestPixmap
 * @param id
 * @param size
 * @param requestedSize
 * @return
 */
QPixmap MagabeIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int width;
    int height;
    QVariantMap options;
    QColor color(getColor(id));
    QString iconName = getIconName(id);

    if(requestedSize.width() <= 0 || requestedSize.height() <= 0){
        width=  26;
        height= 28;
    }else{
        width = requestedSize.width();
        height = requestedSize.height();
    }

    if (size)
        *size = QSize(width, height);
    if(color.isValid()){
        options.insert(QStringLiteral("color"),color );
    }
       // qDebug() <<"-----------"<< requestedSize.width();

    return QPixmap(awesome->icon(getCodePoint(iconName),options).pixmap(width,height));
}


