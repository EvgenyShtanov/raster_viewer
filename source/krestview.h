#ifndef CITYVIEW_H
#define CITYVIEW_H

#include <QGraphicsView>
#include "krest.h"
//#include "form_mainwindow_krest.h"

class KrestView : public QGraphicsView {
public:
	bool cutting;
	//QPushButton *KVpushButtonFit;
	//bool rubberBandIsShown;
	QRect rubberBandRect;
	//enum { Margin = 50 };
	//double scale;//!< ����������� ��������
	//! �����������
	KrestView (QWidget *parent = 0);
	double x0,y0;//!< ���������� ������ �������� ���� ������ � �� xy  
	double c_R,r_R;//!< ��������� ������� �� ������ ������ � �� ������� ������	
	int dr,dc;
	double scaled;
public:;
	//void setPushButtonFit (QPushButton *QPB) {KVpushButtonFit=QPB;};
private:;
	int HEIGHT_WINDOW, WIDTH_WINDOW;
	bool is_move;//!< ���� ������ �����������
	QPoint pressed_left_button;
	QCursor prev_curcor;//!< ������ ����
	QCursor null_curcor;
protected:;
	virtual void	mouseMoveEvent (QMouseEvent * event);
	//virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );  
	//void updateRubberBandRegion();
	//! ��������� ������� �� ������ ����
	//virtual void	mousePressEvent ( QMouseEvent * event );
	//! ��������� ���������� �� ������ ����
	//virtual void	mouseReleaseEvent ( QMouseEvent * event );
	//!��������� ������� ����������� ����  
	//virtual void	mouseMoveEvent ( QMouseEvent * event );    
	//! ��������� ������� �� ������
	//virtual void keyPressEvent ( QKeyEvent * event ) ;  
	//! ��������� �������� �������� ����
	void wheelEvent (QWheelEvent * event);
	//! ��������� ������� ����������� ����
	//virtual void	mouseMoveEvent ( QMouseEvent * event );   
public slots:;
	//void ScaleFit() {this->fitInView(this->scene()->itemsBoundingRect(),Qt::KeepAspectRatio);} ;

signals:;
	//! ������ ��������� ��������
	void change_scale (double scale);

	//! ������ � ��������� ������� �� ������
	void position_on_shot(double x,double y) {};
};

#endif
