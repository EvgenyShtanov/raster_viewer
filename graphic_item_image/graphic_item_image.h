#ifndef __GRAPHIC_ITEM_IMAGE_H__
#define __GRAPHIC_ITEM_IMAGE_H__

#include <gdal_priv.h> 

#include <QGraphicsItem>
#include <QString>
#include <QSize>
#include <QPainter>
#include <QFile>

//! �����, ���������� ������ ����� � �������������� gdal ��� ������ � ����������� QGraphicsScene/GraphicsQView
class QGraphicsItem_Image : public QGraphicsItem {
private:;
	QString name;//!< ��� ����� � ���������� �������
	QSize  size_raster;//!< ������� ������ � ��������
	int number_bands;//!< ���������� band��
	int list_bands[3];//!< ������ ������� band�� ��� ������������ ��������� ������
public:;
	//! �����������
	QGraphicsItem_Image(QGraphicsItem * parent = 0);
	//! ������� ����� ����� � ���������� �������
	void set_image(const QString &name_file);
	//! ��������� ������
	virtual QRectF	boundingRect () const;
	//! ���������
	virtual void	paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	//!
};
#endif
