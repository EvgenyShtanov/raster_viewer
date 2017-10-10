#include "graphic_item_image.h"

//! �����������
QGraphicsItem_Image::QGraphicsItem_Image(QGraphicsItem * parent ) : QGraphicsItem(parent)
{
  number_bands=0;
  //setFlag(QGraphicsItem::ItemIsMovable,true);
}
//! ������� ����� ����� � ���������� �������
void QGraphicsItem_Image::set_image(const QString &name_file)
{
  prepareGeometryChange ();

  name=name_file;
  size_raster=QSize(0,0);
  number_bands=0;

  if (!name.isEmpty()) {
   GDALDataset  *pdata;
   pdata=(GDALDataset*)GDALOpen(name.toLocal8Bit().data(),GA_ReadOnly);
   if (pdata!=0) {
///-----�������� ��������� �����-----------------------------------------
    int numCount = (pdata->GetRasterBand(1))->GetOverviewCount(); 
    QFile ovrfile(name+".ovr");
       if(!ovrfile.exists()&&numCount==0){
           int anOverviewList[3] = { 4, 8, 16};
           pdata->BuildOverviews( "NEAREST", 3, anOverviewList, 0, NULL, 
                                      NULL, NULL );
       }
///----------------------------------------------------------------------
    int w,h;
    // ������ ������ � ������ ������ � ��������
    w=pdata->GetRasterXSize();
    h=pdata->GetRasterYSize();
    size_raster=QSize(w,h);
    // ������ ���������� band��
    number_bands=pdata->GetRasterCount();
    //printf("number_bands=%d\n",number_bands);
    // ������������ ������ ������������ �����
      if (number_bands==1){
        list_bands[0]= 1;
      };
      if (number_bands>=3){
        list_bands[0]=3;
        list_bands[1]=2;
        list_bands[2]=1;        
      };
    // �������� �����
    GDALClose(pdata);
   };    
  };
}

//! ��������� ������
QRectF QGraphicsItem_Image::boundingRect () const
{
  return QRectF(QPointF(0,0),size_raster) ;
}
//! ���������
void QGraphicsItem_Image::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  QTransform transform;// ��������� �������� �� ��������� �� � �� ���������� �����������
  int W,H;
  QPaintDevice *pdevice;
  pdevice=painter->device () ;
  QRectF bound;

  bound=boundingRect ();
  if (pdevice!=0){
    W=pdevice->width ();
    H=pdevice->height ();

    // ��������� ���������� �������� �� ��������� �� � �� ���������� �����������
    transform=painter->combinedTransform();

    QRectF window_rect_in_LSC;// ������� ���������� ����������� � ��������� ��
    QRectF workig_area_in_LSC;// ������� ������������ ������ � ��������� ��
    QRectF workig_area_in_DSC;// ������� ������������ ������ � �� ���������� �����������
    qreal xw1,xw2,yw1,yw2;// ��������� ������� ������������ ��������� ������ � �� ����
    qreal cf1,cf2,rf1,rf2;// ��������� ������� ������������ ��������� ������ � �� ������

    // ���������� ������������� ������� ���������� ����������� ���������� � ��������� ��
    window_rect_in_LSC=transform.inverted().mapRect(QRectF(0,0,W,H));

    //printf("W=%d H=%d\n",W,H);
    // ��������� ��������� ����� ������������� ������� 
    window_rect_in_LSC.getCoords(&cf1, &rf1, &cf2, &rf2);
    
    //printf("1: cf1=%lf cf2=%lf rf1=%lf rf2=%lf\n",cf1,cf2,rf1,rf2);
    // ����������� ������� ��������� ������ ��������� ��� �����������
    workig_area_in_LSC=bound.intersected(QRectF(window_rect_in_LSC));
    
    // ���������� ������� ������������ ������ � �� ���������� �����������
    workig_area_in_DSC=transform.mapRect(workig_area_in_LSC);

    // ��������� ��������� ����� ������������� ������� 
    workig_area_in_LSC.getCoords(&cf1, &rf1, &cf2, &rf2);
    workig_area_in_DSC.getCoords(&xw1, &yw1, &xw2, &yw2);

    //printf("2: cf1=%lf cf2=%lf rf1=%lf rf2=%lf\n",cf1,cf2,rf1,rf2);

    //printf("xw1=%lf xw2=%lf yw1=%lf yw2=%lf\n",xw1,xw2,yw1,yw2);
    // ���������� ���������� ������� painter
    painter->save();
    // ��������� ���������� �������� � ��������� ���������
    painter->resetTransform();
    if (!name.isEmpty()) {
    GDALDataset  *pdata;

      // �������� ����� �� ������
    pdata=(GDALDataset*)GDALOpen(name.toLocal8Bit().data(),GA_ReadOnly);

    if (pdata!=0) {
     int nb_working;// ���������� ������������ �����
     QImage im;

     if ((!workig_area_in_DSC.isEmpty())&&((number_bands==1)||(number_bands>=3))) {
          
      int ww,hw;// ������� ������� ����������� � �������� � �� ��������� �����������
      int bpp;// ���� �� ������� � �������������� ������ 
     
          // ����������� �������� ������������ ������� � �������� � �� ��������� ����������� 
          ww=xw2-xw1+1;
          hw=yw2-yw1+1;

      if (number_bands==1) {
            // ��������� ������������ ������
       im=QImage(ww,hw,QImage::Format_Indexed8);
       bpp=1;
       nb_working=1;

            // ������������ ������� ������
       int k;
       im.setNumColors(256);
       // ������ ������� ������
       GDALColorTable *pcolor_table;
       pcolor_table=(pdata->GetRasterBand(1))->GetColorTable ();
       
       for(k=0;k<256;k++) {
        if (pcolor_table!=0) {
         const GDALColorEntry *pcolor;
         pcolor=pcolor_table->GetColorEntry(k);
         im.setColor(k, QColor(pcolor->c1,pcolor->c2,pcolor->c3).rgb());
            // puts("135");
        }
        else {
         im.setColor(k, QColor(k,k,k).rgb());
            //  puts("139");
        };
       };     
      };
      if (number_bands>=3) {
       im=QImage(ww,hw,QImage::Format_RGB32);
       bpp=4;
       nb_working=3;
      };

      //im.fill(0);
      int wf,hf;// ������� ������� � ��������� �� ����������� ��� �����������

          wf=cf2-cf1+1;
          hf=rf2-rf1+1;

          if ((cf1+wf)>size_raster.width()-1) wf=size_raster.width()-1-cf1;
          if ((rf1+hf)>size_raster.height()-1) hf=size_raster.height()-1-rf1;
          
      // ������ ��������� ������
      pdata->RasterIO(GF_Read,cf1,rf1, wf,hf,im.bits(),im.width(),im.height(),GDT_Byte,
      nb_working,list_bands,bpp,im.bytesPerLine(),1);

          // ����������� ������
          painter->drawImage(workig_area_in_DSC,im);
     };
        // �������� �����
     GDALClose(pdata);
    }; 
    };

    // ������������� ���������� ������� painter
    painter->restore();
  };
}
